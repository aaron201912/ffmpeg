/**
 * \file mpropsheet.h
 * \author XiaoweiYan
 * \date 2009/02/26
 *
 * This file includes the definition of mPropSheet control.
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

#ifdef _MGNCSCTRL_PROPSHEET

#ifndef _MGUI_NCSCTRL_PROPSHEET_H
#define _MGUI_NCSCTRL_PROPSHEET_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

    /**
     * \defgroup Control_PropSheet mPropSheet
     * @{
     */

/**
 * \def NCSCTRL_PROPSHEET
 * \brief The control class name of mPropSheet.
 */
#define NCSCTRL_PROPSHEET        NCSCLASSNAME("propsheet")

/**
 * \def NCSS_PRPSHT_SHIFT
 * \brief The bits used by mPropSheet in style.
 */
#define NCSS_PRPSHT_SHIFT  NCSS_WIDGET_SHIFT+3

#define NCSS_PRPSHT_BTNMASK         (0x0003<<NCSS_WIDGET_SHIFT)
#define NCSS_PRPSHT_TABMASK         (0x0004<<NCSS_WIDGET_SHIFT)

/**
 * \def NCSS_PRPSHT_COMPACTTAB
 * \brief A simple property sheet control. All tabs of the controls will have
 *        the same width.
 */
#define NCSS_PRPSHT_SIMPLE          (0x0000<<NCSS_WIDGET_SHIFT)

/**
 * \def NCSS_PRPSHT_COMPACTTAB
 * \brief Compact tab style. The width of a tab is adaptive to the tab title.
 */
#define NCSS_PRPSHT_COMPACTTAB      (0x0001<<NCSS_WIDGET_SHIFT)

/**
 * \def NCSS_PRPSHT_SCROLLABLE
 * \brief Tab can be scrolled by scrolled button.
 */
#define NCSS_PRPSHT_SCROLLABLE      (0x0002<<NCSS_WIDGET_SHIFT)

/**
 * \def NCSS_PRPSHT_TOP
 * \brief Top tab style. Tabs will be located at the top of the propsheet.
 */
#define NCSS_PRPSHT_TOP             (0x0000<<NCSS_WIDGET_SHIFT)

/**
 * \def NCSS_PRPSHT_BOTTOM
 * \brief Bottom tab style. Tabs will be located at the bottom of the
 *        propsheet.
 */
#define NCSS_PRPSHT_BOTTOM          (0x0004<<NCSS_WIDGET_SHIFT)

typedef struct _mPropSheet mPropSheet;
typedef struct _mPropSheetClass mPropSheetClass;
typedef struct _mPropSheetRenderer mPropSheetRenderer;

#define mPropSheetHeader(className) \
	mWidgetHeader(className)        \
    int     minTabWidth;            \
    int     maxTabWidth;            \
    int     scrollTabWidth;         \
    RECT    headRect;               \
    int     pageCount;              \
    BOOL    btnShow;                \
    mPage*  active;                 \
    mPage*  firstView;              \
    int     tabMargin;

/**
 * \struct mPropSheet
 * \brief propsheet control mananager a group of \a mPage, inherited from mWidget.
 *
 *      - minTabWidth \n
 *        The minimum width of tab.
 *
 *      - maxTabWidth \n
 *        The maximum width of tab.
 *
 *      - scrollTabWidth \n
 *        The effective width of tabs.
 *
 *      - headRect \n
 *        The rectange of header, which is used to show all tabs.
 *
 *      - pageCount \n
 *        The number of pages.
 *
 *      - btnShow \n
 *        The flag whether show scrollable button or not.
 *
 *      - active \n
 *        The pointer to active page \a mPage.
 *
 *      - firstView \n
 *        The pointer to first view page \a mPage.
 *
 *      - tabMargin \n
 *        The intervals between current and next tab.
 */
struct _mPropSheet
{
	mPropSheetHeader(mPropSheet)
};

#define mPropSheetClassHeader(clsName, parentClass)    \
	mWidgetClassHeader(clsName, parentClass)           \
    int (*getPageIndex)(clsName*, mPage* page);        \
    mPage* (*getPageByIndex)(clsName*, int pageIndex); \
    mPage* (*addPage)(clsName*, const PDLGTEMPLATE dlgTemplate, const NCS_EVENT_HANDLER* handlers);\
    BOOL (*removePageByIndex)(clsName*, int pageIndex);\
    BOOL (*removePage)(clsName*, mPage* page);         \
    mPage* (*getNextPage)(clsName*, mPage* page);      \
    mPage* (*getPrevPage)(clsName*, mPage* page);      \
    int (*broadCastMsg)(clsName*, DWORD param1, DWORD param2);

