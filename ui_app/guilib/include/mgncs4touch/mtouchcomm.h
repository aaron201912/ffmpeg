/*
 * \file mtouchcomm.h
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
#ifndef __MGNCS4TOUCH_COMMON_H__
#define __MGNCS4TOUCH_COMMON_H__

#ifdef __cplusplus
extern "C"{
#endif

#ifdef WIN32
#   ifdef __MGNCS4TOUCH_LIB__
#       define MTOUCH_EXPORT       __declspec(dllexport)
#   else
#       define MTOUCH_EXPORT       __declspec(dllimport)
#   endif
#else
#   define MTOUCH_EXPORT
#endif

#ifndef MGNCS4TOUCH_MAJOR_VERSION
#   undef PACKAGE
#   undef VERSION
#   undef PACKAGE_BUGREPORT
#   undef PACKAGE_NAME
#   undef PACKAGE_STRING
#   undef PACKAGE_TARNAME
#   undef PACKAGE_VERSION
#   ifdef __MGNCS4TOUCH_LIB__
#       if defined(__CMAKE_PROJECT__) || defined(WIN32)
#           include "mgncs4touchconfig.h"
#       else
#           include "../mgncs4touchconfig.h"
#       endif
#   else
#       include "mgncs4touchconfig.h"
#   endif
#endif

#define ABGR2ARGB(c) (((c) & 0xFF00FF00) | (((c) & 0x000000FF)<<16) | (((c) & 0x00FF0000)>>16))

typedef enum _eCornerType {
    ECT_NORMAL = 0,
    ECT_TOPR,
    ECT_BOTTOMR,
    ECT_LEFTR,
    ECT_RIGHTR,
    ECT_BOTH,
} ECONER;


MTOUCH_EXPORT void DrawGradientRroundRect(HDC hdc,
        DWORD color , PRECT pRc, unsigned int r, BOOL isConvex);

MTOUCH_EXPORT int DrawPickerRect(HDC hdc, RECT *prc, DWORD beginColor, DWORD endColor, int corner);

MTOUCH_EXPORT int DrawSharpButton(HDC hdc, RECT *prc, DWORD color, BOOL left);

MTOUCH_EXPORT int DrawRoundRectButton(HDC hdc, RECT *prc, DWORD color, unsigned int r);

MTOUCH_EXPORT int DrawTouchTick(HDC hdc, RECT* prc, DWORD color);

MTOUCH_EXPORT int DrawTouchAngle(HDC hdc, RECT* prc, DWORD color);

MTOUCH_EXPORT int Draw3DCircle(HDC hdc, PRECT prc, DWORD color);

MTOUCH_EXPORT BOOL DrawVariableRoundRect(HDC hdc, PRECT prc, DWORD color, int r, ECONER con);

MTOUCH_EXPORT int DrawRectRing(HDC hdc, PRECT pRcOutside, PRECT pRcInside, DWORD color);

#ifdef __cplusplus
}
#endif

#endif //__MGNCS4TOUCH_COMMON_H__

