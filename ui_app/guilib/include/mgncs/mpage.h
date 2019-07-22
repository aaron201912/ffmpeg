/**
 * \file mpage.h
 * \author XiaoweiYan
 * \date 2009/03/05
 *
 * This file includes the definition of mPage control.
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

#ifdef _MGNCSCTRL_PAGE

#ifndef _MGUI_NCSCTRL_PAGE_H
#define _MGUI_NCSCTRL_PAGE_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Control_Page mPage
 * @{
 */

/**
 * \def NCSCTRL_PAGE
 * \brief The control class name of mPage.
 */
#define NCSCTRL_PAGE           NCSCLASSNAME("page")

typedef struct _mPage mPage;
typedef struct _mPageClass mPageClass;
typedef struct _mPageRenderer mPageRenderer;

/**
 * \def NCSS_PAGE_SHIFT
 * \brief The bits used by mPage in style.
 */
#define NCSS_PAGE_SHIFT NCSS_CTNR_SHIFT

#define mPageHeader(className)  \
	mContainerHeader(className) \
    int     titleWidth;         \
    HICON   hIcon;


/**
 * \struct mPage
 * \brief The control structure of mPage, which derived from mContainer.
 *        It is a control which will be used as page in mPage. Please
 *        don't use it separately.
 *
 *      - titleWidth \n
 *        The width of page title.
 *
 *      - hIcon \n
 *        The handle of icon.
 */
struct _mPage
{
	mPageHeader(mPage)
};

#define mPageClassHeader(clsName, parentClass)         \
	mContainerClassHeader(clsName, parentClass)        \
    const char* (*getTitle)(clsName* self);            \
    BOOL (*setTitle)(clsName* self, const char* title);\
    int (*getTitleLength)(clsName* self);              \
    int (*getTitleWidth)(clsName* self, HDC hdc);      \
    BOOL (*setIcon)(clsName* self, HICON icon);        \
    HICON (*getIcon)(clsName* self);                   \
    void (*showPage)(clsName* self, int showCmd);

/**
 * \struct mPageClass
 * \brief The virtual function table of mPage, which derived from mContainerClass.
 *
 *  - const char* (*\b getTitle)(mPage *self);\n
 *    Get the title of page.
 *      \return           The pointer to the title of page.
 *
 *  - BOOL (*\b setTitle)(mPage *self, const char* title);\n
 *    Set the title of specified page \a self.
 *      \param title      The pointer to the new title of page.
 *      \return           TRUE for success, otherwise FALSE.
 *
 *  - int (*\b getTitleLength)(mPage *self);\n
 *    Get the length of a page title.
 *      \return           The length of the page title.
 *
 *  - int (*\b getTitleWidth)(mPage *self, HDC hdc);\n
 *    Get the width of a page title in specified device context.
 *      \param hdc        The device context.
 *      \return           The width of the page title.
 *
 *  - void (*\b showPage) (mPage *self, int showCmd);\n
 *    Shows or hides a page.
 *      \param showCmd    The show command, can be one of the following values:
 *                          - SW_SHOW\n The page will be shown.
 *                          - SW_HIDE\n The page will be hidden.
 *
 *  - BOOL (*\b setIcon)(mPage *self, HICON icon);\n
 *    Set the handle of page icon.
 *      \param icon       The handle of ICON.
 *      \return           TRUE for success, otherwise FALSE.
 *
 *  - HICON (*\b getIcon)(mPage *self);\n
 *    Get the handle of page icon.
 *      \return           The handle of ICON.
 */
struct _mPageClass
{
	mPageClassHeader(mPage, mContainer)
};

/**
 * \var typedef void (*NCS_CB_ONINITPAGE)(mPage *self, DWORD addData);
 * \brief the callback of the event MSG_INITPAGE. Initialize the
 *        controls in page.
 *
 * \param self     The pointer to mContainer.
 * \param addData   The additional data in DLGTEMPLATE structure
 *                  passed through addPage in mPageClass.
 *
 * \note It need to be implemented by user.
 */
typedef void (*NCS_CB_ONINITPAGE)(mPage *self, DWORD addData);

/**
 * \var typedef int (*NCS_CB_ONSHOWPAGE)(mPage *self, HWND focusChild, int showCmd);
 * \brief the callback of the event MSG_SHOWPAGE. Shows or hides page.
 *
 * \param self      The pointer to mContainer.
 * \param focusChild The handle to the child which will gain the
 *                   input focus when showing the page.
 * \param showCmd    The show command, can be one of the following values:
 *                      - SW_SHOW\n The page will be shown.
 *                      - SW_HIDE\n The page will be hidden.
 *
 * \return           Nonzero for success, zero for failure.
 * \note It need to be implemented by user.
 */
typedef int (*NCS_CB_ONSHOWPAGE)(mPage *self, HWND focusChild, int showCmd);

/**
 * \var typedef int (*NCS_CB_ONSHEETCMD)(mPage *self, DWORD param1, DWORD param2);
 * \brief the callback of the MSG_SHEETCMD. Process broadcast message.
 *
 * \param self      The pointer to mContainer.
 * \param param1     The param info1 of function broadCastMsg in mPageClass.
 * \param param2     The param info2 of function broadCastMsg in mPageClass.
 *
 * \return           zero for broadcasting message for other pages continuing,
 *                   Nonzero for end of broadcasting message.
 *
 * \note It need to be implemented by user.
 */
typedef int (*NCS_CB_ONSHEETCMD)(mPage *self, DWORD param1, DWORD param2);

#define mPageRendererHeader(clsName, parentClass) \
	mContainerRendererHeader(clsName, parentClass)

/**
 * \struct mPageRenderer
 * \brief The renderer structure of mPage, which inheried from mContainerRenderer.
 */
struct  _mPageRenderer {
	 mPageRendererHeader(mPage, mContainer)
};


/**
 * \var g_stmPageCls
 * \brief The instance of mPageClass.
 *
 * \sa mPageClass
 */
MGNCS_EXPORT extern mPageClass g_stmPageCls;

    /** @} end of Control_Page*/
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_PAGE_H */
#endif //_MGNCSCTRL_PAGE
