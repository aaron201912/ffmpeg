/**
 * \file mtrackbar.h
 * \author Wangjian<wangjian@minigui.org>
 * \date 2009/03/01
 *
 * This file includes the definition of mTrackBar control.
 *
 \verbatim

    This file is part of mGNCS, a component for MiniGUI.

    Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.

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

#ifdef _MGNCSCTRL_TRACKBAR

#ifndef _MGUI_NCSCTRL_TRACKBAR_H
#define _MGUI_NCSCTRL_TRACKBAR_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Control_TrackBar mTrackBar
 * @{
 */

/*
 * \def NCSCTRL_TRACKBAR
 * \brief the name of TrackBar control
*/
#define NCSCTRL_TRACKBAR   NCSCLASSNAME("trackbar")

typedef struct _mTrackBar            mTrackBar;
typedef struct _mTrackBarClass       mTrackBarClass;
typedef struct _mTrackBarRenderer    mTrackBarRenderer;

#define mTrackBarHeader(Class) \
    mSliderHeader(Class)

/**
 * \struct mTrackBar
 * \brief the TrackBar struct of TrackBar control, derived from \ref mSlider.
 *
 *  \sa mTrackBarRenderer, mTrackBarClass
 */
struct _mTrackBar
{
	mTrackBarHeader(mTrackBar)
};

#define mTrackBarClassHeader(clsName, parentClass) \
	mSliderClassHeader(clsName, parentClass)

/**
 * \struct mTrackBarClass
 * \brief the VTable of \a mTrackBar, derived from \ref mSliderClass.
 *
 * \sa mSliderClass
 */
struct _mTrackBarClass
{
	mTrackBarClassHeader(mTrackBar, mSlider)
};

/**
 * \var g_stmTrackBarCls
 * \brief the instance of mTrackBarClass
 *
 * \sa mTrackBarClass
 */
MGNCS_EXPORT extern mTrackBarClass g_stmTrackBarCls;

#define mHTrackBar mTrackBar
#define mHTrackBarClass mTrackBarClass
#define g_stmHTrackBarCls  g_stmTrackBarCls
#define mVTrackBar mTrackBar
#define mVTrackBarClass mTrackBarClass
#define g_stmVTrackBarCls  g_stmTrackBarCls


#define mTrackBarRendererHeader(clsName, parentClass) \
	mSliderRendererHeader(clsName, parentClass)

/**
 * \struct mTrackBarRenderer
 * \brief TrackBar class's Renderer interface, derived from \ref mSliderRenderer.
 *
 * \sa mTrackBar, mTrackBarClass
 */
struct _mTrackBarRenderer
{
	mTrackBarRendererHeader(mTrackBar, mSlider)
};

enum mTrackBarCalcArea {
	NCSTRACKBAR_AREA_TRICK = 0x10
};


/**
 * \def NCSP_TRKBAR_MAXPOS
 * \brief The value of the maximum position.
 */
#define NCSP_TRKBAR_MAXPOS		NCSP_SLIDER_MAXPOS
/**
 * \def NCSP_TRKBAR_MINPOS
 * \brief The value of the minimum position.
 */
#define NCSP_TRKBAR_MINPOS		NCSP_SLIDER_MINPOS
/**
 * \def NCSP_TRKBAR_CURPOS
 * \brief The value of the current position.
 */
#define NCSP_TRKBAR_CURPOS		NCSP_SLIDER_CURPOS
/**
 * \def NCSP_TRKBAR_LINESTEP
 * \brief The line step length.
 */
#define NCSP_TRKBAR_LINESTEP	NCSP_SLIDER_LINESTEP
/**
 * \def NCSP_TRKBAR_PAGESTEP
 * \brief The page step length.
 */
#define NCSP_TRKBAR_PAGESTEP	NCSP_SLIDER_PAGESTEP

/**
 * \enum mTrackBarProp
 * \brief The properties of mTrackBar
 */
enum mTrackBarProp{
    /** */
	NCSP_TRKBAR_THUMB_WIDTH = NCSP_SLIDER_MAX + 1,
    /** */
	NCSP_TRKBAR_THUMB_HEIGHT,
    /**
     * The maximum value of mTrackBar properties.
     */
	NCSP_TRKBAR_MAX
};

/**
 * \var NCSP_TRKBAR_THUMB_WIDTH
 * \brief the width of thumb
 */

/**
 * \var NCSP_TRKBAR_THUMB_HEIGHT
 * \brief the height of thumb
 */


/**
 * \def NCSS_TRKBAR_HORIZONTAL
 * \brief horizontal TrackBar.
 */
#define NCSS_TRKBAR_HORIZONTAL NCSS_SLIDER_HORIZONTAL

/**
 * \def NCSS_TRKBAR_VERTICAL
 * \brief vertical TrackBar.
 */
#define NCSS_TRKBAR_VERTICAL   NCSS_SLIDER_VERTICAL

/**
 * \def NCSS_TRKBAR_NOTICK
 * \brief TrackBar without tick.
 */
#define NCSS_TRKBAR_NOTICK     (0x0001L<<NCSS_SLIDER_SHIFT)

#define NCSS_TRKBAR_SHIFT      NCSS_SLIDER_SHIFT + 1

/**
 * \def NCSN_TRKBAR_CHANGED
 * \brief positon changed notify code.
 */
#define	NCSN_TRKBAR_CHANGED    NCSN_SLIDER_CHANGED

/**
 * \def NCSN_TRKBAR_REACHMAX
 * \brief reached the maximum position notify code.
 */
#define	NCSN_TRKBAR_REACHMAX   NCSN_SLIDER_REACHMAX

/**
 * \def NCSN_TRKBAR_REACHMIN
 * \brief reached the minimum position notify code.
 */
#define	NCSN_TRKBAR_REACHMIN   NCSN_SLIDER_REACHMIN

/**
 * \def NCSN_TRKBAR_STARTDRAG
 * \brief start drag notify code.
 */
//#define	NCSN_TRKBAR_STARTDRAG  NCSN_SLIDER_STARTDRAG

/**
 * \def NCSN_TRKBAR_STOPDRAG
 * \brief stop drag notify code.
 */
//#define	NCSN_TRKBAR_STOPDRAG   NCSN_SLIDER_STOPDRAG

#define	NCSN_TRKBAR_MAX        NCSN_SLIDER_MAX


#define HORZ_SLIDER_W           24
#define HORZ_SLIDER_H           12

#define VERT_SLIDER_W           12
#define VERT_SLIDER_H           24

#define VERT_TRACK_W            6
#define HORZ_TRACK_H            6

#define LEN_TICK                4
#define GAP_TICK_SLIDER         6

/** @} end of Control_TrackBar*/

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_TRACKBAR_H */
#endif //_MGNCSCTRL_TRACKBAR
