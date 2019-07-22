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
#ifdef _MGNCSCTRL_SPINNER

#ifndef _MGUI_NCSCTRL_SPINNERPIECE_H
#define _MGUI_NCSCTRL_SPINNERPIECE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mSpinnerPieceClass mSpinnerPieceClass;
typedef struct _mSpinnerPiece mSpinnerPiece;

#define mSpinnerPieceClassHeader(clss, superCls) \
	mContainerPieceClassHeader(clss, superCls)  \
	int (*lineStep)(clss*, BOOL bPrev); \
/**protected */ \
	mHotPiece *(*getIncPiece)(clss *); \
	mHotPiece *(*getDecPiece)(clss *); \
	void (*onPosChanged)(clss *);

struct _mSpinnerPieceClass
{
	mSpinnerPieceClassHeader(mSpinnerPiece, mContainerPiece)
};

MGNCS_EXPORT extern mSpinnerPieceClass g_stmSpinnerPieceCls;

#define mSpinnerPieceHeader(clss) \
	mContainerPieceHeader(clss)  \
	int max; \
	int min; \
	int cur_pos;  \
	int line_step; \
    int loop_flag;

struct _mSpinnerPiece
{
	mSpinnerPieceHeader(mSpinnerPiece)
};

enum mSpinnerPieceProps{
	NCSP_SPNRPIECE_MAXPOS   = PIECECOMM_PROP_MAX,
	NCSP_SPNRPIECE_MINPOS   = PIECECOMM_PROP_MIN,
	NCSP_SPNRPIECE_LINESTEP = PIECECOMM_PROP_LINESTEP,
	NCSP_SPNRPIECE_CURPOS   = PIECECOMM_PROP_POS,
	NCSP_SPNRPIECE_LOOP     = SPINBOXPIECE_PROP_BEGIN
};

enum mSpinnerPieceEvents{
	NCSN_SPNRPIECE_INC = SPINBOXPIECE_EVENT_BEGIN,
	NCSN_SPNRPIECE_DEC,
	NCSN_SPNRPIECE_REACHMIN,
	NCSN_SPNRPIECE_REACHMAX,
	NCSN_SPNRPIECE_MAX
};

#ifdef __cplusplus
}
#endif

#endif
#endif //_MGNCSCTRL_SPINNER

