
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
#include "tuicompletedlistmodel.h"
#include "tuidatamodel.h"

#include <QVariant>
#include <QModelIndex>

TUICompletedListModel::TUICompletedListModel(QObject *parent)
    : TUIAbstractModel(parent) {

    qDeleteAll(completedList);
}

TUICompletedListModel::~TUICompletedListModel() {
    qDebug() << __FUNCTION__ << "Deleting Completed List Model";
    qDeleteAll(completedList);
}


int TUICompletedListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return completedList.count();
}

bool TUICompletedListModel::setData(const QModelIndex &index,
    const QVariant &value,  int role) {
    Q_UNUSED(role)
    if (index.isValid() == true) {
        TUIData *dataOriginal = static_cast<TUIData*>(index.internalPointer());
        int row = completedList.indexOf(dataOriginal);
        qDebug() << __FUNCTION__ << "Index of data in the completed list" << row;
        TUIData *data = value.value<TUIData*>();
        completedList.replace(row, data);
        Q_EMIT(dataChanged(index, index));
    }
    return true;
}

bool TUICompletedListModel::insertRows(int position, int rows,
    const QModelIndex &index) {
    beginInsertRows(index, position, position+rows-1);
    for (int row=0; row < rows; row++) {
        TUIData *data = new TUIData();
        qDebug() << __FUNCTION__ << data->transferImage;
        completedList.append(data);
    }
    endInsertRows();
	Q_EMIT(modelEmpty(false));
    return true;
}

bool TUICompletedListModel::removeRows(int position, int rows,
    const QModelIndex &index) {
    beginRemoveRows(index, position, position+rows-1);
    for (int row=0; row < rows; ++row) {
        TUIData *data = completedList.value(position);
        delete data;
        completedList.removeAt(position);

    }
    endRemoveRows();
	if (completedList.count() <= 0) {
		Q_EMIT(modelEmpty(true));
	}
    return true;
}

bool TUICompletedListModel::setItemData ( const QModelIndex & index,
    const QMap<int, QVariant> & roles ) {
    bool retVal = false;
    if (index.isValid() == true) {
        TUIData *dataOriginal = static_cast<TUIData*>(index.internalPointer());
        int row = completedList.indexOf(dataOriginal);
        qDebug() << __FUNCTION__ << "Index of data in the completed list" << row;
        QVariant dataVariant = roles.value(Qt::EditRole);
        TUIData *data = dataVariant.value<TUIData *>();
        completedList.replace(row, data);
        Q_EMIT(dataChanged(index, index));
        retVal = true;
    }
    return retVal;
}

QMap<int, QVariant> TUICompletedListModel::
    itemData( const QModelIndex & index ) const {

    QMap<int, QVariant> varMap;
    void *internalPointerData = index.internalPointer();
    
    TUIData *data = static_cast<TUIData*>(internalPointerData);
    qDebug() << __FUNCTION__ << "Internal Pointer data" << internalPointerData <<
        "completed tui data" << data;
    if(data != 0) {
        varMap.insert(Qt::EditRole ,qVariantFromValue(data));
    }
    return varMap;
}

QModelIndex TUICompletedListModel::index ( int row, int column,
    const QModelIndex& parent) const {
    Q_UNUSED(parent)
    QModelIndex retIndex;
    if (row <= completedList.count()) {
        retIndex =  createIndex(row,column, completedList.at(row));
    } else {
        retIndex = createIndex(row,column);
    }
    return retIndex;
}

QVariant TUICompletedListModel::data(const QModelIndex &index, int role) const {
    Q_UNUSED(role)
    TUIData *data = static_cast<TUIData*>(index.internalPointer());
    qDebug() << __FUNCTION__ << "Returning Completed data" << data;
    return qVariantFromValue(data);
}

QModelIndex TUICompletedListModel::parent(const QModelIndex &child) const {
    Q_UNUSED(child)
    return QModelIndex();
}

int TUICompletedListModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}


void TUICompletedListModel::thumbnailReady(const QModelIndex& index,
    const QImage *image) {
    TUIData *data = completedList.value(index.row());
    data->transferImage = const_cast<QImage*>(image);
    Q_EMIT(dataChanged(index,index));
}

void TUICompletedListModel::clearThumbnails() {
    QList<TUIData*>::const_iterator iter;
    for (iter = completedList.constBegin(); 
        iter != completedList.constEnd(); ++iter) {
        if ((*iter)->transferImage != 0) {
            delete (*iter)->transferImage;
            (*iter)->transferImage = 0;
        }
    }
}

void TUICompletedListModel::clearMessages() {
    QList<TUIData*>::const_iterator iter;
    for (iter = completedList.constBegin(); 
        iter != completedList.constEnd(); ++iter) {
        TUIData *tuiData = (*iter);
        tuiData->message.clear();
        int rowNo = completedList.indexOf(tuiData);
        QModelIndex changedIndex = index(rowNo,0);
        Q_EMIT(dataChanged(changedIndex, changedIndex));
    }
}
