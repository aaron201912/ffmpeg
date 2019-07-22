/**
 * \file mresmgr.h
 * \author XiaoweiYan
 * \date 2009/04/01
 *
 * This file includes the definition of resource managerment.
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

#ifndef _MGUI_RESMANAGER_H
    #define _MGUI_RESMANAGER_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Widget_ResourceManagerment Resource Managerment
 * @{
 */

/**
 * \var typedef GHANDLE HPACKAGE;
 * \brief Handle to resource package.
 */
typedef GHANDLE HPACKAGE;

/**
 * \def HPACKAGE_NULL
 * \brief Null resource package handle.
 */
#define HPACKAGE_NULL          ((GHANDLE)0)

#define NCSRT_MASK             0x0FFF

#define NCSRT_STRING           0x0001
#define NCSRT_TEXT             0x0002
#define NCSRT_IMAGE            0x0004
#define NCSRT_UI               0x0008
#define NCSRT_RDRSET           0x0010
#define NCSRT_RDR              0x0020
#define NCSRT_BINARY           0x0040

/**
 * \def NCSRM_SYSSTR_BASEID
 * \brief Base value of system string id.
 */
#define NCSRM_SYSSTR_BASEID  1

/**
 * \def NCSRM_SYSSTR_BASEID
 * \brief Maximum value of system string id.
 */
#define NCSRM_SYSSTR_MAXID   128

/**
 * \def NCSRM_SYSSTR_DEFRDR
 * \brief The system string id of default renderer.
 */
#define NCSRM_SYSSTR_DEFRDR  (NCSRT_STRING << 16) |NCSRM_SYSSTR_BASEID

/**
 * \def NCSRT_EXT_LEN
 * \brief The maximum length of resource extension.
 */
#define NCSRT_EXT_LEN        15

/**
 * \def NCSRM_VERSION_LEN
 * \brief The maximum length of resource package version.
 */
#define NCSRM_VERSION_LEN    4

/**
 * \def NCSRM_VENDOR_LEN
 * \brief The maximum length of vendor.
 */
#define NCSRM_VENDOR_LEN     16

/**
 * \def NCSRM_ENCODING_LEN
 * \brief The maximum length of encoding.
 */
#define NCSRM_ENCODING_LEN   16

/** Structure defines MiniGUI resource package header. */
typedef struct _NCSRM_HEADER
{
    /** The magic number(MGRP). */
    Uint32  magic;

    /** The default package version: 1.0. \sa NCSRM_VERSION_LEN*/
    char    package_version[NCSRM_VERSION_LEN];

    /** The default version: 1.0. \sa NCSRM_VERSION_LEN*/
    char    format_version[NCSRM_VERSION_LEN];

    char    char_encoding[NCSRM_ENCODING_LEN];

    /** The vendor of resource package. \sa NCSRM_VENDOR_LEN*/
    char    vendor[NCSRM_VENDOR_LEN];

    /** The number of resource section. */
    Uint32  nr_sect;
}NCSRM_HEADER;

/** Structure defines resouce type information. */
typedef struct _NCSRM_TYPEITEM
{
    /** The resouce type. It can be one of the following values:
     *  - NCSRT_STRING \n
     *    The string resource type.
     *  - NCSRT_TEXT \n
     *    The string text resource type.
     *  - NCSRT_IMAGE \n
     *    The image resource type.
     *  - NCSRT_UI \n
     *    The UI resource type.
     *  - NCSRT_RDR \n
     *    The renderer resource type.
     *  - NCSRT_RDRSET \n
     *    The NCSRT_RDR set resource type.
     *  - NCSRT_BINARY \n
     *    The user-defined data resource type.
     */
    Uint32  type;

    /** The ID of file name. */
    Uint32  filename_id;

    /** The offset of the resource type section.
     *  relative to the resource package header.
     */
    Uint32  offset;
}NCSRM_TYPEITEM;

/** Structure defines resource ID item information. */
typedef struct _NCSRM_IDITEM
{
    /** The resource ID */
    Uint32  id;

    /** The ID of file name. */
    Uint32  filename_id;

    /** The offset of incore resource data in data section. */
    Uint32  offset;

}NCSRM_IDITEM;

