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

#ifndef __MGUI_NCSCTRL_SCROLLVIEWPIECE_H
#define __MGUI_NCSCTRL_SCROLLVIEWPIECE_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

typedef enum _ScrollViewBkMode {
    SCROLLVIEW_BKMODE_SOLID_COLOR,
    SCROLLVIEW_BKMODE_IMAGE_CENTER,
    SCROLLVIEW_BKMODE_IMAGE_SCALED,
    SCROLLVIEW_BKMODE_IMAGE_TILED,
    SCROLLVIEW_BKMODE_PATTERN,
} SCROLLVIEWBKMODE;

enum mScrollViewPieceProp
{
    NCSP_BACKGROUND_MODE = USER_PIECE_PROP_BEGIN + 1,
    NCSP_BACKGROUND_DATA,
};

typedef struct _mScrollViewPieceClass mScrollViewPieceClass;
typedef struct _mScrollViewPiece mScrollViewPiece;

#define mScrollViewPieceClassHeader(clss, superCls) \
    mPanelPieceClassHeader(clss, superCls) \
    void (*moveViewport)(clss*, int x, int y); \
    void (*getViewport)(clss*, RECT *rc); \
    void (*showScrollBar)(clss*, BOOL show); \
    void (*enableCache)(clss*, BOOL cachable);

struct _mScrollViewPieceClass
{
    mScrollViewPieceClassHeader(mScrollViewPiece, mPanelPiece)
};

MGNCS_EXPORT extern mScrollViewPieceClass g_stmScrollViewPieceCls;

#define mScrollViewPieceHeader(clss) \
    mPanelPieceHeader(clss) \
    DWORD m_timePressed; \
    BOOL m_bScrollbarAutoHided; \
    BOOL m_bNeedScrollBar; \
    BOOL m_bPressed; \
    BOOL m_bMouseMoved; \
    int m_movingStatus; /* -1, 0, 1 */ \
    unsigned int m_mouseFlag; /* 1: move, 2: cancel animation */ \
    BOOL m_bTimedout; \
    BOOL m_cachable; \
    HDC m_cache; \
    POINT m_pressMousePos; \
    POINT m_oldMousePos; \
    float m_ratioX; \
    float m_ratioY; \
    RECT m_contentDirtyRect; \
    RECT m_cachedViewport; \
    mPieceItem *m_content; \
    mPieceItem *m_scrollbar; \
    MGEFF_ANIMATION m_animation; \
    void *m_phy_ctx; \
    int bkgnd_mode; \
    DWORD bkgnd_data;

struct _mScrollViewPiece
{
    mScrollViewPieceHeader(mScrollViewPiece)
};

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* __MGUI_NCSCTRL_SCROLLVIEWPIECE_H */
