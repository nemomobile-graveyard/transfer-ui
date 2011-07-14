
/*
 * This file is part of Handset UX Transfer user interface
 *
 * Copyright (C) 2010-2011 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
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


/*!
  \file transferenums.h
  \brief Enumerations of Transfer UI
  \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
  \copyright (c) 2010-2011 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef _TRANSFER_ENUMS_H_
#define _TRANSFER_ENUMS_H_

#include <QObject>

namespace TransferUI {

    /*!
        \enum  TransferType
        \brief This is an enumeration for the type of transfer.
    */
    enum TransferType {
        TransferTypeUpload, //!< Transfer is upload.
        TransferTypeDownload, //!< Transfer is download.
        TransferTypeSync //!< Transfer is sync.
    };

    /*!
        \enum  TransferStatus
        \brief This is an enumeration for the status of transfers.
    */
    enum TransferStatus {
        TransferStatusActive, //!< The transfer is active.
        //!< The transfer is not active (paused or not started yet).
        TransferStatusInactive,

        TransferStatusDone, //!< The transfer has been completed.
        TransferStatusPaused, //!< The transfer is paused.
        TransferStatusResume, //!< The transfer is resumed.
        //! The transfer is cancelled by the user, yet to get back from the client.
        TransferStatusCancel,
        TransferStatusCanceled, //!< Transfer is actually canceled.
        TransferStatusError //!< Error State
    };

    /*!
        \enum  TransferAtributes
        \brief This is an enumeration for the attribute changes of transfers.
    */
    enum TransferAtributes {
        Init, //!< Init all attributes
        // Attributes
        Name, //!< Transfer name
        Progress, //!< Transfer progress
        Size, //!< Transfer size
        Thumbnail, //!< Transfer thumbnail
        Icon, //!< Transfer icon
        CanPause, //!< transfer can be paused
        SendNow, //!< Can send now
        Estimate, //!< Estimate
        TotalCount, //!< Total files count in transfer
        CurrentFile, //!< Current file index
        Message, //!< Transfer message
        Type, //!< Transfer type
        Target, //!< Transfer target
        CancelText, //!< Transfer cancel text
        TransferTitle, //!< Transfer title
        TransferImage, //!< Transfer image
        NewTransferAdded, //! < A new transfer is added
        TotalAttribute
    };

    /*!
        \enum  TransferStateChanges
        \brief This is an enumeration for the state changes of transfers.
    */
    enum TransferStateChanges {
        // State Changes
        Active = TotalAttribute + 1, //!< Transfer active state
        Inactive, //!< Transfer inactive state
        Paused, //!< Transfer paused state
        Resumed, //!< Transfer resume state
        Error, //!< Transfer error state
        Cancel, //!< Transfer cancel state
        Done, //!< Transfer done state
        Canceled, //!< Transfer canceled but not acknowledged from the client
        CleanUp, //!< Client crashed clean-up the transfer
        TotalStateChanges
    };

    /*!
        \enum BannerEvents
        \brief This is an enumeration to represent what type of events to display in the
        banner.
    */
    enum BannerEvent {
        ParitalDoneTransfer, //!< Paritally completed transfers
        TransferAlreadyCompleted, //!< Transfer completed
        ClientEvent, //!< Generic event for which clients provide the message
        TotalBannerEvent
    };

    /*!
        \enum TransferDataRole
        \brief This is an enumeration to represent the data roles in the tui data model.
        This role is used to get or set data from/to the model.
    */
    enum TransferDataRole {
        //! Progress data of the transfer, the variable type is double.
        ProgressRole = Qt::UserRole + 1, 
        //! Size of the transfer, the variable type is qlonglong.
        SizeRole,
        //! Estimates of the transfer, the variable type is int.
        EstimateRole,
        //! Current file index of the transfer, the variable type is int.
        CurrentFileIndexRole,
        //! Total files in the transfer, the variable type is int.
        TotalFileCountRole,
        //! Current status of the transfer, the variable type is int.
        StatusRole,
        //! Checks if the error transfer is repariable, the variable is bool.
        CanRepairRole,
        //! Checks if transfer can pause, the variable type is bool.
        CanPauseRole,
        //! Checks if transfer can be sent immdiately, the variable type is bool.
        CanSendImdltRole,
        //! Should transfer be shown in the history or not, the variable type is bool.
        ShowInHistoryRole,
        //! Name of the transfer, the variable type is QString.
        NameRole,
        //! Message of the transfer, if any, the variable type is QString.
        MessageRole,
        //! Transfer error status. The error is of QStringList, first String in the
        //list represents Error Header, second represents Error Details and the
        //last represents action name, if the error is repairable.
        ErrorRole,
        //! Thumbnail information of the transfer. Thumbnail is of QStringList,
        //first provides the thumbnail file uri and the next provides the
        //mimetype.
        ThumbnailRole,
        //! File icon information of the transfer, the variable type is QString.
        FileIconRole,
        //! Target of the transfer, the variable type is QString.
        TargetRole,
        //! Customized cancel text, if provided. The variable type is QString.
        CancelTextRole,
        //! Customized transfer title, if provided. The variable type is 
        //QString.
        TransferTitleRole,
        //! Image for the give transfer, the variable type is QImage pointer.
        ImageRole,
        //! Start time of the transfer, the variable type is QDateTime.
        StartTimeRole,
        //! Completed time of the transfer, the variable type is QDateTime.
        CompletedTimeRole,
        //! Transfer method, the variable type is int.
        MethodRole,
        TotalDataRoles
    };
}
#endif
