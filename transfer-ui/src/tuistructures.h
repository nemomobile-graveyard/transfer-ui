
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






#ifndef _TRANSFER_STRUCTURES_H_
#define _TRANSFER_STRUCTURES_H_

#include "TransferUI/transferenums.h"

#include <QObject>
#include <QImage>
#include <QDateTime>
#include <QSharedPointer>

using namespace TransferUI;

/*!
    \struct TUIData
    \brief Struct for the TUI Data Model
*/
struct TUIData
{

    TUIData();
    ~TUIData();
    double progressData;   //!< Progress Data
    qlonglong bytes;    //!< Total bytes.
    int estimateTime;  //!< Estimated time in seconds
    int currentFileIndex; //!< current file index which is being transfered
    int filesCount; //!< Total No of Items
    TransferStatus status; //!< Status of the transfer.Refer TransferTypes
    TransferType method; //!< Type of the transfer.Refer TransferTypes
    bool canPause; //!< true if transfer can be paused else false
    bool canRepair; //!< true if error can be reparied else false
    bool canSendImdtly; //!< Applicable only for uploads
    bool showInHistory; //!< show this transfer in history
    QString name; //!< Name which will be displayed
    QString message; //!< Message displayed for non-active transfers.
    QString headerMsg; //!< Header Description of the Error
    QString detailMsg; //!< Detail Description of the Error
    QString actionName; //!< Action Name of the Error
    QString thumbnailFile; //!< filename of the transfer
    QString thumbnailMimeType; //!< mimetype of the thumbnail
    QString targetName; //!< Target Name
    QString fileTypeIcon; //!< file type icon for the transfer
    QString cancelButtonText; //!< custom cancel button text
    QString transferTitle; //!< custom transfer dialog title
    QString replaceId; //!< replace id for the given transfer
    QString clientId; //!< clinet interface information for custom dialogs
    QString resultUri; //!< Uri to launch application of the completed transfers
    QDateTime completedTime; //!< completed time of the transfer
    QDateTime startTime; //!< time when transfer started
    QImage *transferImage; //!< transfer image
};
#endif
