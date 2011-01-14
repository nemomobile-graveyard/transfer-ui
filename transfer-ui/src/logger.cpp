 
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




#include "logger.h"
#include <QTime>
#include <stdlib.h>
#include <iostream>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QThread>
#include <QDesktopServices>

// static initialization
Logger * Logger::m_instance = 0;

Logger::Logger (const QString & name) {

    QString myName = name;
    if (myName.isEmpty()) {
        myName = QDateTime::currentDateTime().toString (Qt::ISODate);
        myName.append ("-");
        myName.append (QString::number (QCoreApplication::applicationPid (),
            10));
    }

    //Build path
    QString logPath = QDesktopServices::storageLocation (
        QDesktopServices::DataLocation);
    
    //Make directories
    QDir dir;    
    if (dir.exists (logPath) == false) {
        dir.mkpath (logPath);
        std::cout << "Path " << logPath.toStdString() << " created for the logs"
            << std::endl;        
    }
    
    logPath.append("/");
    logPath.append (myName);
    logPath.append (".log");
    
    m_file.setFileName (logPath);
    if (m_file.exists()) {
        if (m_file.remove()) {
            std::cout << "Identical log file removed" << std::endl;
        }
    }

    std::cout << QCoreApplication::applicationName().toStdString()
        << " logs will be written to " << logPath.toStdString() << std::endl;

    // save ourself in a static variable
    Logger::m_instance = this;

    // log levels
    m_log_levels << "debug" << "warn " << "crit" << "fatal";


    // open the file for appending
    if ( ! m_file.open( QIODevice::WriteOnly | QIODevice::Append ) ) {
        // failed to open...
        std::cerr << "Logger::Logger: failed to open log file:"
            << qPrintable(logPath) << " for appending" << std::endl;
        m_old_msg_handler = 0;
        return;
    }

    // set a stream to the file too
    m_stream.setDevice( &m_file );

    // register ourselves as a debug message handler
    m_old_msg_handler = qInstallMsgHandler( Logger::messageHandler );
    
    Logger::m_instance->m_time.start();
}


Logger::~Logger () {

    m_mutex.lock();
    // According to the trolltech documentation, m_old_msg_handler = 0 means
    // that default message handler will be reinstalled. Hence no need to check
    // for m_old_msg_handler being non-0 here.
    qInstallMsgHandler(m_old_msg_handler);
    if ( m_file.isOpen() ) {
        m_file.close();
    }

    Logger::m_instance = 0;
    m_mutex.unlock();
}


void Logger::setFilter (const QStringList & patterns) {
    m_mutex.lock();
    m_patterns = patterns;
    m_mutex.unlock();    
}


void Logger::messageHandler (QtMsgType type, const char *msg) {

    Q_ASSERT (Logger::m_instance != 0);
    
    Logger::m_instance->m_mutex.lock();

    // check the patterns to see if we should filter out the message
    if ( ! Logger::m_instance->m_patterns.isEmpty() ) {
        QString qmsg(msg);

        for ( int index = 0; index < Logger::m_instance->m_patterns.size(); ++index) {
            if ( qmsg.contains(Logger::m_instance->m_patterns[index]) ) {
                 Logger::m_instance->m_mutex.unlock();
                // don't log
                return;
            }
        }
    }

    Qt::HANDLE threadHandle = QThread::currentThreadId();
    if (Logger::m_instance->m_threads.indexOf (threadHandle) == -1) 
        Logger::m_instance->m_threads.append (threadHandle);

    // write to the log file
    Logger::m_instance->m_stream
            << Logger::m_instance->m_time.elapsed()
            << " [" << Logger::m_instance->m_log_levels[type] << ']'
            << Logger::m_instance->m_threads.indexOf (threadHandle) << ": "
            << msg << endl;
            
    Logger::m_instance->m_mutex.unlock();
}
