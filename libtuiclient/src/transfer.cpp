
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

#include "TransferUI/transfer.h"
#include "transferprivate.h"
#include <QDebug>
#include <QTime>

#include "TransferUI/client.h"

using namespace TransferUI;

#define checkInterface() if (d_ptr->interface == 0) { \
    d_ptr->errorMessage = "Interface not available"; \
    qWarning() << "Interface not available"; \
    return false; \
}

#define notImplemented() { \
    d_ptr->errorMessage = "Function is not implemented"; \
    qWarning() << "libtuiclient function not implemented"; \
    return false; \
}

#define addToCommit(name, param) \
    if (d_ptr->isWaitForCommitSet() == true) { \
        QVariant paramVariant;      \
        paramVariant.setValue(param);   \
        d_ptr->addFunctionToCommit(name, paramVariant); \
        return true;                 \
    }

Transfer::Transfer (Client * parent) : QObject (parent),
    d_ptr (new TransferPrivate(parent)) {
    d_ptr->lastProgressSentTime.start();
}

Transfer::~Transfer() {
    delete d_ptr;
}

QString Transfer::transferId() const {
    return d_ptr->id;
}

QString Transfer::lastError () const {
    return d_ptr->errorMessage;
}

bool Transfer::setTargetName (const QString & name) {

    d_ptr->data.targetName = name;
    addToCommit("setTargetName", name)
    checkInterface()
    d_ptr->interface->setTargetName (transferId(), name);
    return true;
}

bool Transfer::setCurrentFileIndex (int index) {

    d_ptr->data.currentFileIndex = index;
    addToCommit("setCurrentFileIndex", index)

    checkInterface()
    d_ptr->interface->setCurrentFileIndex(transferId(), index);
    return true;
}

bool Transfer::setSize (quint64 bytes) {
    d_ptr->data.bytes = bytes;
    addToCommit("setSize", bytes)

    checkInterface()

    d_ptr->interface->setSize(transferId(), bytes);
    return true;
}

bool Transfer::setProgress (float done) {
    qDebug() << "set progress" << done;
    if(d_ptr->clientPtr->isTUIVisible() == true) {
        qDebug() << "Set Progress " << done;
        checkInterface()
        double valueDelta = d_ptr->clientPtr->progressValueDelta();
        int timeDelta = d_ptr->clientPtr->progressTimeDelta();
        d_ptr->sendProgress( valueDelta, timeDelta, done);
    } else {
        d_ptr->data.progressData = done;
    }
    return true;
}


bool Transfer::setFilesCount (int count) {
    d_ptr->data.filesCount = count;
    addToCommit("setFilesCount", count)
    checkInterface()
    d_ptr->interface->setFilesCount(transferId(), count);
    return true;
}


bool Transfer::setMessage (const QString & message) {
    bool retVal = false;
    if(d_ptr->data.status == TransferStatusInactive) {
        d_ptr->data.message = message;
        addToCommit("setMessage", message)
        checkInterface()
        d_ptr->interface->setMessage(transferId(), message);
        retVal = true;
    } else {
        qWarning() << __FUNCTION__ << "Message can only be set "
                "for Inactive Transfers";
    }
    return retVal;
}

bool Transfer::setEstimate (int seconds) {
    d_ptr->data.estimateTime = seconds;
    addToCommit("setEstimate", seconds)
    checkInterface()
    d_ptr->interface->setEstimate(transferId(), seconds);
    return true;
}

bool Transfer::updateStatus (float done, int seconds) {
    bool ret;

    ret = setProgress (done);
    ret = (setEstimate (seconds) && ret);

    return ret;
}

bool Transfer::setCanPause (bool canPause) {
    d_ptr->data.canPause = canPause;
    addToCommit("setCanPause", canPause)
    checkInterface()
    d_ptr->interface->setCanPause(transferId(), canPause);
    return true;
}

