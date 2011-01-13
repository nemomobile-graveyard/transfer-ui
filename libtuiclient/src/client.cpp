
/*
 * transfer-ui -- Handset UX Transfer user interface
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * Contact: Jukka Tiihonen <jukka.tiihonen@nokia.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "TransferUI/client.h"
#include "clientprivate.h"
#include "TransferUI/transfer.h"
#include "persistenttransfer.h"
#include "transienttransfer.h"
#include "transferui_interface.h"
#include "transferenums.h"

using namespace TransferUI;

Client::Client (QObject * parent) : QObject (parent),
    d_ptr (new ClientPrivate(this)) {

    // Pass private signals to public (TODO: better way to do this?)
    QObject::connect(d_ptr,SIGNAL(startTransfer(Transfer*)),
        this,SIGNAL(startTransfer(Transfer*)));
    QObject::connect(d_ptr,SIGNAL(pauseTransfer(Transfer*)),
        this,SIGNAL(pauseTransfer(Transfer*)));
    QObject::connect(d_ptr,SIGNAL(cancelTransfer(Transfer*)),
        this,SIGNAL(cancelTransfer(Transfer*)));

    QObject::connect(d_ptr, SIGNAL(repairErrorTransfer(Transfer*)),
        this,SIGNAL(repairErrorTransfer(Transfer*)));

}

Client::~Client () {
    delete d_ptr;
}

bool Client::init() {
    return d_ptr->init();
}

Transfer * Client::registerTransfer (const QString& name, TransferType type,
	const QString& clientId) {
	qDebug() << __FUNCTION__ << clientId;
    if (d_ptr->interface == 0) {
        qCritical() << "Client not initialized";
        return 0;
    }

    Transfer * transfer = 0;

    QDBusReply<QString> reply =
        d_ptr->interface->registerTransientTransfer(name, type, clientId);

    if (reply.isValid()) {
        transfer = new TransientTransfer (d_ptr->interface, reply.value(), name,
            type, this);
        if (transfer != 0) {
            d_ptr->transfers[transfer->transferId()] = transfer;

            connect(d_ptr,SIGNAL(sendLastProgress()),transfer,
                SLOT(sendLastProgress()));

        } else {
            qCritical() << "Failed to create transfer";
        }
    } else {
        qWarning() << "Transfer UI returned error.";
    }

    return transfer;
}

bool Client::askForSummaryUpdate () {

    if (d_ptr->interface == 0) {
        qCritical() << "Client not initialized";
        return false;
    }

    d_ptr->interface->sendSummary();

    return true;

}

bool Client::showUI() {
    if (d_ptr->interface == 0) {
        qCritical() << "Client not initialized";
        return false;
    }

    d_ptr->interface->showUI();

    return true;
}

void Client::removeTransfer (const QString& tid) {
    qDebug() << "Removing transfers against " << tid;
    d_ptr->transfers.value(tid)->disconnect();
    int number_removed = d_ptr->transfers.remove (tid);

    if (number_removed != 1) {
        qWarning() << "Removed " << number_removed <<
            " instances of transfers against " << tid;
    }
}


void Client::setProgressValueDelta(double delta) {
    if ((delta < 0.1 ) && (delta > 1)) {
        qDebug () << "Delta is less than threshold" << delta;
    } else {
        qWarning() << " Any changes in the delta value will" <<
             "impact the performance" << " New delta " << delta <<
             "Old delta " << d_ptr->progressValueDelta;
        d_ptr->progressValueDelta = delta;
    }
}

void Client::setProgressTimeDelta(int delta) {
    if (delta < 50) {
        qDebug () << "Delta is less than threshold" << delta;
    } else {
        qWarning() << " Any changes in the delta value will " <<
            "impact the performance" << "New delta " << delta <<
            "Old delta " <<  d_ptr->progressTimeDelta;
        d_ptr->progressTimeDelta = delta;
    }
}

int Client::progressTimeDelta() const {
    return d_ptr->progressTimeDelta;
}

double Client::progressValueDelta() const {
    return d_ptr->progressValueDelta;
}


bool Client::isTUIVisible() const {
    return d_ptr->tuiOpen;
}

// -- Private class functions ------------------------------------------------

ClientPrivate::ClientPrivate(QObject * parent) : QObject (parent) , interface(0)
    , progressTimeDelta(250) , progressValueDelta(0.1) , tuiOpen (false) {

}

ClientPrivate::~ClientPrivate() {
}

bool ClientPrivate::init () {

    interface = new ComMeegoTransferuiInterface ("com.meego.transferui",
        "/com/meego/transferui", QDBusConnection::sessionBus(), this);

    if (interface != 0) {
        QObject::connect(interface, SIGNAL(cancel(QString)),
            this, SLOT(cancelSlot(QString)));
        QObject::connect(interface, SIGNAL(start(QString)),
            this, SLOT(startSlot(QString)));
        QObject::connect(interface, SIGNAL(pause(QString)),
            this, SLOT(pauseSlot(QString)));

        QObject::connect(interface, SIGNAL(pause(QString)),
            this, SLOT(pauseSlot(QString)));

        QObject::connect(interface, SIGNAL(errorRepairRequested(QString)),
            this, SLOT(errorRepairSlot(QString)));

        QObject::connect(interface, SIGNAL(launched()), this,
                SLOT(tuiLaunched()));

        QObject::connect(interface,SIGNAL(tuiOpened()),this,SLOT(tuiOpened()));

        QObject::connect(interface,SIGNAL(tuiClosed()),this,SLOT(tuiClosed()));

    } else {
        qCritical() << "Coundn't open dbus connection";
        return false;
    }

    return true;
}

void ClientPrivate::cancelSlot (const QString & id) {
    qDebug() << "Transfer id " << id;
    if (transfers.contains (id)) {
        Transfer * transfer = transfers[id];
        transfer->cancelSlot ();
        Q_EMIT(cancelTransfer (transfer));
    } else {
        qDebug() << "Ignore cancel signal for" << id;
    }
}

void ClientPrivate::startSlot (const QString & id) {
    if (transfers.contains (id)) {
        Transfer * transfer = transfers[id];
        transfer->startSlot ();
        Q_EMIT(startTransfer (transfer));
    } else {
        qDebug() << "Ignore start signal for" << id;
    }
}

void ClientPrivate::pauseSlot (const QString & id) {
    if (transfers.contains (id)) {
        Transfer * transfer = transfers[id];
        transfer->pauseSlot ();
        Q_EMIT(pauseTransfer (transfer));
    } else {
        qDebug() << "Ignore pause signal for" << id;
    }
}

void ClientPrivate::errorRepairSlot (const QString& id) {
    qDebug()<<transfers.contains(id);
    if (transfers.contains (id)) {
        Transfer * transfer = transfers[id];
        transfer->errorRepairSlot ();
        Q_EMIT(repairErrorTransfer (transfer));
    } else {
        qDebug() << "Ignore repair signal for" << id;
    }
}

void ClientPrivate::tuiLaunched () {
    qDebug () << "ClientPrivate::tuiLaunched";
    QMapIterator<QString, Transfer *> iter (transfers);

    if (transfers.count() == 0) {
        // There are no transfers
        return;
    }

    while (iter.hasNext()) {
        iter.next();
        Transfer *transfer = iter.value ();

        if (iter.key() != transfer->transferId ()) {
            qCritical () <<
                "Client::tuiLaunched -> Key does not match transfer id";
        }

        QDBusReply<bool> exists =
            interface->transferExists (transfer->transferId ());
        if (exists.isValid ()) {
            if (exists.value () == true) {
                qDebug() << transfer->transferId () << "exists in TUI"
                    << "- not registering this. Moving to next";

                // Transfer exists in TUI - go to next transfer
                continue;
            }
        } else {
            qCritical() << "Got invalid reply when checking if transfer "
                << transfer->transferId () << " exists in TUI";
            continue;
        }

        QDBusReply<QString> reply =
            interface->registerTransientTransfer (transfer->transferId ());

        if (reply.isValid()) {
            if (reply.value () == transfer->transferId ()) {
                transfer->tuiLaunched ();
            } else {
                qCritical() << "Got " << reply.value () <<
                    " as reply from registerPersistentTransfer instead of "
                    << "expected value of " << transfer->transferId ();
            }
        } else {
            qWarning() << "Transfer with tracker uri" << transfer->transferId ()
                << "not found.";
        }
    }
}

void ClientPrivate::tuiOpened() {
    qDebug() << "Enter tuiOpened";
    if (tuiOpen != true) {
        tuiOpen = true;
        Q_EMIT(sendLastProgress());
    }
}

void ClientPrivate::tuiClosed() {
    qDebug() << "Enter tuiClosed";
    tuiOpen = false;
}