/** Structure defines UI resource header. */
typedef struct _NCSRM_WINHEADER
{
    /** The class id.*/
    Uint32  class_id;

    /** The window id.*/
    Uint32  wnd_id;

    /** The serial number of window. */
    Uint32  serial_num;

    /** The window caption id.*/
    Uint32  caption_id;

    /** The window renderer resource id.*/
    Uint32  rdr_id;


    /** The x coordinate of the window. */
    Sint32  x;

    /** The y coordinate of the window. */
    Sint32  y;

    /** The width of the window. */
    Uint32  w;

    /** The height of the window. */
    Uint32  h;

    /** The style of the window. */
    DWORD32 style;

    /** The extended style of the window. */
    DWORD32 ex_style;

	/** The back color of window. */
	DWORD32 bk_color;

	/** Reserved. The font id of window. */
	Uint32  font_id;

    /** The size of window information, which contains properties,
     *  additional data  and sub controls. */
    Uint32  size;

    /** The offset of window properties.*/
    Uint32  offset_props;

    /** The offset of controls.*/
    Uint32  offset_ctrls;

    /** The number of window properties.*/
    Uint32  nr_props;

    /** The number of controls.*/
    Uint32  nr_ctrls;

    /** The offset of window additional data.*/
    Uint32  offset_adddata;

    /** The size of window additional data.*/
    Uint32  size_adddata;
}NCSRM_WINHEADER;


/** Structure defines the header of section. */
typedef struct _NCSRM_SECTHEADER
{
    /** The size of section. */
    Uint32  sect_size;

    /** The size of id maps. */
    Uint32  size_maps;
}NCSRM_SECTHEADER;

/** Structure defines default locale information. */
typedef struct _NCSRM_DEFLOCALE_INFO
{
    /** The language code. */
    char    language[4];
    /** The country code. */
    char    country[4];
}NCSRM_DEFLOCALE_INFO;

/** Structure defines locale item information. */
typedef struct _NCSRM_LOCALEITEM
{
    /** The language code. */
    char    language[4];
    /** The country code. */
    char    country[4];

    /** The ID of file name. */
    Uint32  filename_id;

    /** The offset of incore resource data. */
    Uint32  offset;
}NCSRM_LOCALEITEM;

/** Structure defines the header of incore image resource. */
typedef struct _NCSRM_INCORE_IMAGE
{
    /** The size of incore real image data, not include header. */
    Uint32  size;

    /** The extended name of image data. \sa NCSRT_EXT_LEN */
    char    ext[NCSRT_EXT_LEN + 1];

    /** The pointer to real image data. */
    void    *data;
}NCSRM_INCORE_IMAGE;

/**
 * \enum ncsRMRdrResType
 * \brief Renderer resource type.
 */
enum ncsRMRdrResType
{
    /** Color */
    NCSRM_RDRTYPE_COLOR = 0,
    /** Metric */
    NCSRM_RDRTYPE_METRIC,
    /** Image */
    NCSRM_RDRTYPE_IMAGE,
    /** Font */
    NCSRM_RDRTYPE_FONT,
    /** Binary */
    NCSRM_RDRTYPE_BINARY,
	/** String */
	NCSRM_RDRTYPE_STRING,
	/** File */
	NCSRM_RDRTYPE_FILE,
	/** Text */
	NCSRM_RDRTYPE_TEXT,
    /** Maximum value */
    NCSRM_RDRTYPE_MAX,
};

/**
 * Strucutre defines window renderer information.
 */
typedef struct _NCSRM_RDRINFO
{
    /** The identifier of renderer. */
    Uint32  id;
    /** The value of renderer. */
    Uint32  value;
}NCSRM_RDRINFO;

/**
 * Strucutre defines the header of renderer information.
 */
typedef struct _NCSRM_RDRHEADER
{
    /** The identifier of renderer name. */
    Uint32  rdrname_id;
    /** The identifier of class name. */
    Uint32  clsname_id;
}NCSRM_RDRHEADER;

/**
 * Strucutre defines properties information.
 */
typedef struct _NCSRM_PROP
{
    /** The type of property. */
    Sint32 type;
    /** The identifier of property. */
	Sint32 id;
    /** The value of property. */
	DWORD32 value;
} NCSRM_PROP;

/**
 * \fn const char* ncsSetDefaultLocale (char* language, char* country)
 * \brief Set default locale information.
 *
 * \param language The system language environment.
 * \param country The country environment.
 *
 * \return Locale value.
 */
MGNCS_EXPORT const char* ncsSetDefaultLocale (char* language, char* country);

/**
 * \fn const char* ncsGetDefaultLocale(void)
 * \brief Get default locale information.
 *
 * \return Locale value.
 */
MGNCS_EXPORT const char* ncsGetDefaultLocale(void);

/**
 * \fn HPACKAGE ncsLoadResPackageFromFile (const char* fileName)
 * \brief Load a resource package from resource package file.
 *
 * \param fileName  The name of resource package.
 *
 * \return Nonzero on success, otherwize HPACKAGE_NULL.
 */
MGNCS_EXPORT HPACKAGE ncsLoadResPackageFromFile (const char* fileName);