/**
 * \struct mPropSheetClass
 * \brief The virtual function table of mPropSheet.
 *
 *  - int (*\b getPageIndex)(mPropSheet *self, mPage* page);\n
 *    Get the index of specified \a page.
 *      \param page       The pointer to mPage.
 *      \return           The index of page for success, otherwise -1.
 *
 *  - mPage* (*\b getPageByIndex)(mPropSheet *self, int pageIndex);\n
 *    Get the pointer of page according to page index \a pageIndex.
 *      \param pageIndex  The index of page.
 *      \return           The pointer to page for success, otherwise NULL.
 *
 *  - mPage* (*\b addPage)(mPropSheet *self, const PDLGTEMPLATE dlgTemplate,
 *                              const NCS_EVENT_HANDLER* handlers);\n
 *    Add a page to propsheet according to DLGTEMPLATE and handlers.
 *      \param dlgTemplate    The pointer to dialog template.
 *      \param handlers       The pointer to event handlers.
 *      \return               The pointer to page for success, otherwise NULL.
 *      \sa NCS_EVENT_HANDLER
 *
 *  - BOOL (*\b removePageByIndex)(mPropSheet *self, int pageIndex);\n
 *    Remove a page according to page index \a pageIndex.
 *      \param pageIndex  The index of page.
 *      \return           TRUE for success, otherwise FALSE.
 *
 *  - BOOL (*\b removePage)(mPropSheet *self, mPage* page);\n
 *    Remove a page according to the pointer to \a page.
 *      \param page       The pointer to mPage.
 *      \return           TRUE for success, otherwise FALSE.
 *
 *  - mPage* (*\b getNextPage)(mPropSheet *self, mPage* page); \n
 *    Get the next page.
 *      \param page       The pointer to mPage.
 *      \return           The pointer to next page.
 *
 *  - mPage* (*\b getPrevPage)(mPropSheet *self, mPage* page); \n
 *    Get the previous page.
 *      \param page       The pointer to mPage.
 *      \return           The pointer to previous page.
 *
 *  - int (*\b broadCastMsg)(mPropSheet *self, DWORD param1, DWORD param2); \n
 *    Broadcast message to all pages.
 *      \param param1     The first parameter of broadcast message.
 *      \param param2     The second parameter of broadcast message.
 *      \return           The index + 1 of page which has been broken the message,
 *                        Zero indicates no page asserts an error.
 */
struct _mPropSheetClass
{
	mPropSheetClassHeader(mPropSheet, mWidget)
};

/**
 * \enum ncsPrpShtFlags
 * \brief The flag of mPropSheet. It is used by mPropSheetRenderer.
 */
enum ncsPrpShtFlags
{
    /**
     * The flag of border.
     */
    NCSF_PRPSHT_BORDER,

    /**
     * The flag of tab.
     */
    NCSF_PRPSHT_TAB,

    /**
     * The flag of left scrollable button.
     */
    NCSF_PRPSHT_LEFT,

    /**
     * The flag of right scrollable button.
     */
    NCSF_PRPSHT_RIGHT,

    /**
     * The flag of page.
     */
    NCSF_PRPSHT_PAGE,

    /**
     * The maximum value of flags.
     */
    NCSF_PRPSHT_MAX,
};

#define mPropSheetRendererHeader(clsName, parentClass)                      \
	mWidgetRendererHeader(clsName, parentClass)                             \
    void (*resetHeadArea) (clsName*, RECT* rcClient, DWORD style);          \
    void (*getRect)(clsName*, RECT* rcClient, RECT *rcResult, int which);   \
    void (*drawBorder)(clsName*, HDC hdc, RECT *rcBorder);                  \
    void (*drawScrollBtn)(clsName*, HDC hdc, RECT *rcBtn, int which);       \
    void (*drawTab)(clsName*, HDC hdc, RECT* rcTab, const char* title, HICON hIcon, BOOL active);

