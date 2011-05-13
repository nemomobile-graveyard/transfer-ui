
/*
 * transfer-ui -- Handset UX Transfer user interface
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * Contact: Jukka Tiihonen <jukka.tiihonen@nokia.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */
 
/*!
  \file client.h
  \brief Client class that allows communication with Transfer UI
  \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
  \copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
  \license GNU Lesser General Public License, version 2.1
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to 
  deal in the Software without restriction, including without limitation the
  rights to use, copy, modify, merge, publish, distribute, sublicense,     
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:\n\n
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER  
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS  
  IN THE SOFTWARE.
*/

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <QObject>
#include <QString>
#include "export.h"

namespace TransferUI {
    class ClientPrivate;
    class Transfer;

    /*!
        \class Client
        \brief class for applications using transfer-ui
        \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
     */
    class TUI_EXPORT Client : public QObject
    {
        Q_OBJECT

    public:
        /*!
           \enum  TransferType
           \brief This is an enumeration of the kinds of transfers supported by
            TransferUI
        */
        enum TransferType {
            TRANSFER_TYPES_UPLOAD, //!< The transfer is an upload
            TRANSFER_TYPES_DOWNLOAD, //!< The transfer is a download
            TRANSFER_TYPES_SYNC, //!< The transfer is sync'ing
        };

        /*!
          \brief constructor
          \param parent
        */
        Client (QObject * parent = 0);

        virtual ~Client ();

        /*!
          \brief function to initialize the client library.
          \return true if initialization was success, other wise false
          \n
          \dontinclude tuiexample.cpp
          \skipline TUITestClient::initTUIClient()
          \until }
        */
        bool init();

        /*!
          \brief Ask TransferUI to send summary update. TransferUI emits
          back the summaryReport for this request.
          \return true if request was made successfully
        */
        bool askForSummaryUpdate ();

        /*!
          \brief Register a new transient transfer. This function registers a
          transfer which doesn't uses tracker. A unique identifier is generated
          by TransferUI and returned as Id. Id is used to control presentation
          of the transfer registered in TransferUI. The transfer details other
          than title and type will be set to default values. libtuiclient has a
          failsafe mechanism which re-populates the transfers in transfer-ui if
          transfer-ui crashes, hence when the transfer is completed or canceled,
          transfer should be removed from the client list using \ref
          removeTransfer. For the first registered transfer of this client ,
          transfer-ui emits its visibility state signals tuiOpened() /
          tuiClosed()
          \n Size will be set to zero
          \n Total file count will be set to 1
          \n State will be set to pending without any message
          \n Thumbnail is set to default thumbnail
          \n TargetName will be set to Null.
          \param name transfer name which will be presented to the user
          \param type transfer type , \ref TransferType enum
		  \param clientId client id of the client. This parameter is used get
		  special attributes for the client. clients should install config file
		  into /usr/share/transfer-ui/clients/&lt;clientid&gt; path. transfer-ui reads
		  the attributes from the given path. At present "DetailsDBusInterface" is
		  supported by transfer-ui which displays custom details dialog when
		  user clicks on the transfer.
          \return Transfer class you can use to update it's state and get
          signals. Owned by client.
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::registerTransfer()
          \until }
          \sa removeTransfer
        */
        Transfer *registerTransfer(const QString& name, TransferType type, const
        QString& clientId = QString());

        /*!
          \brief Ask transfer-ui to show it's UI
          \return true if request was send successfully
        */
        bool showUI();

        /*!
          \brief Remove the transfer whose id is passed from list of transfers.
          This does not remove transfers from transfer-ui. After removing from
          the list, transfer will not recieve any signals from the transfer-ui.
          \param  tid Id of the transfer to be removed.
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::removeTransfer()
          \until }
        */
        void removeTransfer (const QString& tid);

        /*!
          \brief set the progress value delta. This delta is applicable
          for all the transfers in the client. Changing default delta value
          will impact the performance. Delta values should be in the range of
          0.01 to 1. Other than those, delta values will be ignored
         \param delta modified delta value.
        */
        void setProgressValueDelta(double delta);

        /*!
            \brief set the progress time delta. This delta is applicable
            for all the transfers in the client. Changing default delta value
            will impact the performance.Delta value should be non-negative value
            and greater than 50 ms.
            \param delta modified delta value in milliseconds
        */
        void setProgressTimeDelta(int delta);

        /*!
            \brief get function for the progress value delta
            \return the progress value delata
        */
        double progressValueDelta() const;

        /*!
            \brief get function for the progress time delta
            \return the progress time delata
        */
        int progressTimeDelta() const;

        /*!
            \brief check if the transfer-ui is visible to the user
        */
        bool isTUIVisible() const;

    Q_SIGNALS:

        /*!
          \brief Signal emitted when some of transfers related to this client is
          asked to be cancelled. Transfer will emit signal first then it will be
          emited by client.
          \param transfer transfer that is asked to be cancelled
          \n \dontinclude tuiexample.cpp
          \skipline connect(client,SIGNAL(cancelTransfer
          \until )))
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::transferCancelled(
          \until }
          \sa Transfer::markCancelFailed Transfer::markCancelled
        */
        void cancelTransfer (Transfer * transfer);

        /*!
          \brief Signal emitted when some of transfers related to this client is
          asked to be started/continued. Transfer will emit signal first then it
          will be emited by client.
          \param transfer transfer that is asked to be started/continued
          \n \dontinclude tuiexample.cpp
          \skipline connect(client,SIGNAL(startTransfer
          \until )))
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::transferResumed(
          \until }
          \sa pauseTransfer
        */
        void startTransfer (Transfer * transfer);

        /*!
          \brief Signal emitted when some of transfers related to this client is
          asked to be paused. Transfer will emit signal first then it will be
          emited by client.
          \param transfer Transfer that is asked to be paused
          \n \dontinclude tuiexample.cpp
          \skipline connect(client,SIGNAL(pauseTransfer
          \until )))
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::transferPaused(
          \until }
          \sa startTransfer
        */
        void pauseTransfer (Transfer * transfer);

        /*!
          \brief Summary update of all transfers followed by transfer-ui
          \param failed number of failed transfers
          \param active number of active transfers
          \param pending number of pending transfers
          \param completed number of completed transfers currently presented by
                           Transfer UI
        */
        void summaryReport (int failed, int active, int pending, int completed);

        /*!
          \brief Signal emitted when error repair is requested
          \param transfer Transfer that is asked to be paused
          \n \dontinclude tuiexample.cpp
          \skipline connect(client,SIGNAL(repairErrorTransfer
          \until )))
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::transferErrorRepairRequested(
          \until }
          \sa Transfer::markRepairableFailure
        */
        void repairErrorTransfer(Transfer * transfer);

    private:
        ClientPrivate * const d_ptr;
    };
}

#endif
