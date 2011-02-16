
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








#ifndef _TUI_SERVICE_PRIVATE_H_
#define _TUI_SERVICE_PRIVATE_H_

//local headers
#include "tuidatamodel.h"
#include "tuiclientdatamodel.h"
#include "tuidatamodelproxy.h"
#include "tuireadhistorythread.h"
#include "TransferUI/transferuiimplementationinterface.h"
#include "TransferUI/tuistructures.h"

//Qt headers
#include <QObject>
#include <QSettings>

class MLabel;
class MNotification;
class TUIController;
class TUIDataModel;

using namespace TransferUI;
/*!
    \class TUIServicePrivate
    \brief Private class for TUI Services
    \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
 */
class TUIServicePrivate : public QObject
{
    Q_OBJECT

public:
    bool isUIShown; //!< variable to track UI

    TUIDataModelProxy *proxyModel; //!< proxy data model


    QSettings *historySetting; //!< setting to write history data

	QMap<QString,int> replaceHistoryList;

	TUIReadHistoryThread *readThread;

    TransferUIImplementationInterface *interface;

    //! Data model to track list of clients
    TUIClientDataModel *clientDataModel;

    bool isInSwitcher; //!< variable to track if the application is switcher

public:
    TUIServicePrivate();

    virtual ~TUIServicePrivate();

    /*!
        \brief displays the info banner
        \param image Image ID for the banner
        \param body  Body text for the banner
    */
    void displayInfoBanner(BannerEvent event, const QString& message=QString());

    /*!
        \brief notify that the transfer is partially completed before cancel
        and display the info banner
    */
    void notifyPartialDoneTransfers();

    /*!
        \brief generates a unique identifier for the transfer
    */
    const QString generateTransferId() const;

    /*!
        \brief registers transfers to datamodel and controller
        \param id transfer identifier
        \param clientServiceName service for which transfer was registered
    */
    void registerTransferData(const QString& id, TransferType type,
        const QString& title, const QString& clientServiceName, const QString&
        serviceName);


    void writeToHistory(const QString& id, const TUIData *data, bool showInHistory,
		const QString& replaceId="");

    void readHistory();

    void writeHistoryData(const TUIData *data);
Q_SIGNALS:
    /*
        Signals
    */
    void cancel(const QString& transferId);
    void pause(const QString& transferId);
    void resume(const QString& transferId);
    void repair(const QString& transferId);

public Q_SLOTS:
    /*!
        \brief Function which gets the list of transfers and
        populates into the UI, this method is called from showUI
    */
    void visibilityChanged(bool value);

    void histroyModelEmpty(bool value);
    /*
        handle transfer states
    */
    void cancelTransfer(const QModelIndex &index);
    void pauseTransfer(const QModelIndex &index);
    void resumeTransfer(const QModelIndex &index);
    void repairTransfer(const QModelIndex &index);

    /*!
        \brief clear all the transfers from the completed list and database.
    */

    void clearCompletedList();

    /*!
        \brief history read thread completed
    */
	void threadCompleted();

    /*!
        \brief read completed data from the data base.
    */
	void dataReadFromDB(const QString& id, TUIData *data);

    /*!
        \brief show custom dialog. This function checks if the transfer is
        registered to display a custom transfer. If the transfer is registered
        then , showDetails method from the interface registerd is invoked.
        \param data transfer data
        \param serviceName service interface name.
    */

	void showCustomDialog(const TUIData *data, const QString& serviceName);

    /*!
        \brief user clicked the transfer element
        \param index index of the transfer
    */
    void elementClicked(const QModelIndex &index);

private:
    /*!
        \brief write replace history transfers to the data base
    */
	void writeReplaceHistoryDB(const QString& id, const TUIData *data, 
        const QString& replaceId);

    /*!
        \brief write history to the data base
    */

	void writeHistoryDB(const QString& id, const TUIData *data);

    /*!
        \brief read replace history ids from the data base
    */
	void readReplaceHistoryIds();

    /*!
        \brief write replace history ids to the data base
    */
	void writeReplaceHistoryIds(const QString& replaceId, int index);
};
#endif
