
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

#ifndef _TUI_CLIENT_PRIVATE_H_
#define _TUI_CLIENT_PRIVATE_H_

#include <QObject>
#include <QString>
#include <QMap>
#include "transferui_interface.h"

namespace TransferUI {

    class Transfer;

    class ClientPrivate : public QObject {

        Q_OBJECT

        public:

            ClientPrivate(QObject * parent = 0);
            ~ClientPrivate();

            bool init ();

            ComMeegoTransferuiInterface * interface;

            QMap<QString, Transfer*> transfers;

            //< Delta in seconds for which progress will be sent to transfer-ui
            int progressTimeDelta;

            //!< Delta value for which progress will be sent to transfer-ui
            double progressValueDelta;

            bool tuiOpen; //!< tui visibility state

        Q_SIGNALS:

            void cancelTransfer (Transfer * transfer);

            void startTransfer (Transfer * transfer);

            void pauseTransfer (Transfer * transfer);

            void repairErrorTransfer (Transfer *transfer);

            void sendLastProgress();

        public Q_SLOTS:
            /*!
              \brief Slot for cancel signals from TUI
             */
            void cancelSlot (const QString & id);

            /*!
              \brief Slot for start signals from TUI
             */
            void startSlot (const QString & id);

            /*!
              \brief Slot for pause signals from TUI
             */
            void pauseSlot (const QString & id);

            /*!
             \brief Slot for error repair signals from TUI
            */
            void errorRepairSlot (const QString& id);

            /*!
              \brief Slot invoked when TUI is launched. This slot registers all
                     existing transfers again with the TUI and sends all
                     information all over again
             */
            void tuiLaunched();

            /*!
              \brief slot invoked when the transfer-ui is closed and running in
              background
            */
            void tuiClosed();

            /*!
              \brief slot invoked when the transfer-ui is open and visible to
              the user
            */
            void tuiOpened();
    };
}
#endif