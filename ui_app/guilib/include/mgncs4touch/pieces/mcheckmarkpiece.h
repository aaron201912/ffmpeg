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

#ifndef _MGNCS_CHECKMARKPIECE_H
#define _MGNCS_CHECKMARKPIECE_H

#include <mgplus/mgplus.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

enum mCheckMarkPieceState {
    NCSN_CHECKMARKPIECE_OFF = (USER_PIECE_EVENT_BEGIN + 0x200),
    NCSN_CHECKMARKPIECE_ON,
};

enum mCheckMarkPieceProps {
    NCSP_CHECKMARK_COLOR = USER_PIECE_PROP_BEGIN + 0x810,
};

typedef struct _mCheckMarkPiece mCheckMarkPiece;
typedef struct _mCheckMarkPieceClass mCheckMarkPieceClass;

#define mCheckMarkPieceHeader(clss) \
	mStaticPieceHeader(clss) \
    enum mCheckMarkPieceState state; \
    ARGB color; \
    HGRAPHICS hgs; \
    HBRUSH brush; \
    int hgs_width; \
    int hgs_height;

struct _mCheckMarkPiece
{
	mCheckMarkPieceHeader(mCheckMarkPiece)
};

#define mCheckMarkPieceClassHeader(clss, superCls) \
	mStaticPieceClassHeader(clss, superCls) \
    int (*getState)(clss*); \
    void (*setState)(clss*, enum mCheckMarkPieceState state); \
    void (*changeState)(clss*); \
    void (*resetState)(clss*);

struct _mCheckMarkPieceClass
{
	mCheckMarkPieceClassHeader(mCheckMarkPiece, mStaticPiece)
};

MGNCS_EXPORT extern mCheckMarkPieceClass g_stmCheckMarkPieceCls;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MGNCS_CHECKMARKPIECE_H */
