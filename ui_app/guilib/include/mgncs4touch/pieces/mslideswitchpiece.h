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

#ifndef _MGNCS_SLIDESWITCHPIECE_H
#define _MGNCS_SLIDESWITCHPIECE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _mSlideSwitchPiece mSlideSwitchPiece;
typedef struct _mSlideSwitchPieceClass mSlideSwitchPieceClass;

enum mSlideSwitchPieceState {
    NCSN_SLIDESWITCHPIECE_OFF = (USER_PIECE_EVENT_BEGIN + 0x300),
    NCSN_SLIDESWITCHPIECE_ON,
};

enum mSlideSwitchPieceProps {
    NCSP_SLIDESWITCH_RADIUS = USER_PIECE_PROP_BEGIN + 0x100,
};

#define mSlideSwitchPieceHeader(clss) \
	mPanelPieceHeader(clss) \
    unsigned radius; \
    enum mSlideSwitchPieceState state; \
    mHotPiece *leftBackgroundPiece; \
    mHotPiece *leftTextPiece; \
    mHotPiece *rightBackgroundPiece;\
    mHotPiece *rightTextPiece; \
    mHotPiece *buttonPiece;

struct _mSlideSwitchPiece
{
	mSlideSwitchPieceHeader(mSlideSwitchPiece)
};

#define mSlideSwitchPieceClassHeader(clss, superCls) \
	mPanelPieceClassHeader(clss, superCls) \
    int (*getState)(clss*); \
    void (*setState)(clss*, enum mSlideSwitchPieceState state); \
    void (*changeState)(clss*); \
    void (*resetState)(clss*); \
    void (*setLeftBkgnd)(clss*, int, int, mColorTable*, mColorTable*); \
    void (*setRightBkgnd)(clss*, int, int, mColorTable*, mColorTable*); \
    void (*setButton)(clss*, int, int, mColorTable*, mColorTable*); \
    void (*setLeftText)(clss*, const char*, PLOGFONT, ARGB); \
    void (*setRightText)(clss*, const char*, PLOGFONT, ARGB);

struct _mSlideSwitchPieceClass
{
	mSlideSwitchPieceClassHeader(mSlideSwitchPiece, mPanelPiece)
};

MGNCS_EXPORT extern mSlideSwitchPieceClass g_stmSlideSwitchPieceCls;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MGNCS_SLIDESWITCHPIECE_H */
