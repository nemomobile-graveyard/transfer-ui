
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

/*!
  \file showdetailsinterface.h
  \brief Interface to be implemented if custom tranfer dialog is needed
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
*/

#ifndef _SHOW_DETAILS_INTERFACE_H_
#define _SHOW_DETAILS_INTERFACE_H_
#include <QObject>

namespace TransferUI {
	/*!
		\class ShowDetailsInterface
		\brief Interface for client applications to show custom dialog
        \author Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
        \copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
        \license GNU Lesser General Public License, version 2.1
	*/
	class ShowDetailsInterface : public QObject
	{
	public:
		/*!
			\brief show custom details dialog. transfer-ui invokes this method
			when the user clicks on the transfer, if the transfer is registered
			to display custom dialog.
			\param transferId transfer id for which details dialog should be
			displayed
		*/
		virtual void showDetailsDialog(const QString& transferId) = 0;

        /*!
            \brief show custom dialog.
            \param transferId transfer id for which details dialog should be
            displayed.
            \param windowId window id of the transfer-ui, which will be used
            chain tasking
            \param transferId transfer id for which details dialog should be
            displayed.
        */
        virtual void showDetailsDialog(const QString& transferId, int windowId){
            Q_UNUSED (transferId)
            Q_UNUSED (windowId)
        }
	};
}
#endif
