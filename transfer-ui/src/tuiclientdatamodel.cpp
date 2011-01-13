
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
#include "tuiclientdatamodel.h"

//Qt Headers
#include <QHash>
#include <QListIterator>

TUIClientDataModel::TUIClientDataModel(QObject *parent) :QObject(parent) {

}

TUIClientDataModel::~TUIClientDataModel() {
    qDebug() << "Deleting Client data model";
    tuiClientDataHashList.clear();
}

void TUIClientDataModel::insertTransfer(const QString& clientServiceName, const
    QString& transferId) {

    if(tuiClientDataHashList.contains(clientServiceName)) {
        qDebug() << "Service data model already has service. Adding a new" <<
            "transfer" << transferId << "for the service " <<
                clientServiceName ;
        tuiClientDataHashList[clientServiceName].transfers.append(transferId);
    } else {
        qDebug() << "Service not found in the Data model Adding a new Service"
            << clientServiceName << "with transfer " << transferId;
        TUIClientData data;
        data.transfers.append(transferId);
        tuiClientDataHashList.insert(clientServiceName,data);
    }

}

void TUIClientDataModel::removeTransfer(const QString& clientServiceName, const
    QString& transferId) {

    if(tuiClientDataHashList.contains(clientServiceName)) {
        qDebug() << "Removig transfer from the Service data model "
            << transferId << "for the service " << clientServiceName;
            tuiClientDataHashList[clientServiceName].transfers.removeOne
                (transferId);
    } else {
        qDebug () << "Service not present in the Service data model" <<
            clientServiceName;
    }
}

void TUIClientDataModel::clientRemoved(const QString& clientServiceName) {

    if(tuiClientDataHashList.contains(clientServiceName)) {
        qDebug() << "Deleting Service from the Service data model " <<
            clientServiceName;
        tuiClientDataHashList[clientServiceName].transfers.clear();
        tuiClientDataHashList.remove(clientServiceName);
    }
}

void TUIClientDataModel::clientAdded(const QString& clientServiceName) {

    //Add only if it is not present in the list
    if(!(tuiClientDataHashList.contains(clientServiceName))) {
        qDebug() << "New Client added " << clientServiceName;
        TUIClientData data;
        data.isAutoCleanUp = true;
        tuiClientDataHashList.insert(clientServiceName,data);
    } else {
        qDebug () << "Client name is already present " << clientServiceName;
    }
}

void TUIClientDataModel::clearList() {
    tuiClientDataHashList.clear();
}

bool TUIClientDataModel::isAutoCleanUpEnabled(const QString& clientServiceName)
    const {

    bool retVal = false;
    if(tuiClientDataHashList.contains(clientServiceName)) {
        retVal = tuiClientDataHashList.value(clientServiceName).isAutoCleanUp;
    }
    return retVal;
}

void TUIClientDataModel::setAutoCleanUp(const QString& clientServiceName, bool
    value) {

    if(tuiClientDataHashList.contains(clientServiceName)) {
        tuiClientDataHashList[clientServiceName].isAutoCleanUp = value;
    }
}

int TUIClientDataModel::transferCountForClient(const QString& clientServiceName)
    const {

    int retVal = 0;
    if(tuiClientDataHashList.contains(clientServiceName)) {
        retVal =
            tuiClientDataHashList[clientServiceName].transfers.count();
    }
    return retVal;
}

QListIterator<QString> TUIClientDataModel::transfersIterator(const QString&
    clientServiceName) const {

    QStringList dummy;
    QListIterator<QString> iter(dummy);
    if(tuiClientDataHashList.contains(clientServiceName)) {
        iter = (tuiClientDataHashList[clientServiceName].transfers);
    } else {

    }
    return iter;
}

bool TUIClientDataModel::isServiceRegistered(const QString& clientServiceName)
    const {
    return tuiClientDataHashList.contains(clientServiceName);
}