/**
 * \struct mPropSheetRenderer
 * \brief The structure of mPropSheet renderer, which inheried from mWidgetRenderer.
 *
 *  - void (*\b resetHeadArea) (mPropSheet *self, RECT* rcClient, DWORD style);\n
 *    According to client area and control style, reset the rectangle of header
 *    which is used to show all tabs.
 *      \param rcClient   The pointer to a RECT strucutre which contains the client area.
 *      \param style      The style of control.
 *
 *  - void (*\b getRect)(mPropSheet *self, RECT* rcClient, RECT *rcResult, int which);\n
 *    Get the rectangle of specified region \a which.
 *      \param rcClient   The pointer to a RECT strucutre which contains the client area.
 *      \param rcResult   The pointer to a RECT strucutre which contains the result area.
 *      \param which      The flag of area, can be one of the following values:
 *                          - NCSF_PRPSHT_BORDER \n Border.
 *                          - NCSF_PRPSHT_TAB \n    Tab.
 *                          - NCSF_PRPSHT_LEFT \n   Left scrollable button.
 *                          - NCSF_PRPSHT_RIGHT \n  Right scrollable button.
 *                          - NCSF_PRPSHT_PAGE \n   Page.
 *
 *  - void (*\b drawBorder)(mPropSheet *self, HDC hdc, RECT *rcBorder);\n
 *    Draw border.
 *      \param hdc        The device context.
 *      \param rcBorder   The pointer to a RECT strucutre which contains the border area.
 *
 *  - void (*\b drawScrollBtn)(mPropSheet *self, HDC hdc, RECT *rcBtn, int which);\n
 *    Draw scrollable button.
 *      \param hdc        The device context.
 *      \param rcBtn      The pointer to a RECT strucutre which contains the button area.
 *      \param which      The status of scrollable button, can be one of the following values:
 *                          - NCSR_ARROW_LEFT
 *                          - NCSR_ARROW_RIGHT
 *
 *  - void (*\b drawTab)(mPropSheet *self, HDC hdc, RECT* rcTab, const char* title, HICON hIcon, BOOL active); \n
 *    Draw tab.
 *      \param hdc        The device context.
 *      \param rcTab      The pointer to a RECT strucutre which contains the tab area.
 *      \param title      The pointer to the tab title.
 *      \param hIcon      The handle of the tab icon.
 *      \param active     The status of tab.
 *
 */
struct  _mPropSheetRenderer {
	mPropSheetRendererHeader(mPropSheet, mWidget)
};

/**
 * \var g_stmPropSheetCls
 * \brief The instance of mPropSheetClass.
 *
 * \sa mPropSheetClass
 */
MGNCS_EXPORT extern mPropSheetClass g_stmPropSheetCls;

/**
 * \enum mPropSheetNotify
 * \brief The notification code id of mPropSheet.
 */
enum mPropSheetNotify
{
	/**
	 * \brief the active page changed
	 *
	 */
    NCSN_PRPSHT_ACTIVECHANGED = NCSN_WIDGET_MAX + 1,
    /**
     * The maximum value of mPropSheet notification code id.
     */
    NCSN_PRPSHT_MAX
};


/**
 * \enum mPropSheetProp
 * \brief The properties of mPropSheet
 */
enum mPropSheetProp
{
	/**
	 * \brief Set the minimum width of tab
	 *  - Type int
	 *  - Read Write
	 *  - set the minimum width of tab
	 *
	 */
	NCSP_PRPSHT_MINTABWIDTH = NCSP_WIDGET_MAX + 1,
	/**
	 * \brief Set the tab margin
	 *  - Type int
	 *  - Read Write
	 *  - set the margin of tab
	 *
	 */
	NCSP_PRPSHT_TABMARGIN,
	/**
	 * \brief Set the active page
	 *  - Type mPage*
	 *  - Read Write
	 *  - set the active page pointer
	 *
	 */
    NCSP_PRPSHT_ACTIVEPAGE,
	/**
	 * \brief Set the active page id
	 *  - Type int
	 *  - Read Write
	 *  - set the active page id
	 *
	 */
    NCSP_PRPSHT_ACTIVEPAGEIDX,
	/**
	 * \brief Set the first view page
	 *  - Type mPage*
	 *  - Read
	 *  - set the first view page pointer
	 *
	 */
    NCSP_PRPSHT_FIRSTVIEWPAGE,
	/**
	 * \brief Set the first visible page
	 *  - Type int
	 *  - Read
	 *  - set the index of first visible page
	 *
	 */
    NCSP_PRPSHT_FIRSTVIEWPAGEIDX,
	/**
	 * \brief Set the number of pages
	 *  - Type int
	 *  - Read
	 *  - set the number of pages
	 *
	 */
	NCSP_PRPSHT_PAGECOUNT,
	NCSP_PRPSHT_MAX
};

    /** @} end of Control_PropSheet */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_PROPSHEET_H */
#endif //_MGNCSCTRL_PROPSHEET
