#ifndef _TUI_ABSTRACT_MODEL_H_
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







#define _TUI_ABSTRACT_MODEL_H_
#include <QAbstractItemModel>

/*!
    \class TUIAbstractModel
    \brief Abstract class for transfer-ui data model.
    \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
*/
class TUIAbstractModel : public QAbstractItemModel
{
public:
    TUIAbstractModel(QObject *parent=0);

    virtual ~TUIAbstractModel();

    virtual void thumbnailReady(const QModelIndex& index,
        const QImage* image) = 0;

    //! \reimp
    int columnCount(const QModelIndex &parent) const = 0;
    QModelIndex parent(const QModelIndex &child) const = 0;
    QVariant data(const QModelIndex &index,
        int role = Qt::DisplayRole) const = 0;
    QModelIndex index ( int row, int column,
        const QModelIndex& parent = QModelIndex()) const = 0;

    //! \reimp_end

};
#endif