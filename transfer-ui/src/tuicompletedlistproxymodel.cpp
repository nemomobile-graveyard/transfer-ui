
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








// local headers
#include "tuicompletedlistproxymodel.h"
#include "tuicompletedlistmodel.h"
#include "tuidatamodel.h"
#include "tuicompletedlistsortproxy.h"
#include "transferenums.h"

//Qt Headers
#include <QDateTime>
#include <QDebug>


TUICompletedListProxyModel::TUICompletedListProxyModel(QObject *parent) :
    QObject(parent) {

    model = new TUICompletedListModel(this);
    sortProxy = new TUICompletedListSortProxy(this);
    sortProxy->setSourceModel(model);
	connect(model, SIGNAL(modelEmpty(bool)),this,SIGNAL(modelEmpty(bool)));
}

TUICompletedListProxyModel::~TUICompletedListProxyModel() {
}

void TUICompletedListProxyModel::addTransfer(const TUIData *data,
	bool readFromDB) {

    int rowNo = model->rowCount(QModelIndex());
    model->insertRows(rowNo,1);
    QModelIndex index = model->index(rowNo,0);
    QMap<int, QVariant> mapVariant = model->itemData(index);
    QVariant dataVar = mapVariant.value(Qt::EditRole);

    qDebug() << __FUNCTION__ << data->completedTime;
    TUIData *tuiData = dataVar.value<TUIData*>();
    if(tuiData != 0) {
        tuiData->method = data->method;
        tuiData->name = data->name;
        tuiData->bytes = data->bytes;
        tuiData->thumbnailMimeType = data->thumbnailMimeType;
        tuiData->thumbnailFile = data->thumbnailFile;
        tuiData->targetName = data->targetName;
        tuiData->fileTypeIcon = data->fileTypeIcon;
        tuiData->completedTime = data->completedTime;
        tuiData->message.clear();
        if(data->filesCount > 1) {
            tuiData->filesCount = data->filesCount;
            tuiData->currentFileIndex = data->filesCount;
        }

        tuiData->transferImage = 0;
		if ((tuiData->thumbnailMimeType.isEmpty() == true) &&
			(tuiData->fileTypeIcon.isEmpty() == true)) {
            if(tuiData->thumbnailFile.isEmpty() == false) {
    			tuiData->transferImage = new QImage(tuiData->thumbnailFile);
                tuiData->thumbnailFile.clear(); 
            }
		}
        tuiData->resultUri = data->resultUri;
        QVariant variantData = qVariantFromValue(tuiData);
        QMap<int,QVariant> rolesVariant;
        rolesVariant.insert(Qt::EditRole, variantData);
        model->setItemData(index,rolesVariant);
        sortProxy->sort(0);
    }
	if(readFromDB == true) {
		delete data;
	}
}


void TUICompletedListProxyModel::clearTransfers() {
	if(model->rowCount(QModelIndex()) > 0) {
		clearThumbnails();
		model->removeRows(0,model->rowCount(QModelIndex()));
		sortProxy->removeRows(0, model->rowCount(QModelIndex()));
	}
}

QAbstractItemModel * TUICompletedListProxyModel:: dataModel() const {
    return sortProxy;
}

void TUICompletedListProxyModel::clearThumbnails() {
    model->clearThumbnails();
}

void TUICompletedListProxyModel::dateSettingChanged() {
    model->clearMessages();
}