
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








#ifndef _TUI_READ_HISTORY_THREAD_H_
#define _TUI_READ_HISTORY_THREAD_H_

#include "tuistructures.h"

#include <QThread>
#include <QSettings>

using namespace TransferUI;

/*!
    \class TUIReadHistoryThread
    \brief Thread class to read history transfers.
    \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
*/
class TUIReadHistoryThread : public QThread
{
	Q_OBJECT
public:
	TUIReadHistoryThread(QSettings *settings);

	virtual ~TUIReadHistoryThread();
     //! \reimp
	void run();
     //! \reimp_end
Q_SIGNALS:
    /*!
        \brief Signal emitted when a transfer from the history is read
        sucessfully.
        \param data data for the completed transfer.
    */
    void addCompletedData(const QString& id, QSharedPointer<TUIData>);

private:
    /*!
        \brief method to read replace history
    */
	void readReplaceHistory();

    /*!
        \brief method to read non replacing history
    */
	void readNonReplaceHistory();

    /*!
        \brief method to read history from the database.
    */
	void readHistoryFromDB(int arrayIndex);
private:
	QSettings *historySettings; //!< QSetting object to read completed transfers
};
#endif
