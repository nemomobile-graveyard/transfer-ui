
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








#include "tuiservice.h"
#include "logger.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QSettings>
#include <QCoreApplication>

int main(int argc, char **argv)
{

    QCoreApplication::setApplicationName (QLatin1String ("Transfer UI"));
    QCoreApplication::setOrganizationName (QLatin1String ("MeeGo"));    

    QString logFileInfo = QDir::homePath();
    logFileInfo.append(QLatin1String("/.transferui/log"));
    bool enableLogger = QFile::exists(logFileInfo);
    Logger *logger = 0;
    if(enableLogger == true ) {
        // Let's use custom logging
        logger = new Logger();
    }

    bool showUI = false;
    for (int i = 1; i < argc; ++i) {
        QString param = argv[i];
        if (param == "--showUI") {
            qDebug() << "Show Transfer UI ";
            showUI = true;
        }

        else if (param == "--help") {
            qDebug() << "Transfer UI";
            qDebug() << "--showUI\tShow the Transfer UI";
            qDebug() << "--help\tThis help";
            exit(0);
        }
        else {
            qWarning() << "Unknown parameter:" << argv[i];
        }
    }

    //create a service
    TUIService *tuiService = new TUIService();


    //load the implementation plugin
    tuiService->loadImplementationPlugin();

    //get the application from the plugin
    QApplication *app = tuiService->applicationInstance(argc,argv);
    qDebug() << __FUNCTION__ << app;

    //init dbus functionalities
    qDebug() << __FUNCTION__ << "Initialize Dbus";
    tuiService->init();

    int retVal = 0;
    if(app != 0) {
        app->setQuitOnLastWindowClosed(false);

        if(showUI == true) {
            tuiService->showUI();
        }
        retVal = app->exec();

        tuiService->disconnect();
        qDebug() << "Deleting the service";
        delete tuiService;
        qDebug() << "Deleting the application";
        delete app;
    } else {
        qDebug() << "Plugin returned null application pointer";
    }
    qDebug() << "Clean shutdown of the application";
    delete logger;
    return retVal;
}