bool Transfer::setSendNow (bool canSendNow) {
    d_ptr->data.canSendImdtly = canSendNow;
    addToCommit("setSendNow", canSendNow)
    checkInterface()
    d_ptr->interface->setCanPause(transferId(), canSendNow);
    return true;
}

bool Transfer::setTransferTypeString(const QString &title) {
    d_ptr->data.transferTitle = title;
    addToCommit("setTransferTypeString", title)
    checkInterface()
    d_ptr->interface->setTransferTypeString(transferId(), title);
    return true;
}

bool Transfer::setName (const QString & name) {
    d_ptr->data.name = name;
    addToCommit("setName", name)
    checkInterface()
    d_ptr->interface->setName(transferId(), name);
    return true;
}

bool Transfer::setPending (const QString & reason) {
    checkInterface()
    commit();
    if (TransferStatusInactive == d_ptr->data.status) {
        //state is already set to active
        setMessage(reason);
    } else {
        d_ptr->data.status = TransferStatusInactive;
        d_ptr->data.message = reason;
        d_ptr->interface->pending(transferId(), reason);
    }
    return true;
}

bool Transfer::setActive (float done) {
    checkInterface()
    commit();
    if (TransferStatusActive == d_ptr->data.status) {
        //state is already set to active
        setProgress(done);
    } else {
        d_ptr->data.status = TransferStatusActive;
        d_ptr->interface->started (transferId(), done);
    }
    return true;
}


bool Transfer::setImageFromFilePath(const QString& filePath) {
	checkInterface()

	//Check if file exists, if not don't fwd it to tui
	QFileInfo fInfo(filePath);
	if(fInfo.exists() == true) {
		d_ptr->data.fileTypeIcon.clear();
		d_ptr->data.thumbnailMimeType.clear();
		d_ptr->data.thumbnailFile = filePath;
		addToCommit("setImageFromFile", filePath)
		d_ptr->interface->setImageFromFilePath(transferId(), filePath);
		return true;
	} else {
		qWarning() << __FUNCTION__ << "Invalid input file name" << filePath;
		return false;
	}
}

bool Transfer::markFailure (const QString & headerMessage,
        const QString & description) {
    checkInterface()
    commit();
    d_ptr->data.status = TransferStatusError;
    d_ptr->data.canRepair = false;
    d_ptr->data.headerMsg = headerMessage;
    d_ptr->data.detailMsg = description;
    d_ptr->interface->setNonRepairableError(transferId(), headerMessage,
                    description);
    return true;
}

bool Transfer::markRepairableFailure (const QString& headerMessage,
        const QString& description, const QString& actionName) {
    checkInterface()
    commit();
    d_ptr->data.status = TransferStatusError;
    d_ptr->data.canRepair = true;
    d_ptr->data.headerMsg = headerMessage;
    d_ptr->data.detailMsg = description;
    d_ptr->data.actionName = actionName;
    d_ptr->interface->setRepairableError (transferId(), headerMessage,
                        description,actionName);
    return true;
}

bool Transfer::markCancelled () {
    checkInterface()
    commit();
    d_ptr->data.status = TransferStatusCancel;
    d_ptr->interface->cancelled (transferId());
    return true;
}

bool Transfer::markCancelFailed(const QString& message) {
    checkInterface()
    commit();
    d_ptr->interface->cancelFailed(transferId(),message);
    return true;
}

bool Transfer::markPaused() {
    checkInterface()
    commit();
    d_ptr->data.status = TransferStatusPaused;
    d_ptr->interface->paused(transferId());
    return true;
}

bool Transfer::markResumed() {
    checkInterface()
    commit();
    d_ptr->data.status = TransferStatusResume;
    d_ptr->interface->resumed(transferId());
    return true;
}

bool Transfer::markDone (const QString & message) {
    checkInterface()
    commit();
    d_ptr->data.status = TransferStatusDone;
    if(message.isEmpty() ==  false) {
        d_ptr->interface->done (transferId(),message);
    } else {
        d_ptr->interface->done (transferId());
    }
    return true;
}

