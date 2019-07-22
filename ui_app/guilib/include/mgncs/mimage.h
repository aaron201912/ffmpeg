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

#ifndef _MGUI_NCSCTRL_IMAGESTATIC_H
#define _MGUI_NCSCTRL_IMAGESTATIC_H
 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */
#include "mstatic.h"

/**
 * \defgroup ControlImage mImage
 * @{
 */

/**
 * \def NCSCTRL_IMAGE
 * \brief the name of image control
 */
#define NCSCTRL_IMAGE        NCSCLASSNAME("image")

typedef struct _mImage mImage;
typedef struct _mImageClass mImageClass;
typedef struct _mImageRenderer mImageRenderer;

#define mImageHeader(clsName)   \
	mStaticHeader(clsName)      

/**
 * \struct mImage
 * \brief static control can show a image, derived from \ref mStatic
 */
struct _mImage
{
	mImageHeader(mImage)
};

#define mImageClassHeader(clsName, parentClass) \
	mStaticClassHeader(clsName, parentClass)
/**
 * \struct mImageClass
 * \brief ImageClass struct of image control, derived from \ref mStaticClass
 */

struct _mImageClass
{
	mImageClassHeader(mImage, mStatic)
};

#define mImageRendererHeader(clsName, parentClass) \
	mStaticRendererHeader(clsName, parentClass)
/**
 * \struct mImageRenderer
 * \brief image class renderer interface, derived from \ref mStaticRenderer
 */
/* define the render of image static */
struct _mImageRenderer {
	mImageRendererHeader(mImage, mStatic)
};

/**
 * \var g_stmImageCls
 * \brief global mImageClass
 */

MGNCS_EXPORT extern mImageClass g_stmImageCls;

/**
 * \enum mImageProp
 * \brief the properties id of \ref mImage
 */

/* property */
enum mImageProp {
	/**
	 * \brief Set the image content
	 *  - Type PBITMAP
	 *  - Read Write
	 *  - set the pointer to image content
	 *
	 */
	NCSP_IMAGE_IMAGE = NCSP_STATIC_MAX + 1,
	/**
	 * \brief Set the image content
	 *  - Type char*
	 *  - Read Write
	 *  - set the image file name pointer
	 *
	 */
	NCSP_IMAGE_IMAGEFILE,
	/**
	 * \brief Set image draw mode
	 *  - Type \a enumNCSImageDrawMode
	 *  - Read Write
	 *  - set the image draw mode to NCS_DM_NORMAL/NCS_DM_SCALED/NCS_DM_TILED
	 *
	 */
	NCSP_IMAGE_DRAWMODE,
	NCSP_IMAGE_MAX
};

#define NCSS_IMAGE_SHIFT  NCSS_STATIC_SHIFT

/**
 * \enum enumImageNotify
 * \brief the notification code of \ref mImage
 */
enum enumImageNotify{
	NCSN_IMAGE_MAX = NCSN_STATIC_MAX + 1
};

/**
 * @} end of ControlImage
 */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_STATIC_H */

