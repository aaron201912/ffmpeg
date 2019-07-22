/**
 * \file mnode.h
 * \author XiaoweiYan
 * \date 2009/11/10
 *
 * This file includes the definition of mNode.
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

#ifndef _MGUI_WIDGET_NODE_H
#define _MGUI_WIDGET_NODE_H
 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Widget_Node mNode
 * @{
 */

typedef struct _mNode mNode;
typedef struct _mNodeClass mNodeClass;

/**
 * \def NCSS_NODE_LTEXTRS
 * \brief Image on the left and text on the right. default style.
 */
#define NCSS_NODE_LTEXTRS           0x0000

/**
 * \def NCSS_NODE_SMALLICON
 * \brief Small icon mode. Show small icon in normal status, or enlarge icon 
 *        in selected status.
 */
#define NCSS_NODE_SMALLICON         0x0001

/**
 * \def NCSS_NODE_LTEXTBS
 * \brief Large Icon mode. Image on the top and text on the bottom.
 */
#define NCSS_NODE_LTEXTBS           0x0002

/**
 * \def NCSS_NODE_TICTACTOEMIDLET
 * \brief tictactoemidlet mode. Image includes normal and selected status.
 */
#define NCSS_NODE_TICTACTOEMIDLET   0x0004

#define NCSS_NODE_LAYOUT_MASK       0x00FF

/**
 * \def NCSS_NODE_HIDETEXT
 * \brief Don't show text.
 */
#define NCSS_NODE_HIDETEXT          0x0010

/**
 * \def NCSS_NODE_HIDEIMAGE
 * \brief Don't show image.
 */
#define NCSS_NODE_HIDEIMAGE         0x0020

/**
 * \def NCSS_NODE_DISABLED
 * \brief Disabled Node.
 */
#define NCSS_NODE_DISABLED          0x0040

/**
 * \def NCSS_NODE_SHIFT
 * \brief The bits used by mNode in style.
 */
#define NCSS_NODE_SHIFT             7

/**
 * \var typedef int (*NCS_CB_CMPNODE) (mNode *node1, mNode *node2);
 * 
 * \brief The callback of node comparison function.
 */
typedef int (*NCS_CB_CMPNODE) (mNode *node1, mNode *node2);

#define mNodeDataInfoHeader \
    int     style;      \
    char    *text;      \
    PBITMAP image;      \
    int     height;     \
    DWORD   addData;    \
    mObject *parent;    \
    

typedef struct _NCS_NODE_DATA
{
    mNodeDataInfoHeader
}NCS_NODE_DATA;

//status flags
#define NCSF_NODE_MASK              0xFFFF
#define NCSF_NODE_NORMAL            0x0000
#define NCSF_NODE_SELECTED          0x0001
#define NCSF_NODE_DISABLED          0x0002
#define NCSF_NODE_INVISIBLE         0x0004
#define NCSF_NODE_DEFHEIGHT         0x0008

#define NCSF_NODE_HIDETEXT          0x0010
#define NCSF_NODE_HIDEIMAGE         0x0020

typedef struct _NCS_NODE_LAYOUTOPS
{
    const char name[16];
    int style;
    void (*paint)(mNode *self, HDC hdc, const RECT *rect, mObject *owner);
}NCS_NODE_LAYOUTOPS;

#define mNodeHeader(className)  \
	mObjectHeader(className)    \
    NCS_NODE_LAYOUTOPS *layout; \
    list_t  children;           \
    list_t  sibling;            \
    Uint16  flags;              \
    Uint16  count;              \
    mNodeDataInfoHeader

/**
 * \struct mNode
 *
 * \brief The structure of mNode. It is the basic class of all nodes occur
 *        within specific controls. For example, a list can contain it.
 *
 *      - parent\n
 *        The pointer to parent node or control.
 *
 *      - children\n
 *        The list of children.
 *
 *      - sibling\n
 *        The list of sibling.
 *
 *      - text\n
 *        The text string.
 *
 *      - image\n
 *        The PBITMAP pointer to image.
 *
 *      - addData\n
 *        The additional data.
 *
 *      - height\n
 *        The height of node. If node uses default height, 
 *        this value is invalid.
 *
 *      - count\n
 *        The number of children.
 */
struct _mNode
{
	mNodeHeader(mNode)
};

typedef struct _NCS_MENU_DATA
{
    int     nr_child;
    struct _NCS_MENU_DATA   *child;

    mNodeDataInfoHeader
}NCS_MENU_DATA;
MGNCS_EXPORT void ncsSetMenuData(mObject *self, NCS_MENU_DATA *pMenuData, int count);

