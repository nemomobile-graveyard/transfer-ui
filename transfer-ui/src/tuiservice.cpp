
/*
 * This file is part of Handset UX Transfer user interface
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 * Contact: Jukka Tiihonen <jukka.tiihonen@nokia.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to 
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,     
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER  
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS  
 * IN THE SOFTWARE. 
 */








//local headers
#include "tuiservice.h"
#include "tuiserviceprivate.h"
#include "transferui_adaptor.h"
#include "tuidatamodel.h"
#include "transferenums.h"
#include "tuiclientdatamodel.h"
#include "tuidatamodelproxy.h"
#include "tuicompletedlistproxymodel.h"
#include "tuireadhistorythread.h"

#include "TransferUI/transferuiimplementationinterface.h"

//Qt headers
#include <QGraphicsLinearLayout>
#include <QFileInfo>
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>
#include <QTimer>
#include <QUuid>
#include <QDateTime>
#include <QAbstractItemModel>
#include <QLibrary>

#include <QDBusContext>
#include <QDBusServiceWatcher>
#include <QDBusMessage>
#include <QListIterator>

#include <contentaction/contentaction.h>
#include <qmsystem2/qmtime.h>

using namespace ContentAction;
using namespace TransferUI;
using namespace MeeGo;
TUIService *TUIService::selfInstance = 0;

//standard headers

TUIService::TUIService(QObject *parent)
    :QObject(parent),d_ptr(new TUIServicePrivate) {
    Q_UNUSED(selfInstance)
    //set the tuiState to Invalid
    tuiState = Invalid;

    connect(d_ptr,SIGNAL(cancel(QString)),this,SIGNAL(cancel(QString)));

    connect(d_ptr,SIGNAL(pause(QString)),this,SIGNAL(pause(QString)));

    connect(d_ptr,SIGNAL(resume(QString)),this,SIGNAL(start(QString)));

    connect(d_ptr,SIGNAL(repair(QString)),this,
        SIGNAL(errorRepairRequested(QString)));


    TUIService::selfInstance = this;

    d_ptr->proxyModel = new TUIDataModelProxy(this);

    d_ptr->completedProxyModel = new TUICompletedListProxyModel(this);

    // Create a client data model
    d_ptr->clientDataModel = new TUIClientDataModel(this);


    //listen to date / time format change signal
    QmTime *qmTime = new QmTime(this);

    connect(qmTime, SIGNAL(timeOrSettingsChanged(MeeGo::QmTimeWhatChanged)),
        SLOT(timeOrSettingsChanged (MeeGo::QmTimeWhatChanged)));

    stringEnumFunctionMap.insert("setName",Name);
    stringEnumFunctionMap.insert("setSize",Size);
    stringEnumFunctionMap.insert("setEstimate",Estimate);
    stringEnumFunctionMap.insert("setMessage",Message);
    stringEnumFunctionMap.insert("setTargetName",TargetName);
    stringEnumFunctionMap.insert("setFilesCount",FilesCount);
    stringEnumFunctionMap.insert("setCurrentFileIndex",CurrentFileIndex);
    stringEnumFunctionMap.insert("setThumbnailForFile",ThumbnailForFile);
    stringEnumFunctionMap.insert("setIcon",Icon);
    stringEnumFunctionMap.insert("setCancelButtonText",CancelButtonText);
    stringEnumFunctionMap.insert("setCanPause",CanPause);
    stringEnumFunctionMap.insert("setSendNow",SendNow);
    stringEnumFunctionMap.insert("setTransferTypeString",TransferTypeString);
	stringEnumFunctionMap.insert("setTransferImage",TransferImage);

}

void TUIService::loadImplementationPlugin() {

    qDebug() << __FUNCTION__ << "Loading UI Plugin";
    QString fileName =
        QLatin1String("/usr/lib/transfer-ui/implementations/libdefault.so");

    QPluginLoader loader(fileName);
    loader.setLoadHints (QLibrary::ExportExternalSymbolsHint);
    QObject *plugin = loader.instance();
    if (plugin != 0) {
        d_ptr->interface =
                qobject_cast<TransferUIImplementationInterface *>(plugin);
        if (d_ptr->interface != 0) {

            d_ptr->interface->setDataModel(d_ptr->proxyModel->dataModel());
            d_ptr->interface->setCompletedSortModel(
                d_ptr->completedProxyModel->dataModel());


            connect(plugin,SIGNAL(visibilityChanged(bool)), this,
                SLOT(visibilityChanged(bool)));

            connect(plugin,SIGNAL(canceled(QModelIndex)),
                d_ptr, SLOT(cancelTransfer(QModelIndex)));

            connect(plugin,SIGNAL(paused(QModelIndex)),
                d_ptr, SLOT(pauseTransfer(QModelIndex)));

            connect(plugin,SIGNAL(resumed(QModelIndex)),
                d_ptr, SLOT(resumeTransfer(QModelIndex)));

            connect(plugin,SIGNAL(repair(QModelIndex)),
                d_ptr, SLOT(repairTransfer(QModelIndex)));

            connect(plugin,SIGNAL(elementClicked(QModelIndex)),
                d_ptr, SLOT(elementClicked(QModelIndex)));

            connect(plugin,SIGNAL(completedElementClicked(QModelIndex)),
                d_ptr, SLOT(completedElementClicked(QModelIndex)));

            connect(plugin,SIGNAL(clearCompletedList()),this,
                SLOT(clearCompletedTransfers()));

            connect(plugin,SIGNAL(movedToSwitcher(bool)),
                this,SLOT(movedToSwitcher(bool)));

            connect(d_ptr->completedProxyModel,SIGNAL(modelEmpty(bool)),
        		d_ptr,SLOT(histroyModelEmpty(bool)));

        }
    } else {
        qDebug() << __FUNCTION__ << "Error loading plugin" << loader.errorString () ;
    }
}