bool Transfer::markCompleted(bool showInHistory
    ,  const QString& replaceId , const QString& resultUri) {
	checkInterface()
	commit();
	d_ptr->data.status = TransferStatusDone;
	d_ptr->data.showInHistory = showInHistory;
	d_ptr->data.replaceId = replaceId;
    d_ptr->data.resultUri = resultUri;
	d_ptr->interface->markCompleted(transferId(), showInHistory, replaceId,
    	resultUri);
	return true;
}

bool Transfer::markCompletedTemporary(const QString& resultUri,
            const QString& resultMimeType,  bool removeWhenCleared) {
	checkInterface()
	commit();
	d_ptr->data.status = TransferStatusDone;
	d_ptr->data.showInHistory = true;
	d_ptr->data.replaceId = QString();
    d_ptr->data.resultUri = resultUri;
	d_ptr->interface->markCompletedTemporary(transferId(), resultUri,
    	resultMimeType, removeWhenCleared);
	return true;

}

bool Transfer::setCancelButtonText(const QString& text) {
    d_ptr->data.cancelButtonText = text;
    addToCommit("setCancelButtonText", text)
    checkInterface()
    d_ptr->interface->setCancelButtonText(transferId(), text);
    return true;
}

void Transfer::cancelSlot () {
    Q_EMIT(cancel());
}

void Transfer::startSlot () {
    Q_EMIT(start());
}

void Transfer::pauseSlot () {
    Q_EMIT(pause());
}

void Transfer::errorRepairSlot() {
    Q_EMIT(repairError());
}

bool Transfer::setTransferType(Client::TransferType transferType) {
    checkInterface()
    d_ptr->data.method = (TransferType)transferType;
    d_ptr->interface->setTransferType (transferId(), (int)transferType);
    return true;
}

void Transfer::tuiLaunched() {
    waitForCommit();
    // Set/clear canPause flag
    setCanPause (d_ptr->data.canPause);

    //set Name
    setName(d_ptr->data.name);

    // Set total size
    if (d_ptr->data.bytes > 0) {
        setSize (d_ptr->data.bytes);
    }

    //set method
    setTransferType((Client::TransferType)d_ptr->data.method);

    //set Targetname
    if (d_ptr->data.targetName.isEmpty () == false) {
        setTargetName(d_ptr->data.targetName);
    }

    // Send total file count
    setFilesCount(d_ptr->data.filesCount);

    //first set the type icon
    if (d_ptr->data.fileTypeIcon.isEmpty() == false) {
        setIcon(d_ptr->data.fileTypeIcon);
    }

    if (d_ptr->data.thumbnailMimeType.isEmpty() == false) {
		if ( d_ptr->data.thumbnailFile.isEmpty() == false) {
	        setThumbnailForFile(d_ptr->data.thumbnailFile,
		        d_ptr->data.thumbnailMimeType);
		}
    } else {
        if(d_ptr->data.thumbnailFile.isEmpty() == false) {
            setImageFromFilePath(d_ptr->data.thumbnailFile);
        }
    }

    setCurrentFileIndex(d_ptr->data.currentFileIndex);

    if (d_ptr->data.cancelButtonText.isEmpty() == false) {
        setCancelButtonText(d_ptr->data.cancelButtonText);
    }

    commit();
    switch (d_ptr->data.status) {
        case TransferStatusInactive:
        {
            setPending (d_ptr->data.message);
            break;
        }

        case TransferStatusActive:
        {
            d_ptr->interface->started (transferId(), d_ptr->data.progressData);
            d_ptr->interface->setProgress(transferId(),
                d_ptr->data.progressData);
            if (d_ptr->data.estimateTime > 0) {
                setEstimate (d_ptr->data.estimateTime);
            }

            break;
        }

        case TransferStatusError:
        {
            QString header = d_ptr->data.headerMsg;
            QString detail = d_ptr->data.detailMsg;
             if (d_ptr->data.canRepair) {
                d_ptr->interface->setRepairableError (transferId(), header,
                    detail, d_ptr->data.actionName);
             } else {
                d_ptr->interface->setNonRepairableError(transferId(), header,
                    detail);
             }

             break;
        }
        case TransferStatusCancel:
        {
            markCancelled ();
            break;
        }

        case TransferStatusPaused:
        {
            markPaused ();
            break;
        }

        case TransferStatusResume:
        {
            markResumed ();
            break;
        }

        case TransferStatusDone:
        {

			markCompleted(d_ptr->data.showInHistory, d_ptr->data.replaceId);
            break;
        }

        default:
            // do nothing
            break;
    }

}

