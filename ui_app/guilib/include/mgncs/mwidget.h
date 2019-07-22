/*
 * \file mwidget.h
 * \author ChaiPeng
 * \date 2009/01/01
 *
 * This file include the base widget control definition.
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

#ifndef _MGUI_NCSCTRL_WIDGET_H
#define _MGUI_NCSCTRL_WIDGET_H
 
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#define TOWIDGET(x)    ((mWidget *)x)
#define TOWIDGETCLASS(x)    ((mWidgetClass *)x)

#define RendererType(className) className##Renderer


/**
 * \defgroup ControlWidget mWidget
 * @{
 */

/**
 * \def NCSCTRL_WIDGET
 * \brief the name of widget control
 */
#define NCSCTRL_WIDGET  NCSCLASSNAME("widget")

typedef struct _mWidgetClass mWidgetClass;
typedef struct _mWidget mWidget;

/**
 * \typedef struct _NCS_WND_TEMPLATE NCS_WND_TEMPLATE;
 * \sa _NCS_WND_TEMPLATE
 */
typedef struct _NCS_WND_TEMPLATE NCS_WND_TEMPLATE;




typedef struct _mWidgetRenderer mWidgetRenderer;

#define mWidgetRendererHeader(clsName, parentClass)                         \
    const char* rdr_name;                                                   \
    /*renderer private method and data * every class must set NULL or it's ownd data */ \
    void (*class_init)(RendererType(clsName)* rdr);                         \
    RendererType(parentClass) *ptrParentClass;                              \
    BOOL (*init_self)(RendererType(clsName) *rdr);                          \
    void (*uninit_self)(RendererType(clsName) *rdr);                        \
    void * prv_data;                                                        \
    /*renderer drivedable method and data, * these data can be used directly by drived class */ \
    void (*drawFocusFrame)(clsName*, HDC, const RECT* );                    \
    BOOL (*drawBkground)(clsName*, HDC, const RECT*);                       \
    void (*draw3dbox)(clsName*, HDC, const RECT*, DWORD,DWORD);             \
    void (*drawCheckbox)(clsName*, HDC, const RECT*, DWORD);                \
    void (*drawRadio)(clsName*, HDC, const RECT*, DWORD);                   \
    void (*drawArrow)(clsName*, HDC, const RECT*, int arrow,DWORD, DWORD);  \
    void (*drawItem)(clsName*, HDC, const RECT*, DWORD); 

/**
 * \enum ncsRendererState
 * \brief define the renderer state of widget
 */
enum ncsRendererState{
	NCSRS_NORMAL = 0x00,
	NCSRS_HIGHLIGHT = 0x01,
	NCSRS_SIGNIFICANT = 0x02,
	NCSRS_SELECTED = 0x04,
	NCSRS_HALFSELECTED = 0x10, 
	NCSRS_DISABLE = 0x08
};
/**
 * \var NCSRS_NORMAL
 * \brief normal state
 *
 */
/**
 * \var NCSRS_HIGHLIGHT
 * \brief higlight state
 */
/**
 * \var NCSRS_SELECTED
 * \brief a box or item is selected
 */
/**
 * \var NCSRS_SIGNIFICANT
 * \brief significant state
 */
/**
 * \var NCSRS_HALFSELECTED
 * \brief half selected state
 */
/**
 * \var NCSRS_DISABLE
 * \brief disable state
 */

#define NCSR_STATUS(flag)  ((flag)&0xFFFF)

/**
 * \enum ncsRendererFlag
 * \brief the flag defines of renderer
 */
enum ncsRendererFlag{
	NCSRF_INNERFRAME = 0x010000,
	NCSRF_FILL       = 0x020000,
	NCSRF_FRAME      = 0x040000,
};
/**
 * \var NCSRF_INNERFRAME
 * \brief indicate that draw a inner frame box or shape
 */
/**
 * \var NCSRF_FILL
 * \brief indicate that fill a box or other shape
 */
/**
 * \var NCSRF_FRAME
 * \brief indicate that draw a box or other shape with a frame
 */

/**
 * \enum ncsRDRArrow
 * \brief the arrow define of renderer
 */
enum ncsRDRArrow {
	NCSR_ARROW_LEFT = 0,
	NCSR_ARROW_RIGHT,
	NCSR_ARROW_UP,
	NCSR_ARROW_DOWN
};
/**
 * \var NCSR_ARROW_LEFT
 * \brief indiate a left arrow
 */
/**
 * \var NCSR_ARROW_RIGHT
 * \brief indiate a right arrow
 */
/**
 * \var NCSR_ARROW_UP
 * \brief indiate a up arrow
 */
/**
 * \var NCSR_ARROW_DOWN
 * \brief indiate a down arrow
 */


