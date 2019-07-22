/**
 * \file mlist_layout.h
 * \author XiaoweiYan
 * \date 2009/11/10
 *
 * This file includes the definition of mListLayout.
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


#ifndef _MGUI_WIDGET_LAYOUT_H
#define _MGUI_WIDGET_LAYOUT_H

#ifdef _MGNCSCTRL_LIST

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Widget_ListLayout mListLayout
 * @{
 */

typedef struct _mListLayout mListLayout;
typedef struct _mListLayoutClass mListLayoutClass;

#define mListLayoutHeader(className)\
	mObjectHeader(className) \
    mAbstractList *list;            \
    Uint16  itemHeight;             \
    Uint16  itemWidth;

struct _mListLayout
{
	mListLayoutHeader(mListLayout)
};

#define mListLayoutClassHeader(clsName, superCls)           \
	mObjectClassHeader(clsName, superCls)            \
	BOOL (*setProperty)(clsName*, int id, DWORD value);     \
	DWORD (*getProperty)(clsName*, int id);                 \
    mNode* (*onDirKey)(clsName*,mNode* refNode, int direct);\
    mNode* (*onMouseHit)(clsName*, int mouseX, int mouseY); \
    int (*getNodeRect)(clsName*, mNode *node, RECT *rcNode);\
    BOOL (*recalcContSize)(clsName*, int flags);            \
    void (*init)(clsName*, mAbstractList *list, int defItemW, int defItemH);\
    void (*update)(clsName*, HDC hdc, const RECT* rcVis, RECT *rcCont);     \
    void (*nodeEvent)(clsName*, int eventId, DWORD eventInfo);

struct _mListLayoutClass
{
	mListLayoutClassHeader(mListLayout, mObject)
};

enum mListLayoutStatus
{
     NCSF_LIST_LAYOUT_ITEMHCHANGED = 0x01,
     NCSF_LIST_LAYOUT_ITEMWCHANGED = 0x02,
};

/**
 * \enum mListLayoutProp
 * \brief The properties id of mListLayout.
 */
enum mListLayoutProp
{

    NCSP_LIST_LAYOUT_DEFITEMHEIGHT = 10,//RW
    NCSP_LIST_LAYOUT_DEFITEMWIDTH, //RW
    /**
     * The maximum value of mListLayout properties id.
     */
	NCSP_LIST_LAYOUT_MAX,
};

/**
 * \var g_stmListLayoutCls
 * \brief The instance of mListLayoutClass.
 *
 * \sa mListLayoutClass
 */
MGNCS_EXPORT extern mListLayoutClass g_stmListLayoutCls;

    /** @} end of Widget_ListLayout */

/*=========================================================*/
typedef struct _mLVIconLayout mLVIconLayout;
typedef struct _mLVIconLayoutClass mLVIconLayoutClass;

#define mLVIconLayoutHeader(className)  \
    mListLayoutHeader(className)        \
    int nrCol;

struct _mLVIconLayout
{
	mLVIconLayoutHeader(mLVIconLayout)
};

#define mLVIconLayoutClassHeader(clsName, superCls) \
	mListLayoutClassHeader(clsName, superCls)

struct _mLVIconLayoutClass
{
	mLVIconLayoutClassHeader(mLVIconLayout, mListLayout)
};

MGNCS_EXPORT extern mLVIconLayoutClass g_stmLVIconLayoutCls;

/*=========================================================*/
typedef struct _mLHIconLayout mLHIconLayout;
typedef struct _mLHIconLayoutClass mLHIconLayoutClass;

#define mLHIconLayoutHeader(className)  \
    mListLayoutHeader(className)        \
    Uint16 nrRow;

struct _mLHIconLayout
{
	mLHIconLayoutHeader(mLHIconLayout)
};

#define mLHIconLayoutClassHeader(clsName, superCls) \
	mListLayoutClassHeader(clsName, superCls)

struct _mLHIconLayoutClass
{
	mLHIconLayoutClassHeader(mLHIconLayout, mListLayout)
};

