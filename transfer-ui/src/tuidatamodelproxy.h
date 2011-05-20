
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








#ifndef _TUI_DATA_MODEL_PROXY_H_
#define _TUI_DATA_MODEL_PROXY_H_

#include "tuidatamodel.h"
#include "tuistructures.h"

/*!
    \class TUIDataModelProxy
    \brief Proxy class for Transfer UI Data Model
    \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
*/
class TUIDataModelProxy : public QObject
{
    Q_OBJECT
public:
    TUIDataModelProxy(QObject *parent=0);

    virtual ~TUIDataModelProxy();

    /*!
        return the data model
    */
    TUIAbstractModel * dataModel() const;

    /*!
        \brief A new transfer to the model
        \param id transfer identifier
        \param type transfer type
        \param title transfer title
        \param clientId client identifier, by default it is empty
        \param target transfer target, by default it is empty
        \param bytes total size of the transfer, by default it is 0
    */
    void registerTransfer(const QString& id, TransferType type,
        const QString& title, const QString& clientId = QString(), const
        QString& target= QString(), double bytes=0);

    /*!
        remove the transfer from the model
    */
    void removeTransfer(const QString& id);

    /*!
        display transfers
    */
    void displayTransfers();

    /************************* Attribute changes ******************************/

    /*!
        set transfer name
    */
    void setName(const QString &id, const QString &name);

    /*!
        set transfer progress
    */
    void setProgress(const QString &id, double done);

    /*!
        set size
    */
    void setSize(const QString &id, qulonglong bytes);

    /*!
        set thumbnail
    */
    void setThumbnailForFile(const QString& id, const QString& fileName, const
        QString& mimeType);

    /*!
        set icon
    */
    void setIcon(const QString& id, const QString& iconId);

    /*!
        set can pause
    */
    void setCanPause(const QString &id, bool canPause);

    /*!
        set can send immdtly
    */
    void setCanSendNow(const QString &id, bool canSendNow);

    /*!
        set estimate
    */
    void setEstimate(const QString &id, int seconds);

    /*!
        set files count
    */
    void setFilesCount(const QString &id, int count);

    /*!
        set current file index
    */
    void setCurrentFileIndex(const QString &id, int fileIndex);

    /*!
        set message
    */
    void setMessage(const QString &id, const QString &message);

    /*!
        set transfer type
    */
    void setTransferType(const QString &id, int transferType);

    /*!
        set transfer target name
    */
    void setTargetName(const QString &id, const QString &targetName);

    /*!
        set custom cancel button text
    */
    void setCancelButtonText(const QString& id, const QString& text);

    /*!
        set custom transfer type title
    */
    void setTransferTypeTitle(const QString& id, const QString& title);


	/*!
		set icon id from the file
	*/
	void setImageFromFile(const QString& id, const QString& filePath);

    /************************** State changes *********************************/

    /*!
        handle transfer started state
    */
    void started(const QString &id, double done);

    /*!
        handle transfer pending state
    */
    void pending(const QString &id, const QString &message);

    /*!
        handle transfer paused state
    */
    void paused(const QString &id);

    /*!
        handle transfer resumed state
    */
    void resumed(const QString &id);

    /*!
        handle transfer error state
    */
    void markError(const QString &id, const QString &headerMsg,
        const QString &detailMsg, bool canRepair, const QString &actionName="");

    /*!
        handle transfer cancel
    */
    void cancelled(const QString &id);

    /*!
        handle transfer done
    */
    void done(const QString &id, const QString& resultUri);

    /*!
        handle crashed client transfers
    */
    void cleanUpTransfer(const QString &id);

    /*!
        handle transfer cancel state, user has canceled the transfer, but ack is
        due from the client
    */
    void waitToCancelled(const QString& id);

    /**************************************************************************/

    /*!
        return transfer id for the give tui data
    */
    QString transferId(const TUIData* data) const;

    /*!
        check if the transfer is present in the data model
    */
    bool transferPresent(const QString& id);

    /*!
        get tui data for the give transfer id
    */
    const TUIData * tuiData(const QString& id) const;

    /*!
        get transfer active , inactive and error count
    */
    void getTransfersCount(int& activeCount, int& inactiveCount,
        int& errorCount);

    /*!
        get active inactive error and completed transfers count
    */
    void getTransfersCount(int& activeCount, int& inactiveCount, int&
        errorCount, int& completedCount);

    /*!
        get the total completed transfers count
    */
    int completedCount() const;

	/*!
		clear completed Transfers
	*/
    void clearCompletedTransfers();

	/*!
		Add a new transfer to the data model. 
		Transfer is read from the completed internal database. 
        \param id transfer iddentifier
        \param data TUIData. Shared Pointer data
	*/

    void addTransfer(const QString& id, QSharedPointer<TUIData>);

	
	/*! 
		Date settings changed update the model
	*/
    void dateSettingsChanged();

    /*!
        \brief get total transfer count including cancel state
        \return total count of the transfers.
    */
    int count() const;

    /*!
        \brief return modelIndex for the given transfer id
        \param id transfer id for the given transfer
    */
    QModelIndex modelIndex(const QString& id) const;

private:
    TUIDataModel *model; //!< data model

    QMap<QString,const TUIData*> tuiDataMap; //!< Map for the model

    /*!
        get data for the given model index
    */
    TUIData* data(const QModelIndex& index);

    /*!
        \brief set given data for the give model index
        \param index QModelIndex of the transfer
        \param data Transfer data
    */
    void setModelData(const QModelIndex& index, TUIData* data);
};
#endif
