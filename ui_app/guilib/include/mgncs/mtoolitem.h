/**
 * \file 
 * \author 
 * \date 
 *
 * \brief 
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

#ifndef _MG_MINIMTOOLITEM_H_
#define _MG_MINIMTOOLITEM_H_

#ifdef __cplusplus
extern "C"{
#endif

/**
 * \defgroup ToolItem mToolItem
 * @{
 */


/**
 * \enum mToolItemType
 * \brief define the type of toolitem
 */
enum mToolItemType{
	NCS_UNKNOWNTOOLITEM = 0,
	NCS_PUSHTOOLITEM,
	NCS_MENUTOOLITEM,
	NCS_WIDGETTOOLITEM,
	NCS_SEPARATORTOOLITEM
};

typedef struct _mToolImage{
	PBITMAP pbmp;
	unsigned char flags;
	unsigned char cell_count;
	unsigned short ref;
}mToolImage;

#define TOOLIMGF_UNLOAD   0x01
#define TOOLIMGF_VERTCELL 0x02 //the image of cell a vert arranged

/**
 * \fn mToolImage * ncsNewToolImage(PBITMAP pbmp, int cell_count, BOOL autoUnload, BOOL bVert);
 * \brief create a tool image for tool item
 *
 * \param pbmp - the bitmap pointer used by toolitem
 * \param cell_count - the count of sub bitmaps in the pbmp
 * \param autoUnload - unload the pbmp when toolimage is deleted
 * \param bVert - the sub bitmaps is arranged vertically
 * \return a mToolIamge object
 */
MGNCS_EXPORT mToolImage * ncsNewToolImage(PBITMAP pbmp, int cell_count, BOOL autoUnload, BOOL bVert);
/**
 * \fn mToolImage * ncsNewToolImageFromFile(const char *fileName, int cell_count, BOOL autoUnload, BOOL bVert);
 * \brief create a tool image from file, \sa ncsNewToolImage
 *
 * \param fileName - the image file name
 * \param cell_count - the count of sub bitmaps in the pbmp
 * \param autoUnload - unload the pbmp when toolimage is deleted
 * \param bVert - the sub bitmaps is arranged vertically
 * \return a mToolIamge object
 */
MGNCS_EXPORT mToolImage * ncsNewToolImageFromFile(const char *fileName, int cell_count, BOOL autoUnload, BOOL bVert);
/**
 * \fn void ncsFreeToolImage(mToolImage *mti);
 * \brief free the toolimage
 *
 */
MGNCS_EXPORT void ncsFreeToolImage(mToolImage *mti);
//BOOL ncsGetToolImageCell(mToolImage *mti, int idx, RECT *prc);
/**
 * \fn BOOL ncsDrawToolImageCell(mToolImage *mti, HDC hdc, int idx, const RECT *prc);
 * \brief draw a sub image from toolimage to hdc
 *
 * \param mti - the mToolImage object
 * \param hdc - the destination dc handle
 * \param idx - the index of sub image
 * \param prc - the area to draw bitmap
 */
MGNCS_EXPORT BOOL ncsDrawToolImageCell(mToolImage *mti, HDC hdc, int idx, const RECT *prc);

/**
 * \enum mToolItemEvent
 * \brief define the event code of tool item
 */
enum mToolItemEvent{
	/**
	 * When a tool item is checked
	 */
	NCSN_TOOLITEM_CLICKED = 1,
	/**
	 * When a tool item is pushed
	 */
	NCSN_TOOLITEM_PUSHED,
	/**
	 * When a tool item want show a menu
	 */
	NCSN_TOOLITEM_SHOWMENU,
	/**
	 * When a tool item has changed its state
	 */
	NCSN_TOOLITEM_STATE_CHANGED
};

/**
 * \def NCS_TOOLITEM_FLAG_TEXT_LEFT
 * \brief indicate that create a tool item whoes text at left. default is in the right
 */
#define NCS_TOOLITEM_FLAG_TEXT_LEFT  0x01
/**
 * \def NCS_TOOLITEM_FLAG_TEXT_UP
 * \brief indicate that create a tool item whoes text at up
 * \note must combine with NCS_TOOLITEM_FLAG_VERT
 */
#define NCS_TOOLITEM_FLAG_TEXT_UP    NCS_TOOLITEM_FLAG_TEXT_LEFT
/**
 * \def NCS_TOOLITEM_FLAG_VERT
 * \brief indicate that create a tool item whoes text and image arranged vertically
 */
#define NCS_TOOLITEM_FLAG_VERT       0x02

