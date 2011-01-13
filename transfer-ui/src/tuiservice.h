
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








#ifndef _TUI_SERVICE_H_
#define _TUI_SERVICE_H_

//local headers

//Qt headers
#include <QObject>
#include <QDBusContext>
class QDBusServiceWatcher;

#include <QStringList>
#include <QApplication>

#include <qmsystem2/qmtime.h>

#ifdef _UNIT_TESTING_
#include <QAbstractItemModel>
#include "tuidatamodelproxy.h"
#include "tuicompletedlistproxymodel.h"
#endif
//local class
class TUIServicePrivate;

/*!
    \struct TUIClientData
    \brief Struct for the TUI client model
*/


/*!
    \class TUIService
    \brief This class manages the transfers. This class acts as the service for the
    transfer-ui.
    \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
 */
class TUIService : public QObject , protected QDBusContext

{
    Q_OBJECT

public:
    /*!
        \brief constructor
        \param parent
    */
    TUIService(QObject *parent=0);

    virtual ~TUIService();

    /*!
        \brief init function
        \param showUI true if started with UI, and false otherwise
        \return bool true if no error else false
    */
    bool init();

    static TUIService *instance();

    QApplication *applicationInstance(int argc, char **argv);

    void loadImplementationPlugin();

#ifdef _UNIT_TESTING_
    TUIDataModelProxy *model() const;
#endif

public Q_SLOTS: // METHODS
    /*!
        \brief Transfer is cancelled.This Signal is recvd from the client
        when the transfer is  actually cancelled
        \param id transfer identifier
    */
    void cancelled(const QString &id);


    /*!
        \brief Transfer cancell failed. This Signal is recvd
        from the client when the cancellation request failed
        \param id transfer identifier
        \param message : Message to be displayed
    */
    void cancelFailed(const QString &id , const QString& message);


    /*!
        \brief Transfer is paused. This Signal is recvd from the client when
        the transfer is actually paused
        \param id transfer identifier
    */
    void paused(const QString &id);

    /*!
        \brief Transfer is paused. This Signal is recvd from the client when
        the transfer is actually paused
        \param id transfer identifier
    */
    void resumed(const QString &id);

    /*!
        \brief done : Transfer was done successfully. Transfer UI will
        display "Completed" message for 2 sec and clears the transfer from
        the transfer list.
        \param id transfer identifier
    */
    void done(const QString &id);

    /*!
        \brief done : Transfer was done successfully. Transfer UI will
        display the custom message for 2 sec and clears the transfer from
        the transfer list
        \param id transfer identifier
        \param message custom message to be displayed
    */
    void done(const QString &id, const QString& message);

	/*!
		\brief markCompleted. mark the transfer as completed. This function
		removes transfer from the transfer-ui. Completed transfers can be
		shown in the history list depeneding on the parameters set.
		\param id transfer identifier
		\param showInHistory If this parameter is set to true, then the
		transfer is shown in the history list. Default value of this
		parameter is set to false, by default completed transfer will not be
		shown in history list. If showInHistory is set to false, transfer
		will be displayed for 2 secs with "Completed" message.
		\param replaceId If showInHistory is set to false, this parameter has
		no effect. If replace Id is provided, then the transfer is replaced in
		the history list view for the given replaceId. If there is no transfer
		with the given replaceId, then the transfer is added to history list
		view. Default value is empty, transfers will be added into history
		list and not replaced.
	*/
	void markCompleted(const QString& id, bool showInHistory, const QString&
		replaceId);

    /*!
        \brief overloaded function provided with additional parameter to launch
        the application associated with the result uri when the user taps on the
        transfer.
    */
	void markCompleted(const QString& id, bool showInHistory, const QString&
		replaceId, const QString& resultUri);

    /*!
        \brief Move the transfer to pending state and display the message
        \param id transfer identifier
        \param message Message which will be displayed
    */
    void pending(const QString &id, const QString &message);

    /*!
        \brief  Check if the transfer exists in TUI
        \param  id : TUI id of the transfer
        \return true if the transfer exists, else false
        */
    bool transferExists (const QString &id);

    /*!
        \brief this is the convience method provided. If tui crash and restarts,
        tuiLaunched signal is emitted. libtuiclient re-populates the transfers
        with the same transfer id.
        \param transferId transfer identifier
    */
    QString registerTransientTransfer(const QString& transferId);

    /*!
        \brief Register a new transient transfer. This function registers a
        transfer which doesn't uses tracker. A unique identifier is generated by
        Transfer UI and returned as Id. Id is used to control presentation of
        the transfer registered in UI. The Transfer details other than title and
        type will be set to default values.
        Size will be set to zero
        Total file count will be set to 1
        Transfer State will be set to pending without any message
        Thumbnail is set to default thumbnail
        TargetName will be set to Null.
        \param name Transfer name which will be presented to the user
        \param ty[e Transfer type ,  refer TransferEnums.h
        \return id unique identifier which will be generated and
        returned. This identifier will be used to manage the registered
        transfer
    */
    QString registerTransientTransfer(const QString& name, int type );

