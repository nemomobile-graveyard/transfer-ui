
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








#ifndef _TRANSFER_ENUMS_H_
#define _TRANSFER_ENUMS_H_

#include <QObject>

namespace TransferUI {

    /*!
        \enum  TransferType
        \brief This is an enumeration for the type of transfer
    */
    enum TransferType {
        TransferTypeUpload, //!< Transfer is upload
        TransferTypeDownload, //!< Transfer is download
        TransferTypeSync //!< Transfer is sync
    };

    /*!
        \enum  TransferStatus
        \brief This is an enumeration for the status of transfers
    */
    enum TransferStatus {
        TransferStatusActive, //!< The transfer is active
        //!< The transfer is not active (paused or not started yet)
        TransferStatusInactive,

        TransferStatusDone, //!< The transfer has been completed
        TransferStatusPaused, //!< The transfer is paused
        TransferStatusResume, //!< The transfer is Resumed
        //! The transfer is cancelled by the user, yet to get ack from the client
        TransferStatusCancel,
        TransferStatusError //!< Error State
    };

    /*!
        \enum  TransferAtributes
        \brief This is an enumeration for the attribute changes of transfers
    */
    enum TransferAtributes {
        Init, //!< Init all attributes
        // Attributes
        Name, //!< Transfer name
        Progress, //!< Transfer progress
        Size, //!< Transfer size
        Thumbnail, //!< Transfer thumbnail
        Icon, //!< Transfer Icon
        CanPause, //!< Can Pause
        SendNow, //!< Can Send Now
        Estimate, //!< Estimate
        TotalCount, //!< Total Files count in transfer
        CurrentFile, //!< Current file index
        Message, //!< Transfer Message
        Type, //!< Transfer type
        Target, //!< Transfer target
        CancelText, //!< Transfer Cancel text
        TransferTitle, //!< Transfer title
        TransferImage, //!< Transfer Image
        TotalAttribute
    };

    /*!
        \enum  TransferStateChanges
        \brief This is an enumeration for the state changes of transfers
    */
    enum TransferStateChanges {
        // State Changes
        Active = TotalAttribute + 1, //!< Transfer active state
        Inactive, //!< Transfer inactive state
        Paused, //!< Transfer Paused state
        Resumed, //!< Transfer resume state
        Error, //!< Transfer Error state
        Cancel, //!< Transfer Cancel state
        Done, //!< Transfer Done
        Canceled, //!< Transfer canceled but not ackld from the client
        CleanUp, //!< Client Crashed  clean up the transfer
        TotalStateChanges
    };

    /*!
        \enum BannerEvents
        \brief enumeration to represent what type of events to display in the
        banner.
    */
    enum BannerEvent {
        ParitalDoneTransfer, //!< Paritally Completed Transfers
        TransferAlreadyCompleted, //!< Transfer completed
        ClientEvent, //!< Generic event for which clients provides the message
        TotalBannerEvent
    };

    /*!
        \enum TransferDataRole
        \brief enumeration to represent the data roles in the tui data model.
        This Roles will be used to get /set data from / to the model.
    */
    enum TransferDataRole {
        //! progress data of the transfer, progress data is of type double
        ProgressRole = Qt::UserRole + 1, 
        //! size of the transfer, size is of type qlonglong
        SizeRole,
        //! estimate of the transfer, estimate is of type int
        EstimateRole,
        //! current file index of the transfer, file index is of type int
        CurrentFileIndexRole,
        //! total files in the transfer, total files is of type int
        TotalFileCountRole,
        //! current status of the transfer, status is of type int
        StatusRole,
        //! check if the error transfer is repariable, this is of type bool
        CanRepairRole,
        //! check if transfer can pause, this is of type bool
        CanPauseRole,
        //! check if transfer can be sent immdiately, this is of type bool
        CanSendImdltRole,
        //! should transfer be shown in the history, this is of type bool
        ShowInHistoryRole,
        //! name of the transfer, name is of type QString
        NameRole,
        //! message of the transfer, if any, message is of type QString
        MessageRole,
        //! transfer error status. error is of QStringList, first String in the
        //list represents Error Header, second represents Error Detailsa and the
        //last represents action name ,if the error is repairable.
        ErrorRole,
        //! thumbnail information of the transfer. thumbnail is of QStringList,
        //first provides the thumbnail file uri and the next provides the
        //mimetype.
        ThumbnailRole,
        //! file icon information of the transfer, file icon is of type QString
        FileIconRole,
        //! target of the transfer, target is of type QString
        TargetRole,
        //! customized cancel text, if provided. cancel text is of type QString
        CancelTextRole,
        //! customized transfer title, if provided. transfer title is of type
        //QString
        TransferTitleRole,
        //! image for the give transfer, image is of type QImage pointer
        ImageRole,
        //! start time of the transfer, time is of type QDateTime
        StartTimeRole,
        //! completed time of the transfer, time is of type QDateTime
        CompletedTimeRole,
        //! transfer method, method is of type int
        MethodRole,
        TotalDataRoles
    };
}
#endif