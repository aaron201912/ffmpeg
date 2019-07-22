/**
 * \file mrdr.h
 * \author dongjunjie
 * \date 2009/02/13
 *
 * This file include the base renderer information of ncs.
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

#ifndef _MGNCS_RENDER_H
#define _MGNCS_RENDER_H

#ifdef __cplusplus
extern "C" {
#endif


	/**
	 * \addtogroup global_defines Global defines
	 * @{
	 */
	/**
	 * defgroup Renderer_functions Global Renderer function
	 * @{
	 */

#define NCS_TYPE_MASK               WE_ATTR_TYPE_MASK
#define NCS_TYPE_COLOR_MASK         WE_ATTR_TYPE_COLOR_MASK
#define NCS_TYPE_INDEX_MASK         WE_ATTR_TYPE_INDEX_MASK

#define NCS_TYPE_METRICS            0x6000
#define NCS_TYPE_COLOR              0x7000
#define NCS_TYPE_IMAGE              0x8000
#define NCS_TYPE_INT                0x9000
#define NCS_TYPE_FGC                NCS_TYPE_COLOR
#define NCS_TYPE_BGC                (NCS_TYPE_COLOR | 0x0100) 

#define NCS_TYPE_IS_IMAGE(id)       (((id) & NCS_TYPE_MASK) == NCS_TYPE_IMAGE)

#define NCS_METRICS_BASEID          (NCS_TYPE_METRICS | 0)
#define NCS_FGC_BASEID              (NCS_TYPE_FGC | 0)
#define NCS_BGC_BASEID              (NCS_TYPE_BGC | 0)
#define NCS_INT_BASEID              (NCS_TYPE_INT | 0)
#define NCS_IMAGE_BASEID            (NCS_TYPE_IMAGE | 0)

#define NCS_FONT_NUMBER             0
#define NCS_METRICS_NUMBER          2 
#define NCS_COLORS_NUMBER           9
#define NCS_IMAGE_NUMBER            17 
#define NCS_INT_NUMBER              5

//common for all renderer
#define NCS_FGC_3DBODY              NCS_FGC_BASEID
#define NCS_BGC_3DBODY              NCS_BGC_BASEID 

#define NCS_FGC_WINDOW              NCS_FGC_BASEID  + 1
#define NCS_BGC_WINDOW              NCS_BGC_BASEID  + 1 

#define NCS_FGC_SELECTED_ITEM       NCS_FGC_BASEID  + 2
#define NCS_BGC_SELECTED_ITEM       NCS_BGC_BASEID  + 2 

#define NCS_FGC_HILIGHT_ITEM        NCS_FGC_BASEID  + 3
#define NCS_BGC_HILIGHT_ITEM        NCS_BGC_BASEID  + 3 

#define NCS_FGC_DISABLED_ITEM       NCS_FGC_BASEID  + 4
#define NCS_BGC_DISABLED_ITEM       NCS_BGC_BASEID  + 4 

#define NCS_BGC_SELITEM_NOFOCUS     NCS_BGC_BASEID  + 5 

#define NCS_IMAGE_BKGND             NCS_IMAGE_BASEID
#define NCS_MODE_BKIMAGE            NCS_INT_BASEID

//flat
#define NCS_METRICS_BORDER_WIDTH    (NCS_METRICS_BASEID)
#define NCS_METRICS_BORDER_RADIUS   (NCS_METRICS_BASEID + 1)

//fashion
#define NCS_METRICS_3DBODY_ROUNDX   (NCS_METRICS_BASEID)
#define NCS_METRICS_3DBODY_ROUNDY   (NCS_METRICS_BASEID + 1)
#define NCS_MODE_BGC                NCS_INT_BASEID  + 1
#define NCS_MODE_FGC                NCS_INT_BASEID  + 2
#define NCS_MODE_ITEM               NCS_INT_BASEID  + 3
#define NCS_MODE_USEFLAT            NCS_INT_BASEID  + 4

//fashion.ProgressBar
#define NCS_BGC_PRGBAR_CHUNK        NCS_BGC_BASEID  + 6
//fashion.TrackBar
#define NCS_BGC_TRKBAR_SLIDER       NCS_BGC_BASEID  + 7
#define NCS_BGC_TRKBAR_THUMB        NCS_BGC_BASEID  + 8

//flat
#define NCS_BGC_PRPSHT_NMLTAB       NCS_BGC_BASEID  + 9