TUIService::~TUIService() {
    qDebug() << "Deleting Service";
    if(d_ptr->interface != 0) {
        d_ptr->interface->unloadUI();
    }
    delete d_ptr;
    // d_ptr = 0; //this will give compilation error since d_ptr is readonly

    selfInstance = 0;
}

QApplication *TUIService::applicationInstance(int argc, char **argv) {
    QApplication *app = 0;
    if(d_ptr->interface != 0) {
        app = d_ptr->interface->applicationInstance(argc,argv);
        qDebug() << __FUNCTION__ << app;
    } else {
        qDebug() << "Interface is not ready";
    }
    Q_EMIT(launched());
    return app;
}

bool TUIService::init() {

    //Initialize DBUS
    new TransferuiAdaptor(this);
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerObject("/com/meego/transferui",this);
    connection.registerService("com.meego.transferui");

    serviceWatcher = new QDBusServiceWatcher(this);
    serviceWatcher->setConnection(QDBusConnection::sessionBus());
    serviceWatcher->setWatchMode(QDBusServiceWatcher::WatchForUnregistration);
    connect(serviceWatcher, SIGNAL(serviceUnregistered(QString)), this,
        SLOT(serviceUnregistered(QString)));

    //just started , emit stateChanged to idle
    tuiState = Idle;
    Q_EMIT(stateChanged("idle"));

    // is the shown variable to false
    d_ptr->isUIShown = false;

    return true;
}

TUIService* TUIService::instance() {
    return selfInstance;
}

void TUIService::cancelled(const QString &id) {
    qDebug() << __FUNCTION__ << id;
    const TUIData *data = d_ptr->proxyModel->tuiData(id);
    if (data != 0) {
        if(TransferStatusCancel == data->status) {
            if(data->currentFileIndex > 1) {
                d_ptr->notifyPartialDoneTransfers();
            }
        }
    }

    d_ptr->proxyModel->cancelled(id);
    d_ptr->proxyModel->removeTransfer(id);

    //remove the transfer from the client data model
    //check if the method call is from from the dbus
    if(calledFromDBus()) {
        qDebug() << "Method call is from dbus";
        QString serviceName = message().service();
        d_ptr->clientDataModel->removeTransfer(serviceName, id);

        checkServiceRegister(serviceName);
    }

    // send summary of the transfers
    sendSummary();

}

void TUIService::cancelFailed(const QString &id , const QString& message) {
    qDebug() << __FUNCTION__ << id;
    if(d_ptr->isUIShown == true) {
        d_ptr->displayInfoBanner(ClientEvent,message);
    }
}

void TUIService::resumed(const QString &id) {
    qDebug() << "TUIService --> transfer resumed for the id" << id;
    const TUIData *data = d_ptr->proxyModel->tuiData(id);
    if (data != 0) {
        if((TransferStatusResume == data->status) ||
            (TransferStatusActive == data->status)) {
           qDebug() << "Transfer is already in resumed state";
           return;
        } else if(TransferStatusError == data->status) {
            qDebug() << "Error Transfers can't be resumed";
            return;
        }
    }
    
    d_ptr->proxyModel->resumed(id);
    sendSummary();
}

void TUIService::paused(const QString &id) {
    qDebug() << "TUIService --> transfer is paused" << id;
    const TUIData *data = d_ptr->proxyModel->tuiData(id);
    if (data != 0) {
        if(TransferStatusPaused == data->status) {
           qDebug() << "Transfer is already in paused state";
           return;
        } else if(TransferStatusError == data->status) {
            qDebug() << "Error Transfers can't be Paused";
            return;
        }
    }
    d_ptr->proxyModel->paused(id);
    sendSummary();
}

//TODO remove this function once we have deprecated macro for done
void TUIService::done(const QString &id) {

    qDebug() << "TUIService--> transfer done" << id;
    done(id,"");

    //remove the transfer from the client data model
    //check if the method call is from from the dbus
    if(calledFromDBus()) {
        qDebug() << "Method call is from dbus";
        QString serviceName = message().service();
        d_ptr->clientDataModel->removeTransfer(serviceName, id);

        checkServiceRegister(serviceName);
    }
}

//TODO remove this function once we have deprecated macro for done
void TUIService::done(const QString& id, const QString& msg) {
    const TUIData *data = d_ptr->proxyModel->tuiData(id);
    if (data != 0) {
        if(TransferStatusCancel == data->status) {
            d_ptr->displayInfoBanner(ParitalDoneTransfer);
        }

        //remove transfer from the datamodel
        d_ptr->proxyModel->done(id,msg);


        //remove the transfer from the client data model
        //check if the method call is from from the dbus
        if(calledFromDBus()) {
            qDebug() << "Method call is from dbus";
            QString serviceName = message().service();
            d_ptr->clientDataModel->removeTransfer(serviceName, id);
            checkServiceRegister(serviceName);
        }

		d_ptr->proxyModel->removeTransfer(id);
        //update the summary
        sendSummary();
    }
}

