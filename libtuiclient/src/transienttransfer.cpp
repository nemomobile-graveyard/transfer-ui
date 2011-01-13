
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

#include "transienttransfer.h"
#include "transferprivate.h"

using namespace TransferUI;


TransientTransfer::TransientTransfer (ComMeegoTransferuiInterface * interface,
    const QString & id, const QString & name, int type , const QString&
    imagePath , int state , int totalFiles, qulonglong size, const QString&
    targetName , const QString& msg, Client * parent) : Transfer (parent) {

    d_ptr->interface = interface;
    d_ptr->id = id;

    d_ptr->data.method = static_cast<TransferType>(type);
    d_ptr->data.status = static_cast<TransferStatus>(state);
    d_ptr->data.thumbnailFile = imagePath;
    d_ptr->data.bytes = size;
    d_ptr->data.filesCount = totalFiles;
    d_ptr->data.targetName = targetName;
    d_ptr->data.message = msg;
    d_ptr->data.name = name;
}

TransientTransfer::TransientTransfer(ComMeegoTransferuiInterface *interface,
    const QString& id, const QString& name, Client::TransferType
    type, Client *parent) : Transfer (parent) {

        d_ptr->interface = interface;
        d_ptr->id = id;
        d_ptr->data.method = static_cast<TransferType>(type);
        d_ptr->data.name = name;
}

TransientTransfer::~TransientTransfer() {

}

bool TransientTransfer::markFailed (const QString & message,
    const QString & description) {

    Q_UNUSED (message);
    Q_UNUSED (description);

    return false;
}