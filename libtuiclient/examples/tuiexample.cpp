
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
 * \example tuiexample.cpp
 * This is an example of how to use the libtuiclient.
 */

#include <TransferUI/Client>
#include <TransferUI/Transfer>

/*
 \class TUITestClient
 \brief Example application class communicating with TUI
*\
class TUITestClient : public QObject
{
    Q_OBJECT
    
public:
    TUITestClient();
    virtual ~TUITestClient();
    void initTUIClient();
    void registerTransfer();
    void updateTransferName();
    void updateProgress();
    void removeTransfer();
    void updateTargetName();
    void updateFilesCount();
    void updateSize();
    void updateCurrentFileIndex();
    void updateThumbnail();
    void updateMessage();
    void updateEstimate();
    void updateCanPause();
    void updateStatus();
    void updateStatusPending();
    void updateCurrentFileInfo();
    void updateFileTypeIcon();
    void updateStatusActive();
    void transferDone();
    void transferError();
        
public Q_SLOTS:
    void transferCancelled(Transfer * transfer);
    void transferPaused(Transfer * transfer);
    void transferResumed(Transfer * transfer);
    void transferErrorRepairRequested(Transfer * transfer);

private:
    TransferUI::Client *client;
    TransferUI::Transfer *uploadTransfer;
    TransferUI::Transfer *downloadTransfer;
    TransferUI::Transfer *syncTransfer;
};

using namespace TransferUI;

TUITestClient::TUITestClient() : client(0) , uploadTransfer(0),
    downloadTransfer(0), syncTransfer(0) {

}

TUITestClient::~TUITestClient() {
    if(client!=0) {
        delete client;
    }
    client = 0;
    
    if(uploadTransfer!=0) {
        delete uploadTransfer;
    }
    uploadTransfer = 0;
    
    if(downloadTransfer!=0) {
        delete downloadTransfer;
    }
    downloadTransfer = 0;
    
    if(syncTransfer!=0) {
        delete syncTransfer;
    }
    syncTransfer = 0;
}

void TUITestClient::initTUIClient() {

    // Create libtuiclient
    client = new TransferUI::Client(this);
    
    // Establish the connection between TUIClient and transfer-ui
    if(!client->init()) {
        qDebug()<<"Cannot initialize TUIClient";//error 
        delete client;
    }
    
    // Connect Signals
    // Connect cancel signal
    connect(client,SIGNAL(cancelTransfer(Transfer*)), this,
        SLOT(transferCancelled(Transfer*)));

    // Connect pause signal
    connect(client,SIGNAL(pauseTransfer(Transfer*)), this,
        SLOT(transferPaused(Transfer*)));
        
    // Connect resume / start signal
    connect(client,SIGNAL(startTransfer(Transfer*)), this,
        SLOT(transferResumed(Transfer*)));

    // Connect error repair signal
    connect(client,SIGNAL(repairErrorTransfer(Transfer*)), this,
        SLOT(transferErrorRepairRequested(Transfer*)));
}

void TUITestClient::registerTransfer() {
    //Add a new upload transfer in TransferUI
    uploadTransfer = client->registerTransfer("New Upload Transfer",TransferUI::Client::TRANSFER_TYPES_UPLOAD);
    
    //Add a new download transfer in TransferUI
    downloadTransfer = client->registerTransfer("New Download Transfer", TransferUI::Client::TRANSFER_TYPES_DOWNLOAD);
    
    //Add a new Sync transfer in TransferUI
    syncTransfer = client->registerTransfer("Synchronizing", TransferUI::Client::TRANSFER_TYPES_SYNC);
    
    //Commit usage
    uploadTransfer->waitForCommit(); //start commit
    uploadTransfer->setName("filename.jpg");
    uploadTransfer->setTargetName("picasa");
    uploadTransfer->setFilesCount(10);
    uploadTransfer->setSize(1000);
    uploadTransfer->commit(); //end commit 
    //Reduces three dbus calls to transfer-ui
    
}

void TUITestClient::updateTransferName() {
    //Update upload transfer name
    uploadTransfer->setName("filename.jpg");
    
    //Update download transfer name
    downloadTransfer->setName("Downloading filename.pdf");
}

void TUITestClient::updateProgress() {
    //Update upload transfer progress to 56%
    uploadTransfer->setProgress(.56);
}

void TUITestClient::removeTransfer() {
    //Get the transfer identifier 
    QString tdId = uploadTransfer->transferId();
    
    //remove upload transfer from TransferUI
    client->removeTransfer(tdId);
    
    delete uploadTransfer;
    uploadTransfer= 0;
}

void TUITestClient::updateTargetName() {
    //Update target name for upload transfer
    uploadTransfer->setTargetName("picasa");
}

void TUITestClient::updateFilesCount() {
    //Update total files count in the transfer
    uploadTransfer->setFilesCount(10);
}

void TUITestClient::updateSize() {
    //Update total size of the transfer in bytes
    uploadTransfer->setSize(1000);
    
    //if Size is set to 0, if the transfer is active, then the progress bar in
    //TransferUI will be shown in RUN_UNKNOWN state
    syncTransfer->setSize(0);
}

void TUITestClient::updateCurrentFileIndex() {
    //Update current file index, this is the index of the current file being
    //transferred, TransferUI displays 2/10 in its list view and 1/10 in the
    //dialog view
    uploadTransfer->setCurrentFileIndex(2);
}

void TUITestClient::updateThumbnail() {
    //Update the thumbnail image
    uploadTransfer->setThumbnailForFile("file://user/home/MyDocs/.images/somefile.jpg", "image/jpeg");
}

void TUITestClient::updateMessage() {
    //Update message in TransferUI. Message should be localized string
    //% "New Message"
    QString newMsgString = qtTrid("qtn_tui_xxxx_xxxx");
    uploadTransfer->setMessage(newMsgString);
}

void TUITestClient::updateEstimate() {
    //Update the transfer estimate is seconds
    //set estimate to 2 secs
    uploadTransfer->setEstimate(100);

    //set estimate to 2 secs    
    downloadTransfer->setEstimate(100);
    
    //set estimate to 2 secs    
    syncTransfer->setEstimate(100);
}

void TUITestClient::updateCanPause() {
    //Update if transfer can pause, this is not applicable for uploads
    downloadTransfer->setCanPause(true);
}

void TUITestClient::updateStatus() {
    //Update the current status of the transfer w.r.t to progress and estimate
    //Set progress to 76% and estimate to 2 mins
    uploadTransfer->updateStatus(0.76,100);
}

void TUITestClient::updateStatusPending() {
    //Update the transfer state to pending, Msg is localized string
    //% "Waiting for connection"
    QString uploadMsg = qtTrid("qtn_tui_xxxxx_xxxxx");
    uploadTransfer->setPending(uploadMsg);
    
    //% "Connecting to the service"
    QString downloadMsg = qtTrid("qtn_tui_xxxxx_xxxxx");
    downloadTransfer->setPending(downloadMsg);
}

void TUITestClient::updateCurrentFileInfo() {
    //Update current file indo w.r.t to thumbnail, name and file index
    uploadTransfer->updateCurrentFileInfo("Upload somefile.jpg",
        "file://user/home/MyDocs/.images/somefile.jpg" , 3);
}

void TUITestClient::updateFileTypeIcon() {
    //If the thumbnail image is not availabe for the transfer provide transfer
    //file type icon
    downloadTransfer->setIcon("icon-m-file-unknown");
}

void TUITestClient::updateStatusActive() {
    //Update the transfer state to active and set progress to 75%
    uploadTransfer->setActive(0.75);
}

void TUITestClient::transferError() {
    //Something went wrong in the transfer, update error in TransferUI
    //% "Some header here"
    QString headerMsg = qtTrId("qtn_tui_err_headerxxxx");
    
    //% "Some details description here"
    QString detailsMsg = qtTrId("qtn_tui_err_msgxxxx");
    
    //if error is non - repairable
    uploadTransfer->markFailure(headerMsg, detailsMsg);

    //if error is repairable
    //% "Retry"
    QString actionName = qtTrId("qtn_tui_repairxx");
    uploadTransfer->markRepairableFailure(headerMsg, detailsMsg, actionName);
}

void TUITestClient::transferDone() {
    //Transfer is completed , update TransferUI
    uploadTransfer->markDone();
    
    //Remove from the list of client transfers 
    client->removeTransfer(uploadTransfer->transferId());
    
}

void TUITestClient::transferCancelled(Transfer * transfer) {
    //User has cancelled the transfer, take neccessary action and mark transfer
    //as cancelled
    //eg uploadTransfer was cancelled by the user
    
    //eg. send the cancel notification to the service
    
    //if cancel was sucessful
    transfer->markCancelled();
    
    //else
    //% "Something went wrong in cancel process"
    QString cancelErrMsg = qtTrid("qtn_tui_cancel_errxxxx");
    transfer->markCancelFailed(cancelErrMsg);
    
    //Remove from the list of client transfers 
    client->removeTransfer(uploadTransfer->transferId());
}

void TUITestClient::transferPaused(Transfer * transfer) {
    //User requested transfer to pause, pause transfer and update TransferUI
    
    transfer->markPaused();
}

void TUITestClient::transferResumed(Transfer * transfer) {
    //User requested transfer to resume, resume transfer and update TransferUI
    
    transfer->markResumed();
}

void TUITestClient::transferErrorRepairRequested(Transfer * transfer) {
    //User has requested for repair the error take some action
}