void TUIService::markCompleted(const QString& id, bool showInHistory, const QString&
		replaceId, const QString& resultUri) {
	qDebug() << __FUNCTION__ << "Transfer set to done " << id << showInHistory
	<< replaceId << resultUri;
	const TUIData *data = d_ptr->proxyModel->tuiData(id);
    if (data != 0) {

	    if(TransferStatusCancel == data->status) {
            d_ptr->displayInfoBanner(TransferAlreadyCompleted);
        }

        d_ptr->proxyModel->done(id,resultUri);

	    if (showInHistory == true) {
		    d_ptr->writeToHistory(data, showInHistory, replaceId);
            
		    if(d_ptr->isUIShown == true) {
                qDebug() << __FUNCTION__ << "Adding transfer";
			    d_ptr->completedProxyModel->addTransfer(data);
                d_ptr->invisibleCompletedCount = 
                    d_ptr->completedProxyModel->dataModel()->rowCount();
		    } else {
                d_ptr->invisibleCompletedCount++;
            }
	    }

	    //remove the transfer from the client data model
	    //check if the method call is from from the dbus
	    if(calledFromDBus()) {
	        qDebug() << "Method call is from dbus";
	        QString serviceName = message().service();
	        d_ptr->clientDataModel->removeTransfer(serviceName, id);
	        checkServiceRegister(serviceName);
	    }

        d_ptr->proxyModel->removeTransfer(id);
	    //update the summary
        sendSummary();
	}

}

void TUIService::markCompleted(const QString& id, bool showInHistory, const
	QString& replaceId) {
    markCompleted(id, showInHistory, replaceId, QString());
}

void TUIService::pending(const QString &id, const QString &message) {
    qDebug() << __FUNCTION__ << "Transfer set to pending " << message;
    const TUIData *data = d_ptr->proxyModel->tuiData(id);
    if (data != 0) {
        if (TransferStatusInactive == data->status) {
            qDebug() << "Transfer is already in pending state";
            d_ptr->proxyModel->setMessage(id, message);
        }
    }

    if(message.isEmpty() == false) {
        d_ptr->proxyModel->pending(id, message);
    }
    sendSummary();
}

bool TUIService::transferExists (const QString &id) {
    bool exists = d_ptr->proxyModel->transferPresent (id);
    qDebug() << "Checking if transfer " << id << " exists - " << exists;
    return exists;
}

QString TUIService::registerTransientTransfer(const QString& transferId) {
    QString service = QString::null;

    if(calledFromDBus()==true) {
        service = message().service();
        if (d_ptr->clientDataModel->isServiceRegistered(service) == false) {
            emitVisibilityState(service);
            serviceWatcher->addWatchedService(service);
        }
    }

    d_ptr->registerTransferData(transferId, TransferTypeUpload,
        QString(), service, QString()); //TODO
    sendSummary();
    return transferId;
}

QString TUIService::registerTransientTransfer(const QString& name, int type) {
	return registerTransientTransfer(name,type,QString());
}

QString TUIService::registerTransientTransfer(const QString& name, int type ,
	const QString& clientId ) {
    qDebug() << "Register a new transient transfer client Id " << name << type << clientId;

    QString id = d_ptr->generateTransferId();
    QString service;

    if(calledFromDBus()==true) {
        service = message().service();
        if (d_ptr->clientDataModel->isServiceRegistered(service) == false) {
            emitVisibilityState(service);
            serviceWatcher->addWatchedService(service);
        }
    }

    d_ptr->registerTransferData(id, (TransferType)type, name, service, clientId);

    //set default values
    d_ptr->proxyModel->pending(id,QString());

    if(d_ptr->interface != 0) {
        if(d_ptr->isUIShown == true) {
            d_ptr->interface->setActiveTransfersVisibility(true);
        }
    }
    qDebug() << __FUNCTION__ << id;
    sendSummary();
    return id;
}


void TUIService::setCanPause(const QString &id, bool canPause) {
    qDebug() << __FUNCTION__ << id << "Can Pause Atrribute Set to " << canPause;
    d_ptr->proxyModel->setCanPause(id, canPause);
}

void
TUIService::sendSummary() {
    qDebug() << "TUIService --> update the summary of transfers ";

    //compute summary
    int activeCount = 0;
    int inactiveCount = 0;
    int errorCount = 0;
    //int completedCount = d_ptr->completedProxyModel->dataModel()->rowCount();
    int completedCount = d_ptr->invisibleCompletedCount;
    qDebug() << __FUNCTION__ << completedCount;


    //get transfers count
    d_ptr->proxyModel->getTransfersCount(activeCount,inactiveCount,errorCount);
    Q_EMIT(summaryReport(errorCount,activeCount,inactiveCount,completedCount));

    //emit state changed signal if the TUI state is changed.
    //if there are more than one error transfers , set the state to fail
    //case "fail"
    if(errorCount > 0) {
        if(tuiState != Fail) {
            tuiState = Fail;
            Q_EMIT(stateChanged("fail"));
        }
    } else if(activeCount > 0) { //live
        if(tuiState != Live) {
            tuiState = Live;
            Q_EMIT(stateChanged("live"));
        }
    } else if (inactiveCount > 0) {  //pending state
        if(tuiState != Pending) {
            tuiState = Pending;
            Q_EMIT(stateChanged("pending"));
        }
    } else { //idle
        if(tuiState != Idle) {
            tuiState = Idle;
            Q_EMIT(stateChanged("idle"));
        }
    }
    qDebug() << __FUNCTION__ << "Transfer UI State" << tuiState;

    //if no transfers are there close transfer-ui
    int totalTransfer = activeCount + inactiveCount + errorCount;
    qDebug() << "Total Transfers" << totalTransfer << d_ptr->isUIShown;

    
    if(totalTransfer==0) {
        Q_EMIT(stateChanged("idle")); //fail safe
        if(d_ptr->isUIShown == false) {
            //check if there are any unattended transfers which are canceled
            qDebug() << "Total Data in Model" << d_ptr->proxyModel->count();
            if((d_ptr->proxyModel->count() == 0) && (completedCount == 0)) {
                qDebug() << "UI not visible and transfers count is 0 ";
                if(d_ptr->interface != 0) {
                    d_ptr->interface->unloadUI();
                }
                QCoreApplication::exit();
            }
        } else {
            if(d_ptr->interface != 0) {
                if( d_ptr->isUIShown == true) {
                    d_ptr->interface->setActiveTransfersVisibility(false);
                }
            }
            if(d_ptr->isInSwitcher == true) {
                if((d_ptr->proxyModel->count() == 0) && (completedCount == 0)) {
                    qDebug() << "App in switcher and transfers count is 0 ";
                    if(d_ptr->interface != 0) {
                        d_ptr->interface->unloadUI();
                    }
                    QCoreApplication::exit();
                }
            }
        }
    }
}