/**
 * \def ncsLoadResPackage
 * \brief An alias of \a ncsLoadResPackageFromFile.
 *
 * \sa ncsLoadResPackageFromFile
 */
#define ncsLoadResPackage  ncsLoadResPackageFromFile

/**
 * \fn int ncsAddRefResPackage(HPACKAGE package)
 * \brief add the reference count for resource package
 *
 * \sa ncsLoadResPackage
 */
MGNCS_EXPORT int ncsAddRefResPackage(HPACKAGE package);

/**
 * \fn void ncsUnloadResPackage (HPACKAGE package)
 * \brief Unload a resource package.
 *
 * \param package The handle of resource package.
 */
MGNCS_EXPORT void ncsUnloadResPackage (HPACKAGE package);

/**
 * \fn HPACKAGE ncsLoadResPackageFromMem (const void* mem, int size)
 * \brief Load a resource package from memory. User get the information
 *        of incore resource package by \a getIncoreResPackInfo firstly,
 *        then call this function to load resource package.
 *
 * \param mem   The memory address.
 * \param size  The size of memory.
 *
 * \return Nonzero on success, otherwize HPACKAGE_NULL.
 */
MGNCS_EXPORT HPACKAGE ncsLoadResPackageFromMem (const void* mem, int size);

/**
 * \fn const char* ncsGetImageFileName (HPACKAGE package, Uint32 resId)
 * \brief Get the name of image file.
 *
 * \param package    The handle of resource package.
 * \param resId     The resource id.
 *
 * \return string on success, otherwise NULL.
 */
MGNCS_EXPORT const char* ncsGetImageFileName (HPACKAGE package, Uint32 resId);

/**
 * \fn int ncsGetBitmap(HDC hdc, HPACKAGE package, \
 *               Uint32 resId,  PBITMAP pBitmap)
 * \brief Load a device-dependent bitmap from resource id.
 *
 * \param hdc        The device context.
 * \param package    The handle of resource package.
 * \param resId     The resource id.
 * \param pBitmap    The pointer to the BITMAP object.
 *
 * \return 0 on success, otherwise failure.
 */
MGNCS_EXPORT int ncsGetBitmap(HDC hdc, HPACKAGE package, Uint32 resId,  PBITMAP pBitmap);

/**
 * \fn void ncsReleaseBitmap (PBITMAP pBitmap)
 * \brief Releases a bitmap.
 *
 * This function unloads the specified bitmap \a pBitmap.
 * It will free the private pixel format and the bits of the bitmap.
 *
 * \param pBitmap The BITMAP object.
 *
 * \sa ncsGetBitmapFromID
 */
MGNCS_EXPORT void ncsReleaseBitmap (PBITMAP pBitmap);

/**
 * \fn int ncsGetMyBitmap(HPACKAGE package, Uint32 resId, \
 *                      PMYBITMAP myBmp, RGB* pal)
 * \brief Load a device-independent bitmap from resource id.
 *
 * \param package   The handle of resource package.
 * \param resId    The resource id.
 * \param myBmp    The pointer to the MYBITMAP object.
 * \param pal       The palette will be returned through this pointer.
 *
 * \return 0 on success, otherwise failure.
 */
MGNCS_EXPORT int ncsGetMyBitmap(HPACKAGE package, Uint32 resId,
        PMYBITMAP myBmp, RGB* pal);

/**
 * \fn void ncsReleaseMyBitmap (PMYBITMAP myBmp)
 * \brief Releases a bitmap.
 *
 * This function releases the specified MYBITMAP object \a myBmp.
 *
 * \param myBmp The MYBITMAP object.
 *
 * \sa ncsGetMyBitmapFromID
 */
MGNCS_EXPORT void ncsReleaseMyBitmap (PMYBITMAP myBmp);

/**
 * \fn const char* ncsGetString (HPACKAGE package, Uint32 resId)
 * \brief Get string.
 *
 * This function gets string according to the specified id \a resId.
 *
 * \param package    The handle of resource package.
 * \param resId     The string resource id.
 *
 * \return string on success, otherwise NULL.
 */
MGNCS_EXPORT const char* ncsGetString (HPACKAGE package, Uint32 resId);

/**
 * \fn BOOL ncsSetSysRdr(HPACKAGE package, Uint32 rdrSetId)
 * \brief Set system renderer.
 *
 * This function set system renderer according to the specified renderer
 * set id \a rdrSetId.
 *
 * \param package    The handle of resource package.
 * \param rdrSetId  The renderer set resource id.
 *
 * \return TRUE on success, otherwise FALSE.
 */
MGNCS_EXPORT BOOL ncsSetSysRdr(HPACKAGE package, Uint32 rdrSetId);