#define NCS_TOOLITEM_LABEL_TEXT_LEFT 0x10
#define NCS_TOOLITEM_LABEL_TEXT_RIGHT 0x20
#define NCS_TOOLITEM_LABEL_TEXT_CENTER 0x40

#define NCS_TOOLITEM_LABEL_TEXT_TOP 0x80
#define NCS_TOOLITEM_LABEL_TEXT_BOTTOM 0x100
#define NCS_TOOLITEM_LABEL_TEXT_VCENTER 0x200
/**
 * \fn void* ncsCreatePushToolItem(int id, mToolImage *img, const char* str, UINT flags);
 * \brief create push tool item. a push tool item like a push button
 *
 * \param id - the id of toolitem
 * \param img - the mToolItem image used by this item
 * \param str - the text of tool item, can be NULL
 * \param flags - the flags of tool item, 
 * 		- NCS_TOOLITEM_FLAG_TEXT_LEFT
 * 		- NCS_TOOLITEM_FLAG_TEXT_UP
 * 		- NCS_TOOLITEM_FLAG_VERT
 * \return the object pointer
 */
MGNCS_EXPORT void* ncsCreatePushToolItem(int id, mToolImage *img, const char* str, UINT flags);


/**
 * \fn BOOL ncsInitToolItems(void);
 * \brief initialize tool items
 */
MGNCS_EXPORT BOOL ncsInitToolItems(void);

/**
 * \fn void* ncsCreateMenuToolItem(int id, mToolImage *img, const char* str, UINT flags, mPopMenuMgr *menu);
 * \brief create mnue tool item, like a mMenuButton
 *
 * \param id - the id of toolitem
 * \param img - the mToolItem image used by this item
 * \param str - the text of tool item, can be NULL
 * \param flags - the flags of tool item, 
 * 		- NCS_TOOLITEM_FLAG_TEXT_LEFT
 * 		- NCS_TOOLITEM_FLAG_TEXT_UP
 * 		- NCS_TOOLITEM_FLAG_VERT
 * \param menu - the popmenu pointer \sa mPopMenuMgr
 * \return the object pointer
 *
 */
MGNCS_EXPORT void* ncsCreateMenuToolItem(int id, mToolImage *img, const char* str, UINT flags, mPopMenuMgr *menu);

/**
 * \def NCS_TOOLITEM_UNCHECKED
 * \brief tool item state unckecked
 */
#define NCS_TOOLITEM_UNCHECKED 0x00
/**
 * \def NCS_TOOLITEM_CHECKED
 * \brief tool item state checked
 */
#define NCS_TOOLITEM_CHECKED   0x01

/**
 * \fn void* ncsCreateCheckToolItem(int id, mToolImage *img, const char* str, UINT flags, int state);
 * \brief create a check able tool item, like \ref mCheckButton
 *
 * \param id - the id of toolitem
 * \param img - the mToolItem image used by this item
 * \param str - the text of tool item, can be NULL
 * \param flags - the flags of tool item, 
 * 		- NCS_TOOLITEM_FLAG_TEXT_LEFT
 * 		- NCS_TOOLITEM_FLAG_TEXT_UP
 * 		- NCS_TOOLITEM_FLAG_VERT
 * \param state - the state of tool item
 * \return the object pointer
 *
 */
MGNCS_EXPORT void* ncsCreateCheckToolItem(int id, mToolImage *img, const char* str, UINT flags, int state);
/**
 * \fn void* ncsCreateRadioToolItem(int id, mToolImage *img, const char* str, UINT flags);
 * \brief create a check able tool item, like \ref mRadioButton
 *
 * \param id - the id of toolitem
 * \param img - the mToolItem image used by this item
 * \param str - the text of tool item, can be NULL
 * \param flags - the flags of tool item, 
 * 		- NCS_TOOLITEM_FLAG_TEXT_LEFT
 * 		- NCS_TOOLITEM_FLAG_TEXT_UP
 * 		- NCS_TOOLITEM_FLAG_VERT
 * \return the object pointer
 *
 * \note RadioToolItem auto group between two SeparatorItem
 */
MGNCS_EXPORT void* ncsCreateRadioToolItem(int id, mToolImage *img, const char* str, UINT flags);

/**
 * \fn void* ncsCreateWidgetToolItem(mWidget* widget);
 * \brief create a toolitem which include a mWidget object
 *
 * \param widget the widget object, its parant must be a mToolBar
 *
 * \return the object pointer
 */
MGNCS_EXPORT void* ncsCreateWidgetToolItem(mWidget* widget);