/**
 * \struct mWidgetRenderer
 * \brief Widget class's Renderer interface
 *  
 *  - const char* \b rdr_name \n
 *    The Name of the Renderer
 *
 *  - void \b class_init(\ref mWidgetRenderer* rdr); \n
 *    init the the renderer class members
 *
 *  - mWidgetRenderer * \b ptrParentClass \n
 *    Supper class pointer
 *
 *  - BOOL \b init_self(\ref mWidgetRenderer *rdr); \n
 *    init the \b prv_data for a special mWidgetRenderer object
 *
 *  - void \b uninit_self(\ref mWidgetRenderer *rdr); \n
 *    uninitialize the \b prv_data for the special mWidgetRenderer object
 *
 *  - DWORD \b setElement(\ref mWidgetRenderer * rdr, \a int id, \a DWORD value);
 *    set the element by id and value \n
 *    \param id the id of Renderer element
 *    \param value the value of Renderer element
 *    \return old value or (DWORD)-1 (failed) 
 *
 *  - DWORD \b getElement(\ref mWidgetRenderer *rdr,\a int id); \n
 *    get the element from renderer
 *    \param id - the id of renderer element
 *    \return DWORD - the value of speical element
 *
 *  - void * \b prv_data - the private data used by renderer object
 *
 *  - BOOL \b transIdValue(\ref mWidgetRenderer* rdr, \a int id, \a DWORD *value); \n
 *    Translate from \a value from use type to inner type. e.g. translate image name(const char*) into res key (RES_KEY)
 *    \param id - the id of the element
 *    \param * \a value - in and out param, the value pointer of element
 *    \return BOOL - True or False
 *
 *  - void \b drawFocusFrame(\ref mWidget *, HWND, HDC, const RECT*); \n
 *    draw focus frame when control get focus
 *    \param - handle of control
 *    \param - DC of control
 *    \param RECT * - the Rectangle to draw focus frame
 *
 *  - BOOL \b drawBkground(\ref mWidget * ,HWND, HDC, const RECT*) \n
 *    draw background when control erase background
 *    \param - DC of background, may be 0, user should get dc by \a GetClientDC
 *    \param RECT* - invalidat Rectangle of background
 *    \return BOOL - if the background has been drawed or not
 *
 *  - void \b draw3dbox(\ref mWidgetRenderer* , \a HDC hdc, \a const \a RECT *, \a DWORD color \a DWORD flag); \n
 *    draw 3d box
 *    \param hdc - the DC in which draw 3d box
 *    \param RECT * - the Rectange of the box
 *    \param color - the frame color of box
 *    \param flag - the state (\ref ncsRendererState) and flag (\ref ncsRendererFlag) of 3dbox
 *
 *  - void \b drawCheckbox (\ref mWidget *, \a HDC hdc, \a const \a RECT *rc, \a DWORD flag);\n
 *    draw a checkbox into a DC
 *    \param hdc - the DC in which draw checkbox
 *    \param rc - the location and size of checkbox
 *    \param flag - the flag of check box (\ref ncsRendererFlag)
 *
 *  - void \b drawRadio (\ref mWidget *, \a HDC hdc, \a const \a RECT *rc, \a DWORD flag);\n
 *    draw a radio box into a DC \n
 *    \param hdc - the DC in which draw radiobox
 *    \param rc - the location and size of radio box
 *    \param flag - the flag of radiobox (\ref ncsRendererFlag)
 *
 *  - void \b drawArrow (\ref mWidget *, \a HDC hdc, \a const \a RECT *rc, \a int arrow, \a DWORD color, \a DWORD flag);\n
 *    draw an arrow for dc
 *    \param hdc - the handle to DC
 *    \param rc - the Rectangle around arrow, its size bigger than arrow
 *    \param arrow - arrow type (\ref ncsRDRArrow)
 *    \param color - the filling color of arrow
 *    \param flag - the flag of arrow (\ref ncsRendererFlag)
 *
 *  - void \b drawItem (\ref mWidget*, \a HDC hdc, \a const \a RECT *rc, \a DWORD \a color);\n
 *    draw an item
 *    \param rc - the rectanle of item
 *    \param color - the filling color of item
 *
 *  \sa mWidget, mWidgetClass
 *
 */

struct _mWidgetRenderer {
	mWidgetRendererHeader(mWidget, mWidget)
};


#define NCSSPEC_OBJ_CONTENT  MAX_USER_SPECIAL_KEY + 1

#define mWidgetHeader(clsName)                  \
	mComponentHeader(clsName)                   \
	HWND hwnd;                                  \
	RendererType(clsName) *renderer;            \
	mComponent* comps;                          \
	ImageDrawInfo  bkimg;                       \
	mObject *body;

/**
 * \struct mWidget
 * \brief define the mWidget members, inherit from mComponent
 *
 *   - HWND \b hwnd \n
 *     The handle of Control who own this object
 *
 *   - mWidgetRenderer * \b renderer \n
 *     The Renderer pointer of the widget
 *
 *   - mComponent * \b comps \n
 *     the children component (\ref InvisibleComponent) 
 *
 *   - ImageDrawInfo \b bkimg \n
 *     The back image drawing info
 *
 *  \sa mWidgetRenderer, mWidgetClass
 */
struct _mWidget
{
	mWidgetHeader(mWidget)
};

#define mWidgetClassHeader(clsName, parentClass)                                    \
	mComponentClassHeader(clsName, parentClass)                                     \
	DWORD dlgCode;                                                                  \
	mObject * (*createBody)(clsName*);                                              \
	LRESULT (*wndProc)(clsName* , UINT, WPARAM, LPARAM);                                 \
	BOOL (*callUserHandler)(clsName* self, void *handler,                          \
		UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pRet);                      \
	BOOL (*onCreate)(clsName*, LPARAM);                                             \
	BOOL (*addChildren)(clsName*, NCS_WND_TEMPLATE* children,int count);           \
	void (*onPaint)(clsName*, HDC, const PCLIPRGN pClip);                           \
	int (*onLButtonDown)(clsName*, int x, int y, DWORD keyFlags);                  \
	int (*onLButtonUp)(clsName*, int x, int y, DWORD keyFlags);                    \
	int (*onMouseMove)(clsName*, int x, int y, DWORD keyFlags);                    \
	int (*onKeyDown)(clsName*, int scancode, DWORD keyFlags);                      \
	int (*onKeyUp)(clsName*, int scancode, DWORD keyFlags);                        \
	int (*onSizeChanged)(clsName*, RECT* rtClient);                                 \
	BOOL (*onEraseBkgnd)(clsName*, HDC hdc, const RECT* invRect);                  \
	/** private function, only used by GUI BUIlDER */                               \
	BOOL (*refresh)(clsName *);                                                    


/** 
 * \struct mWidgetClass
 * \brief the VTable of \a mWidget, drived from \ref mComponentClass
 * 
 *  - DWORD \b dlgCode \n
 *    The Dialog Code of this widget, return When the widget recieve the message MSG_GETDLGCODE
 *
 *  - int \b wndProc (\ref mWidgetClass *self, \a UINT message, \a WPARAM wParam, \a LPARAM lParam); \n
 *    The Window Proc which is used to dispatch message 
 *    \param self - this pointer
 *    \param message - the code of message
 *    \param wParam - the wparam of message
 *    \param lParam - the lParam of message
 *    \return int, by message
 *
 *  - BOOL \b callUserHandler (\ref mWidget *self, \a void *handler, \
 *     \a UINT message, \a WPARAM wParam, \a LPARAM lParam, \a int *pRet); \n
 *     call the user defined handlers, \b PRIVATE \b METHOD
 *    \param handler - the handler defined by user
 *    \param message - the message processing
 *    \param wParam - the wParam of message
 *    \param lParam - the lParam of message
 *    \param pret - the pointer of return code, OUT Params
 *    \return BOOL - continue the default processing (TRUE) or return directly (FALSE)
 *
 */
