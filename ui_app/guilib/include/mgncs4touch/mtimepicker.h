/*
 * \file mtimepicker.h
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
#ifndef __MGNCS4TOUCH_TPCK_H__
#define __MGNCS4TOUCH_TPCK_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define NCSCTRL_TIMEPICKER          NCSCLASSNAME("timepicker")

typedef struct _mTimePicker         mTimePicker;
typedef struct _mTimePickerClass    mTimePickerClass;
typedef struct _mTimePickerRenderer mTimePickerRenderer;

#define mTimePickerHeader(clsName) \
    mComboPickerHeader(clsName)    \
    int idAmpm;                    \
    int idHour;                    \
    int idMinute;                  \
    int hour;                      \
    int minute;                    \
    char *ampmStrs;

struct _mTimePicker {
    mTimePickerHeader(mTimePicker)
};

#define mTimePickerClassHeader(clsName, parentClass)        \
    mComboPickerClassHeader(clsName, parentClass)           \
    PUBLIC void (*setTime)(clsName*, int hour, int minute); \
    PUBLIC BOOL (*getTime)(clsName*, int* hour, int* minute);
    
struct _mTimePickerClass {
    mTimePickerClassHeader(mTimePicker, mComboPicker)
};

#define mTimePickerRendererHeader(clsName, parentClass) \
    mComboPickerRendererHeader(clsName, parentClass)

struct _mTimePickerRenderer {
    mTimePickerRendererHeader(mTimePicker, mComboPicker)
};

#define NCSS_TPCK_24H        (0 << NCSS_CMBPCK_SHIFT)
#define NCSS_TPCK_12H        (1 << NCSS_CMBPCK_SHIFT)
#define NCSS_TPCK_SHIFT        (1 + NCSS_CMBPCK_SHIFT)

enum mTimePickerProp {
    NCSP_TPCK_HOUR = NCSP_CMBPCK_MAX + 1,
    NCSP_TPCK_MINUTE,
    NCSP_TPCK_AMPMSTRINGS,
    NCSP_TPCK_MAX
};

enum mTimePickerNotify {
    NCSN_TPCK_TIMECHANGED = NCSN_CMBPCK_MAX + 1,
    NCSN_TPCK_MAX
};

MTOUCH_EXPORT extern mTimePickerClass g_stmTimePickerCls;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __MGNCS4TOUCH_TPCK_H__ */

