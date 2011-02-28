
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
#include <QDebug>

TUIDataModel::TUIDataModel(QObject *parent) : TUIAbstractModel(parent) {
    dataList.clear();
}

TUIDataModel::~TUIDataModel() {
    qDeleteAll(dataList);
    dataList.clear();
    qDebug() << __FUNCTION__ << "Deleting Data Model";
}

int TUIDataModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return dataList.count();
}

QVariant TUIDataModel::data(const QModelIndex &index, int role) const {
    QVariant retVar;
    TUIData *data = static_cast<TUIData*>(index.internalPointer());
    if(data != 0) {
        
        switch(role) {
            case TransferUI::ProgressRole:
                retVar = qVariantFromValue(data->progressData);
            break;
            case TransferUI::SizeRole:
                retVar = qVariantFromValue(data->bytes);
            break;
            case TransferUI::EstimateRole:
                retVar = qVariantFromValue(data->estimateTime);
            break;
            case TransferUI::CurrentFileIndexRole:
                retVar = qVariantFromValue(data->currentFileIndex);
            break;
            case TransferUI::TotalFileCountRole:
                retVar = qVariantFromValue((int)data->filesCount);
            break;
            case TransferUI::StatusRole:
                retVar = qVariantFromValue((int)data->status);
            break;
            case TransferUI::MethodRole:
                retVar = qVariantFromValue((int)data->method);
            break;
            case TransferUI::CanRepairRole:
                retVar = qVariantFromValue(data->canRepair);
            break;
            case TransferUI::CanPauseRole:
                retVar = qVariantFromValue(data->canPause);
            break;
            case TransferUI::CanSendImdltRole: 
                retVar = qVariantFromValue(data->canSendImdtly);
            break;
            case TransferUI::ShowInHistoryRole:
                retVar = qVariantFromValue(data->showInHistory);
            break;
            case TransferUI::NameRole:
                retVar = qVariantFromValue(data->name);
            break;
            case TransferUI::MessageRole:
                retVar = qVariantFromValue(data->message);
            break;
            case TransferUI::ErrorRole:
                {
                    QStringList errorDetails;
                    errorDetails << data->headerMsg << data->detailMsg;
                    if(data->canRepair == true) {
                        errorDetails << data->actionName;
                    }
                    retVar = qVariantFromValue(errorDetails);
                }
            break;
            case TransferUI::ThumbnailRole:
                {
                    QStringList thumbnailDetails;
                    thumbnailDetails << data->thumbnailFile;
                    thumbnailDetails << data->thumbnailMimeType;
                    retVar = qVariantFromValue(thumbnailDetails);
                }
            break;
            case TransferUI::FileIconRole:
                retVar = qVariantFromValue(data->fileTypeIcon);
            break;
            case TransferUI::TargetRole: 
                retVar = qVariantFromValue(data->targetName);
            break;
            case TransferUI::CancelTextRole:
                retVar = qVariantFromValue(data->cancelButtonText);
            break;
            case TransferUI::TransferTitleRole:
                retVar = qVariantFromValue(data->transferTitle);
            break;
            case TransferUI::ImageRole:
                retVar = qVariantFromValue(data->transferImage);
            break;
            case TransferUI::StartTimeRole:
                retVar = qVariantFromValue(data->startTime);
            break;
            case TransferUI::CompletedTimeRole:
                retVar = qVariantFromValue(data->completedTime);
            break;
            default:
                retVar = qVariantFromValue(data);
        }
    }
    return retVar;
}


bool TUIDataModel::setData(const QModelIndex &index,
    const QVariant &value, int role) {
    Q_UNUSED(role)
    bool retVal = false;
    if (index.isValid() == true) {

        TUIData *data = static_cast<TUIData*>(index.internalPointer());
        if(data == 0) {
            qDebug() << __FUNCTION__ << "InternalPointer returned 0";
            return false;
        }
        int row = dataList.indexOf(data);

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
                        data->transferImage = 0;
                    }
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
                        data->transferImage = 0;

                    }
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
        data = 0;
    }
    endRemoveRows();
    return true;
}

