
/**
 \file mainpage.h
 \brief File containing main page of documentation
 \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
 \copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
 \license GNU Lesser General Public License, version 2.1
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to 
 deal in the Software without restriction, including without limitation the
 rights to use, copy, modify, merge, publish, distribute, sublicense,     
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:\n\n
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER  
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS  
 IN THE SOFTWARE.

 \mainpage Tranfer UI Qt Client API Library
 \anchor main

 \section introduction Introduction
  Transfer client API provides way for Qt based applications to request Transfer
  UI to present transfers of application to user.
  
  <table>
  <tr><th>Class</th><th>Description</th></tr>
  <tr><td>TransferUI::Client</td><td>Class used to communicate with Transfer UI. And way to
  get Transfer instances for your transfers.</td></tr>
  <tr><td>TransferUI::Transfer</td><td>Class used to set presentation parameters of transfer
  presentation</td></tr>
  </table>
 
\section getting_started Getting started
  To allow communication between your application and Transfer UI you have to
  start by creating TransferUI::Client instance. Then receive
  TransferUI::Transfer class your each transfer you want to be presented by
  Transfer UI. The set and update presentation values (name, progress,
  estimate...) of transfer via TransferUI::Transfer.

\section examples Examples
  See example pages for examples how to use this library.

*/
