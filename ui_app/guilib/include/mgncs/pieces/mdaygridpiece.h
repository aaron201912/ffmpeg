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

#ifndef _MGUI_NCSCTRL_DAYGRIDPIECE_H
#define _MGUI_NCSCTRL_DAYGRIDPIECE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mDayGridPieceClass mDayGridPieceClass;
typedef struct _mDayGridPiece mDayGridPiece;

#define mDayGridPieceClassHeader(clss, superCls) \
	mGridBoxPieceClassHeader(clss, superCls)  \
	CELL_POINT (*day2Cell)(clss*, int day); \
	int (*cell2Day)(clss *, int row, int col);


struct _mDayGridPieceClass
{
	mDayGridPieceClassHeader(mDayGridPiece, mGridBoxPiece)
};

MGNCS_EXPORT extern mDayGridPieceClass g_stmDayGridPieceCls;

#define mDayGridPieceHeader(clss) \
	mGridBoxPieceHeader(clss)  \
	unsigned char month_days; \
	unsigned char last_month_days; \
	unsigned char weekday_of_first_day;
	
struct _mDayGridPiece
{
	mDayGridPieceHeader(mDayGridPiece)
};

enum mDayGridPieceProps{
	NCSP_DAYGRIDPIECE_MONTH_DAYS = DAYGRIDPIECE_PROP_BEGIN,
	NCSP_DAYGRIDPIECE_LASTMONTH_DAYS,
	NCSP_DAYGRIDPIECE_WEEKDAY_OF_FIRSTDAY
};

enum WeekDays{
	Sunday = 0,
	Monday,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday
};


#ifdef __cplusplus
}
#endif

#endif