    QString registerTransientTransfer(const QString& name, int type, const
	    QString& clientId);

    /*!
        \brief Client request for the summary. This slot will emit the
        summary signal summaryReport
    */
    void sendSummary();

    /*!
        \brief Set if the transfer can pause
        \param id transfer identifiers
        \param canPause true if transfer can pause else false
    */
    void setCanPause(const QString &id, bool canPause);

    /*!
        \brief Set send now property of the transfer
        \param id transfer identifiers
        \param canSendNow
    */
    void setSendNow(const QString &id, bool canSendNow);

    /*!
        \brief Set the transfer as Error . The error is repairable
        \param id transfer identifier
        \param headerMsg Localized Header description what went wrong
        \param detailMsg Localized Detail description what went wrong
        \param actionName Localized String for Action Name
    */
    void setRepairableError(const QString &id, const QString &headerMsg,
        const QString &detailMsg, const QString &actionName);

    /*!
        \brief Set the transfer as Error. The error is non-repairable
        \param id transfer identifier
        \param headerMsg Localized Header description what went wrong
        \param detailMsg Localized Detail description what went wrong
    */
    void setNonRepairableError(const QString &id, const QString &headerMsg,
        const QString &detailMsg);

    /*!
        \brief Set new estimate when transfer is done
        \param id transfer identifier
        \param seconds How many seconds it will take to finish transfer. 0
            or less will tell that estimate can not be calculated. Estimate is
            not shown in case of estimate less than 1
    */
    void setEstimate(const QString &id, int seconds);

    /*!
        \brief Define message that is shown with transfers that are not in
        active state (pending and failed transfers).
        \param id transfer identifier
        \param message message string
    */
    void setMessage(const QString &id, const QString &message);

    /*!
        \brief Define name for transfer. Name is used if files related
        to transfer can't resolved.
        \param id transfer identifier
        \param name localized string
    */
    void setName(const QString &id, const QString &name);

    /*!
        \brief Update progress bar of transfer
        \param id transfer identifier
        \param done how much of transfer is done [0.0 .. 1.0]
    */
    void setProgress(const QString &id, double done);

    /*!
        \brief Define bytes transfered in transfer / size of transfer
        \param id transfer identifier
        \param bytes How many bytes is transfered in transfer. 0 will tell
        that total amount of bytes moved is unknown.
    */
    void setSize(const QString &id, qulonglong bytes);

    /*!
        \brief Set the type of the transfer
        \param id transfer identifier
        \param transferType type of the transfer refer TransferEnums.h
        refer TransferEnums.h enum TransferTypes
    */
    void setTransferType(const QString &id, int transferType);

    /*!
        \brief set the total no of files in the transfer. The default value is
        1, if this is not set
        \param id transfer identifier
        \param count total no of files
    */
    void setFilesCount(const QString &id, int count);

    /*!
        \brief set Index of file currently being sent. This is applicable only
        for Transfers with multiple files
        \param id transfer identifier
        \param fileIndex index of file currently being sent
    */
    void setCurrentFileIndex(const QString &id, int fileIndex);

    /*!
        \brief This string will be displayed along with transfer title
        Eg. In case of upload , target name will be service name for which files
        are uploaded. In case of bt transfer it may be target phone name.
        \param id transfer identifier
        \param targetName target name
    */
    void setTargetName(const QString &id, const QString &targetName);

    /*!
        \brief showUI : show TUI view
    */
    void showUI();

    /*!
        \brief Move transfer to active state
        \param id transfer identifier
        \param done How much of transfer is done so far.
    */
    void started(const QString &id, double done);

    /*!
        \brief show the error details dialog to the user, this slots get
        called when the user clicks on the notification icon or clicks on
        the show error details button.
        \param id transfer identifier
    */
    void showErrorDetails(const QString &id);

    /*!
        \brief set the icon of the file type.If there are no thumbnails for the
        transfer, this method can be used to set the icon of the file type.
        \param id transfer identifier
        \param iconId for the given transfer
    */
    void setFileTypeIcon(const QString& id, const QString& iconId);

    /*!
          \brief set thumbnail file for the transfer
          \param fileName file name for which transfer-ui will fetch thumbnail
          from the thumbnailer
          \param mimeType file mime type used to fetch thumbnail from
          thumbnailer
    */
    void setThumbnailForFile(const QString& id, const QString& fileName, const
        QString& mimeType);

    /*!
        \brief set the icon of the file type.If there are no thumbnails for the
        transfer, this method can be used to set the icon of the file type.
        \param id transfer identifier
        \param iconId for the given transfer
    */
    void setIcon(const QString& id, const QString& iconId);