void TUIService::setRepairableError(const QString &id, const QString &headerMsg,
    const QString &detailMsg, const QString &actionName) {

    qDebug() << __FUNCTION__ <<id << headerMsg <<
        detailMsg << actionName;

    d_ptr->proxyModel->markError(id, headerMsg, detailMsg, true, actionName);
    sendSummary();

}

void TUIService::setNonRepairableError(const QString &id, const QString
    &headerMsg, const QString &detailMsg) {
    qDebug() << __FUNCTION__ << id << headerMsg <<
        detailMsg;

    d_ptr->proxyModel->markError(id, headerMsg, detailMsg, false, "");
    sendSummary();
}

void TUIService::setEstimate(const QString &id, int seconds) {
    qDebug() << __FUNCTION__ << id << "Estimate Set to " << seconds;
    const TUIData *data = d_ptr->proxyModel->tuiData(id);
    if (data != 0) {
        if((TransferStatusActive == data->status) ||
        (TransferStatusResume == data->status) ) {
            d_ptr->proxyModel->setEstimate(id, seconds);
        }
    }
}

void TUIService::setMessage(const QString &id, const QString &message) {
    qDebug() << __FUNCTION__ << id << "Message Set to " << message;
    const TUIData *data = d_ptr->proxyModel->tuiData(id);
    if (data != 0) {
        if(TransferStatusInactive == data->status) {
            if(message.isEmpty() == false) {
                d_ptr->proxyModel->setMessage(id, message);
            }
        } else {
            qDebug() << __FUNCTION__ << "Message can only be set "
                "for Inactive Transfers";
        }
    }
}

void TUIService::setName(const QString &id, const QString &name) {
    qDebug() << __FUNCTION__ << id << "Name Set to " << name;
    if(name.isEmpty() == false) {
        d_ptr->proxyModel->setName(id, name);
    }
}

void TUIService::setProgress(const QString& id, double done) {
    qDebug() << __FUNCTION__ << id << "Progress Set to " << done;
    const TUIData *data = d_ptr->proxyModel->tuiData(id);
    if (data != 0) {
        if(TransferStatusActive == data->status) {
            d_ptr->proxyModel->setProgress(id, done);
        } else {
            qDebug() << __FUNCTION__ << "Progress can be set for only"
                "Active transfers";
        }
    }
}

void TUIService::setSize(const QString &id, qulonglong bytes) {
    qDebug() << __FUNCTION__ << id << "Size Set to " << bytes;
    d_ptr->proxyModel->setSize(id, bytes);
}

void TUIService::setTransferType(const QString &id, int transferType) {
    qDebug() << __FUNCTION__ << id << "Transfer Type Set to " << transferType;
    d_ptr->proxyModel->setTransferType(id, transferType);
}

void TUIService::setImageFromFilePath(const QString& id, const QString& fileName) {
    qDebug() << __FUNCTION__ << id << "Transfer Image Set to " << fileName;
	d_ptr->proxyModel->setImageFromFile(id, fileName);
}

void TUIService::showUI() {
    qDebug() << __FUNCTION__ << d_ptr->isUIShown;
    //check if plugin is already loaded
    if(d_ptr->interface != 0) {
        d_ptr->interface->loadUI();
        if((d_ptr->proxyModel->count() == 0) &&
            (d_ptr->completedProxyModel->dataModel()->rowCount() == 0)) {
                d_ptr->interface->setNoTransfersVisibility(true);
        }
        Q_EMIT (tuiOpened());
    }

}

void TUIService::started(const QString &id, double done) {
    qDebug() << __FUNCTION__ << id << "State changed with progress " << done;
    const TUIData *data = d_ptr->proxyModel->tuiData(id);
    if (data != 0) {
        if (TransferStatusActive == data->status) {
            qDebug() << "Transfer is already in active state";
            d_ptr->proxyModel->setProgress(id, done);
        }
    }

    d_ptr->proxyModel->started(id, done);
    sendSummary();
}

void TUIService::showErrorDetails(const QString &id) {

    if (d_ptr->interface != 0) {
        if(d_ptr->isUIShown == false) {
            //raise the TUI window and show the notification
            showUI();
        }
        d_ptr->interface->showErrorDetails(d_ptr->proxyModel->modelIndex(id));
    }
}

void TUIService::setFilesCount(const QString &id, int count) {
    qDebug() << __FUNCTION__ << id << "Set total files to " << count;
    d_ptr->proxyModel->setFilesCount(id, count);
}

void TUIService::setCurrentFileIndex(const QString &id, int fileIndex) {
    qDebug() << __FUNCTION__ << id << "Current file count " << fileIndex;
    const TUIData *data = d_ptr->proxyModel->tuiData(id);
    if (data != 0) {
        if(TransferStatusCancel == data->status) {
            if(data->currentFileIndex > 1) {
                d_ptr->notifyPartialDoneTransfers();
            }
        }
    }
    d_ptr->proxyModel->setCurrentFileIndex(id, fileIndex);
}

void TUIService::setTargetName(const QString &id, const QString &targetName) {
    qDebug() << __FUNCTION__ << id << "Set Target Name to " << targetName;
    d_ptr->proxyModel->setTargetName(id, targetName);
}