MGNCS_EXPORT extern mLHIconLayoutClass g_stmLHIconLayoutCls;

/*=========================================================*/
typedef struct _mLHCenterBoxLayout mLHCenterBoxLayout;
typedef struct _mLHCenterBoxLayoutClass mLHCenterBoxLayoutClass;

#define mLHCenterBoxLayoutHeader(className)     \
    mLHIconLayoutHeader(className)              \
    int offx;

struct _mLHCenterBoxLayout
{
	mLHCenterBoxLayoutHeader(mLHIconLayout)
};

#define mLHCenterBoxLayoutClassHeader(clsName, superCls) \
	mLHIconLayoutClassHeader(clsName, superCls)

struct _mLHCenterBoxLayoutClass
{
	mLHCenterBoxLayoutClassHeader(mLHCenterBoxLayout, mLHIconLayout)
};

/**
 * \enum mLHCenterBoxLayoutProp
 * \brief The properties id of mLHCenterBoxLayoutProp.
 */
enum mLHCenterBoxLayoutProp
{
    /**
     * The x offset value.
     */
    NCSP_HCBOX_LAYOUT_OFFX = NCSP_LIST_LAYOUT_MAX + 1, //RO
    /**
     * The maximum value of mHCenterBoxLayout properties id.
     */
	NCSP_HCBOX_LAYOUT_MAX,
};

MGNCS_EXPORT extern mLHCenterBoxLayoutClass g_stmLHCenterBoxLayoutCls;

/*=========================================================*/
typedef struct _NCS_GROUP_INFO
{
    int id;
    char *text;
    DWORD addData;
}NCS_GROUP_INFO;

typedef struct _mLGroupLayout mLGroupLayout;
typedef struct _mLGroupLayoutClass mLGroupLayoutClass;

#define NCSID_UNNAMED_GROUP         0
#define NCSID_UNNAMED_GROUPTEXT     "Unnamed"
//return group id, if no group return NCSID_UNNAMED_GROUP
typedef int (*NCS_CB_INGROUP)(mNode *node);

#define mLGroupLayoutHeader(className)  \
    mLVIconLayoutHeader(className)      \
    NCS_CB_CMPNODE  cbCmpNode;          \
    NCS_CB_CMPNODE  cbCmpGroup;         \
    NCS_CB_INGROUP  cbInGroup;          \
    BOOL            defaultGroup;       \
    mNode           *groupRoot;         \
    int             defGroupH;

struct _mLGroupLayout
{
	mLGroupLayoutHeader(mLGroupLayout)
};

#define mLGroupLayoutClassHeader(clsName, superCls) \
	mLVIconLayoutClassHeader(clsName, superCls)       \
    void (*setGroupInfo)(clsName*, NCS_CB_INGROUP inGroup, NCS_GROUP_INFO *groupInfo, int groupSize);\
    NCS_CB_CMPNODE (*decorateNodeCmp)(clsName*, NCS_CB_CMPNODE func);   \
    NCS_CB_INGROUP (*setInGroupFunc)(clsName*, NCS_CB_INGROUP func);    \
    BOOL (*resetGroup)(clsName*, NCS_GROUP_INFO *groupInfo, int groupSize);

struct _mLGroupLayoutClass
{
	mLGroupLayoutClassHeader(mLGroupLayout, mLVIconLayout)
};


/**
 * \enum mLGroupLayoutProp
 * \brief The properties id of mLGroupLayout.
 */
enum mLGroupLayoutProp
{

    NCSP_LGROUP_LAYOUT_NODECMPFUNC = NCSP_LIST_LAYOUT_MAX + 1, //RO

    NCSP_LGROUP_LAYOUT_GROUPCMPFUNC,//RW
    /**
     * The maximum value of mLGroupLayout properties id.
     */
	NCSP_LGROUP_LAYOUT_MAX,
};

MGNCS_EXPORT extern mLGroupLayoutClass g_stmLGroupLayoutCls;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_WIDGET_LAYOUT_H */
#endif //_MGNCSCTRL_LIST