struct _mWidgetClass
{
	mWidgetClassHeader(mWidget, mComponent)
};

#define SET_DLGCODE(code)  _class->dlgCode = (code);

/**
 * \var g_stmWidgetCls
 * \brief the instance of mWigetClass
 *
 * \sa mWidgetClass
 */
MGNCS_EXPORT extern mWidgetClass g_stmWidgetCls;


/* property support */

/**
 * \enum mWidgetProp
 * \brief the properties id of \ref mWidget
 */

enum mWidgetProp{
	NCSP_WIDGET_RDR = 1,
	NCSP_WIDGET_TEXT,
	NCSP_WIDGET_BKIMAGE,
	NCSP_WIDGET_BKIMAGE_MODE,
	NCSP_WIDGET_BKIMAGE_FILE,
	NCSP_WIDGET_MAX
};
/**
 * \var NCSP_WIDGET_RDR
 * \brief Set the renderer class name
 *  - Type   : const char*
 *  - WriteOnly
 *  - Repaint all the control
 */
/**
 * \var NCSP_WIDGET_TEXT
 * \brief Set the widget 's text, same as SetWindowText
 *  - Type : const char*
 *  - WritedOnly
 *  - Repaint the text of widget
 */
/**
 * \var NCSP_WIDGET_BKIMAGE
 * \brief Set the widget's back image
 *  - Type : PBITMAP
 *  - ReadWirte
 *  - Repaint the background when set it
 *
 */
/**
 * \var NCSP_WIDGET_BKIMAGE_MODE
 * \brief Set the widget's back image mode
 *  - Type : int
 *    - NCS_DM_NORMAL 
 *    - NCS_DM_SCALED
 *    - DrawMode_Title
 *  - ReadWrite
 *  - Repaint the background when set it
 */

/** 
 * \var NCSP_WIDGET_BKIMAGE_FILE
 * \brief Set the widget's back image from file
 *  - Type : const char*, file name of image
 *  - WriteOnly
 *  - Repaint the background when set it
 *
 * \note widget will load a PBITMAP from the file, and release it automatically
 */

/**
 * \def NCSS_NOTIFY
 */
#define NCSS_NOTIFY (0x1)

#define IsNotify(self) \
	(GetWindowStyle(self->hwnd)&NCSS_NOTIFY)

#define NCSS_WIDGET_SHIFT 1

/*
 * \def NCSS_EX_UNNEEDIME
 * \brief the edit uneed ime or not
 */
#define NCSS_EX_UNNEEDIME  0x1 

/**
 * \enum mWidgetNotify
 * \brief the notification code of \ref mWidget
 */

enum mWidgetNotify {
	NCSN_WIDGET_CLICKED = 1,
	NCSN_WIDGET_DBCLICKED,
	NCSN_WIDGET_ENABLED,
	NCSN_WIDGET_DISABLED,
	NCSN_WIDGET_MAX
};

/**
 * \var NCSN_WIDGET_CLICKED
 * \brief send by a widget is cliced by left button
 *
 */
/**
 * \var NCSN_WIDGET_DBCLICKED
 * \brief send by a widget is double cliced by left button
 *
 */
/**
 * \var NCSN_WIDGET_ENABLED
 * \brief send by a widget is enabled
 *
 */
/**
 * \var NCSN_WIDGET_DISABLED
 * \brief send by a widget is disabled
 *
 */


/**
 * \def NCSR_CONTINUE_MSG
 * \brief return value, pass the message to the default window process
 *
 * \sa NCSR_STOP_MSG
 */
#define NCSR_CONTINUE_MSG  FALSE

/**
 * \def NCSR_STOP_MSG
 * \brief return value, return directly, don't pass the message to default window process
 *
 * \sa NCSR_CONTINUE_MSG
 */
#define NCSR_STOP_MSG     TRUE

/**
 * \typedef typedef void (*NCS_CB_ONNCCREATE)(mWidget *);
 *
 * \brief the Callback of Event \a MSG_NCCREATE, which is the first 
 *        message of a control
 *
 * \param mWidget* the event sender's pointer
 *
 * \return none
 */
typedef void (*NCS_CB_ONNCCREATE)(mWidget*);

/**
 * \typedef BOOL (*NCS_CB_ONCREATE)(mWidget*, DWORD dwAddData);
 *
 * \brief the callback of event \a MSG_CREATE
 *
 * \param mWidget * the event sender's pointer
 * \param dwAddData the additional data when create this window, passed by
 *        \a ncscreatewindow, \a ncscreatewindowindirect, \a ncscreatemainwindow
 *        \a ncscreatemainwindowindirect
 *
 * \return True - allow NCS create control continue; FALSE - abort the creating
 *
 * \sa ncsCreateWindow, ncsCreateWindowIndirect, ncsCreateMainWindow, ncsCreateMainWindowIndirect
 */
typedef BOOL (*NCS_CB_ONCREATE)(mWidget*, DWORD dwAddData);

/**
 * \typedef  BOOL (*NCS_CB_ONINITDLG)(mWidget*, HWND hFocus, DWORD dwAddData);
 * \brief the callback of event \a MSG_INITDIALOG, only valid in \a mDialogBox, when a dialog is created
 *
 * \param mWidget * the sender pointer of event
 * \param hFocus the focus window when dialog is created
 * \param dwAddData the additional data passed by \a ncscreatewindow, \a ncscreatewindowindirect, 
 *        \a ncscreatemainwindow \a ncscreatemainwindowindirect
 *
 * \return True - allow dialog continue; FALSE - abort the dialog
 *
 * \sa ncsCreateWindow, ncsCreateWindowIndirect, ncsCreateMainWindow, ncsCreateMainWindowIndirect
 */
typedef BOOL (*NCS_CB_ONINITDLG)(mWidget*, HWND hFocus, DWORD dwAddData);