void TUIService::setThumbnailForFile(const QString& id, const QString& fileName,
    const QString& mimeType) {
    qDebug() << __FUNCTION__ << id << "Set Thumbnail " << fileName << mimeType;
    d_ptr->proxyModel->setThumbnailForFile(id, fileName, mimeType);
}

void TUIService::setIcon(const QString& id, const QString& iconId) {
    qDebug() << __FUNCTION__ << id << "Set Icon " << iconId;
    d_ptr->proxyModel->setIcon(id, iconId);
}

void TUIService::setCancelButtonText(const QString& id, const QString& text) {
    qDebug() << __FUNCTION__ << id << "Set Cancel Button Text " << text;
    d_ptr->proxyModel->setCancelButtonText(id, text);
}


void TUIService::serviceUnregistered(const QString& clientServiceName) {
    qDebug() << "Service unregistered " << clientServiceName;
    //remove the transfers from the datamodel and controller if there are any
    //transfers
    if(d_ptr->clientDataModel->transferCountForClient(clientServiceName) > 0) {
        qDebug () << "Checking for unattended transfers";
        QListIterator<QString> iter =
            d_ptr->clientDataModel->transfersIterator(clientServiceName);

        QString transferId;
         while (iter.hasNext()) {

            transferId = iter.next();
            if(d_ptr->proxyModel->transferPresent(transferId)) {

                qDebug () << "Found unattended transfer " << transferId;
                d_ptr->proxyModel->cleanUpTransfer(transferId);
                d_ptr->proxyModel->removeTransfer(transferId);
            }
        }
    }

    d_ptr->clientDataModel->clientRemoved(clientServiceName);

    serviceWatcher->removeWatchedService(clientServiceName);
    sendSummary();
}

void TUIService::checkServiceRegister(const QString& clientServiceName) {
    if(d_ptr->clientDataModel->transferCountForClient(clientServiceName) == 0) {
        d_ptr->clientDataModel->clientRemoved(clientServiceName);
        serviceWatcher->removeWatchedService(clientServiceName);
    }
}


void TUIService::setFileTypeIcon(const QString& id, const QString& iconId) {
    qDebug() << "Deprecated function set file type icon for the transfer " << id << iconId;

    //mime type is set to null for file icons
    setIcon(id, iconId);
}

void TUIService::emitVisibilityState(const QString& serviceName) {
    qDebug() << "visibility state for service" << serviceName;

    if(d_ptr->isUIShown == true) {
        Q_EMIT (tuiOpened());
    } else {
        Q_EMIT (tuiClosed());
    }

}

void TUIService::setTransferTypeString(const QString& id,
    const QString &title) {
    qDebug() << __FUNCTION__ << id << "Set Transfer Type String to " << title;
    d_ptr->proxyModel->setTransferTypeTitle(id, title);
}

void TUIService::setSendNow (const QString& id, bool canSendNow) {
    qDebug() << __FUNCTION__ << id << "Set Send Now attribute to " << canSendNow;
    d_ptr->proxyModel->setCanSendNow(id, canSendNow);
}

void TUIService::setValues(const QString& id, const QVariantMap& keyValues) {
    if(keyValues.count() < 1 ) {
        qDebug() << "Key value count < 1 ";
    } else {
        QMap<QString, QVariant>::const_iterator i;
        for (i = keyValues.constBegin(); i != keyValues.constEnd(); i++) {
            qDebug() << __FUNCTION__ << "Setting values for key" << i.key();
            switch(stringEnumFunctionMap[i.key()]) {
                case Name:
                    setName(id, i.value().toString());
                break;
                case Size:
                    setSize(id, i.value().toLongLong());
                break;
                case Estimate:
                    setEstimate(id, i.value().toInt());
                break;
                case Message:
                    setMessage(id, i.value().toString());
                break;
                case TargetName:
                    setTargetName(id, i.value().toString());
                break;
                case FilesCount:
                    setFilesCount(id, i.value().toInt());
                break;
                case CurrentFileIndex:
                    setCurrentFileIndex(id, i.value().toInt());
                break;
                case ThumbnailForFile:
                {
                    QStringList params = i.value().toStringList();
                    if(params.count() < 2) {
                        qWarning() << "Invalid no of parameters";
                    } else {
                        setThumbnailForFile(id, params[0] , params[1]);
                    }
                }
                break;
                case Icon:
                    setIcon(id, i.value().toString());
                break;
                case CancelButtonText:
                    setCancelButtonText(id, i.value().toString());
                break;
                case CanPause:
                    setCanPause(id, i.value().toBool());
                break;
                case SendNow:
                    setSendNow(id, i.value().toBool());
                break;
                case TransferTypeString:
                    setTransferTypeString(id, i.value().toString());
                break;
				case TransferImage:
					setImageFromFilePath(id, i.value().toString());
				break;
                default:
                    qDebug() << "Invalid function key";
            }
        }
    }
}


void TUIService::visibilityChanged(bool value) {
    d_ptr->visibilityChanged(value);
    d_ptr->isInSwitcher = !value;        
    emitVisibilityState();
    sendSummary();
}

void TUIService::movedToSwitcher(bool value) {
    qDebug() << __FUNCTION__ << value;
    d_ptr->isInSwitcher = value;
    if(value == true) {
        sendSummary();
    }
}

void TUIService::clearCompletedTransfers() {
    d_ptr->clearCompletedList();
    sendSummary();
}

void TUIService::timeOrSettingsChanged (MeeGo::QmTimeWhatChanged what) {
    Q_UNUSED(what)
    d_ptr->completedProxyModel->dateSettingChanged();
}

