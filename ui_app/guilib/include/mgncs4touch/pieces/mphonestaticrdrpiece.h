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

#ifndef _MGNCS_PHONESTATICRDRPIECE_H
#define _MGNCS_PHONESTATICRDRPIECE_H

#ifdef __cplusplus
extern "C" {
#endif
#include <mgplus/mgplus.h>
#include <minigui/common.h>

typedef struct _mPhoneStaticRDRPieceClass mPhoneStaticRDRPieceClass;
typedef struct _mPhoneStaticRDRPiece mPhoneStaticRDRPiece;

#define mPhoneStaticRDRPieceClassHeader(clss, superCls) \
	mStaticPieceClassHeader(clss, superCls)\

struct _mPhoneStaticRDRPieceClass
{
	mPhoneStaticRDRPieceClassHeader(mPhoneStaticRDRPiece, mStaticPiece)
};

MGNCS_EXPORT extern mPhoneStaticRDRPieceClass g_stmPhoneStaticRDRPieceCls;

#define mPhoneStaticRDRPieceHeader(clss) \
	mStaticPieceHeader(clss) \
    PLOGFONT main_font;     \
    PLOGFONT sub_font;      \
    RECT main_rc;           \
    RECT sub_rc;            \
    char* main_text_str;     \
    char* sub_text_str;      \
    ARGB main_font_color;\
    ARGB sub_font_color;

struct _mPhoneStaticRDRPiece
{
	mPhoneStaticRDRPieceHeader(mPhoneStaticRDRPiece)
};

#define PHONESTATICRDRPIECE_PROP_BEGIN    USER_PIECE_PROP_BEGIN + 400

enum mPhoneStaticRDRPieceProps {   
    NCSP_PHONESTATICRDRPIECE_MAIN_TEXT = PHONESTATICRDRPIECE_PROP_BEGIN,
    NCSP_PHONESTATICRDRPIECE_SUB_TEXT,
    NCSP_PHONESTATICRDRPIECE_MAIN_FONT,
    NCSP_PHONESTATICRDRPIECE_SUB_FONT,
    NCSP_PHONESTATICRDRPIECE_MAIN_COLOR,
    NCSP_PHONESTATICRDRPIECE_SUB_COLOR
};


#ifdef __cplusplus
}
#endif

#endif

