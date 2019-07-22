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

#ifndef _MGNCS_PHONESEPARATORPIECE_H
#define _MGNCS_PHONESEPARATORPIECE_H

#ifdef __cplusplus
extern "C" {
#endif
#include <mgplus/mgplus.h>

typedef enum{
    PHONESEPARATORPIECE_VERT = NCS_PIECE_PAINT_VERT,
    PHONESEPARATORPIECE_HERT
}PHONESEPARATORPIECE_DIRECT_MODE_E;

typedef struct _mPhoneSeparatorPieceClass mPhoneSeparatorPieceClass;
typedef struct _mPhoneSeparatorPiece mPhoneSeparatorPiece;

#define mPhoneSeparatorPieceClassHeader(clss, superCls) \
	mStaticPieceClassHeader(clss, superCls)

struct _mPhoneSeparatorPieceClass
{
	mPhoneSeparatorPieceClassHeader(mPhoneSeparatorPiece, mStaticPiece)
};

MGNCS_EXPORT extern mPhoneSeparatorPieceClass g_stmPhoneSeparatorPieceCls;

#define mPhoneSeparatorPieceHeader(clss) \
	mStaticPieceHeader(clss) \
    DWORD darkercolor;           \
    DWORD lightercolor;\
    PHONESEPARATORPIECE_DIRECT_MODE_E direct_mode;

struct _mPhoneSeparatorPiece
{
	mPhoneSeparatorPieceHeader(mPhoneSeparatorPiece)
};


#define PHONESEPARATORPIECE_PROP_BEGIN    USER_PIECE_PROP_BEGIN + 300

enum mPhoneSeparatorPieceProps {   
    NCSP_PHONESEPARATORPIECE_DARKER_COLOR = PHONESEPARATORPIECE_PROP_BEGIN,
    NCSP_PHONESEPARATORPIECE_LIGHTER_COLOR,
    NCSP_PHONESEPARATORPIECE_DIRECT_MODE,
};

#ifdef __cplusplus
}
#endif

#endif