//skin
#define NCS_IMAGE_ARROWS            NCS_IMAGE_BASEID + 1
#define NCS_IMAGE_ARROWSHELL        NCS_IMAGE_BASEID + 2
//skin.Button
#define NCS_IMAGE_BUTTON            NCS_IMAGE_BASEID + 3
//skin.CheckButton
#define NCS_IMAGE_CHKBTN            NCS_IMAGE_BASEID + 4
//skin.RadioButton
#define NCS_IMAGE_RDOBTN            NCS_IMAGE_BASEID + 5
//skin.ListView
#define NCS_IMAGE_LISTV_TREE        NCS_IMAGE_BASEID + 6
#define NCS_IMAGE_LISTV_HDR         NCS_IMAGE_BASEID + 7
//skin.PropSheet
#define NCS_IMAGE_PRPSHT_TAB        NCS_IMAGE_BASEID + 8
//skin.ProgressBar
#define NCS_IMAGE_PRGBAR_HCHUNK     NCS_IMAGE_BASEID + 9
#define NCS_IMAGE_PRGBAR_VCHUNK     NCS_IMAGE_BASEID + 10
#define NCS_IMAGE_PRGBAR_HSLIDER    NCS_IMAGE_BASEID + 11
#define NCS_IMAGE_PRGBAR_VSLIDER    NCS_IMAGE_BASEID + 12
//skin.TrackBar
#define NCS_IMAGE_TRKBAR_HSLIDER    NCS_IMAGE_BASEID + 13
#define NCS_IMAGE_TRKBAR_VSLIDER    NCS_IMAGE_BASEID + 14
#define NCS_IMAGE_TRKBAR_HTHUMB     NCS_IMAGE_BASEID + 15
#define NCS_IMAGE_TRKBAR_VTHUMB     NCS_IMAGE_BASEID + 16

/**
 * \fn DWORD ncsGetElementEx (mWidget *self, const char* rdrName, int id);
 * \brief Get the value of element id in specified window or renderer.
 *
 * When the window class is valid, it will return the element id value in
 * current window. Otherwise return the value in specifed renderer.
 *
 * \param self      The instance.
 * \param rdrName   The renderer name.
 * \param id        The window element id number, can be one of the 
 *                  following value:
 *                  - NCS_FGC_3DBODY\n
 *                  - NCS_BGC_3DBODY\n
 *                  - NCS_FGC_WINDOW\n
 *                  - NCS_BGC_WINDOW\n
 *                  - NCS_FGC_SELECTED_ITEM\n
 *                  - NCS_BGC_SELECTED_ITEM\n
 *                  - NCS_FGC_HILIGHT_ITEM\n
 *                  - NCS_BGC_HILIGHT_ITEM\n
 *                  - NCS_FGC_DISABLED_ITEM\n
 *                  - NCS_BGC_DISABLED_ITEM\n
 *                  - NCS_BGC_SELITEM_NOFOCUS\n
 *
 *                  - NCS_METRICS_3DBODY_ROUNDX\n
 *                  - NCS_METRICS_3DBODY_ROUNDY\n
 *
 *                  - NCS_MODE_BGC\n
 *                  - NCS_MODE_FGC\n
 *                  - NCS_MODE_ITEM\n
 *                  - NCS_MODE_USEFLAT\n
 *
 *                  - NCS_BGC_PRGBAR_CHUNK\n
 *                  - NCS_BGC_TRKBAR_SLIDER\n
 *                  - NCS_BGC_TRKBAR_THUMB\n
 *
 *                  - NCS_IMAGE_ARROWS\n
 *                  - NCS_IMAGE_ARROWSHELL\n
 *                  - NCS_IMAGE_BUTTON\n
 *                  - NCS_IMAGE_CHKBTN\n
 *                  - NCS_IMAGE_RDOBTN\n
 *                  - NCS_IMAGE_LISTV_TREE\n
 *                  - NCS_IMAGE_LISTV_HDR\n
 *                  - NCS_IMAGE_PRPSHT_TAB\n
 *                  - NCS_IMAGE_PRGBAR_HCHUNK\n
 *                  - NCS_IMAGE_PRGBAR_VCHUNK\n
 *                  - NCS_IMAGE_PRGBAR_HSLIDER\n
 *                  - NCS_IMAGE_PRGBAR_VSLIDER\n
 *                  - NCS_IMAGE_TRKBAR_HTHUMB\n
 *                  - NCS_IMAGE_TRKBAR_VTHUMB\n
 *                  - NCS_IMAGE_TRKBAR_HSLIDER\n
 *                  - NCS_IMAGE_TRKBAR_VSLIDER\n
 *
 * \sa ncsSetElementEx
 */