typedef struct _NCS_MENU_INFO
{
    int     count; 
    char    *caption;
    NCS_MENU_DATA *data;
}NCS_MENU_INFO;
MGNCS_EXPORT void ncsSetMenu(mObject *self, NCS_MENU_INFO *pMenuInfo);

/**
 * \fn mNode* ncsCreateNode(mObject *parent, const char* text, \
        PBITMAP image, int height, int style, DWORD addData);
 * \brief Create a new node according to parameter information.
 *
 * \param parent    The pointer to parent object.
 * \param text      The text string. 
 * \param image     PBITMAP structure, the pointer to image.
 * \param height    The height. If height < 0, it will use the default 
 *                  height of node in associated control.
 * \param style     The style of node.
 * \param addData   The additional data.
 *
 * \return The new pointer to node.
 */
MGNCS_EXPORT mNode* ncsCreateNode(mObject *parent, const char* text, 
        PBITMAP image, int height, int style, DWORD addData);

/**
 * \fn mNode* ncsAddNode(mObject *parent, const char* text, \
        PBITMAP image, int height, int style, DWORD addData);
 *
 * \brief Create and add a new node to control according 
 *        to parameter information.
 *
 * \param parent    The pointer to parent object.
 * \param text      The text string. 
 * \param image     PBITMAP structure, the pointer to image.
 * \param height    The height. If height < 0, it will use the default 
 *                  height of node in associated control.
 * \param style     The style of node.
 * \param addData   The additional data.
 *
 * \return The new pointer to node.
 *
 * \sa ncsCreateNode
 */
MGNCS_EXPORT mNode* ncsAddNode(mObject *parent, const char* text, 
        PBITMAP image, int height, int style, DWORD addData);

/**
 * \enum ncsNodeFindType
 * \brief The found information type used by findNode.
 */
enum ncsNodeFindType
{
    /**
     * According to text string.
     */
    NCS_NODE_FTSTRING,

    /**
     * According to additional data.
     */
    NCS_NODE_FTDATA,

    /**
     * According to image.
     */
    NCS_NODE_FTIMAGE,

    NCS_NODE_FTOTHER,
};

#define mNodeClassHeader(clsName, superCls)             \
	mObjectClassHeader(clsName, superCls)               \
	BOOL (*setProperty)(clsName*, int id, DWORD value); \
	DWORD (*getProperty)(clsName*, int id);             \
    BOOL (*computeSize)(clsName*, HDC hdc, SIZE *size); \
    BOOL (*canAsRoot)(clsName*);                        \
    int (*addNode)(clsName*, mNode *node);              \
    int (*removeNode)(clsName*, mNode *node);           \
    int (*insertNode)(clsName*, mNode *node, mNode *prev, mNode *next, int index);\
    int (*moveNode)(clsName*, mNode *curNode, int count, mNode *prevNode);  \
    mNode* (*findNode)(mNode *self, DWORD info, int type, BOOL recursion, int startIndex);\
    mNode* (*findNodeEx)(mNode *self, DWORD info, int type, BOOL recursion, int startIndex, \
            int (*cb)(mNode *node, DWORD info));        \
    void (*removeAll)(clsName*);                        \
    void (*sendEvent)(clsName*, int eventId);           \
    mNode* (*getNode)(clsName*, int index);             \
    int (*indexOf)(clsName*);                           \
    mNode* (*getPrev)(clsName*);                        \
    mNode* (*getNext)(clsName*);                        \
    BOOL (*isEnabled)(clsName*);                        \
    BOOL (*isSelected)(clsName*);                       \
    BOOL (*hasChildren)(clsName*);                      \
    BOOL (*setText)(clsName*, const char* text);        \
    BOOL (*setImage)(clsName*, const PBITMAP image);    \
    const char* (*getText)(clsName*);                   \
    PBITMAP (*getImage)(clsName*);                      \
    void (*paint)(clsName*, HDC hdc, const RECT *rect, mObject *owner);  \
	int (*msgHandler)(clsName*, int msg, WPARAM wParam, LPARAM lParam, mObject *owner);
    
