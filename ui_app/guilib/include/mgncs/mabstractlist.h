/**
 * \file mabstractlist.h
 * \author XiaoweiYan
 * \date 2009/11/10
 *
 * \brief This file includes the definition of mAbstractList.
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

#ifndef _MGUI_NCSCTRL_ASTLST_H
#define _MGUI_NCSCTRL_ASTLST_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Widget_AbstractList mAbstractList
 * @{
 */

typedef struct _mAbstractList mAbstractList;
typedef struct _mAbstractListClass mAbstractListClass;
typedef struct _mAbstractListRenderer mAbstractListRenderer;

/**
 * \fn mAbstractList* mAbstractList_getControl(mNode *node);
 * \brief Get the associated control handle of the specified node.
 *
 * \param node  The specified node.
 * \return The handle of associated control, otherwise NULL.
 */
mAbstractList* mAbstractList_getControl(mNode *node);

/**
 * \def NCSS_ASTLST_AUTOSORT
 * \brief Sort nodes automatically.
 */
#define NCSS_ASTLST_AUTOSORT        (0x0001<<NCSS_SWGT_SHIFT)

/**
 * \def NCSS_ASTLST_SHIFT
 * \brief The bits used by mAbstractList in style.
 */
#define NCSS_ASTLST_SHIFT           (NCSS_SWGT_SHIFT+1)   

static inline int _ncs_defcmp_node(mNode *node1, mNode *node2)
{
    const char* str1 = _c(node1)->getText(node1);
    const char* str2 = _c(node2)->getText(node2);

    return strcasecmp(str1, str2);
}

#define mAbstractListHeader(clsName)\
	mScrollWidgetHeader(clsName)    \
    NCS_CB_CMPNODE  nodeCmp;        \
    mNode   *root;                  \
    int     textIndent;             \
    int     imageIndent;            \
    Uint16  flags; 

/**
 * \struct mAbstractList
 *
 * \brief The structure of mAbstractList, which derived from mScrollWidget.
 *        It is the abstract superclass of all classes which represent 
 *        controls that have nodes.
 *
 *  - root\n
 *    The root node.
 *
 *  - defNodeH\n
 *    The default height of node.
 *
 *  - flags\n
 *    Internal used.
 */
struct _mAbstractList
{
	mAbstractListHeader(mAbstractList)
};

#define mAbstractListClassHeader(clsName, superCls) \
	mScrollWidgetClassHeader(clsName, superCls)     \
    int (*addNode)(clsName*, mNode *node);          \
    int (*insertNode)(clsName*, mNode *node, mNode *prev,mNode *next, int index);\
    int (*removeNode)(clsName*, mNode *node);       \
    mNode* (*getNode)(clsName*, int index);         \
    int (*indexOf)(clsName*, mNode *node);          \
    BOOL (*isFrozen)(clsName*);                     \
    BOOL (*showNode)(clsName*, mNode *node);        \
    void (*refreshNode)(clsName*, mNode *node, const RECT *rcInv);  \
    mNode* (*findNode)(clsName*, DWORD info, int type, BOOL recursion, int startIndex); \
    \
    BOOL (*setCurSel)(clsName*, int index);                 \
    mNode* (*getCurSel)(clsName*);                          \
    BOOL (*selectNode)(clsName*, mNode *node, BOOL select); \
    \
    int (*getRect)(clsName*, mNode *node, RECT *rcNode, BOOL bConv);    \
    void (*freeze)(clsName*, BOOL locked);                              \
    void (*sortNodes)(clsName*, NCS_CB_CMPNODE func, mNode *parentNode);\
    void (*notifyEvent)(clsName*, int eventId, DWORD eventInfo);

