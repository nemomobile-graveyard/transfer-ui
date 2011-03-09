
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








#ifndef _TRANSFER_UI_IMPLEMENTATION_INTERFACE_H_
#define _TRANSFER_UI_IMPLEMENTATION_INTERFACE_H_

#include <QModelIndex>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include "transferenums.h"
#include <QApplication>

/*!
    \class TransferUIImplementationInterface
    \brief Interface to manage UI. This class provides methods and signal to
    manage UI.
    \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
*/

class TransferUIImplementationInterface
{
public:
    virtual ~TransferUIImplementationInterface() { }
    /*!
        \brief function to load UI
    */
    virtual void loadUI() = 0;

    /*!
        \brief function to unload UI
    */
    virtual void unloadUI() = 0;

    /*!
        \brief show details of the transfer.
        \param index model index of the item for which details should be shown.
    */
    virtual void showDetails (const QModelIndex& index) = 0;


    /*!
        \brief set history visibility. This function hides / displays the
        history list.
        \param value , set visible if the value is true, else false.
    */
    virtual void setHistoryVisibility(bool value) = 0;


    /*!
        \brief show No Active or Completed Transfers.
        \param value, set visible if the value is true, else false
    */
    virtual void setNoTransfersVisibility(bool value) = 0;

    /*!
        \brief set data model for the tui main list view. dataChanged signal
        should be used to get the UI changes. Enums TUIAtributes &
        TUIStateChanges provided as column of the model index identifies the
        changes in the element.
        \param dataModel data model for the main list view.

    */
    virtual void setDataModel(QAbstractItemModel *dataModel) = 0;

    /*!
        \brief display non system banners
        \param event eunumeration describing nature of the event which should be
        displayed in the banner
    */
    virtual void displayBanner(TransferUI::BannerEvent event,
        const QString& message=QString()) = 0;


    /*!
        \brief returns the application instance from the plugin
        \param argc
        \param argv
    */
    virtual QApplication* applicationInstance(int &argc, char **argv) = 0;

protected:
    /*!
        \brief When user pauses the transfer, paused signal is emitted.
        \param data TUI data of the transfer.
    */
    virtual void paused(const QModelIndex& index) = 0;

    /*!
        \brief When user resumes the paused transfer, resumed signal is emitted.
        \param data TUI data of the transfer.
    */
    virtual void resumed(const QModelIndex& index) = 0;

    /*!
        \brief When user cancels the transfer, canceled signal is emitted.
        \param data TUI data of the transfer.
    */
    virtual void canceled(const QModelIndex& index) = 0;

    /*!
        \brief When user repairs an error transfer, repait signal is emitted.
        \param data TUI data of the transfer.
    */
    virtual void repair(const QModelIndex& index) = 0;

    /*!
        \brief signal emitted when a transfer is clicked.
        \param data TUI data of the transfer.
    */
    virtual void elementClicked(const QModelIndex& index) = 0;

    /*!
        \brief signal emitted when the visibility of the ui changes.
        \param value true if the ui is visible, else false
    */
    virtual void visibilityChanged(bool value) = 0;

    /*!
        \brief signal emitted when user clears the completed transfers.
    */
    virtual void clearCompletedList() = 0;

    /*!
        \brief signal emitted when the application moves to recent views
    */
    virtual void movedToSwitcher(bool value) = 0;
};

Q_DECLARE_INTERFACE(TransferUIImplementationInterface,
    "com.meego.transfer-ui.interface")
#endif