    /*!
        \brief set the custom cancel text for the transfer. This text will be
        displayed both in Details Dialog as well as in Error Dialog
        \param id transfer identifier
        \param text custom localized cancel text for the transfer.
    */
    void setCancelButtonText(const QString& id, const QString& text);

    /*!
        \brief set the transfer type string. This string will be displayed in
        title of transfer details dialog in transfer-ui.
        e.g. for bluethooth, if the transfer is from device , then the title
        can be "Send" , if the transfer is to device, then the title can be
        "Receive".
        \param title localized title which will be displayed in the transfer
        details dialog
    */
    void setTransferTypeString(const QString& id, const QString &title);

	void setImageFromFilePath(const QString& id, const QString& filePath);

    /*!
        \brief set parameters of the transfer.
    */
    void setValues(const QString& id, const QVariantMap& keyValues);

    /*!
        \brief application moved to switcher
    */
    void movedToSwitcher(bool value);

    /*!
        \brief clear all the transfers from the completed list and database.
    */

    void clearCompletedTransfers();

    void timeOrSettingsChanged (MeeGo::QmTimeWhatChanged what);

Q_SIGNALS: // SIGNALS

    /*!
        \brief Signal emitted when TUI is launched. The clients can connect
                to this signal, and send information about existing transfers
                when they recieve this signal. This signal is particularily
                used in the case where TUI crashed and was re-started.
                This signal has no parameters.
        */
    void launched();

    /*!
        \brief Send transfer was canceld signal to the client
        \param id transfer identifier
    */
    void cancel(const QString &id);

    /*!
        \brief Send error (ID not found / Invalid Transfer Type...)
        \param id transfer identifier
        \param message Message for debug printing
    */
    void invalidID(const QString &id, const QString &message);

    /*!
        \brief Send transfer paused to client
        \param id transfer identifier
    */
    void pause(const QString &id);

    /*!
        \brief Send transfer started (Resumed) to client
        \param id transfer identifier
    */
    void start(const QString &id);

    /*!
        \brief Send the summary to the client
        \param failed no of failed transfers
        \param active no of active transfers
        \param pending no of pending transfers
        \param completed no of completed transfers
    */
    void summaryReport(int failed, int active, int pending, int completed);

    /*!
        \brief send error repair was requested
        \param id transfer identifier
    */
    void errorRepairRequested(const QString& id);

    /*!
        \brief notify clients ( e.g. status-menu indicator ) the collective
        changes in the transfer items.
        \param state "idle" - when there no transfers in Transfer UI.
                    "live" - when there are transfers in Transfer UI. These
                    transfers can be on-going / pending.
                    "fail" - when there are one or more failed transfers. Even,
                    if there are on-going / pending transfer , if one or more
                    transfer is failed, then the state is "fail".
    */
    void stateChanged(const QString& state);

    /*!
        \brief notify clients that the transfer-ui opened and is in
        foreground (transfer-ui is visible to the user)
    */
    void tuiOpened();

    /*!
        \brief notify clients that the transfer-ui is closed and running in
        background, user pressed close button while there are transfers
    */
    void tuiClosed();

private Q_SLOTS:

    /*!
        \brief listen to the client crash / closed events. This is to handle
        zombie transfers, where in which the clients doesn't respond for any
        user events for the transfer.
        \param clientServiceName service which crashed / closed
    */

    void serviceUnregistered(const QString& clientServiceName);

    void checkServiceRegister(const QString& clientServiceName);

    void visibilityChanged(bool value);

private:
    enum State
    {
        Fail, //! < One or more transfer falied"
        //! TUI has 0 error transfer , more than one active
        Live,
        Idle, //!< No transfers in TUI
        Pending, //!< pending transfers (no active and no Error transfers)
        Invalid
    };
    State tuiState; //!< State of the TransferUI (idle , live , fail)

    enum FunctionEnums {
        Name, //!< setName
        Size, //!< setSize
        Estimate, //!< setEstimate
        Message, //!< setMessage
        TargetName, //!< setTargetName
        FilesCount, //!< setFilesCount
        CurrentFileIndex, //!< setCurrentFileIndex
        ThumbnailForFile, //!< setThumbnailForFile
        Icon, //!< setFileTypeIcon
        CancelButtonText, //!< setCancelButtonText
        CanPause, //!< setCanPause
        SendNow, //!< setSendNow
        TransferTypeString, //!< setTransferTypeString
		TransferImage //!< setTransferImage
    };
    //! String map for function names and enum values
    QMap<QString, FunctionEnums> stringEnumFunctionMap;

    TUIServicePrivate * const d_ptr; //!< private constructor class

    static TUIService *selfInstance;

    QDBusServiceWatcher *serviceWatcher; //!< Dbus service watcher

    void emitVisibilityState(const QString& serviceName);
};

#endif