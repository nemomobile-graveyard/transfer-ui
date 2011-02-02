 
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




#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QMutex>
#include <QElapsedTimer>

/**
 * @brief Simple Qt logging redirector class.
 *
 * This class is used to redirect normal Qt debug log messages from stdout into a given file.
 * The messages are appended to the file and prefixed with a date and log level. The purpose of this
 * class is to make logging available for applications where stdout isn't normally visible.
 **/
class Logger {

public:

    /*!
      \brief Create new logger instance
      \param name Name of log. By default timestamp.
     */
    Logger (const QString & name = QString());

    /*!
      \brief Destructor, closes the log file.
     */
    virtual ~Logger ();

    /**
     * Enables filtering of so called "crap" that should not be logged. This can be enabled to filter out stuff
     * that makes debugging hard because of flooding. All strings in @p patterns are checked against each logged message
     * and a case sensitive match means that the message is not logged. By default no patterns or filtering are used.
     * Set to an empty list to disable.
     *
     * @param patterns the patterns to check against.
     **/
    void setFilter (const QStringList & patterns);


private:

    /**
     * The actual message handler method. This method will get called for each logged @p msg. The log @p type
     * is mapped to a string and prefixed, along with the current time. The @p msg is simply appended to a log file.
     *
     * @param type the log level type.
     * @param msg the message to be logged.
     **/
    static void messageHandler (QtMsgType type, const char *msg);

    //! an instance pointer to the class
    static Logger * m_instance;

    //! the stream we're writing to
    QTextStream m_stream;

    //! an open file where the above stream writes
    QFile m_file;

    //! a list of log level strings
    QStringList m_log_levels;

    //! the old msg handler
    QtMsgHandler m_old_msg_handler;

    //! a list of patterns to filter out
    QStringList m_patterns;
    
    //! mutex to make code thread safe
    QMutex m_mutex;
    
    //! list keeping track of all threads
    QList<Qt::HANDLE> m_threads;

    //! timer used to get timestamps
    QElapsedTimer m_time;
};

#endif // #ifndef _LOGGER_H_
