/**
 * \file minvisible-component.h
 * \author dongjunjie
 * \date 2009/04/19
 *
 * This file include the base object defines
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


#ifndef _MGUI_NCSCTRL_INVISIBLECOMP_H
#define _MGUI_NCSCTRL_INVISIBLECOMP_H

#ifdef __cplusplus
extern "C"{
#endif

/**
 * \defgroup InvisibleComponent mInvisibleComponent
 * @{
 */
typedef struct _mInvsbCompClass mInvsbCompClass;
typedef struct _mInvsbComp mInvsbComp;


#define mInvsbCompClassHeader(clss, superCls) \
	mComponentClassHeader(clss, superCls) 

/**
 * \struct mInvsbCompClass
 * \brief define the VTable of mInvsbComp, inherit mComponentClass
 *
 * \sa mComponentClass, mInvsbComp
 * 
 */

struct _mInvsbCompClass {
	mInvsbCompClassHeader(mInvsbComp, mComponent)
};

/**
 * \var g_stmInvsbCompCls
 * \brief the singleton \ref mInvsbCompClass
 */
MGNCS_EXPORT extern mInvsbCompClass g_stmInvsbCompCls; //Class(mInvsbComp);


#define mInvsbCompHeader(clss) \
	mComponentHeader(clss) \
	int id; \
	mComponent* parent, *prev, *next; 

/**
 * \struct mInvsbComp
 * \brief define the mInvsbComp members , inherit from mComponent. 
 *  An Invisible Component is a Component only support some functions, and user cannot see them
 *
 *  - int \b id - the id of an invisible component
 *  - mComponent * \b parant - the parent of invsible component
 *  - mComponent * \b prev - the prev sliding component of invsible component
 *  - mComponent * \b next - the next sliding component of invsible component
 */

struct _mInvsbComp {
	mInvsbCompHeader(mInvsbComp)
};

/**
 * \enum ncsInvsbCompProp 
 * \brief the invisible component property define
 */
enum ncsInvsbCompProp {
	NCSP_IVCMPT_MAX  = 0
};

/** @} end of InvisibleComponent */

///////////////////////////////////////


/**
 * \addtogroup global_defines Global defines
 * @{
 */

/**
 * \defgroup global_invisiblecomp_functions Gloal InvisibleComponent Functions
 * @{
 */

/**
 * A struct include Invisible Component Create info
 *
 * \sa ncsCreateInvsbCompIndirect
 */
typedef struct _NCS_INVSB_CREATE_INFO {
	/**
	 * The parent Component
	 */
	mComponent        * parent;
	/**
	 * The id of component
	 */
	int                 id;
	/**
	 * The property of Component
	 * 
	 * \sa NCS_PROP_ENTRY
	 */
	NCS_PROP_ENTRY    * props;
	/**
	 * The event handlers array
	 * 
	 * \sa NCS_EVENT_HANDLER
	 */
	NCS_EVENT_HANDLER * handlers;

	/**
	 * Use defined data
	 */
	DWORD               user_data;
}NCS_INVSB_CREATE_INFO;

/**
 * \fn mInvsbComp * ncsCreateInvsbComp(const char* class_name, mComponent* parent, int id, NCS_PROP_ENTRY *props, NCS_EVENT_HANDLER * handlers, DWORD user_data);
 * \brief create an Invisible Component
 *
 * \param class_name  the class name of Invisible Component
 * \param parent the parent of creating Invisible Component
 * \param id  the id of Invisible Component
 * \param props the properties array of Invisible Component 
 * \param handlers the event handler array of Invisible Component
 * \param user_data user data
 *
 * \return mInvsbComp * - the new created Invisible Component pointer, NULL or failed
 *
 * \sa NCS_INVSB_CREATE_INFO, ncsCreateInvsbCompIndirect
 */
MGNCS_EXPORT mInvsbComp * ncsCreateInvsbComp(const char* class_name, mComponent* parent, int id, NCS_PROP_ENTRY *props, NCS_EVENT_HANDLER * handlers, DWORD user_data);

/**
 * \fn mInvsbComp * ncsCreateInvsbCompIndirect(const char* class_name, NCS_INVSB_CREATE_INFO *create_info);
 * \brief create an Invisible Component from creating info
 *
 * \param class_name the class name of Invisible Component
 * \param create_info the creating information pointer
 *
 * \return mInvsbComp * - the Invisible Component pointer if success, NULL or failed
 *
 * \sa NCS_INVSB_CREATE_INFO, ncsCreateInvsbComp
 */
MGNCS_EXPORT mInvsbComp * ncsCreateInvsbCompIndirect(const char* class_name, NCS_INVSB_CREATE_INFO *create_info);


/** @} end of global_defines */

/** @} end of global_invisiblecomp_functions */

#ifdef __cplusplus
}
#endif


#endif


