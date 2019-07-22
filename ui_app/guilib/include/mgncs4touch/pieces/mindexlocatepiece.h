/*
 * \file 
 * \author FMSoft
 * \date 
 *
 \verbatim

    This file is part of mGNCS4Touch, one of MiniGUI components.

    Copyright (C) 2008-2018 FMSoft (http://www.fmsoft.cn).

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Or,

    As this program is a library, any link to this program must follow
    GNU General Public License version 3 (GPLv3). If you cannot accept
    GPLv3, you need to be licensed from FMSoft.

    If you have got a commercial license of this program, please use it
    under the terms and conditions of the commercial license.

    For more information about the commercial license, please refer to
    <http://www.minigui.com/en/about/licensing-policy/>.

 \endverbatim
*/

#ifndef _MGNCS_INDEXLOCATEPIECE_H
#define _MGNCS_INDEXLOCATEPIECE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "mpanelpiece.h"

    typedef struct _mIndexLocatePiece mIndexLocatePiece;
    typedef struct _mIndexLocatePieceClass mIndexLocatePieceClass;

    enum mIndexLocatePieceEvent {
	NCSN_INDEXLOCATEPIECE_TOUCHED = USER_PIECE_EVENT_BEGIN +  0x200,
    };

#define mIndexLocatePieceHeader(clss)		\
    mPanelPieceHeader(clss)			\
    mHotPiece* parentPiece;			\
    mHotPiece* backgroundPiece;			\
    int touchedFlag;

    struct _mIndexLocatePiece
    {
	mIndexLocatePieceHeader(mIndexLocatePiece)
    };

#define mIndexLocatePieceClassHeader(clss, superCls)		\
    mPanelPieceClassHeader(clss, superCls)			\
    void (*reloadData)(clss* self);

    struct _mIndexLocatePieceClass
    {
	mIndexLocatePieceClassHeader(mIndexLocatePiece, mPanelPiece)
    };

    MGNCS_EXPORT extern mIndexLocatePieceClass g_stmIndexLocatePieceCls;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MGNCS_INDEXLOCATEPIECE_H */
