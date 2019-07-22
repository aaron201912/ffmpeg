/**
 * \file 
 * \author 
 * \date 
 *
 * \brief 
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

#ifdef _MGNCSCTRL_RECTANGLE

#ifndef _MGUI_NCSCTRL_RECTANGLE_H
#define _MGUI_NCSCTRL_RECTANGLE_H


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup ControlRectangle mRectangle
 * @{
 */

/**
 * \def NCSCTRL_RECTANGLE
 * \brief the name of rectangle control
 */
#define NCSCTRL_RECTANGLE        NCSCLASSNAME("rectangle")

typedef struct _mRectangle mRectangle;
typedef struct _mRectangleClass mRectangleClass;
typedef struct _mRectangleRenderer mRectangleRenderer;

#define mRectangleHeader(clsName)   \
	mWidgetHeader(clsName)

/**
 * \struct mRectangle
 * \brief The control show a rectangle or round  rectangle, derived from \ref mWidget
 *
 */

struct _mRectangle
{
	mRectangleHeader(mRectangle)
};

#define mRectangleClassHeader(clsName, parentClass) \
	mWidgetClassHeader(clsName, parentClass)

/**
 * \struct mRectangleClass
 * \brief struct of rectangle control, derived from \ref mWidgetClass
 */

struct _mRectangleClass
{
	mRectangleClassHeader(mRectangle, mWidget)
};

#define mRectangleRendererHeader(clsName, parentClass) \
	mWidgetRendererHeader(clsName, parentClass)

/**
 * \struct mRectangleRenderer
 * \brief rectangle renderer interface, derived from \ref mWidgetRenderer
 */
/* define the render of image static */
struct _mRectangleRenderer {
	mRectangleRendererHeader(mRectangle, mWidget)
};

/**
 * \var g_stmRectangleCls
 * \brief global mRectangleClass
 */
MGNCS_EXPORT extern mRectangleClass g_stmRectangleCls;

/**
 * \enum mRectangleProp
 * \brief properties id of rectangle
 */

/* property */
enum mRectangleProp {
	/**
	 * \brief Set the size of rect border
	 *  - Type int
	 *  - Read Write
	 *  - set the size of rectangle border
	 *
	 */
	NCSP_RECTANGLE_BORDERSIZE = NCSP_WIDGET_MAX + 1,
	/**
	 * \brief Set the color of rect border
	 *  - Type DWORD(ARGB)
	 *  - Read Write
	 *  - set the color value of rect border
	 *
	 */
	NCSP_RECTANGLE_BORDERCOLOR,
	/**
	 * \brief Set the color to fill in rect
	 *  - Type DWORD(ARGB)
	 *  - Read Write
	 *  - set the color value to fill in rect
	 *
	 */
	NCSP_RECTANGLE_FILLCOLOR,
	/**
	 * \brief Set the x radius
	 *  - Type int
	 *  - Read Write
	 *  - set x radius of rectangle corner
	 *
	 */
	NCSP_RECTANGLE_XRADIUS,
	/**
	 * \brief Set the y radius
	 *  - Type int
	 *  - Read Write
	 *  - set y radius of rectangle corner
	 *
	 */
	NCSP_RECTANGLE_YRADIUS,
	/**
	 * \brief Set the red color to fill in rect
	 *  - Type int (0 - 255)
	 *  - Read Write
	 *  - set the color value to fill in rect
	 *
	 */
	NCSP_RECTANGLE_FILLCLR_RED,
	/**
	 * \brief Set the green color to fill in rect
	 *  - Type int (0 - 255)
	 *  - Read Write
	 *  - set the color value to fill in rect
	 *
	 */
	NCSP_RECTANGLE_FILLCLR_GREEN,
	/**
	 * \brief Set the blue color to fill in rect
	 *  - Type int (0 - 255)
	 *  - Read Write
	 *  - set the color value to fill in rect
	 *
	 */
	NCSP_RECTANGLE_FILLCLR_BLUE,
	/**
	 * \brief Set the alpha value to fill in rect
	 *  - Type int (0 - 255)
	 *  - Read Write
	 *  - set the color value to fill in rect
	 *
	 */
	NCSP_RECTANGLE_FILLCLR_ALPHA,
	/**
	 * \brief Set the red color value of the border
	 *  - Type int (0 - 255)
	 *  - Read Write
	 *  - set the color value to fill in rect
	 *
	 */
	NCSP_RECTANGLE_BRDCLR_RED,
	/**
	 * \brief Set the green color value of the border
	 *  - Type int (0 - 255)
	 *  - Read Write
	 *  - set the color value to fill in rect
	 *
	 */
	NCSP_RECTANGLE_BRDCLR_GREEN,
	/**
	 * \brief Set the blue color value of the border
	 *  - Type int (0 - 255)
	 *  - Read Write
	 *  - set the color value to fill in rect
	 *
	 */
	NCSP_RECTANGLE_BRDCLR_BLUE,
	/**
	 * \brief Set the alpha value of the border
	 *  - Type int (0 - 255)
	 *  - Read Write
	 *  - set the color value to fill in rect
	 *
	 */
	NCSP_RECTANGLE_BRDCLR_ALPHA,
	NCSP_RECTANGLE_MAX
};

#define NCSS_RECT_SHIFT NCSS_WIDGET_SHIFT

/**
 * \enum enumRectNotify
 * \brief notification code of \ref mRectangle
 */
enum enumRectNotify{
	NCSN_RECT_MAX = NCSN_WIDGET_MAX + 1
};

/**
 * @} end of ControlRectangle
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_RECTANGLE_H */
#endif //_MGNCSCTRL_RECTANGLE
