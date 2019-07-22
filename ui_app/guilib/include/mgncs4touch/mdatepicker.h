/*
 * \file mdatepicker.h
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

#ifndef __MGNCS4TOUCH_DPCK_H__
#define __MGNCS4TOUCH_DPCK_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define NCSCTRL_DATEPICKER          NCSCLASSNAME("datepicker")

typedef struct _mDatePicker         mDatePicker;
typedef struct _mDatePickerClass    mDatePickerClass;
typedef struct _mDatePickerRenderer mDatePickerRenderer;

#define mDatePickerHeader(clsName) \
    mComboPickerHeader(clsName)    \
    PUBLIC int idYear;             \
    PUBLIC int idMonth;            \
    PUBLIC int idDay ;             \
    PUBLIC int year;               \
    PUBLIC int month;              \
    PUBLIC int day;

struct _mDatePicker {
    mDatePickerHeader(mDatePicker)
};

#define mDatePickerClassHeader(clsName, parentClass)                 \
    mComboPickerClassHeader(clsName, parentClass)                    \
    PUBLIC void (*setDate)(clsName*, int year, int month, int day);  \
    PUBLIC BOOL (*getDate)(clsName*, int* year, int* month, int* day);

struct _mDatePickerClass {
    mDatePickerClassHeader(mDatePicker, mComboPicker)
};

#define mDatePickerRendererHeader(clsName, parentClass) \
    mComboPickerRendererHeader(clsName, parentClass)

struct _mDatePickerRenderer {
    mDatePickerRendererHeader(mDatePicker, mComboPicker)
};

#define NCSS_DPCK_YYYYMMDD  (0 << NCSS_CMBPCK_SHIFT)
#define NCSS_DPCK_MMDDYYYY  (1 << NCSS_CMBPCK_SHIFT)
#define NCSS_DPCK_YYMMDD    (2 << NCSS_CMBPCK_SHIFT)
#define NCSS_DPCK_MMDDYY    (4 << NCSS_CMBPCK_SHIFT)

#define NCSS_DPCK_SHIFT     (3 + NCSS_CMBPCK_SHIFT)

enum mDatePickerProp {
    NCSP_DPCK_YEAR = NCSP_CMBPCK_MAX + 1,
    NCSP_DPCK_MONTH,
    NCSP_DPCK_DAY,
    NCSP_DPCK_MAX
};

enum mDatePickerNotify {
    NCSN_DPCK_DATECHANGED = NCSN_CMBPCK_MAX + 1,
    NCSN_DPCK_MAX
};

MTOUCH_EXPORT extern mDatePickerClass g_stmDatePickerCls;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __MGNCS4TOUCH_DPCK_H__ */