/**
 * \struct mNodeClass
 * \brief The virtual function table of mNodeClass.
 *
 *  - BOOL (*\b setProperty)(mNode *self, int id, DWORD value);\n
 *    The function sets the value of the specified properties.
 *      \param id       The property id.
 *      \param value    The property value.
 *      \return TRUE on success, otherwise FALSE. 
 *
 *  - DWORD (*\b getProperty)(mNode *self, int id); \n
 *    The function gets the value of the specified properties. 
 *      \param id       The property id. 
 *      \return the property value on success, otherwise -1. 
 *
 *  - int (*\b addNode)(mNode *self, mNode *node);\n              
 *    Add a new node.
 *      \param node     The pointer to new node.
 *      \return The index of new node in parent.
 *
 *  - int (*\b removeNode)(mNode *self, mNode *node);\n           
 *    Remove a child node. It deletes all chilren nodes of the specified node.
 *      \param node     The pointer to child node.
 *      \return 0 on success, otherwise -1.
 *
 *  - int (*\b insertNode)(mNode *self, mNode *node, mNode *prev, mNode *next, int index);\n
 *    Insert a node.
 *      \param node     The pointer to new node.
 *      \param prev     The specified previous sibling node. It has the highest priority.
 *      \param next     The specified next sibling node. The priority is less than prev.
 *      \param index    The specified insert position. It has the lowest priority.
 *      \return The position in which item has been inserted on success, otherwise -1.
 *
 *  - int (*\b moveNode)(mNode *self, mNode *curNode, int count, mNode *prevNode);\n  
 *    Move the specfied nodes to the specified position.
 *      \param curNode  The pointer to first moved node.
 *      \param count    The count of moved nodes.
 *      \param prevNode The pointer to target previous sibling node.
 *      \return Zero on success, otherwise -1.
 *
 *  - mNode* (*\b findNode)(mNode *self, DWORD info, int type, BOOL recursion, int startIndex);\n
 *    Find the node which contains the specified information from start index to end of nodes.
 *      \param info         The valid information.
 *      \param type         The type of valid information.
 *      \param recursion    Whether find in children or not.
 *      \param startIndex   The started index.
 *      \return The pointer to found node.
 *
 *  - void (*\b removeAll)(mNode *self);\n                        
 *    Remove all nodes.
 *
 *  - void (*\b sendEvent)(mNode *self, int eventId);\n           
 *    Send event to associated control.
 *
 *  - mNode* (*\b getNode)(mNode *self, int index);\n             
 *    Get the specified node.
 *      \param index    The specified index.
 *      \return The pointer to the specified node.
 *
 *  - int (*\b indexOf)(mNode *self);\n                           
 *    Get the index of the specified node in parent node.
 *      \return The index of the specified node.
 *
 *  - mNode* (*\b getPrev)(mNode *self);\n                        
 *    Get the privous sibling node.
 *
 *  - mNode* (*\b getNext)(mNode *self);\n                        
 *    Get the next sibling node.
 *
 *  - BOOL (*\b isEnabled)(mNode *self);\n                        
 *    Whether the current node is enabled or not.
 *
 *  - BOOL (*\b isSelected)(mNode *self);\n                       
 *    Whether the current node is selected or not.
 *
 *  - BOOL (*\b hasChildren)(mNode *self);\n                      
 *    Whether the current node has children node or not.
 *
 *  - BOOL (*\b setText)(mNode *self, const char* text);\n        
 *    Set the text of the current node.
 *      \param text     The new text.
 *      \return TRUE on success, otherwise FALSE.
 *
 *  - BOOL (*\b setImage)(mNode *self, const PBITMAP image);\n    
 *    Set the image of the current node.
 *      \param text     The new image.
 *      \return TRUE on success, otherwise FALSE.
 *
 *  - const char* (*\b getText)(mNode *self);\n                   
 *    Get the text of the current node.
 *
 *  - PBITMAP (*\b getImage)(mNode *self);\n                      
 *    Get the image of the current node.
 *
 *  - void (*\b paint)(mNode *self, HDC hdc, const RECT *rect, mObject *owner);\n  
 *    Paint the content of the current node in specified region.
 *
 *  - int (*\b msgHandler)(mNode *self, int msg, WPARAM wParam, LPARAM lParam, mObject *owner);\n
 *    Process the messge.
 */
struct _mNodeClass
{
	mNodeClassHeader(mNode, mObject)
};

typedef struct _mNodeEventData
{
    mNode *source;
    DWORD data;
}mNodeEventData;
/**
 * \enum mNodeEvent
 * \brief The event id of mNode.
 */
enum mNodeEvent
{
    /**
     * The content has been changed. 
     *  - param : mNode*, the pointer to changed node.
     */
    NCSE_NODE_CONTENTCHANGED = 1, 

    /**
     * The height of node has been changed. 
     *  - param : mNodeEventData*, the data is the different value of height.
     */
    NCSE_NODE_HEIGHTCHANGED,

    /**
     * Refresh node. 
     *  - param : mNode*, the pointer to refresh node.
     */
    NCSE_NODE_REFRESH,   

    /**
     * The node has been added.
     *  - param : mNode*, the pointer to added node.
     */
    NCSE_NODE_NODEADDED,  