/**
 * \typedef BOOL (*NCS_CB_ONVOID)(mWidget*, UINT message);
 * \brief the callback of events \a MSG_ACTIVE \a MSG_FONTCHANGED \a MSG_KEYLONGPRESS \a MSG_KEYALWAYSPRESS \a MSG_DESTROY
 *
 * \param mWidget * the pointer of sender
 * \param message the event code, indicate the event
 *
 * \return NCSR_CONTINUE_MSG, NCSR_STOP_MSG 
 *         except MSG_DESTROY, the return value woulbe be ignored
 */
typedef BOOL (*NCS_CB_ONVOID)(mWidget*, UINT message);
/**
 * \typedef void (*NCS_CB_ONSZCHGING)(mWidget*, const PRECT prcExpect, PRECT prcResult);
 * \brief the callback of event \a MSG_SIZECHANGING, when a widget's bound is changing.
 *        If you want change the widget bounds, set the \a prcResult, by default, copy
 *        prcExpect to prcResult
 *
 * \param mWidget * the sender pointer
 * \param prcExpect the excepted bound, IN param
 * \param prcResult the resulted bound, OUT param,
 *
 */
typedef void (*NCS_CB_ONSZCHGING)(mWidget*, const PRECT prcExpect, PRECT prcResult);

/**
 * \typedef BOOL (*NCS_CB_ONSZCHGED)(mWidget*,PRECT prcClient);
 * \brief the callback of event \a MSG_SIZECHANGED
 *
 * This callback is called after a widget's bound changed
 * if you want change the client size of the sender, reset 
 * the \a prcClient
 *
 * \param mWidget * the sender pointer
 * \param prcClient IN & OUT param, the client bound of the sender
 *
 * \return TRUE - the \a prcClient is changed, FALSE - \a prcClient is not changed
 */
typedef BOOL (*NCS_CB_ONSZCHGED)(mWidget*,PRECT prcClient);
//MSG_CSIZECHANGED
/**
 * \typedef void (*NCS_CB_ONCSZCHGED)(mWidget*, int clientWidth, int clientHeight);
 * \brief the callbace of event \a MSG_CSIZECHANGED
 *
 * This is the last message when changing a widget bounds
 *
 * \param mWidget * sender pointer
 * \param clientWidth the new width of sender's client
 * \param clientHeight the new height of sender's client
 *
 */
typedef void (*NCS_CB_ONCSZCHGED)(mWidget*, int clientWidth, int clientHeight);

/**
 * \typedef BOOL (*NCS_CB_ONFONTCHGING)(mWidget*, PLOGFONT logFont);
 * \brief the callback of event MSG_FONTCHANGING
 *
 * This event is called when \b SetWindowFont is called
 *
 * \param mWidget * then sender pointer
 * \param logFont the logic font would be set to sender
 *
 * \return TRUE - tell the SetWindowFont continue set the font, FALSE - abort SetWindowFont
 */
typedef BOOL (*NCS_CB_ONFONTCHGING)(mWidget*, PLOGFONT logFont);
/**
 * \typedef BOOL (*NCS_CB_ONERASEBKGND)(mWidget*, HDC hdc, const PRECT clip);
 * \brief the callback of event \a MSG_ERASEBKGND
 *
 * \param mWidget * the sender pointer
 * \param hdc the DC of background, hdc may be 0, 
 *        so you shoud check it, and call GetClientDC or GetDC when hdc == 0
 * \param clip the area should to be erased. clip may be equal NULL,
 *        so, call GetWindowRect, or GetClientRect when clip == NULL
 *
 * \return NCSR_CONTINUE_MSG, NCSR_STOP_MSG 
 */
typedef BOOL (*NCS_CB_ONERASEBKGND)(mWidget*, HDC hdc, const PRECT clip);
/**
 * \typedef void (*NCS_CB_ONPAINT)(mWidget*, HDC hdc, const PCLIPRGN clip_rgn);
 * \brief the callback of event MSG_PAINT
 *
 * \param mWidget * sender pointer
 * \param hdc the DC for painting
 * \param clip_rgn invlidate region
 *
 */
typedef void (*NCS_CB_ONPAINT)(mWidget*, HDC hdc, const PCLIPRGN clipRgn);
/**
 * \typedef  BOOL (*NCS_CB_ONCLOSE)(mWidget*);
 * \brief the callback of event MSG_CLOSE
 *
 * \param mWidget* the sender pointer
 *
 * \return NCSR_CONTINUE_MSG, NCSR_STOP_MSG 
 */
typedef BOOL (*NCS_CB_ONCLOSE)(mWidget*);

/**
 * \typedef BOOL (*NCS_CB_ONKEY)(mWidget*, UINT message, int code, DWORD keyStatus);
 * \brief the callback of event \a MSG_KEYDOWN \a MSG_KEYUP \a MSG_CHAR \a MSG_SYSKEYDOWN 
 *        \a MSG_SYSKEYUP \a MSG_SYSCHAR
 *
 * \param mWidget *  the sender pointer
 * \param message event code, distinguish the events
 * \param code the scancode (\a MSG_KEYDOWN \a MSG_KEYUP \a MSG_SYSKEYDOWN \a MSG_SYSKEYUP)
 *        or ascii code (\a MSG_CHAR \a MSG_SYSCHAR)
 * \param keyStatus The shift key status when this message occurred
 *
 * \return NCSR_CONTINUE_MSG, NCSR_STOP_MSG 
 */
typedef BOOL (*NCS_CB_ONKEY)(mWidget*, UINT message, int code, DWORD keyStatus);

/**
 * \typedef BOOL(*NCS_CB_ONMOUSE)(mWidget*, UINT message, int x, int y, DWORD keyStatus);
 * \brief the callback of events \a MSG_LBUTTONDOWN \a MSG_LBUTTONUP \a MSG_LBUTTONDBCLK
 *        \a MSG_MOUSEMOVE \a MSG_RBUTTONDOWN \a MSG_RBUTTONUP \a MSG_RBUTTONDBCLK
 *
 * \param mWidget  * sender pointer
 * \param message  the event code
 * \param x the x-coordinate of mouse
 * \param y the y-coordinate of mouse
 * \param keyStatus The shift key status when this message occurred.
 *
 * \return NCSR_CONTINUE_MSG, NCSR_STOP_MSG 
 */