QVariantMap TUIService::retrieveData(const QString &id) {
    qDebug() << __FUNCTION__ << id;
    QVariantMap keyValues;
    const TUIData *data = d_ptr->proxyModel->tuiData(id);
    if (data != 0) {
        QStringList errorInfo;

        //common attributes
        keyValues.insert("bytes",QVariant(data->bytes));
        keyValues.insert("currentFileIndex",QVariant(data->currentFileIndex));
        keyValues.insert("totalFiles",QVariant(data->filesCount));
        keyValues.insert("canPause",QVariant(data->canPause));
        keyValues.insert("canSendImmediatly",QVariant(data->canSendImdtly));

        keyValues.insert("name",QVariant(data->name));
        
        if((data->thumbnailFile.isEmpty() == false) 
            && (data->thumbnailMimeType.isEmpty() == false)) {
            QStringList thumbnailInfo;
            thumbnailInfo << data->thumbnailFile << data->thumbnailMimeType;
            keyValues.insert("thumbnailfile",QVariant(thumbnailInfo));
        }

        if((data->thumbnailMimeType.isEmpty() == true) 
            && (data->fileTypeIcon.isEmpty() == false)) {
            keyValues.insert("imagePath",QVariant(data->thumbnailFile));    
        }

        keyValues.insert("icon",QVariant(data->fileTypeIcon));
        keyValues.insert("target",QVariant(data->targetName));
        keyValues.insert("cancelButtonText",QVariant(data->cancelButtonText));
        keyValues.insert("transferTitle",QVariant(data->transferTitle));

        //status 
        keyValues.insert("transferStatus",QVariant(data->status));

        if((TransferStatusActive == data->status) || 
            (TransferStatusResume == data->status)) {
            keyValues.insert("estimate",QVariant(data->estimateTime));
            keyValues.insert("progress",QVariant(data->progressData));
        } else if(TransferStatusError == data->status) {
            errorInfo << data->headerMsg << data->detailMsg;
            if(data->canRepair == true) {
                errorInfo << data->actionName;
            }
        } else if(TransferStatusInactive == data->status) {
            keyValues.insert("message",QVariant(data->message));
        }
        keyValues.insert("errorInfo",QVariant(errorInfo));

    }
    qDebug() << __FUNCTION__ << keyValues.count();
    return keyValues;    
}

#ifdef _UNIT_TESTING_
TUIDataModelProxy *TUIService::model() const {
    return d_ptr->proxyModel;
}
#endif

/******************************************************************************/

TUIServicePrivate::TUIServicePrivate() : proxyModel(0) ,
    completedProxyModel(0), historySetting(0), readThread(0),
    interface(0), invisibleCompletedCount(0) {

    clientDataModel = 0;

    isUIShown = false;
    QString historyFilePath = QDir::homePath();
    historyFilePath.append(QLatin1String("/.transferui/"));
    historyFilePath.append(QLatin1String("completedlist"));
    historySetting = new QSettings(historyFilePath,QSettings::IniFormat,this);

    //create read thread
    qDebug() << __FUNCTION__ << "Creating Thread";
	readThread = new
		TUIReadHistoryThread(historySetting);
	
	connect(readThread, SIGNAL(addCompletedData(const
		TUIData*)),this, SLOT(dataReadFromDB(const TUIData*)),
		Qt::QueuedConnection);
	connect(readThread, SIGNAL(finished()), this, SLOT(threadCompleted()));
	connect(readThread, SIGNAL(terminated()), this, SLOT(threadCompleted()));

}

TUIServicePrivate::~TUIServicePrivate() {

    if(proxyModel!=0) {
        //clear datamodel
        delete proxyModel;
    }
    proxyModel = 0;

    if(clientDataModel!=NULL) {
        clientDataModel->clearList();
        delete clientDataModel;
    }
    clientDataModel = 0;

    if(completedProxyModel!=0) {
        //clear datamodel
        delete completedProxyModel;
    }
    completedProxyModel = 0;


	if(readThread != 0) {
		if(readThread->isRunning() == true) {
			readThread->quit();
		}
		delete readThread;
		readThread = 0;
	}

    if (historySetting != 0) {
        delete historySetting;
        historySetting = 0;
    }
    
    if(interface != 0) {
        delete interface;
    }

}

void TUIServicePrivate::visibilityChanged(bool value) {
    qDebug() << __FUNCTION__ << value;
    isUIShown = value;
    if(value == true) {
        proxyModel->displayTransfers();
        if(proxyModel->count() > 0) {
            interface->setActiveTransfersVisibility(true);
        }
        readHistory();
    } else {
        completedProxyModel->clearTransfers();
	    if(readThread->isRunning() == true) {
		    readThread->quit();
	    }
    }
}

void TUIServicePrivate::histroyModelEmpty(bool value) {
    qDebug() << __FUNCTION__ << value << isUIShown;
    if (interface != 0) {
        if (isUIShown == true) {
            interface->setHistoryVisibility(!value);
        }
    }
}


void TUIServicePrivate::displayInfoBanner(BannerEvent event, const QString& msg) {
    qDebug () << "Display Infobanner" << event << msg;
    if(interface != 0) {
        if(isUIShown == true) {
            interface->displayBanner(event,msg);
        }
    }
}

void TUIServicePrivate::notifyPartialDoneTransfers() {

    displayInfoBanner(ParitalDoneTransfer);
}

const QString TUIServicePrivate::generateTransferId() const {
    QString id = "transferui::";
    id.append(QDateTime::currentDateTime().toString(Qt::ISODate));
    id.append(QUuid::createUuid().toString());
    id.append("#");
    QString base_id = id;
    int index = 0;
    qDebug() << __FUNCTION__ << id;
    //sanity check if the transfer id is present in the transfer datamodel
    while(proxyModel->transferPresent(id)==true) {
        QString num;
        num.setNum(++index);
        id = base_id;
        id.append(num);
    }
    qDebug()<<"Transient transfer id"<< id;
    return id;
}