    /**
     * The content has been deleted, Only have removed from sibling list. 
     *  - param : mNode*, the pointer to deleted node.
     */
    NCSE_NODE_NODEDELETED, 
};

/**
 * \enum mNodeProp
 * \brief The properties id of mNode.
 */
enum mNodeProp
{
    /**
     * The height.(Uint16)
     */
    NCSP_NODE_HEIGHT = 1,

    /**
     * The text string.(char*)
     */
    NCSP_NODE_TEXT,

    /**
     * The image. (PBITMAP)
     */
    NCSP_NODE_IMAGE,

    /**
     * The additional data. (DWORD)
     */
    NCSP_NODE_ADDDATA,

    /**
     * The enabled status of node. RW.
     */
    NCSP_NODE_ENABLED,

    /**
     * The selected status of node. RO.
     */
    NCSP_NODE_SELECTED,

    /**
     * The parent node.(mObject*). RO.
     */
    NCSP_NODE_PARENTNODE,

    /**
     * The first child node.(mNode*). RO.
     */
    NCSP_NODE_FIRSTCHILD,

    /**
     * The last child node. (mNode*). RO.
     */
    NCSP_NODE_LASTCHILD,

    /**
     * The number of children. (int). RO.
     */
    NCSP_NODE_CHILDRENCOUNT,

    /**
     * The layout information.
     */
    NCSP_NODE_LAYOUT,

    /**
     * Whether hides text or not.
     */
    NCSP_NODE_HIDETEXT,

    /**
     * Whether hides image or not.
     */
    NCSP_NODE_HIDEIMAGE,

    /**
     * The maximum value of mNode properties id.
     */
	NCSP_NODE_MAX,
};

/**
 * \var g_stmNodeCls
 * \brief The instance of mNodeClass.
 *
 * \sa mNodeClass
 */
MGNCS_EXPORT extern mNodeClass g_stmNodeCls;

/*============================================*/
typedef struct _mGroupNode mGroupNode;
typedef struct _mGroupNodeClass mGroupNodeClass;

typedef struct _NCS_GROUPNODE_DATA
{
    mNodeDataInfoHeader
    int id;
}NCS_GROUPNODE_DATA;

#define mGroupNodeHeader(className)  \
	mNodeHeader(className)      \
    int id;
struct _mGroupNode
{
	mGroupNodeHeader(mGroupNode)
};

#define mGroupNodeClassHeader(clsName, superCls)    \
	mNodeClassHeader(clsName, superCls)
struct _mGroupNodeClass
{
	mGroupNodeClassHeader(mGroupNode, mNode)
};

/**
 * \var g_stmGroupNodeCls
 * \brief The instance of mGroupNodeClass.
 *
 * \sa mGroupNodeClass
 */
MGNCS_EXPORT extern mGroupNodeClass g_stmGroupNodeCls;

MGNCS_EXPORT mNode* ncsAddGroupNode(mObject *parent, const char* text, 
        PBITMAP image, int height, int style, DWORD addData, int id);


#define NCS_CREATE_NODE(_retnode, _clstype, _parent, _text, _image, _height, _style, _add_data)\
do{\
    NCS_NODE_DATA data;         \
    if (!_parent || (!INSTANCEOF(_parent, mNode) && !INSTANCEOF(_parent, mAbstractList))){  \
        _retnode = NULL;        \
    }\
    data.text = (char*)_text;   \
    data.image = _image;        \
    data.height = _height;      \
    data.style = _style;        \
    data.addData = _add_data;   \
    data.parent = _parent;      \
    _retnode = NEWEX(_clstype, (DWORD)&data);  \
}while(0)

#define NCS_ADD_NODE(_retnode, _clstype, _parent, text, image, height, style, _add_data)\
do {\
    NCS_CREATE_NODE(_retnode, _clstype, _parent, text, image, height, style, _add_data);\
    int ret = -1;               \
    if (_retnode && _parent && INSTANCEOF(_parent, mAbstractList)) {\
        mAbstractList *list = (mAbstractList*)_parent;              \
        if (_c(list)->addNode)                                      \
            ret = _c(list)->addNode(list, (mNode*)_retnode);              \
    }                           \
    else if (_retnode && _parent && INSTANCEOF(_parent, mNode)) {   \
        mNode *list = (mNode *)_parent;                             \
        if (_c(list)->addNode)                                      \
            ret = _c(list)->addNode(list, (mNode*)_retnode);              \
    }                           \
    if (ret < 0) {              \
        DELETE(_retnode);        \
        _retnode = NULL;         \
    }\
}while(0)

    /** @} end of Widget_Node */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_WIDGET_NODE_H */

