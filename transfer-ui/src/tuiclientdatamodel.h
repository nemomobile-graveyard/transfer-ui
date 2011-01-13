
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







#ifndef _TUI_CLIENT_DATA_MODEL_H_
#define _TUI_CLIENT_DATA_MODEL_H_

//local headers
#include "tuistructures.h"

//Qt headers
#include <QObject>
#include <QHash>
#include <QListIterator>

using namespace TransferUI;

/*!
    \class TUIClientData
    \brief Strcuture for client details.
    \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
*/
struct TUIClientData
{
    TUIClientData() : transfers(0) , isAutoCleanUp(true) {
    }
    QStringList transfers; //!< List of transfer for this client
    bool isAutoCleanUp; //!< is auto clean up enabled
};

/*!
    \class TUIClientDataModel
    \brief Class for client details. Holds the client service name and the
    transfers list for the client. When the client crashes, these data helps to
    remove unattended transfers from the client.
    \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
*/
class TUIClientDataModel : public QObject
{
    Q_OBJECT

public:
    /*!
        \brief constructor
        \param parent
    */
    TUIClientDataModel(QObject *parent=0);

    virtual ~TUIClientDataModel();

    /*!
        \brief A new client added. Keep track of the client
        \param clientServiceName service name of the client
    */
    void clientAdded(const QString& clientServiceName);

    /*!
        \brief a new transfer is added from the client
        \param clientServiceName service name of the client
        \param transferId transfer identifier
    */
    void insertTransfer(const QString& clientServiceName, const QString&
        transferId);

    /*!
        \brief remove the transfer
        \param clientServiceName service name of the client
        \param transferId transfer identifier
    */
    void removeTransfer(const QString& clientServiceName, const QString&
        transferId);

    /*!
        \brief client closed or crashed. Remove from the list
        \param clientServiceName service name of the client
    */
    void clientRemoved(const QString& clientServiceName);

    /*!
        \brief clear the list
    */
    void clearList();

    /*!
        \brief check for auto clean is enabled or not
        \param clientServiceName service name of the client
    */
    bool isAutoCleanUpEnabled(const QString& clientServiceName) const;

    /*!
        \brief set auto clean up enable / disable
        \param clientServiceName service name of the client
        \param value true or false
    */
    void setAutoCleanUp(const QString& clientServiceName, bool value);

    /*!
        \brief get the transfers count for the client
        \param clientServiceName service name of the client
        \return no of transfers for the client
    */
    int transferCountForClient(const QString& clientServiceName) const;

    /*!
        \brief get the iterator for the transfers list
        \param clientServiceName service name of the client
        \return iterator
    */
    QListIterator<QString> transfersIterator(const QString& clientServiceName) const;

    /*!
        \brief get if the service is already registered
    */
    bool isServiceRegistered(const QString& clientServiceName) const;

private:
    QHash<QString,TUIClientData> tuiClientDataHashList; //!<  Hash list

//    bool isAutoCleanUp; //!< is Auto clean up enabled
};

#endif