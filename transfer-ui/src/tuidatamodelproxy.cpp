
/*
 * This file is part of Handset UX Transfer user interface
 *
 * Copyright (C) 2010-2011 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
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
#include "tuidatamodelproxy.h"
#include "transferenums.h"
#include "tuidatamodel.h"

//Qt headers
#include <QObject>
#include <QStringList>
#include <QDateTime>
#include <QDebug>
#include <QFile>

TUIDataModelProxy::TUIDataModelProxy(QObject *parent) : QObject(parent) {
    model = new TUIDataModel(this);
}

TUIDataModelProxy::~TUIDataModelProxy() {
    if(model != 0) {
        delete model;
    }
    qDebug() << __FUNCTION__ << "Deleting Data Model Proxy";
}

TUIAbstractModel * TUIDataModelProxy::dataModel() const {
    return model;
}

void TUIDataModelProxy::registerTransfer(const QString& id, TransferType type,
    const QString& title, const QString& clientId, const QString& target, double
    bytes) {
    int rowNo = model->rowCount(QModelIndex());
    model->insertRows(rowNo,1);
    QModelIndex index = model->index(rowNo,Init);
    QMap<int, QVariant> mapVariant = model->itemData(index);
    QVariant data = mapVariant.value(Qt::EditRole);
    TUIData *tuiData = data.value<TUIData*>();
    if(tuiData != 0) {
        tuiData->name = title;
        tuiData->targetName = target;
        tuiData->bytes = bytes;
        tuiData->status = TransferStatusInactive;
        tuiData->message.clear();
        tuiData->method = type;
        tuiData->thumbnailMimeType.clear();
        tuiData->fileTypeIcon.clear();
		tuiData->clientId = clientId;
        tuiData->startTime = QDateTime::currentDateTime();
        QVariant variantData = qVariantFromValue(tuiData);
        QMap<int,QVariant> rolesVariant;
        rolesVariant.insert(Qt::EditRole, variantData);
        model->setItemData(index,rolesVariant);
        tuiDataMap.insert(id, tuiData);
    }
    QModelIndex newTransferAddedIndex = model->index(rowNo,NewTransferAdded);
    model->setData(newTransferAddedIndex,QVariant());
}

void TUIDataModelProxy::removeTransfer(const QString& id) {
    if(tuiDataMap.contains(id)) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        model->removeRows(rowNo,1);
        tuiDataMap.remove(id);
    }
}

void TUIDataModelProxy::displayTransfers() {
    model->refreshUIList();
}

QModelIndex TUIDataModelProxy::modelIndex(const QString& id) const {
    QModelIndex index;
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        index = model->index(rowNo,0);
    }
    return index;
}

/************************** Attribute changes *********************************/

void TUIDataModelProxy::setName(const QString &id, const QString &name) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Name);
        model->setData(index,name);
    }
}

void TUIDataModelProxy::setProgress(const QString &id, double done) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Progress);
        model->setData(index,done);
    }
}

void TUIDataModelProxy::setSize(const QString &id, qulonglong bytes) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Size);
        model->setData(index,bytes);
    }
}

void TUIDataModelProxy::setThumbnailForFile(const QString& id,
    const QString& fileName, const QString& mimeType) {

    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Thumbnail);
        QStringList param;
        param << fileName << mimeType;
        model->setData(index,param);
    }
}

void TUIDataModelProxy::setIcon(const QString& id, const QString& iconId) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Icon);
        model->setData(index,iconId);
    }
}

void TUIDataModelProxy::setCanPause(const QString &id, bool canPause) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,CanPause);
        model->setData(index,canPause);
    }
}

void TUIDataModelProxy::setCanSendNow(const QString &id, bool canSendNow) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,SendNow);
        model->setData(index,canSendNow);
    }
}

void TUIDataModelProxy::setEstimate(const QString &id, int seconds) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Estimate);
        model->setData(index,seconds);
    }
}

void TUIDataModelProxy::setFilesCount(const QString &id, int count) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,TotalCount);
        model->setData(index,count);
    }
}

void TUIDataModelProxy::setCurrentFileIndex(const QString &id, int fileIndex) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,CurrentFile);
        model->setData(index,fileIndex);
    }
}

void TUIDataModelProxy::setMessage(const QString &id, const QString &message) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Message);
        model->setData(index,message);
    }
}

void TUIDataModelProxy::setTransferType(const QString &id, int transferType) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Type);
        model->setData(index,transferType);
    }
}

void TUIDataModelProxy::setTargetName(const QString &id,
    const QString &targetName) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Target);
        model->setData(index,targetName);
    }
}

void TUIDataModelProxy::setCancelButtonText(const QString& id,
    const QString& text) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,CancelText);
        model->setData(index,text);
    }
}

void TUIDataModelProxy::setTransferTypeTitle(const QString& id,
    const QString& title) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,TransferTitle);
        model->setData(index,title);
    }
}

