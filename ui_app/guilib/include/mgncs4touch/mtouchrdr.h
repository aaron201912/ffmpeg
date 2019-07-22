/*
 * \file mtouchrdr.h
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

#ifndef __MGNCS4TOUCH_RENDER_H__
#define __MGNCS4TOUCH_RENDER_H__

#ifdef __cplusplus
extern "C" {
#endif

#define NCS_TYPE4TOUCH                0xA000

#define NCS4TOUCH_RRECT_RADIUS        (NCS_TYPE4TOUCH + 0x01)
#define NCS4TOUCH_BGC_BLOCK           (NCS_TYPE4TOUCH + 0x11)
#define NCS4TOUCH_FGC_SWBON           (NCS_TYPE4TOUCH + 0x12)
#define NCS4TOUCH_BGC_SWBON           (NCS_TYPE4TOUCH + 0x13)
#define NCS4TOUCH_FGC_SWBOFF          (NCS_TYPE4TOUCH + 0x14)
#define NCS4TOUCH_BGC_SWBOFF          (NCS_TYPE4TOUCH + 0x15)

#define NCS4TOUCH_BGC_NTB_HITEM       (NCS_TYPE4TOUCH + 0x21)
#define NCS4TOUCH_BGC_NTB_DITEM       (NCS_TYPE4TOUCH + 0x22)
#define NCS4TOUCH_BGC_NTB_BLOCK       (NCS_TYPE4TOUCH + 0x23)

#define NCS4TOUCH_BGC_BNB_BUTTON      (NCS_TYPE4TOUCH + 0x26)
#define NCS4TOUCH_FGC_BNB_BUTTON      (NCS_TYPE4TOUCH + 0x27)
#define NCS4TOUCH_FGC_BNB_TEXT        (NCS_TYPE4TOUCH + 0x28)
#define NCS4TOUCH_FGC_INB_TEXT        (NCS_TYPE4TOUCH + 0x29)

#define NCS4TOUCH_BGC_PCK_MAIN        (NCS_TYPE4TOUCH + 0x30)
#define NCS4TOUCH_BGC_PCK_PICKER      (NCS_TYPE4TOUCH + 0x31)
#define NCS4TOUCH_FGC_PCK_PICKER      (NCS_TYPE4TOUCH + 0x32)
#define NCS4TOUCH_BGC_PCK_SELECT      (NCS_TYPE4TOUCH + 0x33)
#define NCS4TOUCH_FGC_PCK_DISABLE     (NCS_TYPE4TOUCH + 0x34)

#define NCS4TOUCH_BGC_ITB                 (NCS_TYPE4TOUCH + 0x41)
#define NCS4TOUCH_BGC_ITB_LIGHT           (NCS_TYPE4TOUCH + 0x42)
#define NCS4TOUCH_FGC_ITB_TICK            (NCS_TYPE4TOUCH + 0x43)
#define NCS4TOUCH_FGC_ITB_TICK_LIGHT      (NCS_TYPE4TOUCH + 0x44)
#define NCS4TOUCH_FGC_ITB_ANGLE           (NCS_TYPE4TOUCH + 0x45)
#define NCS4TOUCH_FGC_ITB_ANGLE_LIGHT     (NCS_TYPE4TOUCH + 0x46)
#define NCS4TOUCH_FGC_ITB_LTEXT           (NCS_TYPE4TOUCH + 0x47)
#define NCS4TOUCH_FGC_ITB_LTEXT_LIGHT     (NCS_TYPE4TOUCH + 0x48)
#define NCS4TOUCH_FGC_ITB_RTEXT           (NCS_TYPE4TOUCH + 0x49)
#define NCS4TOUCH_FGC_ITB_RTEXT_LIGHT     (NCS_TYPE4TOUCH + 0x4a)

#define NCS4TOUCH_RENDERER       "fashion"

BOOL ncsTouchInitRenderers(void);

#ifdef __cplusplus
}
#endif

#endif