typedef BOOL(*NCS_CB_ONMOUSE)(mWidget*, UINT message, int x, int y, DWORD keyStatus);
/**
 * \typedef BOOL (*NCS_CB_ONNCMOUSE)(mWidget*, UINT message, int x, int y, int  hitCode);
 * \brief the callback of events \a MSG_NCLBUTTONDOWN \a MSG_NCLBUTTONUP \a MSG_NCLBUTTONDBCLK
 *        \a MSG_NCMOUSEMOVE \a MSG_NCRBUTTONDOWN \a MSG_NCRBUTTONUP \a MSG_NCRBUTTONDBCLK
 *
 * \param mWidget * the sender pointer
 * \param message the event code
 * \param x the x-coordinate of mouse
 * \param y the y-coordinate of mouse
 * \param hitCode the hit code 
 *
 * \return NCSR_CONTINUE_MSG, NCSR_STOP_MSG 
 */
typedef BOOL (*NCS_CB_ONNCMOUSE)(mWidget*, UINT message, int x, int y, int  hitCode);
/**
 * \typedef int (*NCS_CB_ONHITTEST)(mWidget*, UINT message,int x, int y);
 * \brief the callback of events \a MSG_HITTEST \a MSG_NCHITTEST
 *
 * \param mWidget * the sender pointer
 * \param message the event code
 * \param x the x-coordinate of hit point
 * \param y the y-coordinate of hit point
 *
 * \return int hit code 
 */
typedef int (*NCS_CB_ONHITTEST)(mWidget*, UINT message,int x, int y);

/**
 * \typedef void (*NCS_CB_ONSCROLL)(mWidget*, UINT message, int code, int pos);
 * \brief the callback of events \a MSG_HSCROLL , \a MSG_VSCROLL
 *
 * \param mWidget * the sender pointer
 * \param message the event code
 * \param nc the scrolling code, see \a MSG_HSCROLL and \a MSG_VSCROOL in MiniGUI
 *
 */
typedef void (*NCS_CB_ONSCROLL)(mWidget*, UINT message, int code, int pos);
/**
 * \typedef BOOL (*NCS_CB_ONCMD)(mWidget*, int id, int nc, HWND hCtrl);
 * \brief the callback of event \a MSG_COMMAND
 *
 * \param mWidget* the sender pointer
 * \param id the child(control or menuitem) id 
 * \param nc the notification code of child
 * \param hCtrl the child's handle ( or 0 if child is a menuitem)
 *
 * \return NCSR_CONTINUE_MSG, NCSR_STOP_MSG 
 */
typedef BOOL (*NCS_CB_ONCMD)(mWidget*, int id, int nc, HWND hCtrl);
/**
 * \typedef void (*NCS_CB_WIDGET_ONTIMER)(mWidget*, int id, DWORD count);
 * \brief the callback of event \a MSG_TIMER
 *
 * \param mWidget* the sender pointer
 * \param id the id of timer
 * \param count the total tick count of timer
 */
typedef void (*NCS_CB_WIDGET_ONTIMER)(mWidget*, int id, DWORD count);
/**
 * \typedef int (*NCS_CB_ONMSG)(mWidget*, UINT message, WPARAM wParam, LPARAM lParam);
 * \brief the callback of a common message event
 *
 * \param mWidget * sender pointer
 * \param message the event code
 * \param wParam the wParam of message event
 * \param lParam the lParam of message event
 *
 * \return int - this value will be as the finally return value of event, and none 
 *         default processing would be called
 */
typedef int (*NCS_CB_ONMSG)(mWidget*, UINT message, WPARAM wParam, LPARAM lParam);
/**
 * \typedef void (*NCS_CB_NOTIFY)(mWidget*, int id, int ncCode, DWORD param);
 * \brief the callback of all notification event
 *
 * \param mWidget * the sender pointer
 * \param ncCode the notification code of event
 * \param param the param of notify message
 *
 */
typedef void (*NCS_CB_NOTIFY)(mWidget*, int id, int ncCode, DWORD addData);

/**
 * \def NCS_NOTIFY_CODE
 * Add a speical flags into a notification code, so that
 * the NCS can distinguish notification events from message
 * events
 */
#define NCS_NOTIFY_CODE(code)   (0xFFFF0000 | (code))
/**
 * \def NCS_MAP_NOTIFY
 * Map a notification event into a handler
 */
#define NCS_MAP_NOTIFY(code, handler) { NCS_NOTIFY_CODE(code), (void*)(handler) }


/** public static function */
void mWidget_captureHotPiece(mWidget *self, mObject *hotpice);
mObject * mWidget_getCapturedHotPiece(mWidget *self);
void mWidget_releaseCapturedHotPiece(void);

void mWidget_setHoveringFocus(mWidget *self, mObject *hotpiece);
mObject * mWidget_getHoveringFocus(mWidget *self);
void mWidget_releaseHoveringFocus(void);

void mWidget_setInputFocus(mWidget *self, mObject *hotpiece);
mObject * mWidget_getInputFocus(mWidget *self);
void mWidget_releaseInputFocus(void);

#ifdef _MGNCS_GUIBUILDER_SUPPORT
//only for GUI Builder 
//used to create default content
#define NCSP_DEFAULT_CONTENT  10000
#endif


    /** @} end of ControlWidget */


	/**
	 * \addtogroup global_defines Global defines
	 * @{
	 */

	/**
	 * \defgroup gobal_control_function Global Control function
	 * @{
	 */


/**
 * \fn BOOL ncsSetFont (HWND hWnd, const char *fontName)
 * \brief set control's font
 *
 * \param hWnd - the handle of control
 * \param fontName - the name of a logical font
 *
 * \return suceess or not
 */
MGNCS_EXPORT BOOL ncsSetFont (HWND hWnd, const char *fontName);

/**
 * \fn BOOL ncsSetProperty(HWND hWnd, int id, DWORD value)
 * \brief set control's property
 *
 * \param hWnd - the handle of control
 * \param id - the property id of control
 * \param value - the value of property
 *
 * \return suceess or not
 *
 * \sa ncsGetProperty
 */
