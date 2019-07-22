/*
 * \file mnewtrackbar.h
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
#ifndef __MGNCS4TOUCH_NTRKBAR_H__
#define __MGNCS4TOUCH_NTRKBAR_H__

/**
 * \defgroup ControlNewTrackBar mNewTrackBar
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \def NCSCTRL_NEWTRACKBAR
 * \brief the name of newtrackbar control
 */
#define NCSCTRL_NEWTRACKBAR          NCSCLASSNAME("newtrackbar")

typedef struct _mNewTrackBar         mNewTrackBar;
typedef struct _mNewTrackBarClass    mNewTrackBarClass;
typedef struct _mNewTrackBarRenderer mNewTrackBarRenderer;

#define mNewTrackBarHeader(clsName) \
    mTrackBarHeader(clsName)

/**
 * \struct mNewTrackBar
 * \brief NewTrakBar class, derived from \ref mTrackBar
 *
 * \sa \ref mTrackBar
 */
struct _mNewTrackBar {
    mNewTrackBarHeader(mNewTrackBar)
};

#define mNewTrackBarClassHeader(clsName, parentClass) \
    mTrackBarClassHeader(clsName, parentClass)

/**
 * \struct mNewTrackBarClass
 * \brief  newtrackbar struct of newtrackbar control, derived from \ref mTrackBarClass
 *
 * \sa \ref mTrackBarClass
 */
struct _mNewTrackBarClass {
    mNewTrackBarClassHeader(mNewTrackBar, mTrackBar)
};

#define mNewTrackBarRendererHeader(clsName, parentClass) \
    mTrackBarRendererHeader(clsName, parentClass)

/**
 * \struct mNewTrackBarRenderer
 * \brief NewTrakBar class renderer interface, derived from \ref mTrackBarRenderer
 *
 * \sa mNewTrackBar, mNewTrackBarClass, mTrackBarRenderer
 */
struct _mNewTrackBarRenderer {
    mNewTrackBarRendererHeader(mNewTrackBar, mTrackBar)
};

/**
 * \var g_stmNewTrackBarCls
 * \brief global mNewTrackBarClass
 */
MTOUCH_EXPORT extern mNewTrackBarClass g_stmNewTrackBarCls;

/**
 * \define mNewTrackBarStyle
 * \brief the style id of \ref mNewTrackBar
 *
 * \var NCSS_NTRKBAR_CIRCLE
 * \brief Identified as a circular slider.
 */
#define NCSS_NTRKBAR_CIRCLE       (1 << (NCSS_TRKBAR_SHIFT))
#define NCSS_NTRKBAR_SHIFT        (1 + NCSS_TRKBAR_SHIFT)

/**
 * \enum mNewTrackBarProp
 * \brief the properties id of \ref mNewTrackBar
 */
enum mNewTrackBarProp {
    NCSP_NTRKBAR_MAX = NCSP_TRKBAR_MAX + 1,
};

#define HORZ_NTRKBAR_SLIDER_W        16
#define HORZ_NTRKBAR_SLIDER_H        16
#define VERT_NTRKBAR_SLIDER_W        16
#define VERT_NTRKBAR_SLIDER_H        16

/**
 * @} end of ControlNewTrakBar
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __MGNCS4TOUCH_NTRKBAR_H__ */

