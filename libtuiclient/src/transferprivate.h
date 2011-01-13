
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

#ifndef _TUI_TRANSFER_PRIVATE_H_
#define _TUI_TRANSFER_PRIVATE_H_

#include <QString>
#include "transferui_interface.h"
#include "transferenums.h"
#include "tuistructures.h"

//forward decl
class QTime;

namespace TransferUI {
    //Forward decl
    class Client;
    class TransferPrivate : public QObject
    {
        Q_OBJECT
        public:
            TransferPrivate(Client *client);

            QString id;
            QString errorMessage;
            ComMeegoTransferuiInterface * interface;

            TUIData data;

            //! Last progress sent time
            QTime lastProgressSentTime;

            //! client pointer which is passed as parent in the constructor
            Client * const clientPtr;

        public Q_SLOTS:

            /*!
                send the progress can be sent to transfer-ui
            */
            void sendProgress(double valueDelta, int timeDelta, double done);

            /*!
                is wait for commit flag set
            */
            bool isWaitForCommitSet() const;

            /*!
                set wait for commit
            */
            void waitForCommit();

            /*!
                commit the functions
            */
            bool commit();

            /*!
                add function to commit queue
            */
            void addFunctionToCommit(const QString& functionKeyName, QVariant arguments);

        private:
            QVariantMap keyValues;

            //! flag for wait for commit transactions
            bool flagWaitForCommit;

    };
}


#endif