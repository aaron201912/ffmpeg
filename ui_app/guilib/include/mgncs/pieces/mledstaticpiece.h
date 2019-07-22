/*
 *   This file is part of mGNCS, a component for MiniGUI.
 * 
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *   Or,
 * 
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 * 
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 * 
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */
#ifdef _MGNCSCTRL_LEDLABEL

#ifndef _MGUI_NCSCTRL_LEDLABELPIECE_H
#define _MGUI_NCSCTRL_LEDLABELPIECE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mLedstaticPieceClass mLedstaticPieceClass;
typedef struct _mLedstaticPiece mLedstaticPiece;

#define mLedstaticPieceClassHeader(clss, superCls) \
	mStaticPieceClassHeader(clss, superCls)

struct _mLedstaticPieceClass
{
	mLedstaticPieceClassHeader(mLedstaticPiece, mStaticPiece)
};

MGNCS_EXPORT extern mLedstaticPieceClass g_stmLedstaticPieceCls;

#define mLedstaticPieceHeader(clss) \
	mStaticPieceHeader(clss)   \
	char *text;		\
	unsigned int width:16;		\
	unsigned int height:16;	\
	DWORD color;	\
	unsigned int format;

struct _mLedstaticPiece
{
	mLedstaticPieceHeader(mLedstaticPiece)
};

#define mLedstaticPiece_setFlag(self, bitmask) ((self)->format = (((bitmask) << 8) | (self->format & 0xFFFF00FF)))
#define mLedstaticPiece_clearFlag(self, bitmask) (((self)->format) &= (~(bitmask << 8)))
#define mLedstaticPiece_isFlag(self, bitmask) (((self->format) & (bitmask << 8)))

#define mLedstaticPiece_setAutoWrap(self, autoWrap) ((autoWrap)?mLedstaticPiece_setFlag(self, 0x1):mLedstaticPiece_clearFlag(self, 0x1))
#define mLedstaticPiece_isAutoWrap(self)  mLedstaticPiece_isFlag(self, 0x1)

enum mLedstaticPieceProps {
	NCSP_LEDLBLPIECE_TEXT = LEDLBLPIECE_PROP_BEGIN,
	NCSP_LEDLBLPIECE_COLOR,
	NCSP_LEDLBLPIECE_ALIGN = PIECECOMM_PROP_ALIGN,
	NCSP_LEDLBLPIECE_VALIGN = PIECECOMM_PROP_VALIGN,
	NCSP_LEDLBLPIECE_AUTOWRAP = PIECECOMM_PROP_AUTOWRAP,
	NCSP_LEDLBLPIECE_WIDTH,
	NCSP_LEDLBLPIECE_HEIGHT,
	NCSP_LEDLBLPIECE_GAP,
};

#ifdef __cplusplus
}
#endif

#endif
#endif //_MGNCSCTRL_LEDLABEL
