
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

#ifndef _TRANSFER_UI_TESTS
#define _TRANSFER_UI_TESTS

#include <QtTest/QtTest>
#include <QObject>
#include "tuiservice.h"
class TransferUTTests : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testAddTransfer();
    void testTransferStatePending();
    void testProgress();
    void testTransferStateActive();
    void testTransferStatePause();
    void testTransferStateResume();
    void testTransferStateError();
    void testSize();
    void testCurrentIndex();
    void testFilesCount();
    void testCanPause();
    void testSendNow();
    void testName();
    void testMessage();
    void testThumbnail();
    void testFileTypeIcon();
    void testTarget();
    void testCancelText();
    void testTransferTypeString();
    void initTestCase();
private:
    TUIService *service;
    QString transferId;
};
#endif