MGNCS_EXPORT DWORD ncsGetElementEx (mWidget *self, const char* rdrName, int id);

/**
 * \fn DWORD ncsSetElementEx (mWidget* self, const char* rdrName, int id, DWORD value);
 * \brief Set the value of element id in specified window or renderer.
 *
 * When the window class is valid, it will set the element id value in
 * current window. Otherwise set the value in specifed renderer.
 *
 * \param self      The instance.
 * \param rdrName   The renderer name.
 * \param id        The element id.
 * \param value     The new value.
 *
 * \return The old value.
 * \sa ncsGetElementEx
 */
MGNCS_EXPORT DWORD ncsSetElementEx (mWidget* self, const char* rdrName, int id, DWORD value);

typedef struct _NCS_RDR_ID_NAME {
    const char* we_name;
    int         we_id;
}NCS_RDR_ID_NAME;

// id_names can be NULL, or end with (NULL, -1)
MGNCS_EXPORT BOOL ncsLoadRdrElementsFromEtcFile(const char* etcfile, const char** rdrNames, int count, NCS_RDR_ID_NAME * id_names);

MGNCS_EXPORT BOOL ncsLoadRdrElementsFromEtcHandle(GHANDLE hEtc, const char** rdrNames, int count, NCS_RDR_ID_NAME* id_names);

/**
 * \fn GHANDLE ncsLoadRdrEtcFile (const char* etcfile, const char** rdrName, int count);
 * \brief Load renderer configure file from file system or incore mode.
 *
 * \param etcfile   The name of configuration file.
 * \param rdrName   The renderer name set.
 * \param count     The array size of \a rdrName.
 */
MGNCS_EXPORT GHANDLE ncsLoadRdrEtcFile (const char* etcfile, const char** rdrName, int count);

/**
 * \fn int ncsUnloadRdrEtcFile(GHANDLE hEtcFile);
 * \brief Unload renderer configuration file.
 *
 * \return 0 on success, otherwise -1.
 */
MGNCS_EXPORT int ncsUnloadRdrEtcFile(GHANDLE hEtcFile);

/*
 * \def var g_ncsEtcHandle
 * \breif the MGNCS configuration handle
 */
MGNCS_EXPORT extern GHANDLE g_ncsEtcHandle;

/*
 * \fn ncsSetEtcHandler(GHANDLE hEtc);
 * \brief set the outside of etc handler of mgnc.cfg. 
 * can used for incore resource
 *
 * \sa g_ncsEtcHandler
 */
static inline void ncsSetEtcHandle(GHANDLE hEtc)
{
    g_ncsEtcHandle = hEtc;
}



/**
 * \def ncsGetElement(self, id)
 * \brief Get the value of element id in specified window.
 */
#define ncsGetElement(self, id) \
    ncsGetElementEx((mWidget*)self, NULL, id)

/**
 * \def ncsSetElement(self, id, value)
 * \brief Set the value of element id in specified window.
 */
#define ncsSetElement(self, id, value) \
    ncsSetElementEx((mWidget*)self, NULL, id, value)

/**
 * \fn mWidgetRenderer *ncsRetriveCtrlRDR (const char *rendererName, const char *className);
 * \brief get a renderer of special control
 *
 * \param rendererName - the renderer name , eg. classic, skin, fashion, flat, and so
 * \param className - the control's className
 *
 * \return the implemention of renderer
 *
 * \sa ncsRegisterCtrlRDR, ncsRegisterCtrlRDRs
 */
MGNCS_EXPORT mWidgetRenderer *ncsRetriveCtrlRDR (const char *rendererName, const char *className);

/**
 * \fn BOOL ncsRegisterCtrlRDR (const char *rendererName, const char *className, \
 * 		mWidgetRenderer *);
 * \brief register one of implemention of a special control into system
 *
 * \param rendererName - the name of renderer, eg. skin, classic, fashion, flat, and so on
 * \param className - the control's classname
 * \param renderer - the renderer which would be rensigered
 *
 * \note if the system have not any renderer of special "rendererName", system will create a renderer table to recieve it
 *
 * \return sucesses or not
 *
 * \sa ncsRegisterCtrlRDRs, ncsRetriveCtrlRDR, ncsUnregisterCtrlRDRs
 */
MGNCS_EXPORT BOOL ncsRegisterCtrlRDR (const char *rendererName, const char *className,
			  mWidgetRenderer *renderer);