bool TUIDataModel::setItemData ( const QModelIndex & index,
    const QMap<int, QVariant> & roles ) {
    bool retVal = false;
    bool replaceData = true;
    if (index.isValid() == true) {

        TUIData *dataOriginal = static_cast<TUIData*>(index.internalPointer());
        if(dataOriginal == 0) {
            qDebug() << __FUNCTION__ << "InternalPointer returned 0";
            return false;
        }
        int row = dataList.indexOf(dataOriginal);
        QMap<int, QVariant>::const_iterator iter;
        for (iter = roles.constBegin(); iter != roles.constEnd(); ++iter) {
            switch(iter.key()) {
                case TransferUI::ProgressRole:
                    dataOriginal->progressData = iter.value().toDouble();
                break;
                case TransferUI::SizeRole:
                    dataOriginal->bytes = iter.value().toLongLong();
                break;
                case TransferUI::EstimateRole:
                    dataOriginal->estimateTime = iter.value().toInt();
                break;
                case TransferUI::CurrentFileIndexRole:
                    dataOriginal->currentFileIndex = iter.value().toInt();
                break;
                case TransferUI::TotalFileCountRole:
                    dataOriginal->filesCount = iter.value().toInt();
                break;
                case TransferUI::StatusRole:
                    dataOriginal->status = TransferStatus(iter.value().toInt());
                break;
                case TransferUI::MethodRole:
                    dataOriginal->method = TransferType(iter.value().toInt());
                break;
                case TransferUI::CanRepairRole:
                    dataOriginal->canRepair = iter.value().toBool();
                break;
                case TransferUI::CanPauseRole:
                    dataOriginal->canPause = iter.value().toBool();
                break;
                case TransferUI::CanSendImdltRole: 
                    dataOriginal->canSendImdtly = iter.value().toBool();
                break;
                case TransferUI::ShowInHistoryRole:
                    dataOriginal->showInHistory = iter.value().toBool();
                break;
                case TransferUI::NameRole:
                    dataOriginal->name = iter.value().toString();
                break;
                case TransferUI::MessageRole:
                    dataOriginal->message = iter.value().toString();
                break;
                case TransferUI::ErrorRole:
                    {
                        QStringList errorDetails = iter.value().toStringList();
                        dataOriginal->headerMsg = errorDetails[0];
                        dataOriginal->detailMsg = errorDetails[1];
                        if(errorDetails.count() > 2) {
                            dataOriginal->actionName = errorDetails[2];
                            dataOriginal->canRepair = true;
                        } else {
                            dataOriginal->canRepair = false;
                        }
                    }
                break;
                case TransferUI::ThumbnailRole:
                    {
                        QStringList thumbnailDetails = 
                            iter.value().toStringList();
                        dataOriginal->thumbnailFile = thumbnailDetails[0];
                        dataOriginal->thumbnailMimeType = thumbnailDetails[1];
                        dataOriginal->fileTypeIcon.clear();
                        if(dataOriginal->transferImage != 0) {
                            delete dataOriginal->transferImage;
                            dataOriginal->transferImage = 0;
                        }
                    }
                break;
                case TransferUI::FileIconRole:
                    dataOriginal->fileTypeIcon = iter.value().toString();
                break;
                case TransferUI::TargetRole: 
                    dataOriginal->targetName = iter.value().toString();
                break;
                case TransferUI::CancelTextRole:
                    dataOriginal->cancelButtonText = iter.value().toString();
                break;
                case TransferUI::TransferTitleRole:
                    dataOriginal->transferTitle = iter.value().toString();
                break;
                case TransferUI::ImageRole:
                    {
						if(dataOriginal->transferImage != 0) {
							delete dataOriginal->transferImage;
							dataOriginal->transferImage = 0;
						}
                        dataOriginal->transferImage = 
                            iter.value().value<QImage*>();
                        dataOriginal->fileTypeIcon.clear();
                    }
                break;
                case TransferUI::StartTimeRole:
                    dataOriginal->startTime = iter.value().toDateTime();
                break;
                case TransferUI::CompletedTimeRole:
                    dataOriginal->completedTime = iter.value().toDateTime();
                break;
                default:
                    qDebug() << iter.key() << "Updating for default Role";
                    QVariant dataVariant = roles.value(Qt::EditRole);
                    TUIData *data = dataVariant.value<TUIData *>();
                    replaceData = false;
                    dataList.replace(row, data);
            }
        }
        if(replaceData == true) {
            dataList.replace(row, dataOriginal);
        }
        retVal = true;
    }
    Q_EMIT(dataChanged(index, index));
    return retVal;
}

