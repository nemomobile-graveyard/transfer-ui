
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

#include "transferuitests.h"
#include "tuiservice.h"
#include "tuistructures.h"
#include "transferenums.h"

void TransferUTTests::testAddTransfer() {
    transferId =
        service->registerTransientTransfer("Transfer Unit Test", 1
            , "com.nokia.transfertests");
    QVERIFY(transferId.isEmpty() == false);

    QVERIFY(service->model()->count() == 1);
//    TransferType method;
}

void TransferUTTests::testTransferStatePending() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        //state pending
        QString pendingMessage = "Pending Message";
        service->pending(transferId, pendingMessage);
        QCOMPARE(TransferStatusInactive, data->status);
        QCOMPARE(pendingMessage, data->message);
    } else {
        testAddTransfer();
    }
}

void TransferUTTests::testTransferStateActive() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        double progress = 0.5;
        service->started(transferId, progress);
        QCOMPARE(progress, data->progressData);
        QCOMPARE(TransferStatusActive, data->status);
        int estimate = 300;
        service->setEstimate(transferId, estimate);
        QCOMPARE(estimate, data->estimateTime);
    } else {
        testAddTransfer();
    }
}

void TransferUTTests::testTransferStatePause() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        //State paused
        //set setCanPause
        service->paused(transferId);
        QCOMPARE(TransferStatusPaused, data->status);
    } else {
        testAddTransfer();
    }
}

void TransferUTTests::testTransferStateResume() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        //State resume
        service->resumed(transferId);
        QCOMPARE(TransferStatusResume, data->status);
    } else {
        testAddTransfer();
    }
}

void TransferUTTests::testTransferStateError() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        QString headerMsg = "Error Header";
        QString detailMsg = "Error Details";
        service->setNonRepairableError(transferId, headerMsg, detailMsg);
        QCOMPARE(headerMsg, data->headerMsg);
        QCOMPARE(detailMsg, data->detailMsg);
        QVERIFY(data->canRepair == false);

        QString actionName = "Retry Now";
        service->setRepairableError(transferId, headerMsg, detailMsg, actionName);
        QCOMPARE(headerMsg, data->headerMsg);
        QCOMPARE(detailMsg, data->detailMsg);
        QVERIFY(data->canRepair == true);
        QCOMPARE(actionName, data->actionName);
    } else {
        testAddTransfer();
    }
}

//verify progress
void TransferUTTests::testProgress() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        double progress = 0.5;
        service->setProgress(transferId, progress);
        QCOMPARE(progress, data->progressData);
    } else {
        testAddTransfer();
    }
}

void TransferUTTests::testSize() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        qlonglong transferSize = 10000;
        service->setSize(transferId, transferSize);
        QCOMPARE(transferSize, data->bytes);
    } else {
        testAddTransfer();
    }
}


void TransferUTTests::testCurrentIndex() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        int currentFileIndex = 10;
        service->setCurrentFileIndex(transferId, currentFileIndex);
        QCOMPARE(currentFileIndex, data->currentFileIndex);
    } else {
        testAddTransfer();
    }
}

void TransferUTTests::testFilesCount() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        int filesCount = 20;
        service->setFilesCount(transferId, filesCount);
        QCOMPARE(filesCount, data->filesCount);
    } else {
        testAddTransfer();
    }
}

void TransferUTTests::testCanPause() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        bool canPause = true;
        service->setCanPause(transferId, canPause);
        QCOMPARE(canPause, data->canPause);
        canPause = false;
        service->setCanPause(transferId, canPause);
        QCOMPARE(canPause, data->canPause);
    } else {
        testAddTransfer();
    }
}

void TransferUTTests::testSendNow() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        bool canSendImdtly = false;
        service->setSendNow(transferId, canSendImdtly);
        QCOMPARE(canSendImdtly, data->canSendImdtly);
        canSendImdtly = true;
        service->setSendNow(transferId, canSendImdtly);
        QCOMPARE(canSendImdtly, data->canSendImdtly);
    } else {
        testAddTransfer();
    }
}

void TransferUTTests::testName() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        QString newName = "New Name";
        service->setName(transferId, newName);
        QCOMPARE(newName, data->name);

    } else {
        testAddTransfer();
    }
}

void TransferUTTests::testMessage() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        QString newMessage = "New Message";
        service->setMessage(transferId, newMessage);
        QCOMPARE(newMessage, data->message);

    } else {
        testAddTransfer();
    }
}

void TransferUTTests::testThumbnail() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        QString thumbnailFile = "/home/user/.images/image.jpg";
        QString thumbnailMimeType = "image/jpeg";
        service->setThumbnailForFile(transferId, thumbnailFile,
            thumbnailMimeType);
        QCOMPARE(thumbnailFile, data->thumbnailFile);
        QCOMPARE(thumbnailMimeType, data->thumbnailMimeType);
    } else {
        testAddTransfer();
    }
}

void TransferUTTests::testFileTypeIcon() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        QString fileTypeIcon = "m-icon";
        service->setIcon(transferId, fileTypeIcon);
        QCOMPARE(fileTypeIcon, data->fileTypeIcon);
    } else {
        testAddTransfer();
    }
}

void TransferUTTests::testTarget() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        QString newTarget = "New Target";
        service->setTargetName(transferId, newTarget);
        QCOMPARE(newTarget, data->targetName);

    } else {
        testAddTransfer();
    }
}

void TransferUTTests::testCancelText() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        QString cancelButtonText = "Canel Download";
        service->setCancelButtonText(transferId, cancelButtonText);
        QCOMPARE(cancelButtonText, data->cancelButtonText);
    } else {
        testAddTransfer();
    }
}

void TransferUTTests::testTransferTypeString() {
    if (transferId.isEmpty() == false) {
        const TUIData *data = service->model()->tuiData(transferId);
        QString transferTitle = "New Title";
        service->setTransferTypeString(transferId, transferTitle);
        QCOMPARE(transferTitle, data->transferTitle);

    } else {
        testAddTransfer();
    }
}


void TransferUTTests::initTestCase() {
    service = new TUIService(this);
    service->init();
    service->loadImplementationPlugin();
}


QTEST_MAIN(TransferUTTests)

