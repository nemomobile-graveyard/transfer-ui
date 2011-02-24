
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

#include "tuistructures.h"
#include <QDebug>

using namespace TransferUI;

TUIData::TUIData(): progressData(0), bytes(0), estimateTime(0),
           currentFileIndex(0), filesCount(1),
           status(TransferStatusInactive),
           method(TransferTypeUpload), canPause(0),
           canRepair(0),canSendImdtly(0) , showInHistory(false) {
}

TUIData::~TUIData() {
    qDebug() << "Deleteing TUI Data" ;
}