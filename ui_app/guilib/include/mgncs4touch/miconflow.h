/**
 * \file miconflow.h
 * \author ZhaolinHu
 * \date 2010/04/16
 *
 * This file includes the definition of IconView.
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

#ifndef _MGUI_CTRL_ICONFLOW_H
#define _MGUI_CTRL_ICONFLOW_H
 
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Widget_IconView mIconFlow
 * @{
 */

/** 
 * Structure of the iconflow item info, contains information about an item.
 * This structure is used for creating or retrieving an item. 
 */
typedef struct _NCS_ICONFLOW_ITEMINFO
{
    /**
     * The index of the item
     */
    int index;

    /**
     * The bitmap icon of the item
     */
    PBITMAP bmp;

    /**
     * The text label of the item
     */
    const char *label;

    /** Attached additional data of this item */
    DWORD addData;
}NCS_ICONFLOW_ITEMINFO;

typedef struct _mIconFlow mIconFlow;
typedef struct _mIconFlowClass mIconFlowClass;
typedef struct _mIconFlowRenderer mIconFlowRenderer;

/**
 * \def NCSCTRL_ICONFLOWIEW
 * \brief The control class name of mIconFlow.
 */
#define NCSCTRL_ICONFLOW NCSCLASSNAME("iconflow")

/**
 * \def NCSS_ICONFLOW_SHIFT
 * \brief The bits used by mIconFlow in style.
 */
#define NCSS_ICONFLOW_SHIFT     (NCSS_ITEMV_SHIFT+1)

/**
 * \def NCSS_ICONFLOW_SORT
 * \sa NCSS_ITEMV_AUTOSORT
 */
#define NCSS_ICONFLOW_SORT      (NCSS_ITEMV_AUTOSORT)

#define NCSS_ICONFLOW_VERTICAL  (0x0001<<NCSS_ITEMV_SHIFT)

enum ICONFLOW_DIRECTION
{
    ICONFLOW_DIR_NONE,
    ICONFLOW_DIR_PREV,
    ICONFLOW_DIR_NEXT
};

typedef struct _ICONFLOW_POINT3D {
    int x;
    int y;
    float z;
} ICONFLOW_POINT3D;

/**
 * \def NCSS_ICONFLOW_LOOP
 * \sa NCSS_ITEMV_LOOP
 */
#define NCSS_ICONFLOW_LOOP      NCSS_ITEMV_LOOP

#define mIconFlowHeader(className)  \
    mItemViewHeader(className)      \
    mHotPiece *bkgndPiece;          \
    HDC iconFrameDC;                \
    HDC bmpFrameDC;                 \
    HDC drawBuffDC;                 \
    float key;                      \
    int visItemCount;               \
    int prevIndex;                  \
    ICONFLOW_POINT3D point3d[20];   \
    int defItemWidth;               \
    unsigned span;                  \
    POINT mouse;                    \
    BOOL isVertical;                \
    unsigned iconBorder;            \
    BOOL iconUseZoom;               \
    enum ICONFLOW_DIRECTION direction;

/**
 * \struct mIconFlow
 * \brief The structure of mIconFlow control, which derived from
 *        mItemView. It is the abstract superclass of all classes
 *        which represent controls that have items.
 *
 *  - defItemWidth\n
 *    The default icon width.
 *
 *  - nrCol\n
 *    The number of column.
 *
 *  - flags\n
 *    The iconflow flags.
 */
struct _mIconFlow
{
	mIconFlowHeader(mIconFlow)
};

#define mIconFlowClassHeader(clsName, parentClass)                      \
    mItemViewClassHeader(clsName, parentClass)                         \
    void (*beginAnimation)(clsName*, float startvalue, float endvalue);      \
    void (*endAnimation)(clsName*);                                          \
    void (*drawItem)(clsName*, HITEM hItem, HDC hdc, RECT *rcDraw);     \
    void (*runAnimation)(clsName*, float startvalue, float endvalue);    \
    void (*setVisItemCenter)(clsName*, int count ,int width, int height);   \
    void (*setIconSize)(clsName*, int width, int height);               \
    void (*setVisItemCount)(clsName*, int count);                       \
    void (*setSpan)(clsName*, int span);                                \
    HITEM (*addItem)(clsName*, NCS_ICONFLOW_ITEMINFO *info, int *pos);

struct _mIconFlowClass
{
	mIconFlowClassHeader(mIconFlow, mItemView)
};

/**
 * \struct mIconFlowClass
 * \brief The virtual function table of mIconFlow, which derived from
 *        mItemViewClass.
 *
 * - void (*\b setIconSize)(mIconFlow *self, int width, int height);\n
 *   Sets the icon size.
 *
 * - HITEM (*\b addItem)(mIconFlow *self, \ref NCS_ICONFLOW_ITEMINFO *info, int *pos);\n
 *   Add a new item according to \a info to iconflow.
 *      \param info     The item information.
 *      \param pos      The position in which item has been inserted. It can be NULL.
 *      \return         The handle to new item.
 */
#define mIconFlowRendererHeader(clsName, parentClass) \
	mItemViewRendererHeader(clsName, parentClass)

/**
 * \struct mIconFlowRenderer
 * \brief The structure of mIconFlow effector, which derived
 *        from mItemViewRenderer.
 */
struct  _mIconFlowRenderer {
	mIconFlowRendererHeader(mIconFlow, mItemView)
};

/**
 * \enum mIconFlowProp
 * \brief The properties id of mIconFlow.
 */
enum mIconFlowProp
{
    /**
     * The default icon height.
     */
	NCSP_ICONFLOW_DEFICONHEIGHT = NCSP_ITEMV_DEFITEMHEIGHT,

    /**
     * The default icon width.
     */
	NCSP_ICONFLOW_DEFICONWIDTH = NCSP_ITEMV_MAX + 1,
    NCSP_ICONFLOW_BKGNDPIECE,
    NCSP_ICONFLOW_VISITEMCOUNT,
    NCSP_ICONFLOW_ICONFRAME,
    NCSP_ICONFLOW_SPAN,
    NCSP_ICONFLOW_ICONBORDER,
    NCSP_ICONFLOW_ICONUSEZOOM,
    /**
     * The maximum value of mIconFlow properties id.
     */
    NCSP_ICONFLOW_MAX,
};

/**
 * \enum mIconFlowNotify
 * \brief The notification code id of mIconFlow.
 */
enum mIconFlowNotify
{
    NCSN_ICONFLOW_ENTER = NCSN_ITEMV_ENTER,
    NCSN_ICONFLOW_CLICKED = NCSN_ITEMV_CLICKED,
    NCSN_ICONFLOW_SELCHANGED = NCSN_ITEMV_SELCHANGED,
    /**
     * The maximum value of mIconFlow notification code id.
     */
    NCSN_ICONFLOW_MAX = NCSN_ITEMV_MAX + 1,
};

/**
 * \var g_stmIconFlowCls
 * \brief The instance of mIconFlowClass.
 *
 * \sa mIconFlowClass
 */
MGNCS_EXPORT extern mIconFlowClass g_stmIconFlowCls;
    /** @} end of Widget_IconView */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_CTRL_ICONFLOW_H */