MGNCS_EXPORT BOOL ncsSetProperty(HWND hWnd, int id, DWORD value);
/**
 * \fn void* ncsGetProperty(HWND hWnd, int id)
 * \brief get control's property
 *
 * \param hWnd - the handle of control
 * \param id - the property id
 *
 * \return the value of property
 *
 * \sa ncsSetProperty
 */
MGNCS_EXPORT DWORD ncsGetProperty(HWND hWnd, int id);

/**
 * A property entry
 * 
 * \note this struct is used when calling ncsCreateXXX functions
 *
 * \sa NCS_CREATE_INFO NCS_WND_TEMPLATE NCS_MNWND_TEMPLATE
 */
typedef struct _NCS_PROP_ENTRY {
	/**
	 * The property id
	 */
	int id;
	/**
	 * The property value
	 */
	DWORD value;
}NCS_PROP_ENTRY;

/**
 * A renderer element struct
 *
 * \note This struct is used by \ref NCS_RDR_INFO when create a NCS widget
 *
 * \sa NCS_RDR_INFO
 */
typedef struct _NCS_RDR_ELEMENT {
	/**
	 * The renderer element id
	 */
	int id;
	/**
	 * The remderer element value
	 */
	DWORD value;
}NCS_RDR_ELEMENT;

/**
 * A struct used to include all the renderer info when create a widget
 * 
 * \sa NCS_CREATE_INFO, NCS_MAIN_CREATE_INFO, NCS_WND_TEMPLATE, NCS_MNWND_TEMPLATE
 */
typedef struct _NCS_RDR_INFO {
	/**
	 * The global renderer, the minigui 3.0 defined renderer
	 */
	const char* glb_rdr;
	/**
	 * The control renderer, defined by NCS
	 */
	const char* ctl_rdr;
	/**
	 * The Renderer element array, end by  { -1, 0} 
	 */
	NCS_RDR_ELEMENT* elements;
}NCS_RDR_INFO;

/**
 * A struct include the creating info for creating window
 *
 * \note This struct is used by \ref ncsCreateWindow, donot
 * use it directly. If you want to create a NCS widget by
 * MiniGUI API \b CreateWindow, pass the NCS_CREATE_INFO* as
 * Additional data
 *
 */

typedef struct _NCS_CREATE_NOTIFY_INFO {
    BOOL (*onCreated)(struct _NCS_CREATE_NOTIFY_INFO*, mComponent*, DWORD special_id);
}NCS_CREATE_NOTIFY_INFO;

typedef struct _NCS_CREATE_INFO{
	/**
	 * The properties array pointer, end by {-1, 0}
	 *
	 * \sa NCS_PROP_ENTRY
	 */
	NCS_PROP_ENTRY * props;
	/**
	 * The renderer info pointer
	 *
	 * \sa NCS_RDR_INFO
	 */
	NCS_RDR_INFO * rdr_info;
	/**
	 * The handler events array pointer, end by { -1, NULL }
	 * 
	 * \sa NCS_EVENT_HANDLER
	 */
	NCS_EVENT_HANDLER *handlers;
	/** 
	 * The children controls array pointer
	 *
	 * \sa NCS_WND_TEMPLATE
	 */
	NCS_WND_TEMPLATE * ctrls;
	/**
	 * The count of children controls
	 */
	int ctrl_count;
	/**
	 * The user data passed by \ref ncsCreateWindow \ref ncsCreateMainWindow
	 */
	DWORD user_data;
	
	//FIXED ME Maybe I  should not put these two param here
	/**
	 * The back color of widget, 
	 * 
	 * \note 0 indicate that use the default back color.
	 * Use \a MakeRGBA to make a backcolor, the param 'a' of MakeRGBA 
	 * should be '255' always. e.g. a black back color is MakeRGBA(0,0,0,255)
	 *
	 */
	DWORD				bk_color;
	/**
	 * The logfont of widget
	 */
	const char*			font_name;

    // don't use in the control, to make the NCS_MNWND_TEMPLATE and NCS_WND_TEMPLATE same
    HICON               hIcon;
    HMENU               hMenu;

    NCS_CREATE_NOTIFY_INFO * notify_info;
    DWORD                    special_id;

}NCS_CREATE_INFO;

/**
 * \fn mWidget* ncsCreateWindow (const char *className, const char *caption, DWORD style, DWORD exStyle, \
 *		int id, int x, int y, int w, int h, HWND parent, \
 *		NCS_PROP_ENTRY * props, \
 *		NCS_RDR_INFO * rdrInfo, \
 *		NCS_EVENT_HANDLER * handlers, \
 *		DWORD addData);
 *
 *	\brief create a NCS widget 
 *
 *	\param className the class name of widget. 
 *	       the class name must be register by \ref ncsRegisterComponent. 
 *	       Any drived from \ref mComponent and registered component is validate
 *	\param caption  the caption of the widget, ignored if class_name is a \ref mInvsbComp
 *	\param style the style of widget, ignored if class_name is a \ref mInvsbComp
 *	\param exStyle  the extend style of widget, ignored if class_name is a \ref mInvsbComp
 *	\param id  the id of component
 *	\param x  the x position of widget, ignored if class_name is a \ref mInvsbComp
 *	\param y  the y position of widget, ignored if class_name is a \ref mInvsbComp
 *	\param w  the width of widget ,ignored if class_name is a \ref mInvsbComp
 *	\param h  the height of widget , ignored if class_name is a \ref mInvsbComp
 *  \param parent  the handle of parent, if class_name is a \ref mInvsbComp, the \a parent must
 *         releated a mWidget object
 *   \param props  the properties array pointer, end by {-1, 0} if it's not NULL
 *   \param rdrInfo the renderer info pointer
 *   \param handlers the handlers of event array pointer, end by {-1, NULL}, if it's not NULL
 *   \param addData the additional data send to callback \ref NCS_CB_ONCREATE
 *
 *   \return mWidget* - a mWidget pointer, if class_name is a \ref mInvsbComp, it's a mInvisibleComponent* pointer
 *
 *   \sa ncsCreateMainWindow, nscCreateWindowIndirect, NCS_PROP_ENTRY, NCS_RDR_INFO, NCS_EVENT_HANDLER, NCS_CB_ONCREATE, mWidget, mInvisibleComponent
 */