void TUIServicePrivate::registerTransferData(const QString& id,
    TransferType type, const QString& title,
    const QString& clientServiceName, const QString&
        serviceName) {

    proxyModel->registerTransfer(id, type, title, serviceName);


    if(clientServiceName.isEmpty() == false) { //might not be from dbus
        //for now autoCleanUp is always enabled
        clientDataModel->clientAdded(clientServiceName);

        if(clientDataModel->isAutoCleanUpEnabled(clientServiceName) == true) {
            qDebug() << "Auto cleanup is enabled - register service " <<
                clientServiceName;
            clientDataModel->insertTransfer(clientServiceName, id);
        } else {
            qDebug() << "Auto cleanup is disabled" << clientServiceName;
        }
    }

}

void TUIServicePrivate::cancelTransfer(const QModelIndex &index) {
    qDebug() << __FUNCTION__ << index;
    QVariant data = index.data();
    TUIData *tuiData = data.value<TUIData*>();
    if(tuiData != 0) {
        QString transferId = proxyModel->transferId(tuiData);
        qDebug() << __FUNCTION__ << transferId;
        proxyModel->waitToCancelled(transferId);
        Q_EMIT(cancel(transferId));
    }
}

void TUIServicePrivate::pauseTransfer(const QModelIndex &index) {
    qDebug() << __FUNCTION__ << index;
    QVariant data = index.data();
    TUIData *tuiData = data.value<TUIData*>();
    if(tuiData != 0) {
        QString transferId = proxyModel->transferId(tuiData);
	    qDebug() << __FUNCTION__ << transferId;
        Q_EMIT(pause(transferId));
    }
}

void TUIServicePrivate::resumeTransfer(const QModelIndex &index) {
    QVariant data = index.data();
    TUIData *tuiData = data.value<TUIData*>();
    if(tuiData != 0) {
        QString transferId = proxyModel->transferId(tuiData);
	    qDebug() << __FUNCTION__ << transferId;
        Q_EMIT(resume(transferId));
    }
}

void TUIServicePrivate::repairTransfer(const QModelIndex &index) {
    QVariant data = index.data();
    TUIData *tuiData = data.value<TUIData*>();
    if(tuiData != 0) {
        QString transferId = proxyModel->transferId(tuiData);
	    qDebug() << __FUNCTION__ << transferId;
        Q_EMIT(repair(transferId));
    }
}

void TUIServicePrivate::showCustomDialog(const TUIData *data,
	const QString& serviceName) {
	qDebug() << __FUNCTION__ << serviceName;
	QDBusInterface iface(serviceName, "/",
		"", QDBusConnection::sessionBus());
	if (iface.isValid()) {
		iface.asyncCall(QLatin1String("showDetailsDialog"),
		QVariant(proxyModel->transferId(data)));
	} else {
		qDebug() << __FUNCTION__ << iface.lastError ();
	}
}

void TUIServicePrivate::elementClicked(const QModelIndex &index) {
    qDebug() << __FUNCTION__ << index;
    QVariant data = index.data();
    TUIData *tuiData = data.value<TUIData*>();
    if(tuiData != 0) {
        if(TransferStatusError == tuiData->status) {
                interface->showErrorDetails(index);
        } else {
            if (tuiData->clientId.isEmpty() == false) {
                qDebug() << __FUNCTION__ << "Check for service Name";
			    QString attributeFilePath;
			    attributeFilePath.append(
				    QLatin1String("/usr/share/transfer-ui/clients/"));
			    attributeFilePath.append(tuiData->clientId);
			    QSettings settings(attributeFilePath,
				    QSettings::IniFormat);
			    settings.sync();
			    if(settings.contains(QLatin1String("DetailsDBusInterface"))) {
				    QString serviceName = settings.value(
					    QLatin1String("DetailsDBusInterface")).toString();
				    showCustomDialog(tuiData, serviceName);
			    } else {
				    interface->showDetails(index);
			    }
            } else {
                interface->showDetails(index);
            }
        }
    }
}

void TUIServicePrivate::completedElementClicked(const QModelIndex &index) {
    QVariant data = index.data();
    TUIData *tuiData = data.value<TUIData*>();
    if(tuiData != 0) {
        qDebug() << __FUNCTION__ << tuiData->resultUri;
        if (tuiData->resultUri.isEmpty() == false) {
            qDebug() << __FUNCTION__ << "Trying to launch application";
            Action action = Action::defaultAction(tuiData->resultUri);
            if (action.isValid() == true) {
                action.trigger();
            } else {
                qDebug() << __FUNCTION__ << "Failed to launch application" <<
                    "Displaying standard dialog";
                interface->showCompletedDetails(index);
            }
        } else {
            qDebug() << __FUNCTION__ << "Result Uri is Empty";
            interface->showCompletedDetails(index);
        }
    }
}

void TUIServicePrivate::clearCompletedList() {
    if(readThread != 0) {
        if(readThread->isRunning() == true) {
			readThread->quit();
		}
    }
    completedProxyModel->clearTransfers();
    historySetting->clear();
    historySetting->sync();
    if(isUIShown == true) {
        if(proxyModel->count() <= 0) {
            interface->setNoTransfersVisibility(true);
        }
    }
    invisibleCompletedCount = completedProxyModel->dataModel()->rowCount();
}

void TUIServicePrivate::threadCompleted() {

	qDebug() << __FUNCTION__;
    if(completedProxyModel->dataModel()->rowCount() > 0) {
        if(isUIShown == true) {
            interface->setHistoryVisibility(true);
        }
    }

}