QMap<int, QVariant> TUIDataModel::itemData ( const QModelIndex & index ) const {
    QMap<int, QVariant> varMap;
    void *internalPointerData = index.internalPointer();
    
    TUIData *data = static_cast<TUIData*>(internalPointerData);

    if(data != 0) {
        
        varMap.insert(TransferUI::ProgressRole,
            qVariantFromValue(data->progressData));

        varMap.insert(TransferUI::SizeRole, 
            qVariantFromValue(data->bytes));

        varMap.insert(TransferUI::EstimateRole,
            qVariantFromValue(data->estimateTime));

        varMap.insert(TransferUI::CurrentFileIndexRole,
            qVariantFromValue(data->currentFileIndex));

        varMap.insert(TransferUI::TotalFileCountRole,
            qVariantFromValue(data->filesCount));

        varMap.insert(TransferUI::StatusRole,
            qVariantFromValue((int)data->status));

        varMap.insert(TransferUI::CanRepairRole,
            qVariantFromValue(data->canRepair));

        varMap.insert(TransferUI::CanPauseRole,
            qVariantFromValue(data->canPause));

        varMap.insert(TransferUI::CanSendImdltRole,
            qVariantFromValue(data->canSendImdtly));

        varMap.insert(TransferUI::ShowInHistoryRole,
            qVariantFromValue(data->showInHistory));

        varMap.insert(TransferUI::NameRole,
            qVariantFromValue(data->name));

        if(data->message.isEmpty() == false) {
            varMap.insert(TransferUI::MessageRole,
                qVariantFromValue(data->message));
        }

        if(data->targetName.isEmpty() == false) {
            varMap.insert(TransferUI::TargetRole,
                qVariantFromValue(data->targetName));
        }

        if(data->cancelButtonText.isEmpty() == false) {
            varMap.insert(TransferUI::CancelTextRole,
                qVariantFromValue(data->cancelButtonText));
        }
    
        if(data->transferTitle.isEmpty() == false) {
            varMap.insert(TransferUI::TransferTitleRole,
                qVariantFromValue(data->transferTitle));
        }
        
        if(data->transferImage != 0) {
            varMap.insert(TransferUI::ImageRole, 
                qVariantFromValue(data->transferImage));
        } else if(data->thumbnailMimeType.isEmpty() == false) {
            QStringList thumbnailDetails;
            thumbnailDetails << data->thumbnailMimeType << data->thumbnailFile;
            varMap.insert(TransferUI::ThumbnailRole, 
                qVariantFromValue(thumbnailDetails));
        } else {
            varMap.insert(TransferUI::FileIconRole,
                qVariantFromValue(data->fileTypeIcon));
        }
        if(TransferUI::TransferStatusError == data->status) {
            QStringList errorDetails;
            errorDetails << data->headerMsg << data->detailMsg;
            if(data->canRepair == true) {
                errorDetails << data->actionName;
            }
            varMap.insert(TransferUI::ErrorRole ,
                qVariantFromValue(errorDetails));
        }

        varMap.insert(TransferUI::StartTimeRole,
            qVariantFromValue(data->startTime));

        varMap.insert(TransferUI::CompletedTimeRole,
            qVariantFromValue(data->completedTime));

        varMap.insert(TransferUI::MethodRole,
            qVariantFromValue((int)data->method));
        
        varMap.insert(Qt::EditRole, qVariantFromValue(data));
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
//    if QSortFilterProxyModel is used as filter / sort prxy model,
//    QSortFilterProxyModel checks for the column count and asserts if the column
//    is more than total column count.
    return TotalColumns;
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

