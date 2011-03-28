
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

/*!
  \file transfer.h
  \brief Class for presented Transfer
  \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
  \copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
  \license GNU Lesser General Public License, version 2.1\n
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

#ifndef _TRANSFER_H_
#define _TRANSFER_H_

#include <QObject>
#include "export.h"
#include "TransferUI/client.h"

namespace TransferUI {
    class TransferPrivate;

    /*!
        \class Transfer
        \brief Class for modifing information of transfer shown in transfer-ui
        \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
    */
    class TUI_EXPORT Transfer : public QObject
    {
        Q_OBJECT

    public:

        /*!
          \brief constructor
          \param parent
        */
        Transfer (Client * parent);

        virtual ~Transfer();

        /*!
          \brief Get unique id of transfer
          \return ID of transfer or empty string if not defined
        */
        virtual QString transferId() const;

        /*!
          \brief Get last error message of class (note: this is not error
          messages you send to TransferUI)
          \returns error message or empty string if no error
        */
        virtual QString lastError () const;

        /*!
          \brief  Set the name of the account or target used in transfer
          \param  name Name of the service or target of the file
          \return true if request was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::updateTargetName()
          \until }
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool setTargetName (const QString & name);

       /*!
          \brief Define how many total files are present in transfer
          \param count Number of files in the transfer
          \return true if request was send successfully \n
          \dontinclude tuiexample.cpp
          \skipline TUITestClient::updateFilesCount()
          \until }
          \sa setCurrentFileIndex updateCurrentFileInfo
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool setFilesCount (int count);

        /*!
          \brief Define size of transfer
          \param bytes Size in bytes. If size is set to 0, no size will be
          displayed.
          \return true if request was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::updateSize()
          \until }
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool setSize (quint64 bytes);

        /*!
          \brief Define how much of transfer is done
          \param done From 0.0 (=0.0%) to 1.0 (=100%) how much of transfer is
          done. Other values will change progress to undefined.
          \return true if request was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::updateProgress()
          \until }
          \sa setActive updateStatus
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool setProgress (float done);

        /*!
          \brief Define current file index which being transferred
          \param index current file index
          \return true if request was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::updateCurrentFileIndex()
          \until }
          \sa updateCurrentFileInfo setFilesCount
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool setCurrentFileIndex (int index);

        /*!
          \brief Define current state message. This function returns false if
          the state of the transfer is not inactive.
          \param message New state message
          \return true if request was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::updateMessage()
          \until }
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool setMessage (const QString & message);

        /*!
          \brief Define how much time will transfer last
          \param seconds How many seconds transfer will still take (values
          bigger than 0). Other values will tell that estimation can't be
          calculated and will not be shown in TransferUI.
          \return true if request was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::updateEstimate()
          \until }
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool setEstimate (int seconds);

        /*!
          \brief Update how much of transfer is done and what is the current
          estimate.
          \param done How much of transfer is done [0.0 ... 1.0]
          \param seconds How many seconds until transfer is done
          \return true if reguest was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::updateStatus()
          \until }
          \sa setEstimate setProgress
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool updateStatus (float done, int seconds);

        /*!
          \brief Define if transfer can be paused. This is not applicable for
          uploads. Default implementation is set to false, and transfers can't
          be paused. If pause is enabled, when user pauses the transfer in
          TransferUI , Signals Client::pauseTransfer / Transfer::pause are
          emitted.
          \param canPause true if transfer can be paused
          \return true If request was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::updateCanPause()
          \until }
          \sa Client::pauseTransfer markPaused
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool setCanPause (bool canPause);

        /*!
          \brief Set name for transfer
          \param name New name for transfer. Empty string will disable name
          \return true If request was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::updateTransferName()
          \until }
          \sa updateCurrentFileInfo
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool setName (const QString & name);


        /*!
          \brief set the Type of the Transfer
          \param transferType type of the transfer. \ref Client::TransferType enum
          \return true if request was send successfully
        */
        virtual bool setTransferType(Client::TransferType transferType);

        /*!
          \brief set thumbnail file for the transfer. transfer-ui try to resolve
          thumbnail for the give file. Resolving thumbnail is asyncronus call,
          any new setIcon function call or a new setThumbnailForFile function
          call will ignore this request. fileName should be tracker indexed for
          thumbnailer to fetch the thumbnail. For non tracker indexed file use
          setImageFromFilePath
          \param fileName file path for which transfer-ui will fetch thumbnail
          from the thumbnailer
          \param mimeType file mime type used to fetch thumbnail from
          thumbnailer
          \return true if request was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::updateThumbnail()
          \until }
          \sa setIcon
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool setThumbnailForFile(const QString& fileName, const QString&
            mimeType);
        
        /*!
          \brief set icon id for the transfer. This function igonres any
          previous requests made to show thumbnail using setThumbnailForFile
          function. 
          \param iconId icon id which will be displayed in transfer-ui
          \return true if request was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::updateFileTypeIcon()
          \until }
          \n \ref transfer_ui_main "Overview of TransferUI"          
        */
        virtual bool setIcon(const QString& iconId);
        
        /*!
          \brief set the custom cancel text for the transfer. This text will be
          displayed both in Details Dialog as well as in Error Dialog
          \param text custom localized cancel text for the transfer.
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::updateFileTypeIcon()
          \until }
          \sa setThumbnail
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool setCancelButtonText(const QString& text);
        
        /*!
          \brief Define if transfer can be sent immediately. This is not
          applicable for downloads. Default implementation is set to false. If
          send now is enabled, when user clicks on send now in TransferUI ,
          Signals Client::startTransfer / Transfer::start is emitted
          \param canSendNow set if transfer can be sent immediately.
        */
        virtual bool setSendNow (bool canSendNow);

        /*!
          \brief set the transfer type string. This string will be displayed in
          title of transfer details dialog in transfer-ui. 
          e.g. for bluethooth, if the transfer is from device , then the title
          can be "Send" , if the transfer is to device, then the title can be
          "Receive".
          \param title localized title which will be displayed in the transfer
          details dialog
        */
        virtual bool setTransferTypeString(const QString &title);

		/*!
		  \brief set transfer image from the give filename path.
          \param fileNamePath absolute path of the file name from which Image
          will be loaded
		*/
        bool setImageFromFilePath(const QString& filePath);

        /*!
          \brief wait for the commit call. Once the commit is recvd forward set
          functions to transfer-ui. All set functions can be commitable functions.
          When this function is called, all the subsequent set function will be
          queued. After recieving commit() call, these set functions will be
          forwarded to transfer-ui. \n These are the functions which are
          commitable \n
              <table border="1">
                  <tr>
                        <td> <b> Commitable functions </b> </td>
                  </tr>
                  <tr>
                        <td>setTargetName</td>
                  </tr>
                  <tr>
                      <td>setFilesCount</td>
                  </tr>
                  <tr>
                      <td>setSize</td>
                  </tr>
                  <tr>
                      <td>setCurrentFileIndex</td>
                  </tr>
                  <tr>
                      <td>setThumbnailForFile</td>
                  </tr>
                  <tr>
                      <td>setMessage</td>
                  </tr>
                  <tr>
                      <td>setEstimate</td>
                  </tr>
                  <tr>
                      <td>setCanPause</td>
                  </tr>
                  <tr>
                      <td>setName</td>
                  </tr>
                  <tr>
                      <td>setIcon</td>
                  </tr>
                  <tr>
                      <td>setCancelButtonText</td>
                  </tr>
              </table>
            
            <b> setProgress has a different mechanism, hence setProgress can not be
            a commitable functions. </b> \n
            <b> <i> Note on auto commit </i> </b> \n
            When the state change functions \n
            <table border="1">
                <tr>
                    <td> <b> Auto Commit functions </b> </td>
                </tr>
                <tr>
                    <td>setPending</td>
                </tr>
                                <tr>
                    <td>setActive</td>
                </tr>
                                <tr>
                    <td>markFailed</td>
                </tr>
                                <tr>
                    <td>markCancelled</td>
                </tr>
                                <tr>
                    <td>markResumed</td>
                </tr>
                                <tr>
                    <td>markDone</td>
                </tr>
                                <tr>
                    <td>markFailure</td>
                </tr>
                                <tr>
                    <td>markRepairableFailure</td>
                </tr>
            </table>
            are called , previous set functions after waitForCommit will be sent
            to transfer-ui automatically.
            \dontinclude tuiexample.cpp
            \skipline TUITestClient::registerTransfer()
            \until }
            \sa commit
        */
        virtual void waitForCommit();
        
        /*!
           \brief commit any previous set functions after waitForCommit called
           \return if the commit was successfull.
            \dontinclude tuiexample.cpp
            \skipline TUITestClient::registerTransfer()
            \until }
            \sa waitForCommit
        */
        virtual bool commit();


        /*!
          \brief Move transfer back to pending state
          \param reason Reason why transfer went back to pending state, for
          example "Waiting for connection..."
          \return true if request was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::updateStatusPending()
          \until }
          \sa setActive setMessage
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool setPending (const QString & reason);

        /*!
          \brief Move transfer to active state
          \param done How much of tranfer is done
          \return true if request was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::updateStatusActive()
          \until }
          \sa setPending setProgress
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool setActive (float done = 0.0);

        /*!
          \brief Mark transfer cancelled
          \return true if request was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline connect(client,SIGNAL(cancelTransfer
          \until )))
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::transferCancelled(
          \until }
          \sa markCancelFailed Client::cancelTransfer
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool markCancelled ();

        /*!
          \brief Mark transfer paused
          \return true if request was sent successfully
          \n \dontinclude tuiexample.cpp
          \skipline connect(client,SIGNAL(pauseTransfer
          \until )))
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::transferPaused(
          \until }
          \sa Client::pauseTransfer markResumed
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool markPaused();

        /*!
          \brief Mark transfer resumed.
          \return true if request was sent successfully
          \n \dontinclude tuiexample.cpp
          \skipline connect(client,SIGNAL(startTransfer
          \until )))
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::transferResumed(
          \until }
          \sa Client::startTransfer markPaused
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool markResumed();

        /*!
          \brief markDone transfer Done
          \param message localized string describing the completion message.
          Default message is set to "Completed" in TransferUI.
          \return true if request was sent successfully
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::transferDone(
          \until }
          \n \ref transfer_ui_main "Overview of TransferUI"
        */

		virtual bool markDone (const QString & message = "");

		/*!
		  \brief markCompleted. mark the transfer as completed. This function
		  removes transfer from the transfer-ui or shown in the history list.
		  Completed transfers can be shown in the history list depeneding on the
		  parameters set. 
   		  \param showInHistory If this parameter is set to true, then the
		  transfer is shown in the history list. Default value of this
		  parameter is set to false, by default completed transfer will not be
		  shown in history list. If showInHistory is set to false, transfer
		  will be displayed for 2 secs with "Completed" message and will be
		  removed from the transfers list.
		  \param replaceId If showInHistory is set to false, this parameter has
		  no effect. If replace Id is provided, then the transfer is replaced in
		  the history list view for the given replaceId. If there is no transfer
		  with the given replaceId, then the transfer is added to history list
		  view. Default value is empty, transfers will be added into history
		  list and not replaced.
          \param resultUri uri of the result file. If provided, transfer-ui
          launches the application associated with the given uri. If the Uri is
          not valid, default dialog is displayed when the user taps on the
          transfers. resultUri represents an object stored in Tracker. 
		  \return true if request was sent successfully
		*/
		bool markCompleted(bool showInHistory=false, 
			const QString& replaceId ="",const QString& resultUri = QString());


        /*!
          \brief Marks the transfer as failed. This error can't be repaired. The
          messages are not localized in TransferUI, it should be done in the
          applications.
          \param headerMessage localized string describing error header message
          \param description localized string describing error details message
          \return true if request was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::transferError(
          \until }
          \sa markRepairableFailure
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool markFailure (const QString & headerMessage,
                const QString & description);

        /*!
          \brief Marks the transfer as failed. This error can be repaired.
          Signal Client::repairErrorTransfer \ Transfer::repairError is emitted
          if user requested to repair error.
          \param headerMessage Error Message Header
          \param description Error Message details
          \param actionName Action Name displayed in the Action Button
          \return true if request was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::transferError(
          \until }
          \sa Client::repairErrorTransfer markFailure
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool markRepairableFailure (const QString& headerMessage,
                    const QString& description, const QString& actionName);

        /*!
          \brief set the transfer cancel as failed
          \param message localized string describing what went wrong
          \return true if request was send successfully
          \n \dontinclude tuiexample.cpp
          \skipline connect(client,SIGNAL(cancelTransfer
          \until )))
          \n \dontinclude tuiexample.cpp
          \skipline TUITestClient::transferCancelled(
          \until }
          \sa markCancelled Client::cancelTransfer
          \n \ref transfer_ui_main "Overview of TransferUI"
        */
        virtual bool markCancelFailed(const QString& message);
        
        
    public Q_SLOTS:
        /*!
          \brief  Slot connecting to the signal from TUI when it launches
        */
        void tuiLaunched();

        /*!
          \brief Slot used by client
        */
        void cancelSlot ();

        /*!
          \brief Slot used by client
        */
        void startSlot ();

        /*!
          \brief Slot used by client
        */
        void pauseSlot ();

        /*!
          \brief Slot uesd by client for error repair
        */
        void errorRepairSlot();

        /*!
          \brief user opened transfer-ui and send the last stored progress value
          to the transfer-ui
        */
        void sendLastProgress();
        

    Q_SIGNALS:

        /*!
          \brief Signal emited when user has asked transfer to be cancelled
        */
        void cancel ();

        /*!
          \brief Signal emited when user has asked transfer to be
          started. In case of failed transfer repairError signal is used
          instead of this.
          \sa repairError
        */
        void start ();

        /*!
          \brief Signal emited when user has asked transfer to be paused
        */
        void pause ();

        /*!
          \brief Signal emited when user has asked to retry or to fix the failed
                 transfer
          \sa start
        */
        void repairError();
        

    protected:
        TransferPrivate * const d_ptr; /*!< private class */
    };
}

#endif