/**
 * \fn void* ncsCreateSeparatorItem(void);
 * \brief create separator item
 *
 * \return the object pointer
 */
MGNCS_EXPORT void* ncsCreateSeparatorItem(void);

/**
 * \fn int ncsGetToolItemType(void *toolitem);
 * \brief get the type of item 
 *
 * \param toolitem - the pointer of toolitem
 * \return the type of tool item
 *
 * \sa mToolItemType
 */
MGNCS_EXPORT int ncsGetToolItemType(void *toolitem);

/**
 * \fn BOOL ncsIsPushToolItem(void *toolitem);
 * \brief check the toolitem is push item or not
 *
 * \param toolitem - the pointer of toolitem
 * \return TRUE or FALSE
 */
MGNCS_EXPORT BOOL ncsIsPushToolItem(void *toolitem);
/**
 * \fn BOOL ncsIsMenuToolItem(void *toolitem);
 * \brief check the toolitem is menu tool item or not
 *
 * \param toolitem - the pointer of toolitem
 * \return TRUE or FALSE
 */
MGNCS_EXPORT BOOL ncsIsMenuToolItem(void *toolitem);
/**
 * \fn BOOL ncsIsSeparatorToolItem(void *toolitem);
 * \brief check the toolitem is separator tool item or not
 *
 * \param toolitem - the pointer of toolitem
 * \return TRUE or FALSE
 */
MGNCS_EXPORT BOOL ncsIsSeparatorToolItem(void *toolitem);
/**
 * \fn BOOL ncsIsWidgetToolItem(void *toolitem);
 * \brief check the toolitem is widget tool item or not
 *
 * \param toolitem - the pointer of toolitem
 * \return TRUE or FALSE
 */
MGNCS_EXPORT BOOL ncsIsWidgetToolItem(void *toolitem);

//////////////operator of toolitem
/**
 * \fn int ncsToolItem_getId(void *self);
 * \brief get the id of toolitem
 *
 * \param self - the toolitem pointer
 * \return id of toolitem
 */
MGNCS_EXPORT int ncsToolItem_getId(void *self);
/**
 * \fn int ncsToolItem_setId(void *self, int id);
 * \brief get the id of toolitem
 *
 * \param self - the toolitem pointer
 * \param id - the new id of toolitem
 * \return old id of toolitem
 */
MGNCS_EXPORT int ncsToolItem_setId(void *self, int id);
/**
 * \fn BOOL ncsToolItem_setCheck(void *self, int check_state);
 * \brief set the check state of toolitem
 *
 * \param self - the toolitem pointer
 * \param check_state - the state of check
 * \return TRUE or FALSE
 */
MGNCS_EXPORT BOOL ncsToolItem_setCheck(void *self, int check_state);
/**
 * \fn int ncsToolItem_getCheck(void *self);
 * \brief get the check state of toolitem
 *
 * \param self - the toolitem pointer
 * \return check state
 */
MGNCS_EXPORT int ncsToolItem_getCheck(void *self);

/**
 * \fn BOOL ncsToolItem_showMenu(void*self, mObject *owner);
 * \brief show the menu of MenuToolItem
 *
 * \param self - the pointer of Menu Tool Item
 * \param owner - the owner of the menu
 *
 * \return TRUE or FALSE
 */
MGNCS_EXPORT BOOL ncsToolItem_showMenu(void*self, mObject *owner);


/**
 * \fn mObject* ncsNewToolItem(int id, mObjectClass* _class, DWORD param);
 * \brief new a tool item object
 *
 * \param id     -- the id of toolid
 * \param _class -- the class of toolitem implememnted, should be a mHotPiece class
 * \param param  -- the construct param of class
 *
 * \return return a new item
 */

MGNCS_EXPORT mObject* ncsNewToolItem(int id, mObjectClass* _class, DWORD param);

#define NEWTOOLITEM(id, Object, param)  \
	(Object*)(ncsNewToolItem((id), (mObjectClass*)&Class(Object), (DWORD)(param)))

/**
 * \fn void ncsFreeToolItem(mObject * obj)
 * \brief free the tool item object
 *
 * \param obj -- the object want to free
 *
 * \return void
 */

MGNCS_EXPORT void ncsFreeToolItem(mObject* obj);

#define FREETOOLITEM(obj)  \
	ncsFreeToolItem((mObject*)(obj))

    /** @} end of ToolItem*/

#ifdef __cplusplus
 }
#endif

#endif /* _MG_MINIMTOOLITEM_H_ */