/**
 * \fn void ncsUnregisterCtrlRDRs(const char* rdrName=NULL, const char* className=NULL);
 * \brief unregister a special rdr
 *
 * \param rdrName the name of the unregistering renender
 * \param className the special class of the render
 *
 * \sa ncsRegisterCtrlRDRs
 */
MGNCS_EXPORT void ncsUnregisterCtrlRDRs(const char* rdrName/*=NULL*/, const char* className/*=NULL*/);

/**
 * A struct to include a  Renderer register info
 *
 * \sa ncsRegisterCtrlRDRs
 */
typedef struct _NCS_RDR_ENTRY
{
  /**
   * class name of a mWidget Control
   */
  const char *className;
  /**
   * An implemention of interface of mWidget whose name is className
   */
  mWidgetRenderer *renderer;
} NCS_RDR_ENTRY;

/**
 * \fn BOOL ncsRegisterCtrlRDRs (const char *rendererName, NCS_RDR_ENTRY * entries,
 *		int count);
 * \brief register a group of renderers into system
 *
 * \param rendererName - the name of special renderer
 * \param entries - the table of renderers, \sa NCS_RDR_ENTRY
 * \param count - the table size of entries
 *
 * \return sucess or not
 *
 * \sa ncsRegisterCtrlRDR, NCS_RDR_ENTRY, ncsRetriveCtrlRDR
 */
MGNCS_EXPORT BOOL ncsRegisterCtrlRDRs (const char *rendererName, NCS_RDR_ENTRY * entries,
			   int count);

/**
 * \fn BOOL ncsSetSystemRenderer(const char* name);
 * \brief Set the System renderer. If a system renderer would be used by all 
 *        the controls who haven't been set the property "renderer", see 
 *        \ref mWidget and \ref mWidgetRenderer.
 *
 * \param name - The name of system renderer.
 *
 * \return TRUE - sucess, FALSE - failed
 *
 * \note same as basic renderer. If the renderer specialed by param \a name 
 * is not exist, it will be store the name, and wait user call \ref 
 * ncsRegisterCtrlRDR or \ref ncsRegisterCtrlRDRs; the new registered 
 * renderer would be set as new system renderer if the thir renderer name 
 * are same.
 *
 */
MGNCS_EXPORT BOOL ncsSetSystemRenderer(const char* name);

/**
 * \fn const char* ncsGetSystemRenderer(void); 
 * \brief get the system renderer name
 *
 * \return return the current system renderer name or "\0";
 *
 */
MGNCS_EXPORT const char* ncsGetSystemRenderer(void);

	/** @} end of Renderer_functions */

/** @} end of global_defines */

/** 
 * \addtogroup global_rdr_elements Renderer Elements 
 * @{
 */

/**
 * \defgroup CommonRdrElements Common Renderer Elements
 * @{
 */

/**
 * \def INVALID_RDR_VALUE
 * \brief invalid renderer element value
 * 
 */
#define INVALID_RDR_VALUE  ((DWORD)-1)

static inline gal_pixel ncsColor2Pixel(HDC hdc, DWORD color)
{
	return RGBA2Pixel(hdc,GetRValue(color),
		GetGValue(color),
		GetBValue(color),
		GetAValue(color));
}

MGNCS_EXPORT void ncsCommRDRDrawFocusFrame(HWND hWnd, HDC hdc, const RECT *rc);
MGNCS_EXPORT BOOL ncsCommRDRDrawBkgnd(HWND hWnd, HDC hdc, const RECT* inv_rc);

MGNCS_EXPORT void ncsCommRDRDraw3dbox(HDC hdc, const RECT* rc,  DWORD color, DWORD flag);
MGNCS_EXPORT void ncsCommRDRDraw3DRoundBox(HDC hdc, const RECT *rc, int rx, int ry, DWORD color, DWORD flag);

#define NCS_ROUND_CORNER_LEFT_TOP  			(0x1)
#define NCS_ROUND_CORNER_LEFT_BOTTOM		(0x2)
#define NCS_ROUND_CORNER_RIGHT_TOP          (0x4)
#define NCS_ROUND_CORNER_RIGHT_BOTTOM       (0x8)
#define NCS_ROUND_CORNER_LEFTS              (NCS_ROUND_CORNER_LEFT_TOP|NCS_ROUND_CORNER_LEFT_BOTTOM)
#define NCS_ROUND_CORNER_RIGHTS             (NCS_ROUND_CORNER_RIGHT_TOP|NCS_ROUND_CORNER_RIGHT_BOTTOM)
#define NCS_ROUND_CORNER_TOPS               (NCS_ROUND_CORNER_LEFT_TOP|NCS_ROUND_CORNER_RIGHT_TOP)
#define NCS_ROUND_CORNER_BOTTOMS            (NCS_ROUND_CORNER_LEFT_BOTTOM|NCS_ROUND_CORNER_RIGHT_BOTTOM)
#define NCS_ROUND_CORNER_ALL                (NCS_ROUND_CORNER_LEFT_TOP|NCS_ROUND_CORNER_LEFT_BOTTOM| \
												NCS_ROUND_CORNER_RIGHT_TOP|NCS_ROUND_CORNER_RIGHT_BOTTOM)