void TUIDataModelProxy::setImageFromFile(const QString& id, const QString&
	filePath) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,TransferImage);
        model->setData(index,filePath);
    }
}

/******************************State changes **********************************/
void TUIDataModelProxy::started(const QString &id, double done) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Active);
        TUIData *tuiData = data(index);
        if(tuiData != 0) {
            tuiData->status = TransferStatusActive;
            tuiData->progressData = done;
            tuiData->message.clear();
            setModelData(index, tuiData);
        }
    }
}

void TUIDataModelProxy::pending(const QString &id, const QString &message) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Inactive);
        TUIData *tuiData = data(index);
        if(tuiData != 0) {
            tuiData->status = TransferStatusInactive;
            tuiData->message = message;
            setModelData(index, tuiData);
        }
    }
}

void TUIDataModelProxy::paused(const QString &id) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Paused);
        TUIData *tuiData = data(index);
        if(tuiData != 0) {
            tuiData->status = TransferStatusPaused;
            setModelData(index, tuiData);
        }
    }
}

void TUIDataModelProxy::resumed(const QString &id) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Resumed);
        TUIData *tuiData = data(index);
        if(tuiData != 0) {
            tuiData->status = TransferStatusActive;
            tuiData->message.clear();
            setModelData(index, tuiData);
        }
    }
}

void TUIDataModelProxy::markError(const QString &id, const QString &headerMsg,
    const QString &detailMsg, bool canRepair, const QString &actionName) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Error);
        TUIData *tuiData = data(index);
        if(tuiData != 0) {
            tuiData->status = TransferStatusError;
            tuiData->headerMsg = headerMsg;
            tuiData->detailMsg = detailMsg;
            tuiData->actionName = actionName;
            tuiData->canRepair = canRepair;
            tuiData->message.clear();
            setModelData(index, tuiData);
        }
    }
}

void TUIDataModelProxy::cancelled(const QString &id) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Canceled);
        TUIData *tuiData = data(index);
        if(tuiData != 0) {
            tuiData->status = TransferStatusCanceled;
            setModelData(index, tuiData);
        }
    }
}

void TUIDataModelProxy::done(const QString &id, const QString& resultUri, 
    const QString& resultMimeType, bool removeWhenCleared) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Done);
        TUIData *tuiData = data(index);
        if(tuiData != 0) {
            tuiData->status = TransferStatusDone;
            tuiData->resultUri = resultUri;
            tuiData->completedTime = QDateTime::currentDateTime();
            tuiData->resultMimeType = resultMimeType;
            tuiData->removeWhenCleared = removeWhenCleared;
            tuiData->message.clear();
            setModelData(index, tuiData);
        }
    }
}

void TUIDataModelProxy::cleanUpTransfer(const QString &id) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,CleanUp);
        TUIData *tuiData = data(index);
        if(tuiData != 0) {
            tuiData->status = TransferStatusDone;
            tuiData->message.clear();
            setModelData(index, tuiData);
        }
    }
}

void TUIDataModelProxy::waitToCancelled(const QString& id) {
    if (tuiDataMap.contains(id) == true) {
        int rowNo = model->rowIndex(tuiDataMap.value(id));
        QModelIndex index = model->index(rowNo,Cancel);
        TUIData *tuiData = data(index);
        if(tuiData != 0) {
            if( tuiData->status != TransferStatusError) {
                tuiData->status = TransferStatusCancel;
                setModelData(index, tuiData);
            }
        }
    }
}

QString TUIDataModelProxy::transferId(const TUIData* data) const {
    QString retString = tuiDataMap.key(data);
    return retString;
}

bool TUIDataModelProxy::transferPresent(const QString& id) {
    bool retVal = false;
    if(tuiDataMap.contains(id)) {
        retVal = true;
    }
    return retVal;
}

const TUIData * TUIDataModelProxy::tuiData(const QString& id) const {
    const TUIData *data = 0;
    if (tuiDataMap.contains(id)) {
        data = tuiDataMap.value(id);
    }
    return data;
}

TUIData* TUIDataModelProxy::data(const QModelIndex& index) {
    TUIData *tuiData = 0;
    QMap<int, QVariant> mapVariant = model->itemData(index);
    QVariant dataVariant = mapVariant.value(Qt::EditRole);
    if(dataVariant.isValid()) {
        tuiData = dataVariant.value<TUIData*>();
    }
    return tuiData;
}

void TUIDataModelProxy::getTransfersCount(int& activeCount ,
    int& inactiveCount, int& errorCount) {
    activeCount = 0;
    inactiveCount = 0;
    errorCount = 0;
    TransferStatus status;
    QMap<QString, const TUIData*>::const_iterator iter
                = tuiDataMap.constBegin();
    while (iter != tuiDataMap.constEnd()) {

        status = iter.value()->status;
        //error count
        if(TransferStatusError==status) {
                ++errorCount;
        }
        //inactive count
        else if((TransferStatusInactive==status)
            ||(TransferStatusPaused==status)) {
                ++inactiveCount;
        }
        //active count
        else if((TransferStatusActive==status)
            || (TransferStatusResume==status)) {
                ++activeCount;
        }
        ++iter;
    }
}