MGNCS_EXPORT mWidget* ncsCreateWindow (const char *className, const char *caption, DWORD style, DWORD exStyle, \
	int id, int x, int y, int w, int h, HWND parent, \
	NCS_PROP_ENTRY * props, \
	NCS_RDR_INFO * rdrInfo, \
	NCS_EVENT_HANDLER * handlers, \
	DWORD addData);

/**
 * A struct wrap the NCS_CREATE_INFO
 * 
 * \note only allowed using in \ref ncsCreateMainWindow
 * Don't use it directly
 *
 * \sa NCS_CREATE_INFO, ncsCreateMainWindow , ncsCreateMainWindowIndirect
 */
typedef struct _NCS_MAIN_CREATE_INFO {
	/**
	 * The class name of a mMainWnd or its child class
	 *
	 * \note if className is NULL or an invalidate class name
	 * \ref ncsCreateMainWindow and ncsCreateMainWindowIndirect use \ref NCSCTRL_MAINWND
	 * replaced
	 *
	 * \sa NCSCTRL_MAINWND
	 */
	const char* className;
	/**
	 * NCS_CREATE_INFO pointer
	 */
	NCS_CREATE_INFO * create_info;
}NCS_MAIN_CREATE_INFO;

/**
 * \fn mWidget* ncsCreateMainWindow (const char *className, const char *caption, 
 * 			DWORD style, DWORD exStyle, \
 * 			int id, int x, int y, int w, int h, HWND host, \
 * 			HICON hIcon, HMENU hMenu, NCS_PROP_ENTRY * props, \
 * 			NCS_RDR_INFO * rdrInfo, \
 * 			NCS_EVENT_HANDLER * handlers, \
 * 			DWORD addData);
 *
 *	\brief create a NCS main window 
 *
 *	\param className the class name of widget. 
 *	       the class name must be register by \ref ncsRegisterComponent. 
 *	       And must be \ref NCSCTRL_MAINWND or its dirved class.
 *	\param caption the caption of the main window
 *	\param style  the style of main window
 *	\param exStyle  the extend style of main window
 *	\param id the id of main window
 *	\param x the x position of main window
 *	\param y the y position of main window
 *	\param w the width of main window
 *	\param h the height of main window
 *  \param host the handle of host window, can be NULL
 *  \param hIcon the icon of main window
 *  \param hMenu the menu bar handle 
 *  \param props the properties array pointer, end by {-1, 0} if it's not NULL
 *  \param rdrInfo the renderer info pointer
 *  \param handlers the handlers of event array pointer, end by {-1, NULL}, if it's not NULL
 *  \param addData the additional data send to callback \ref NCS_CB_ONCREATE and \ref NCS_CB_ONINITDLG
 *
 *  \return mWidget* - a mWidget pointer, must be a mMainWnd instance 
 *
 *  \sa ncsCreateWindow,ncsCreateWindowIndirect, nscCreateMainWindowIndirect, NCS_PROP_ENTRY, NCS_RDR_INFO, NCS_EVENT_HANDLER, NCS_CB_ONCREATE, mWidget, mInvisibleComponent , mMainWnd
 *
 *  
 */
MGNCS_EXPORT mWidget* ncsCreateMainWindow (const char *className, const char *caption, 
        DWORD style, DWORD exStyle, \
        int id, int x, int y, int w, int h, HWND host, \
        HICON hIcon, HMENU hMenu,
        NCS_PROP_ENTRY * props, \
        NCS_RDR_INFO * rdrInfo, \
        NCS_EVENT_HANDLER * handlers, \
        DWORD addData);

/**
 * A struct include all the creating info, used by ncsCreateWindowIndirect
 *
 * \sa NCS_CREATE_INFO
 */
struct _NCS_WND_TEMPLATE{
	/**
	 * The class name of mComponent, must be registered by \ref ncsRegisterComponent
	 *
	 * \note support \ref mInvsbComp class
	 */
	const char*         class_name;
	/**
	 * The id of commponet
	 */
	intptr_t            id;
	/**
	 * The Location and Size of mWidget, ignored if class_name is a \ref mInvsbComp
	 */
	int                 x, y, w, h;
	/**
	 * The style of mWidget, ignored if class_name is a \ref mInvsbComp
	 */
	DWORD               style;
	/**
	 * The extend style of mWidget, ignored if class_name is a \ref mInvsbComp
	 */
	DWORD               ex_style;
	/**
	 * The caption of mWidget, ignored if class_name is a \ref mInvsbComp
	 */
	const char*         caption;

	//same struct as NCS_CREATE_INFO
	/**
	 * Same as NCS_CREATE_INFO
	 *
	 * \sa NCS_CREATE_INFO
	 */
	NCS_PROP_ENTRY*     props;
	NCS_RDR_INFO*       rdr_info;
	NCS_EVENT_HANDLER*  handlers;
	NCS_WND_TEMPLATE*   ctrls;
	intptr_t            count;
	DWORD               user_data;

	//FIXED ME Maybe I  should not put these two param here
	DWORD				bk_color;
	const char* 		font_name;

    // don't use in the control, to make the NCS_MNWND_TEMPLATE and NCS_WND_TEMPLATE same
    HICON               hIcon;
    HMENU               hMenu;

    NCS_CREATE_NOTIFY_INFO * notify_info;
    DWORD                    special_id;
};

//create control window indirect
/**
 * \fn mWidget* ncsCreateWindowIndirect( const NCS_WND_TEMPLATE* tmpl, HWND hParent);
 * \brief create a mComponent by \ref NCS_WND_TEMPLATE,
 *
 * \param tmpl the template pointer
 * \param hParent the parent handle, if NCS_WND_TEMPLATE.class_name is a \ref mInvsbComp, 
 *        hParent must releated a mWidget object
 *
 * \return mWidget* - then pointer of object or NULL if failed
 *
 * \sa ncsCreateWindow, NCS_WND_TEMPLATE
 */
MGNCS_EXPORT mWidget* ncsCreateWindowIndirect( const NCS_WND_TEMPLATE* tmpl, HWND hParent);

