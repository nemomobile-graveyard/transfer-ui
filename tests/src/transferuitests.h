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