void TUIDataModelProxy::getTransfersCount(int& activeCount, int& inactiveCount,
    int& errorCount, int& completedCount) {
    activeCount = 0;
    inactiveCount = 0;
    errorCount = 0;
    completedCount = 0;
    TransferStatus status;
    QMap<QString, const TUIData*>::const_iterator iter
                = tuiDataMap.constBegin();
    while (iter != tuiDataMap.constEnd()) {

        status = iter.value()->status;
        //error count
        if(TransferStatusError==status) {
            ++errorCount;
        } else if((TransferStatusInactive==status) //inactive count
            ||(TransferStatusPaused==status)) {
            ++inactiveCount;
        } else if((TransferStatusActive==status) //active count
            || (TransferStatusResume==status)) {
            ++activeCount;
        } else if(TransferStatusDone==status) {
            ++completedCount;            
        }
        ++iter;
    }    
}

int TUIDataModelProxy::completedCount() const {
    int completedTransfersCount = 0;
    TransferStatus status;
    QMap<QString, const TUIData*>::const_iterator iter
                = tuiDataMap.constBegin();
    while (iter != tuiDataMap.constEnd()) {
        status = iter.value()->status;
        if(TransferStatusDone==status) {
            ++completedTransfersCount;            
        }
        ++iter;
    }
    return completedTransfersCount;
}

void TUIDataModelProxy::clearCompletedTransfers() {
    QMap<QString, const TUIData*>::const_iterator iter
                = tuiDataMap.constBegin();
    TransferStatus status;
    while (iter != tuiDataMap.constEnd()) {    
        const TUIData *data = iter.value();
        status = data->status;

        if(TransferStatusDone==status) {
//TODO Decision is not yet made on how to handle the temp files. This part
//deletes the file from the system when user clears the transfers from TUI

            bool tempInfoProvided = data->removeWhenCleared;
            qDebug() << __FUNCTION__ << "Clear when completed" << tempInfoProvided;
            if(tempInfoProvided == true) {
                if(QFile::remove(data->resultUri) == true) {
                    qDebug() << __FUNCTION__ << "File Removed from the system"
                        << data->resultUri;
                } else {
                    qWarning() << __FUNCTION__ << "File could not be removed" <<
                        data->resultUri;
                }
            }
            
            removeTransfer(iter.key());          
        }
        ++iter;
    }
}

void TUIDataModelProxy::addTransfer(const QString& id, QSharedPointer<TUIData> data) {
    int rowNo = model->rowCount(QModelIndex());
    model->insertRows(rowNo,1);
    QModelIndex index = model->index(rowNo,Init);
    QMap<int, QVariant> mapVariant = model->itemData(index);
    QVariant variantData = mapVariant.value(Qt::EditRole);
    TUIData *tuiData = variantData.value<TUIData*>();
    if(tuiData != 0) {
        tuiData->name = data->name;
        tuiData->targetName = data->targetName;
        tuiData->bytes = data->bytes;
        tuiData->status = TransferStatusDone;
        tuiData->message = QString();
        tuiData->method = data->method;
        tuiData->thumbnailFile = data->thumbnailFile;
        tuiData->thumbnailMimeType = data->thumbnailMimeType;
        tuiData->thumbnailImagePath = data->thumbnailImagePath;
        tuiData->completedTime = data->completedTime;
        tuiData->fileTypeIcon = data->fileTypeIcon;
        tuiData->resultUri = data->resultUri;
        tuiData->resultMimeType = data->resultMimeType;
        tuiData->removeWhenCleared = data->removeWhenCleared;
        tuiData->startTime = data->startTime;
        tuiData->transferTitle = data->transferTitle;
        QVariant variantData = qVariantFromValue(tuiData);
        QMap<int,QVariant> rolesVariant;
        rolesVariant.insert(Qt::EditRole, variantData);
        model->setItemData(index,rolesVariant);
        tuiDataMap.insert(id, tuiData);
    }
    data.clear();
    QModelIndex newTransferAddedIndex = model->index(rowNo,NewTransferAdded);
    model->setData(newTransferAddedIndex,QVariant());
}

void TUIDataModelProxy::dateSettingsChanged() {
    model->clearCompletedMessages();
}


int TUIDataModelProxy::count() const {
    return tuiDataMap.count();
}

void TUIDataModelProxy::setModelData(const QModelIndex& index, TUIData* data) {
    QVariant variantData = qVariantFromValue(data);
    QMap<int,QVariant> rolesVariant;
    rolesVariant.insert(Qt::EditRole, variantData);
    model->setItemData(index,rolesVariant);
}
