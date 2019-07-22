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

#ifndef _MGUI_NCSCTRL_SPRTR_H
#define _MGUI_NCSCTRL_SPRTR_H
 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup ControlSeparator mSeparator
 * @{
 */

/**
 * \def NCSCTRL_SEPARATOR
 * \brief the name of separator control
 */

#define NCSCTRL_SEPARATOR        NCSCLASSNAME("separator")

typedef struct _mSeparator mSeparator;
typedef struct _mSeparatorClass mSeparatorClass;
typedef struct _mSeparatorRenderer mSeparatorRenderer;

#define mSeparatorHeader(clsName) \
	mStaticHeader(clsName) 

/**
 * \struct mSeparator
 * \brief separator class, derived from \ref mStatic
 */

struct _mSeparator
{
	mSeparatorHeader(mSeparator)
};

#define mSeparatorClassHeader(clsName, parentClass)  \
	mStaticClassHeader(clsName, parentClass)        

/**
 * \struct mSeparatorClass
 * \brief struct of separator control, derived from \ref mStaticClass
 */

struct _mSeparatorClass
{
	mSeparatorClassHeader(mSeparator, mStatic)
};

#define mHSeparator mSeparator
#define mHSeparatorClass mSeparatorClass
#define g_stmHSeparatorCls  g_stmSeparatorCls
#define mVSeparator mSeparator
#define mVSeparatorclass mSeparatorclass
#define g_stmVSeparatorCls  g_stmSeparatorCls

#define mSeparatorRendererHeader(clsName, parentClass) \
	mStaticRendererHeader(clsName, parentClass) 

/**
 * \struct mSeparatorRenderer
 * \brief separator class renderer interface, derived from \ref mStaticRenderer
 */

/* define the render of image static */
struct _mSeparatorRenderer {
	mSeparatorRendererHeader(mSeparator, mStatic)
};

/**
 * \var g_stmSeparatorCls
 * \brief global mSeparatorClass
 */

MGNCS_EXPORT extern mSeparatorClass g_stmSeparatorCls;

/**
 * \enum mSeparatorProp
 * \brief the properties id of \ref mSeparator
 */

/* property */
enum mSeparatorProp {
	NCSP_SPRTR_MAX = NCSP_STATIC_MAX + 1
};

/**
 * \def NCSS_SPRTR_VERT
 * \brief indicate a vertical separator
 */
#define NCSS_SPRTR_VERT  (0x1<<NCSS_STATIC_SHIFT)
#define NCSS_SPRTR_SHIFT (1+NCSS_STATIC_SHIFT)

/**
 * \enum enumSeparatorNotify
 * \brief the notification code of \ref mSeparator
 */
enum enumSeparatorNotify{
	NCSN_SPRTR_MAX = NCSN_STATIC_MAX + 1
};

/**
 * @} end of ControlSeparator
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_SPRTR_H */

