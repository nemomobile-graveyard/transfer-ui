
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
}
#endif