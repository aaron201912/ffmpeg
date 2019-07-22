/*!============================================================================
 * \file mgeff-common.h
 * \brief common macro and type define.
 * \author WanZheng
 * \version 1.0
 * \date 02/03/2010 05:22:39 PM
 *
 \verbatim

    This file is part of mGEff, a component for MiniGUI.

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

#ifndef _MGEFF_COMMON_H_
#define _MGEFF_COMMON_H_

#if !defined(__NODLL__) && (defined (WIN32) || defined (__NUCLEUS_MNT__))
#   if defined(__MGEFF_LIB__)
#       define MGEFF_EXPORT       __declspec(dllexport)
#   else
#       define MGEFF_EXPORT       __declspec(dllimport)
#   endif
#else
#   define MGEFF_EXPORT /**< define MGEFF_EXPORT to NULL */
#endif

#ifndef MGEFF_MAJOR_VERSION
#  undef PACKAGE
#  undef VERSION
#  undef PACKAGE_BUGREPORT
#  undef PACKAGE_NAME
#  undef PACKAGE_STRING
#  undef PACKAGE_TARNAME
#  undef PACKAGE_VERSION
#  include "mgeffconfig.h"
#endif

/** \brief type of MGEFF_BOOL */
typedef int MGEFF_BOOL;

/** \brief type of MGEFF_TRUE */
#define MGEFF_TRUE  1
/** \brief type of MGEFF_FALSE */
#define MGEFF_FALSE 0

/**
 * MGEFF_DECLARE_HANDLE
 */
#ifdef MGEFF_FORWARD_COMPATIBLE

/** \brief type of MGEFF_HANDLE */
typedef void *MGEFF_HANDLE;
/** \brief type of MGEFF_DECLARE_HANDLE */
#define MGEFF_DECLARE_HANDLE(name) typedef MGEFF_HANDLE name

#else

/** \brief type of MGEFF_HANDLE */
typedef void *MGEFF_HANDLE;
/** \brief type of MGEFF_DECLARE_HANDLE */
#define MGEFF_DECLARE_HANDLE(name) struct _mgeff_##name##___{int unused;}; \
typedef struct _mgeff_##name##___ * name

#endif

/** \brief BITBLT macro define */
#define BITBLT(shdc, sx, sy, sw, sh, dhdc, dx, dy, rop) \
    do { \
        if (0 != sw && 0 != sh) BitBlt(shdc, sx, sy, sw, sh, dhdc, dx, dy, rop); \
    } while(0)

/** \brief STRETCHBLT macro define */
#define STRETCHBLT(shdc, sx, sy, sw, sh, dhdc, dx, dy, dw, dh, rop) \
    do { \
        if (0 != sw && 0 != sh && 0 != dw && 0 != dh)   \
        StretchBlt(shdc, sx, sy, sw, sh, dhdc, dx, dy, dw, dh, rop); \
    } while(0)

/** \brief Structure defines a int point */
typedef struct _EffPoint {
    int x;    /**< the x coordinate */
    int y;    /**< the y coordinate */
} EffPoint;

/** \brief Structure defines a float point */
typedef struct _EffPointF {
    float x;  /**< the x coordinate */
    float y;  /**< the y coordinate */
} EffPointF;

/** \brief Structure defines a int 3D point */
typedef struct _EffPoint3D {
    int x;    /**< the x coordinate */
    int y;    /**< the y coordinate */
    int z;    /**< the z coordinate */
} EffPoint3D;

/** \brief Structure defines a float 3D point */
typedef struct _EffPointF3D {
    float x;  /**< the x coordinate */
    float y;  /**< the y coordinate */
    float z;  /**< the z coordinate */
} EffPointF3D;

/** \brief Structure defines a rectangle */
typedef struct _EffRect {
    int left;   /**< left   */
    int top;    /**< top    */
    int right;  /**< right  */
    int bottom; /**< bottom */
} EffRect;

/** \brief type defines a color */
typedef unsigned int EffColor;

/**
 * \fn int mGEffInit (void)
 *
 * This function initializes the mgeff library. It creates the default
 * \a schedular.
 *
 * \return If succeed, return 0; otherwise return -1;
 *
 * \sa mGEffDeinit
 */
MGEFF_EXPORT int mGEffInit (void);

/**
 * \fn void mGEffDeinit (void)
 *
 * This function de-initializes the mgeff library. It will desktroy the
 * \a default scheduar.
 *
 * \sa mGEffInit
 */
MGEFF_EXPORT void mGEffDeinit (void);

#endif