#define NCS_BORDER_LEFT                     0x1000
#define NCS_BORDER_RIGHT                    0x2000
#define NCS_BORDER_TOP                      0x4000
#define NCS_BORDER_BOTTOM                   0x8000
#define NCS_BORDER_ALL                      (NCS_BORDER_LEFT|NCS_BORDER_RIGHT|NCS_BORDER_TOP|NCS_BORDER_BOTTOM)

MGNCS_EXPORT void ncsCommRDRDraw3DHalfRoundBox(HDC hdc, const RECT *rc, int rx, int ry, DWORD color, DWORD flag, DWORD part_flag);
MGNCS_EXPORT void ncsCommRDRDrawHalfRoundRect(HDC hdc, const RECT *rc, int rx, int ry, DWORD color, DWORD part_flag);

MGNCS_EXPORT void ncsCommRDRFillHalfRoundRect(HDC hdc, const RECT *rc, int rx, int ry, DWORD color, DWORD part_flag);

MGNCS_EXPORT void ncsCommRDRDraw3DEllipse(HDC hdc, const RECT *rc, DWORD color1, DWORD color2, DWORD flag);

MGNCS_EXPORT void ncsCommRDRDrawCheckbox(HDC hdc, const RECT* rc, DWORD fgcolor, DWORD bgcolor, DWORD discolor, DWORD flag);

MGNCS_EXPORT void ncsCommRDRDrawRadio(HDC hdc, const RECT* rc,  DWORD fgcolor, DWORD bgcolor, DWORD discolor,DWORD flag);

MGNCS_EXPORT void ncsCommRDRDrawArrow(HDC hdc, const RECT* rc, int arrow,DWORD color, BOOL bFill);

MGNCS_EXPORT void ncsCommRDRDrawItem(HDC hdc, const RECT* rc, DWORD color);

MGNCS_EXPORT void ncsCommRDRDrawSmallHalfRoundBox(HDC hdc, const RECT *rc, DWORD flags, BOOL bFill);

enum ncsCommRDRColorCalc {
	NCSR_COLOR_DARKER = -100,
	NCSR_COLOR_DARKEST = -200 ,
	NCSR_COLOR_LIGHTER = 100 ,
	NCSR_COLOR_LIGHTEST = 200
};

MGNCS_EXPORT DWORD ncsCommRDRCalc3dboxColor(DWORD color, int degree);

//skin

#define DI_FILL_TILE    0x01
#define DI_FILL_STRETCH 0x02

/* Draw info */
typedef struct _DRAWINFO
{
    /* The pointer to the whole bitmap. */
    const BITMAP* bmp;

    /* The number of line in this bitmap. */
    unsigned int nr_line;

    /* The number of colume in this bitmap. */
    unsigned int nr_col;

    /* The index of line to current sub bitmap. */
    unsigned int idx_line;

    /* The index of colume to current sub bitmap. */
    unsigned int idx_col;

    /* The left/top margin value of current sub bitmap. */
    unsigned int margin1;

    /* The right/bottom margin value of current sub bitmap. */
    unsigned int margin2;

    /* The margin direct. FALSE for left-to-right (default), 
     * TRUE for top-to-bottom. */
    BOOL direct;

    /* The flip flag. FALSE for not flip sub bitmap (default), 
     * TRUE for flip. It can be used by draw_tab function.*/
    BOOL flip;

    /* The style of drawing bitmap. It can be one of the 
     * following value:
     *  - DI_FILL_TILE
     *  - DI_FILL_STRETCH
     */
    unsigned int style;

} DRAWINFO;

MGNCS_EXPORT void ncsSkinDraw(HDC hdc, const RECT* rc, const DRAWINFO *di);

/** @} end of CommonRdrElements */

/** @} end of global_rdr_elements */
#ifdef __cplusplus
}
#endif

#endif