/**
 * A struct include all the creating info for ncsCreateMainWindowIndirect,
 *
 * \note same as \ref ncsCreateMainWindow 's params
 *
 * \sa NCS_WND_TEMPLATE, ncsCreateMainWindow
 */
typedef struct _NCS_MNWND_TEMPLATE{
	const char*         class_name;
	intptr_t            id;
	int                 x, y, w, h;
	DWORD               style;
	DWORD               ex_style;
	const char*         caption;

	NCS_PROP_ENTRY*     props;
	NCS_RDR_INFO*       rdr_info;
	NCS_EVENT_HANDLER*  handlers;
	NCS_WND_TEMPLATE*   ctrls;
	intptr_t            count;
	DWORD               user_data;

	//FIXED ME Maybe I  should not put these two param here
	DWORD				bk_color;
	const char*			font_name;

	HICON               hIcon;
	HMENU               hMenu;


    NCS_CREATE_NOTIFY_INFO * notify_info;
    DWORD                    special_id;

}NCS_MNWND_TEMPLATE;

/**
 * \fn mWidget* ncsCreateMainWindowIndirect(const NCS_MNWND_TEMPLATE* tmpl, HWND hHost);
 * \brief create a main window from a template
 *
 * \param tmpl - the template of main window
 * \param hHost - the host window handler of the main window
 *
 * \return mWidget * - the Instance of mMainWnd or NULL
 *
 * \sa ncsCreateMainWindow, NCS_MNWND_TEMPLATE
 */
MGNCS_EXPORT mWidget* ncsCreateMainWindowIndirect(const NCS_MNWND_TEMPLATE* tmpl, HWND hHost);

/**
 * \fn mWidgetClass* ncsGetMainWndClass(const char* className);
 * \brief Get a Main window Class by className
 *
 * \note this function is used to develop a drived class of mMainWnd
 *
 * \param className - the class name of main window , if className == NULL or invalidate name, 
 *        the function return mMainWnd's class
 *
 * \return mWidgetClass* - the mMainWndClass pointer or its drived class pointer
 */
MGNCS_EXPORT mWidgetClass* ncsGetMainWndClass(const char* className);

/**
 * \fn void ncsNotifyParentEx(mWidget *self, int code, DWORD addData);
 * \brief raise a notification event 
 *
 * \note this function is used to develop a drived class of mWidget
 *
 * \param self - the mWidget object who want to raise a event
 * \param code - the notification event code
 * \param addData - the additional data. 
 *
 */
MGNCS_EXPORT void ncsNotifyParentEx(mWidget * self, int code, DWORD addData);

#define ncsNotifyParent(self, code)  ncsNotifyParentEx((mWidget*)(self), code, 0)

/**
 * \fn static inline mWidget* ncsObjFromHandle(HWND hwnd);
 * \brief Get a Object from window handle
 *
 * \param hwnd - the handle of window
 *
 * \return mWidget * the instance releated this handle, or NULL
 */
static inline mWidget* ncsObjFromHandle(HWND hwnd)
{
	if(IsWindow(hwnd))
		return (mWidget*)(GetWindowAdditionalData2(hwnd));
	return NULL;
}

/**
 * \fn static inline mWidget* ncsGetChildObj(HWND hwnd, int id);
 * \brief Get the child object pointer of window 
 *
 * \param hwnd - the handle of window, which can be a normal MiniGUI window or a NCS window
 * \param id - the child id. The child must be a NCS window
 *
 * \return mWidget *  the instance releated id, or NULL
 */
static inline mWidget* ncsGetChildObj(HWND hwnd, int id)
{
	mWidget * self = ncsObjFromHandle(hwnd);
	return (mWidget*)(self?_c(self)->getChild(self, id):NULL);
}

/**
 * \fn static inline mWidget* ncsGetParentObj(HWND hwnd)
 * \brief Get the parent object pointer of window
 *
 * \param hwnd - the handle of child window, which can be a normal MiniGUI window or a NCS window
 * 
 * \return mWidget * the instance of parent of hwnd
 */
static inline mWidget* ncsGetParentObj(HWND hwnd)
{
	return ncsObjFromHandle(GetParent(hwnd));
}


/**
 * \fn static inline DWORD ncsGetDlgCode(HWND hwnd)
 * \brief Get the dialog code of a NCS window
 *
 * \param hwnd - the handle of NCS window
 *
 * \return DWORD - the dialog code or 0
 */
static inline DWORD ncsGetDlgCode(HWND hwnd)
{

#if 0
	mWidget * widget;
	if((widget=ncsObjFromHandle(hwnd)))
	{
		return _c(widget)->dlgCode;
	}
	return 0;
#endif
	return (DWORD)SendMessage(hwnd, MSG_GETDLGCODE, 0, 0);
}

/**
 * \fn void ncsDestroyWindow(mWidget*self, DWORD endCode);
 * \brief delete the widget window created by ncs.
 *
 */
MGNCS_EXPORT void ncsDestroyWindow(mWidget* self, DWORD endCode);

#define ncsDeleteWidget(self) ncsDestroyWindow(self, 0)


#define NCS_FETCH_MSGBINDINFO(sender, param, lParam) do{ \
	void ** __tmp = (void**)(lParam);                    \
	if(__tmp){                                           \
		register DWORD* __param = (DWORD*)(void*)&(param);        \
		register void** __sender = (void**)(void*)&(sender);     \
		*__param  = (DWORD)__tmp[0];                       \
		*__sender = __tmp[1];                            \
		free(__tmp);                                     \
	}                                                    \
}while(0)

MGNCS_EXPORT BOOL ncsBindEventsToMessage(mObject *sender, HWND hwnd_listener, UINT message, int* event_ids);
MGNCS_EXPORT BOOL ncsBindEventToMessage(mObject* sender, HWND hwnd_listener, UINT message, int event_ids);
MGNCS_EXPORT void ncsRemoveBindWindow(HWND hwnd_listener);
MGNCS_EXPORT void ncsRemoveBindMessage(HWND hwnd_listener, UINT message, mObject* sender/*=NULL*/, int event_id/*=0*/);


/** @} end of global_control_function */

/** @} end of global_defines */


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_WIDGET_H */

