
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








#ifndef _TUI_COMPLETED_LIST_PROXY_MODEL_H_
#define _TUI_COMPLETED_LIST_PROXY_MODEL_H_

//local headers
#include "tuicompletedlistmodel.h"
#include "tuistructures.h"
#include "tuicompletedlistsortproxy.h"

//Qt headers
#include <QObject>

/*!
    \class TUICompletedListProxyModel
    \brief proxy class to handle completed list. This class provides wrapper
    functionalities to add / remove / modify completed list.
    \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
*/
class TUICompletedListProxyModel : public QObject
{
    Q_OBJECT
public:
    TUICompletedListProxyModel(QObject *parent = 0);
    virtual ~TUICompletedListProxyModel();

    /*!
        return the data model
    */
    QAbstractItemModel * dataModel() const;

public Q_SLOTS:
    /*!
        add a new transfer to completed list
    */
    void addTransfer(const TUIData *data, bool readFromDB = false);

    /*!
        clear all transfers
    */
    void clearTransfers();

    /*!
        clear thumbnails from the model
    */
    void clearThumbnails();

    void dateSettingChanged();

Q_SIGNALS:
    /*!
        Signal emitted when the  model becomes empty or non-empty
        \param value , true if model is empty else false.

    */
	void modelEmpty(bool value);

private:
    TUICompletedListModel *model; //!< Completed list model

    TUICompletedListSortProxy *sortProxy; //!< Sort model for completed list

};
#endif