void TUIServicePrivate::writeToHistory(const TUIData *data,
     bool showInHistory, const QString& replaceId) {
	if ((showInHistory == true) && (replaceId.isEmpty()==true)) {
		writeHistoryDB(data);
	} else {
		writeReplaceHistoryDB(data, replaceId);
	}

}

void TUIServicePrivate::readHistory() {
	readReplaceHistoryIds();
    readThread->start();
}

void TUIServicePrivate::dataReadFromDB(const TUIData *data) {
	completedProxyModel->addTransfer(data,true);
    invisibleCompletedCount = completedProxyModel->dataModel()->rowCount();
}

void TUIServicePrivate::readReplaceHistoryIds() {
	QString replaceIdPath = QDir::homePath();
    replaceIdPath.append(QLatin1String("/.transferui/"));
    replaceIdPath.append(QLatin1String("replaceIdList"));

	QSettings settings(replaceIdPath,QSettings::IniFormat);

	int count = settings.beginReadArray("ReplaceIds");
	qDebug() << __FUNCTION__ << count;
	for (int counter = 0; counter < count ; counter++) {
		settings.setArrayIndex(counter);
		QString replaceId = settings.value("replaceId").toString();
		int index = settings.value("index").toInt();
		replaceHistoryList.insert(replaceId, index);
	}
	settings.endArray();
}

void TUIServicePrivate::writeReplaceHistoryDB(const TUIData *data,
	const QString& replaceId) {
	int arrayIndex = 0;
	if (replaceHistoryList.contains(replaceId) == false) {
		historySetting->beginGroup("Indexes");
		arrayIndex = historySetting->value("ReplaceHistory").toInt();
		int nextIndex = arrayIndex + 1;
		historySetting->setValue("ReplaceHistory", nextIndex);
		historySetting->endGroup();
		replaceHistoryList.insert(replaceId,arrayIndex);
		writeReplaceHistoryIds(replaceId, arrayIndex);
	} else {
		arrayIndex = replaceHistoryList.value(replaceId);
		historySetting->beginWriteArray("ReplaceTransfers");
		historySetting->setArrayIndex(arrayIndex);
		historySetting->remove("");
		historySetting->endArray();
	}
    historySetting->beginWriteArray("ReplaceTransfers");
    historySetting->setArrayIndex(arrayIndex);
    historySetting->setValue("name", data->name);
    historySetting->setValue("size", data->bytes);
	historySetting->setValue("replaceId", replaceId);
    if(data->targetName.isEmpty() == false) {
        historySetting->setValue("target", data->targetName);
    }
    historySetting->setValue("type", data->method);
    historySetting->setValue("count", data->filesCount );
    historySetting->setValue("time", data->completedTime );
    if(data->thumbnailMimeType.isEmpty() == false) {
        historySetting->setValue("thumbnailfile", data->thumbnailFile );
        historySetting->setValue("mimetype", data->thumbnailMimeType);
    } else {
		if (data->fileTypeIcon.isEmpty() == true) {
            if(data->thumbnailFile.isEmpty() == false) {
    			historySetting->setValue("imagePath", data->thumbnailFile);
            }
		} else {
        	historySetting->setValue("iconId", data->fileTypeIcon);
		}
    }
    qDebug() << __FUNCTION__ << data->resultUri;
    historySetting->setValue("resulturi", data->resultUri);
    historySetting->endArray();
    historySetting->sync();
}

void TUIServicePrivate::writeHistoryDB(const TUIData *data) {
	int arrayIndex = 0;
	historySetting->beginGroup("Indexes");
	arrayIndex = historySetting->value("History").toInt();
	historySetting->endGroup();

    historySetting->beginWriteArray("Transfers");
    historySetting->setArrayIndex(arrayIndex);
    historySetting->setValue("name", data->name);
    historySetting->setValue("size", data->bytes);

    if(data->targetName.isEmpty() == false) {
        historySetting->setValue("target", data->targetName);
    }
    historySetting->setValue("type", data->method);
    historySetting->setValue("count", data->filesCount );
    historySetting->setValue("time", data->completedTime );
	qDebug() << __FUNCTION__ << data->completedTime;
    if(data->thumbnailMimeType.isEmpty() == false) {
        historySetting->setValue("thumbnailfile", data->thumbnailFile );
        historySetting->setValue("mimetype", data->thumbnailMimeType);
    } else {
		if (data->fileTypeIcon.isEmpty() == true) {
            if(data->thumbnailFile.isEmpty() == false) {
    			historySetting->setValue("imagePath", data->thumbnailFile);
            }
		} else {
        	historySetting->setValue("iconId", data->fileTypeIcon);
		}
    }
    historySetting->setValue("resulturi", data->resultUri);
    qDebug() << __FUNCTION__ << data->resultUri;
    historySetting->endArray();
	++arrayIndex;
	historySetting->beginGroup("Indexes");
	historySetting->setValue("History", arrayIndex);
	historySetting->endGroup();

    historySetting->sync();
}

void TUIServicePrivate::writeReplaceHistoryIds(const QString& replaceId,
	int index) {
	QString replaceIdPath = QDir::homePath();
    replaceIdPath.append(QLatin1String("/.transferui/"));
    replaceIdPath.append(QLatin1String("replaceIdList"));

	QSettings settings(replaceIdPath,QSettings::IniFormat);

	int arrayIndex = settings.beginReadArray("ReplaceIds");
	settings.endArray();
	settings.beginWriteArray("ReplaceIds");
	settings.setArrayIndex(arrayIndex);
	settings.setValue("replaceId", replaceId);
	settings.setValue("index", index);
	settings.endArray();
	settings.sync();
}