/**
 * \fn BOOL ncsSetWinRdr(HWND hWnd, HPACKAGE package, Uint32 rdrId)
 * \brief Set window renderer.
 *
 * This function set window renderer according to the specified renderer
 * id \a rdrId.
 *
 * \param hWnd       The handle of window.
 * \param package    The handle of resource package.
 * \param rdrId      The renderer resource id.
 *
 * \return TRUE on success, otherwise FALSE.
 */
MGNCS_EXPORT BOOL ncsSetWinRdr(HWND hWnd, HPACKAGE package, Uint32 rdrId);

typedef struct _NCS_EVENT_HANDLER_INFO
{
    int id;
    NCS_EVENT_HANDLER *handler;
}NCS_EVENT_HANDLER_INFO;

/**
 * \fn mMainWnd* ncsCreateMainWindowIndirectFromID (HPACKAGE package, \
                     Uint32 wndId, HWND owner, HICON hIcon, \
                     HMENU hMenu, NCS_EVENT_HANDLER_INFO* handlers, \
					 NCS_EVENT_CONNECT_INFO *connects, \
				 	 DWORD user_data);
 * \brief Create main window.
 *
 * This function create main window according to UI resource ID,
 * parent handle, and other informations.
 *
 * \param package    The handle of resource package.
 * \param wndId     The UI resource id.
 * \param owner      The handle of parent window.
 * \param hIcon     The handle of icon.
 * \param hMenu     The handle of menu.
 * \param handlers   The handler set.
 * \param connects   The handler set of connect
 * \param user_data  The user_data
 *
 * \return window class on success, otherwise NULL.
 */
MGNCS_EXPORT mMainWnd* ncsCreateMainWindowIndirectFromID (HPACKAGE package,
                    Uint32 wndId, HWND owner, HICON hIcon,
                    HMENU hMenu, NCS_EVENT_HANDLER_INFO* handlers,
					NCS_EVENT_CONNECT_INFO *connects,
					DWORD user_data);

#ifdef _MGNCSCTRL_DIALOGBOX
/**
 * \fn int ncsCreateModalDialogFromID (HPACKAGE package, Uint32 dlgId,\
                    HWND owner, HICON hIcon, HMENU hMenu,\
                    NCS_EVENT_HANDLER_INFO* handlers, NCS_EVENT_CONNECT_INFO *connects);
 * \brief Create modal dialog.
 *
 * This function create modal dialog according to UI resource ID,
 * parent handle, and other informations.
 *
 * \param package    The handle of resource package.
 * \param dlgId      The UI resource id.
 * \param owner      The handle of parent window.
 * \param hIcon      The handle of icon.
 * \param hMenu      The handle of menu.
 * \param handlers   The handler set.
 * \param connects   The event connect info.
 *
 * \return If success, the value of the dialog box, i.e., the second argument
 *         passed to EndDialog function which closes the dialog box. Otherwise
 *         -1.
 */
MGNCS_EXPORT int ncsCreateModalDialogFromID (HPACKAGE package, Uint32 dlgId,
                    HWND owner, HICON hIcon, HMENU hMenu,
                    NCS_EVENT_HANDLER_INFO* handlers, NCS_EVENT_CONNECT_INFO *connects);
#endif


/**
 * \fn BOOL ncsGetWndTemplFromID(HPACKAGE package, Uint32 wndId, \
                 NCS_MNWND_TEMPLATE* ptempl, NCS_EVENT_HANDLER_INFO* handlers);
 * \brief get the window template from package
 *
 * This function get the info from package. It will create the memory to save the children controls and properties.
 *
 * \param package  The handler of resource package
 * \param wndId    The window template id in resource
 * \param ptempl   The template buffer
 * \param handlers The event handlers of window
 *
 * \return If success, return TRUE, or Failed return FALSE
 *
 */
MGNCS_EXPORT BOOL ncsGetWndTemplFromID(HPACKAGE package, Uint32 wndId, \
			NCS_MNWND_TEMPLATE * ptempl,
			NCS_EVENT_HANDLER_INFO* handlers);

/**
 * \fn void ncsFreeWndTemplate(NCS_MNWND_TEMPLATE* ptempl);
 * \brief free the window template create by ncsGetWndTemplFromID
 *
 * free the content of window template, but don't free itself
 *
 * \param ptempl the template to be free
 *
 * \return void
 *
 * \sa ncsGetWndTemplFromID
 */
MGNCS_EXPORT void ncsFreeWndTemplate(NCS_MNWND_TEMPLATE *ptempl);


    /** @} end of Widget_ResourceManagerment*/
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* _MGUI_RESMANAGER_H */