void Transfer::sendLastProgress() {
    if(d_ptr->interface != 0) {
        if(TransferStatusActive == d_ptr->data.status) {
            qDebug() << "Send Last Progress";
            d_ptr->interface->setProgress(transferId(),
                d_ptr->data.progressData);
            d_ptr->lastProgressSentTime.restart();
        }
    }
}

bool Transfer::setThumbnailForFile(const QString& fileName, const QString&
    mimeType) {

    d_ptr->data.thumbnailFile = fileName;
    d_ptr->data.thumbnailMimeType = mimeType;

    QStringList arguments;
    arguments << fileName << mimeType;
    addToCommit("setThumbnailForFile", arguments)

    checkInterface()
    d_ptr->interface->setThumbnailForFile(transferId(), fileName, mimeType);
    return true;
}

bool Transfer::setIcon(const QString& iconId) {
    d_ptr->data.fileTypeIcon = iconId;
    addToCommit("setIcon", iconId)
    checkInterface()
    d_ptr->interface->setIcon(transferId(), iconId);
    return true;
}

void Transfer::waitForCommit() {
    d_ptr->waitForCommit();
}

bool Transfer::commit() {
    return d_ptr->commit();
}

// --- Private class functions -----------------------------------------------
TransferPrivate::TransferPrivate (Client *client) : interface(0) ,
    clientPtr(client) , flagWaitForCommit(false) {

}


void TransferPrivate::sendProgress(double valueDelta, int timeDelta, double
    done) {
    bool sendProgress = false;
    double lastSentValue = data.progressData;
    qDebug() << "sendProgress: done = " << done << "lastSentValue " <<
        lastSentValue;

    if (done == lastSentValue) {
        return;
    }

    if (done >= 1.0) {
        sendProgress = true;
    } else {
        if (done < lastSentValue) {
            sendProgress = true;
        } else {
            int timeElapsed = lastProgressSentTime.elapsed();
            qDebug() << "timeElapsed = " << timeElapsed << "valueDelta = " <<
                valueDelta << "timeDelta = " << timeDelta;
            if((done - lastSentValue) > valueDelta) {
                sendProgress = true;
            }
            if(timeElapsed > timeDelta) {
                sendProgress = true;
            }
        }
    }

    if(sendProgress == true) {
        qDebug() << "sendProgress is true - sending and resetting everything else";
        interface->setProgress(id, done);
        data.progressData = done;
        lastProgressSentTime.restart();
    }
}

void TransferPrivate::waitForCommit() {
    if(flagWaitForCommit != true) {
        flagWaitForCommit = true;
        if(keyValues.count() == 0 ) {
            keyValues.clear();
        } else {
            qDebug() << "There are more function !!! some thing wrong";
        }
    } else {
        qDebug() << "Already waiting for commit";
    }
}

bool TransferPrivate::commit() {
    bool retVal = false;
    if(keyValues.count() < 1) {
        qDebug() << "No functions to commit";
        retVal = false;
    } else {
        interface->setValues(id, keyValues);
        keyValues.clear();
        flagWaitForCommit = false;
        retVal = true;
    }
    return retVal;
}

void TransferPrivate::addFunctionToCommit(const QString& functionKeyName, QVariant
    arguments) {
    keyValues.insert(functionKeyName, arguments);
}

bool TransferPrivate::isWaitForCommitSet() const {
    return flagWaitForCommit;
}
