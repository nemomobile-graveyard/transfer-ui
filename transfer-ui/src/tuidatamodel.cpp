
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








#include "tuidatamodel.h"
#include "transferenums.h"

#include <QStringList>
#include <QObject>

#include <QImage>
#include <QFileInfo>


TUIDataModel::TUIDataModel(QObject *parent) : TUIAbstractModel(parent) {
    dataList.clear();
}

TUIDataModel::~TUIDataModel() {
    dataList.clear();
    qDebug() << __FUNCTION__ << "Deleting Data Model";
}

int TUIDataModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return dataList.count();
}

QVariant TUIDataModel::data(const QModelIndex &index, int role) const {
    Q_UNUSED(role)
    TUIData *data = static_cast<TUIData*>(index.internalPointer());
    qDebug() << __FUNCTION__ << "Returning data" << data;
    return qVariantFromValue(data);
}

bool TUIDataModel::setData(const QModelIndex &index,
    const QVariant &value, int role) {

    Q_UNUSED(role)
    bool retVal = false;
    if (index.isValid() == true) {

        TUIData *data = static_cast<TUIData*>(index.internalPointer());
        int row = dataList.indexOf(data);
        qDebug() << __FUNCTION__ << "Index of data in the list" << row;

        switch (index.column()){
            case Name:
                data->name = value.toString();
            break;
            case Progress:
                data->progressData = value.toDouble();
            break;
            case Icon:
                {
                    data->fileTypeIcon = value.toString();
                    data->thumbnailFile.clear();
                    data->thumbnailMimeType.clear();
                    if(data->transferImage != 0) {
                        delete data->transferImage;
                    }
                    data->transferImage = 0;
                }
            break;
            case Size:
                data->bytes = value.toLongLong();
            break;
            case Thumbnail:
                {
                    QStringList param = value.toStringList();
                    data->thumbnailFile = param.at(0);
                    data->thumbnailMimeType = param.at(1);
                    data->fileTypeIcon.clear();
                    if(data->transferImage != 0) {
                        delete data->transferImage;
                    }
                    data->transferImage = 0;
                }
            break;
            case CanPause:
                data->canPause = value.toBool();
            break;
            case SendNow:
                data->canSendImdtly = value.toBool();
            break;
            case Estimate:
                data->estimateTime = value.toInt();
            break;
            case TotalCount:
                data->filesCount = value.toInt();
            break;
            case CurrentFile:
                data->currentFileIndex = value.toInt();
            break;
            case Message:
                data->message = value.toString();
            break;
            case Type:
                data->method = (TransferType)(value.toInt());
            break;
            case Target:
                data->targetName = value.toString();
            break;
            case CancelText:
                data->cancelButtonText = value.toString();
            break;
            case TransferTitle:
                data->transferTitle = value.toString();
            break;
			case TransferImage:
				{
					QString fileName = value.toString();
					QFileInfo fInfo(fileName);
					if(fInfo.exists() == true) {

						data->transferImage = new QImage(fileName);
						data->thumbnailMimeType.clear();
						data->fileTypeIcon.clear();
						data->thumbnailFile = fileName;
					}
				}
			break;
        }
        dataList.replace(row, data);
        Q_EMIT(dataChanged(index, index));
        retVal = true;
    }
    return retVal;
}

bool TUIDataModel::insertRows(int position, int rows,
    const QModelIndex &index) {

    beginInsertRows(index, position, position+rows-1);
    for (int row=0; row < rows; row++) {
        TUIData *data = new TUIData();
        dataList.append(data);
    }
    endInsertRows();
    return true;
}

bool TUIDataModel::removeRows(int position, int rows, const QModelIndex &index) {
    beginRemoveRows(index, position, position+rows-1);
    for (int row=0; row < rows; ++row) {
        TUIData *data = dataList.value(position);
        if (data->transferImage != 0) {
            delete data->transferImage;
            data->transferImage = 0;
        }
        delete data;
        dataList.removeAt(position);

    }
    endRemoveRows();
    return true;
}

bool TUIDataModel::setItemData ( const QModelIndex & index,
    const QMap<int, QVariant> & roles ) {
    bool retVal = false;
    if (index.isValid() == true) {
        TUIData *dataOriginal = static_cast<TUIData*>(index.internalPointer());
        int row = dataList.indexOf(dataOriginal);
        qDebug() << __FUNCTION__ << "Index of data in the list" << row;
        QVariant dataVariant = roles.value(Qt::EditRole);
        TUIData *data = dataVariant.value<TUIData *>();
        dataList.replace(row, data);
        Q_EMIT(dataChanged(index, index));
        retVal = true;
    }
    return retVal;
}

QMap<int, QVariant> TUIDataModel::itemData ( const QModelIndex & index ) const {
    QMap<int, QVariant> varMap;
    void *internalPointerData = index.internalPointer();
    
    TUIData *data = static_cast<TUIData*>(internalPointerData);
    qDebug() << __FUNCTION__ << "Internal Pointer data" << internalPointerData <<
        "tui data" << data;
    if(data != 0) {
        varMap.insert(Qt::EditRole ,qVariantFromValue(data));
    }
    return varMap;
}

int TUIDataModel::rowIndex(const TUIData *data) const {
    return dataList.indexOf(const_cast<TUIData *>(data));
}

QModelIndex TUIDataModel::index ( int row, int column,
    const QModelIndex & parent) const {
    Q_UNUSED(parent)
    QModelIndex retIndex;
    if (row <= dataList.count()) {
        retIndex =  createIndex(row,column, dataList.at(row));
    } else {
        retIndex = createIndex(row,column);
    }
    return retIndex;
}

void TUIDataModel::refreshUIList() {
    QList<TUIData*>::const_iterator iter;
    for (iter = dataList.constBegin(); iter != dataList.constEnd(); ++iter) {
        int rowNo = rowIndex(*iter);
        QModelIndex modelIndex = index(rowNo, Init);
        Q_EMIT(dataChanged(modelIndex, modelIndex));
    }
}

QModelIndex TUIDataModel::parent(const QModelIndex &child) const {
    Q_UNUSED(child)
    return QModelIndex();
}

int TUIDataModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 1;
}

void TUIDataModel::clearCompletedMessages() {
    QList<TUIData*>::const_iterator iter;
    for (iter = dataList.constBegin(); 
        iter != dataList.constEnd(); ++iter) {
        TUIData *tuiData = (*iter);
        if(TransferStatusDone == tuiData->status) {
            tuiData->message.clear();
            int rowNo = dataList.indexOf(tuiData);
            QModelIndex changedIndex = index(rowNo,0);
            Q_EMIT(dataChanged(changedIndex, changedIndex));
        }
    }
}