/**
 * \struct mAbstractListClass
 * \brief The virtual function table of mAbstractList, which derived from
 *        mScrollWidgetClass.
 *
 *  - int (*\b addNode)(mAbstractList *self, mNode *node);\n          
 *    Add a new node. Please used by subclass directly.
 *      \param node     The pointer to new node.
 *      \return The index of new node in parent.
 *
 *  - int (*\b insertNode)(mAbstractList *self, mNode *node, mNode *prev,mNode *next, int index);\n
 *    Insert a node. Please used by subclass directly.
 *      \param node     The pointer to new node.
 *      \param prev     The specified previous sibling node. It has the highest priority.
 *      \param prev     The specified next sibling node. The priority is less than prev.
 *      \param index    The specified insert position. It has the lowest priority.
 *      \return The position in which item has been inserted on success, otherwise -1.
 *
 *  - int (*\b removeNode)(mAbstractList *self, mNode *node);\n       
 *    Remove a child node. It deletes all chilren nodes of the specified node. 
 *    Please used by subclass directly.
 *      \param node     The pointer to child node.
 *      \return 0 on success, otherwise -1.
 *
 *  - mNode* (*\b getNode)(mAbstractList *self, int index);\n         
 *    Get the specified node. Please used by subclass directly.
 *      \param index    The specified index.
 *      \return The pointer to the specified node.
 *
 *  - int (*\b indexOf)(mAbstractList *self, mNode *node);\n          
 *    Get the index of the specified node in parent node. 
 *    Please used by subclass directly.
 *      \return The index of the specified node.
 *
 *  - BOOL (*\b isFrozen)(mAbstractList *self);\n                     
 *    The function determines whether control is frozen. If frozen,
 *    control will not refresh nodes until unfrozen it. 
 *    Please used by subclass directly.
 *
 *  - BOOL (*\b showNode)(mAbstractList *self, mNode *node);\n        
 *    Refresh the specified node and make it visible. Please used directly 
 *    by subclass.
 *
 *  - void (*\b refreshNode)(mAbstractList *self, mNode *node, const RECT *rcInv);\n  
 *    Refresh the specified node. Please used by subclass directly.
 *
 *  - mNode* (*\b findNode)(mAbstractList *self, DWORD info, int type, BOOL recursion, int startIndex);\n 
 *    Find the node which contains the specified information from start 
 *    index to end of nodes. Please used by subclass directly.
 *      \param info         The valid information.
 *      \param type         The type of valid information.
 *      \param recursion    Whether find in children or not.
 *      \param startIndex   The started index.
 *      \return The pointer to found node.
 *
 *  - int (*\b getRect)(mAbstractList *self, mNode *node, RECT *rcNode, BOOL bConv);\n
 *    Get the specified node's rectangle. It should be implemented by subclass.
 *      \param node         The specified node.
 *      \param rcNode       The node's rectangle.
 *      \param bConv        Whether transform content to window coordinate or not.
 *      \return Zero on success, otherwise -1.\n
 *
 *  - void (*\b freeze)(mAbstractList *self, BOOL locked);\n                          
 *    Freeze the control or not. It should be implemented by subclass.
 *
 *  - void (*\b sortNodes)(mAbstractList *self, NCS_CB_CMPNODE func, mNode *parentNode);\n
 *    Sort all children of specified node using the specified function. 
 *    Please used by subclass directly.
 *      \param func         The node comparision function.
 *      \param parentNode   The parent node.
 *
 *  - void (*\b notifyEvent)(mAbstractList *self, int eventId, DWORD eventInfo);\n
 *    Process the event from the node. It should be implemented by subclass.
 *      \param eventId      The event id.
 *      \param eventInfo    The event information.
 */
struct _mAbstractListClass
{
	mAbstractListClassHeader(mAbstractList, mScrollWidget)
};

#define mAbstractListRendererHeader(clsName, parentClass) \
	mScrollWidgetRendererHeader(clsName, parentClass)

/**
 * \struct mAbstractListRenderer
 * \brief The structure of mAbstractList renderer, which inheried
 *        from mScrollWidgetRenderer.
 */
struct  _mAbstractListRenderer {
	mAbstractListRendererHeader(mAbstractList, mScrollWidget)
};

/**
 * \enum mAbstractListNotify
 * \brief The notification id of mAbstractList.
 */
enum mAbstractListNotify
{
    /**
     * The maximum value of mAbstractList notification code id.
     */
    NCSN_ASTLST_MAX = NCSN_SWGT_MAX + 1,
};

/**
 * \enum mAbstractListProperty
 * \brief The properties id of mAbstractList.
 */
enum mAbstractListProperty
{
    /**
     * The default node height. It should be implemented by subclass.
     */
    NCSP_ASTLST_DEFNODEHEIGHT = NCSP_SWGT_MAX + 1,

    /**
     * The window font size. RO.
     */
    NCSP_ASTLST_FONTSIZE, 

    /**
     * Whether the window sorts nodes automatically or not. RO.
     */
    NCSP_ASTLST_AUTOSORT, 

    /**
     * Whether the window is transparent or not. RO.
     */
    NCSP_ASTLST_TRANSPARENT,

    /**
     * The node comparision function. (NCS_CB_CMPNODE)
     */
    NCSP_ASTLST_CMPNODEFUNC, 

    /**
     * The root node. RO.
     */
    NCSP_ASTLST_ROOTNODE,  

    NCSP_ASTLST_TEXTINDENT,

    NCSP_ASTLST_IMAGEINDENT,
    /**
     * The maximum value of mAbstractList properties id.
     */
    NCSP_ASTLST_MAX, 
};

/**
 * \var g_stmAbstractListCls
 * \brief The instance of mAbstractListClass.
 *
 * \sa mAbstractListClass
 */
MGNCS_EXPORT extern mAbstractListClass g_stmAbstractListCls;

    /** @} end of Widget_AbstractList */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_ASTLST_H */

