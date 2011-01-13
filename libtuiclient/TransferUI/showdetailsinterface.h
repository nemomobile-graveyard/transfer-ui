
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
#ifndef _SHOW_DETAILS_INTERFACE_H_
#define _SHOW_DETAILS_INTERFACE_H_
#include <QObject>

namespace TransferUI {
	/*!
		\class ShowDetailsInterface
		\brief Interface for client applications to show custom dialog
        \author  Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
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
	};
}
#endif