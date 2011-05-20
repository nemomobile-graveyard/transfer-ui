
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







#ifndef _TUI_DATA_MODEL_
#define _TUI_DATA_MODEL_


#include "tuistructures.h"
#include "tuiabstractmodel.h"
#include "transferenums.h"

#include <QImage>

//if QSortFilterProxyModel is used as filter / sort prxy model,
//QSortFilterProxyModel checks for the column count and asserts if the column is
//more than total column count.
static const int TotalColumns = TotalStateChanges;

using namespace TransferUI;

/*!
    \class TUIDataModel
    \brief This class provides a data model for Transfer UI.
    \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
*/
class TUIDataModel : public TUIAbstractModel
{
    Q_OBJECT
public:
    TUIDataModel(QObject *parent=0);

    virtual ~TUIDataModel();

    //! \reimp
    int rowCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value,
        int role=Qt::EditRole);

    bool insertRows(int position, int rows,
        const QModelIndex &index=QModelIndex());

    bool removeRows(int position, int rows,
        const QModelIndex &index=QModelIndex());

    bool setItemData ( const QModelIndex & index,
        const QMap<int, QVariant> & roles );

    QMap<int, QVariant> itemData ( const QModelIndex & index ) const;

    QModelIndex index ( int row, int column,
        const QModelIndex& parent = QModelIndex()) const;

    QModelIndex parent(const QModelIndex &child) const;

    int columnCount(const QModelIndex &parent) const;
    //! \reimp_end


    /*!
        \brief get the row index for give tui data structure
        \param data tui data structure
    */
    int rowIndex(const TUIData *data) const;

    /*!
        \brief refresh the transfers in the data model. This emits datachanged
        signal for all the items present in the data model.
    */
    void refreshUIList();

    void clearCompletedMessages();

private:
    QList<TUIData*> dataList; //!< List of client data
};
Q_DECLARE_METATYPE(TUIData *)
//This is required for the QImage pointer
Q_DECLARE_METATYPE(QImage *)
#endif
