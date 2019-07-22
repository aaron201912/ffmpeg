/*
 * \file mcombopicker.h
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
#ifndef __MGNCS4TOUCH_COMBOPICKER_H__
#define __MGNCS4TOUCH_COMBOPICKER_H__

/**
 * \defgroup Control_ComboPicker mComboPicker
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

typedef struct _PICKITM {
    list_t   list;
    mPicker* picker;
    int      pickerID;
} CMBPICKITM, *PCMBPICKITM;

/**
 * \def NCSCTRL_CMBPICKER
 * \brief The control class name of combopicker.
 */
#define NCSCTRL_CMBPICKER            NCSCLASSNAME("combopicker")

typedef struct _mComboPicker         mComboPicker;
typedef struct _mComboPickerClass    mComboPickerClass;
typedef struct _mComboPickerRenderer mComboPickerRenderer;

#define mComboPickerHeader(clsName) \
    mPanelHeader(clsName)           \
    list_t pickerList;              \
    int pickerCount;                \
    int hMargin;                    \
    int vMargin;                    \
    int visLine;

struct _mComboPicker {
    mComboPickerHeader(mComboPicker)
};

#define mComboPickerClassHeader(clsName, parentClass)                   \
    mPanelClassHeader(clsName, parentClass)                             \
    BOOL (*addPicker)(clsName*, const NCS_WND_TEMPLATE* pick, int idx); \
    BOOL (*removePicker)(clsName*, int id);                             \
    BOOL (*removePickerByIdx)(clsName*, int idx);                       \
    BOOL (*removeAllPickers)(clsName*);                                 \
    void (*getClientArea)(clsName*, PRECT prc);                         \
    BOOL (*listenerCallBack)(mObject* self,                             \
            mObject* sender, int eventId, DWORD param); 

struct _mComboPickerClass {
    mComboPickerClassHeader(mComboPicker, mPanel)
};

#define mComboPickerRendererHeader(clsName, parentClass) \
    mPanelRendererHeader(clsName, parentClass)

/**
 * \struct mComboPickerRenderer
 * \brief The renderer structure of mComboPicker, which inheried from
 *        mComboPickerRenderer.
 */
struct _mComboPickerRenderer {
    mComboPickerRendererHeader(mComboPicker, mPanel)
};

/**
 * \var g_stmComboPickerCls
 * \brief The instance of mComboPickerClass.
 *
 * \sa mComboPickerClass
 */
MTOUCH_EXPORT extern mComboPickerClass g_stmComboPickerCls;

/**
 * \def NCSS_CMBPCK_SHIFT
 * \brief The bits used by mComboPicker in style.
 */
#define NCSS_CMBPCK_SHIFT        NCSS_PANEL_SHIFT

/**
 * \enum mComboPickerProperty
 * \brief the properties id of \ref mComboPicker
 *
 * \var NCSP_CMBPCK_HMARGIN
 * \brief Left and right ends of the blank reserved.
 *
 * \var NCSP_CMBPCK_VMARGIN
 * \brief Reserved for the upper and lower ends of the blank.
 *
 * \var NCSP_CMBPCK_VISLINE
 * \brief Information on the number of rows to display.
 */
enum mComboPickerProperty {
    NCSP_CMBPCK_HMARGIN = NCSP_PANEL_MAX + 1,
    NCSP_CMBPCK_VMARGIN,
    NCSP_CMBPCK_VISLINE,
    NCSP_CMBPCK_MAX,
};

/**
 * \enum mComboPickerNotify
 * \brief The notification code id of mComboPicker.
 */
enum mComboPickerNotify {
    /**
     * The maximum value of mComboPicker notification code id.
     */
    NCSN_CMBPCK_MAX = NCSN_PANEL_MAX + 1
};

/**
 * @} end of Control_ComboPicker
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __MGNCS4TOUCH_COMBOPICKER_H__ */

