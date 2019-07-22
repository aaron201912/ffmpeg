/*
 * \file mimgnavbar.h
 * \author miniStudio team of FMSoft
 * \date 2010/10/09
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
#ifndef __MGNCS4TOUCH_INB_H__
#define __MGNCS4TOUCH_INB_H__

/**
 * \defgroup ControlImgNavBar mImgNavBar
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \def NCSCTRL_IMGNAVBAR
 * \brief the name of imgnavbar control
 */
#define NCSCTRL_IMGNAVBAR          NCSCLASSNAME("imgnavbar")

typedef struct _mImgNavBar         mImgNavBar;
typedef struct _mImgNavBarClass    mImgNavBarClass;
typedef struct _mImgNavBarRenderer mImgNavBarRenderer;

/**
 * \define mImgNavBarStyle
 * \brief the style id of \ref mImgNavBar
 */
#define NCSS_INB_SHIFT      (NCSS_WIDGET_SHIFT + 0)

/**
 * \enum mImgNavBarProp
 * \brief the properties id of \ref mImgNavBar
 *
 * \var NCSP_INB_LBITMAP
 * \brief Placed on the left of the picture.
 *
 * \var NCSP_INB_RBITMAP
 * \brief Placed on the right of the picture.
 *
 * \var NCSP_INB_FONT
 * \brief Set the font size on the navigation bar.
 *
 * \var NCSP_INB_MARGIN
 * \brief Left and right ends of the blank reserved for control.
 *
 * \var NCSP_INB_LEFT_W
 * \brief The width of the left image.
 *
 * \var NCSP_INB_RIGHT_W
 * \brief The width of the right image.
 *
 * \var NCSP_INB_IMG_H
 * \brief Left and right images of the high.
 *
 * \var NCSP_INB_DRAWMODE
 * \brief About the image display modes.
 */
enum mImgNavBarProp {
    NCSP_INB_LBITMAP = NCSP_WIDGET_MAX + 1,
    NCSP_INB_RBITMAP,
    NCSP_INB_FONT,
    NCSP_INB_MARGIN,
    NCSP_INB_LEFT_W,
    NCSP_INB_RIGHT_W,
    NCSP_INB_IMG_H,
    NCSP_INB_DRAWMODE,
    NCSP_INB_MAX
};

#define mImgNavBarHeader(clsName) \
    mWidgetHeader(clsName)        \
    PLOGFONT caption_font;        \
    int margin_w;                 \
    int leftbt_w;                 \
    int rightbt_w;                \
    int img_h;

/**
 * \struct mImgNavBar
 * \brief ImgNavBar class, derived from \ref mWidget
 *
 * \sa \ref mWidget
 */
struct _mImgNavBar {
    mImgNavBarHeader(mImgNavBar)
};

#define mImgNavBarClassHeader(clsName, parentClass) \
    mWidgetClassHeader(clsName, parentClass)

/**
 * \struct mImgNavBarClass
 * \brief  toolcaption struct of toolcaption control, derived from \ref mWidgetClass
 *
 * \sa \ref mWidgetClass
 */
struct _mImgNavBarClass {
    mImgNavBarClassHeader(mImgNavBar, mWidget)
};

#define mImgNavBarRendererHeader(clsName, parentClass) \
    mWidgetRendererHeader(clsName, parentClass)

/**
 * \struct mImgNavBarRenderer
 * \brief ImgNavBar class renderer interface, derived from \ref mWidgetRenderer
 *
 * \sa mImgNavBar, mImgNavBarClass, mWidgetRenderer
 */
struct _mImgNavBarRenderer {
    mImgNavBarRendererHeader(mImgNavBar, mWidget)
};

/**
 * \var g_stmImgNavBarCls
 * \brief global mImgNavBarClass
 */
MTOUCH_EXPORT extern mImgNavBarClass g_stmImgNavBarCls;

/**
 * \enum mImgNavBarData
 * \brief define the clicked add
 *
 * \var NCSD_INB_LEFT
 * \brief Left image click event listener param.
 *
 * \var NCSD_INB_RIGHT
 * \brief Right image click event listener param.
 */
enum mImgNavBarData {
    NCSD_INB_LEFT = 0,
    NCSD_INB_RIGHT,
};

/**
 * \enum eImgNavBarNotify
 * \brief the notification code of \ref mImgNavBar
 *
 * \var NCSN_IBN_IMGCLICK
 * \brief Around both ends of the image click event listener.
 */
enum mImgNavBarNotify {
    NCSN_INB_IMGCLICK = NCSN_WIDGET_MAX + 1,
    NCSN_INB_MAX,
};

/**
 * \enum mImgNavBarBoxLayoutPieceIndexID
 * \brief the piece index id of \ref mImgNavBar
 *
 * \var INB_LEFT_PIECE_ID
 * \brief The left piece of the index id.
 *
 * \var INB_RIGHT_PIECE_ID
 * \brief The right piece of the index id.
 */
enum mImgNavBarBoxLayoutPieceIndexID {
    INB_LEFT_PIECE_ID = 0,
    // 1 is the text piece (NULL)
    INB_RIGHT_PIECE_ID = 2,
    INB_MAX_PIECE_ID,
};

#define GetLeftPiece(self)   _M((mHBoxLayoutPiece *)self->body, getCell, INB_LEFT_PIECE_ID)
#define GetRightPiece(self)  _M((mHBoxLayoutPiece *)self->body, getCell, INB_RIGHT_PIECE_ID)

/**
 * @} end of ControlImgNavBar
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __MGNCS4TOUCH_INB_H__ */

