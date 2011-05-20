
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






#include "tuireadhistorythread.h"
#include "tuistructures.h"
#include "tuiserviceprivate.h"

#include <QDebug>

TUIReadHistoryThread::TUIReadHistoryThread(QSettings *settings)
	: historySettings (settings) {
    qRegisterMetaType<QSharedPointer<TUIData> >("TUIDataStructurePointer");
}

TUIReadHistoryThread::~TUIReadHistoryThread() {
    qDebug() << __FUNCTION__ ;
}

void TUIReadHistoryThread::run() {
	readReplaceHistory();
	readNonReplaceHistory();
}


void TUIReadHistoryThread::readReplaceHistory() {
	historySettings->beginGroup("Indexes");
	int totalHistory = historySettings->value("ReplaceHistory").toInt();
	historySettings->endGroup();
	if (totalHistory == 0) {
		return;
	}
	int historyRead = 0;
	historySettings->beginReadArray("ReplaceTransfers");
	for (int counter = historyRead ; counter <= totalHistory ; counter+= 10) {
		for(int readCounter = 0 ; readCounter <= 10 ;
				readCounter++, historyRead++) {
			if (historyRead < totalHistory) {
				readHistoryFromDB(historyRead);
			} else {
				break;
			}
		}
		sleep(1);
	}
	historySettings->endArray();
}

void TUIReadHistoryThread::readNonReplaceHistory() {
	historySettings->beginGroup("Indexes");
	int totalHistory = historySettings->value("History").toInt();
	historySettings->endGroup();
	if (totalHistory == 0) {
		return;
	}
	int historyRead = 0;
	historySettings->beginReadArray("Transfers");
	for (int counter = historyRead ; counter <= totalHistory ; counter+= 10) {
		for(int readCounter = 0 ; readCounter <= 10 ;
				readCounter++, historyRead++) {
			if (historyRead < totalHistory) {
				readHistoryFromDB(historyRead);
			} else {
				break;
			}
		}
		sleep(1);
	}
	historySettings->endArray();
}

void TUIReadHistoryThread::readHistoryFromDB(int arrayIndex) {
    historySettings->setArrayIndex(arrayIndex);

	TUIData *data = new TUIData;
	data->name = historySettings->value("name").toString();
	data->bytes = historySettings->value("size").toLongLong();
	data->targetName = historySettings->value("target").toString();
	data->method = (TransferType)historySettings->value("type").toInt();
	data->filesCount = historySettings->value("count").toInt();
	data->completedTime = historySettings->value("time").toDateTime();
    data->startTime = historySettings->value("starttime").toDateTime();
	data->thumbnailFile = historySettings->value("thumbnailfile").toString();
	data->thumbnailMimeType = historySettings->value("mimetype").toString();
	data->fileTypeIcon = historySettings->value("iconId").toString();
    data->resultUri = historySettings->value("resulturi").toString();
	QString imageFile;
    if((data->thumbnailMimeType.isEmpty() == true)
		&& (data->fileTypeIcon.isEmpty() == true)) {
    	data->thumbnailFile = historySettings->value("imagePath").toString();
    }
    QString id = historySettings->value("transferid").toString();
    Q_EMIT(addCompletedData(id, QSharedPointer<TUIData>(data)));
}
