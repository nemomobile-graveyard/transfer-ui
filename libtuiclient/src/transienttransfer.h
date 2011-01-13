
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

#ifndef _TUI_TRANSIENT_TRANSFER_H_
#define _TUI_TRANSIENT_TRANSFER_H_

#include <QObject>
#include "TransferUI/transfer.h"

#include "TransferUI/client.h"

class ComMeegoTransferuiInterface;

namespace TransferUI {

    class TransientTransfer : public Transfer
    {
        Q_OBJECT

          public:

            TransientTransfer(ComMeegoTransferuiInterface * interface,
                const QString & id,  const QString & name, int type ,
                const QString& imagePath ,  int state , int totalFiles,
                qulonglong size,   const QString& serviceName ,
                const QString& msg,  Client * parent);

            TransientTransfer(ComMeegoTransferuiInterface *interface,
                const QString& id, const QString& name, Client::TransferType
                type, Client *parent);

            ~TransientTransfer();

            bool markFailed (const QString & message = "",
                const QString & description = "");
    };
}

#endif