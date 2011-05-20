
/*
 * transfer-ui -- Handset UX Transfer user interface
 * Copyright (c) 2010-2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef _TUI_PERSISTENT_TRANSFER_H_
#define _TUI_PERSISTENT_TRANSFER_H_

#include <QObject>
#include "TransferUI/transfer.h"
#include "TransferUI/client.h"

class ComMeegoTransferuiInterface;

namespace TransferUI {

    class PersistentTransfer : public Transfer
    {
        Q_OBJECT

        public:
            PersistentTransfer (ComMeegoTransferuiInterface * interface,
                const QString & id, Client * parent);
            ~PersistentTransfer();

    };
}

#endif
