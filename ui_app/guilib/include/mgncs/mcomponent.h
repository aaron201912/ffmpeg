/**
 * \file mcomponent.h
 * \author dongjunjie
 * \date 2009/04/16
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

#ifndef _MGUI_NCSCTRL_CMPT_H
#define _MGUI_NCSCTRL_CMPT_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdarg.h>

/**
 * \defgroup Component mComponent
 * @{
 */

/**
 * \def MAXLEN_CLASSNAME
 * \brief the max length of class name
 * \note This value should be same as the MiniGUI's definition.
 */
#define MAXLEN_CLASSNAME    31

/**
 * \def NCSCLASSNAME
 * \brief Define a NCS class name with a default suffix, distinguish the class name from old constrol set
 */
#define NCSCLASSNAME(name)     name _MGNCS_CLASS_SUFFIX

#define BEGIN_CMPT_CLASS(clss, superCls) \
	static const char comp_##clss##_class_name[] = (#clss _MGNCS_CLASS_SUFFIX) ; \
	BEGIN_MINI_CLASS(clss, superCls) \
	_class->className = comp_##clss##_class_name + 1; 

#define END_CMPT_CLASS END_MINI_CLASS

#ifndef BIND_PROP
#define BIND_PROP
typedef struct _mBindProp mBindProp;
#endif

typedef void (*PFreeSpecificData)(DWORD key, DWORD value);

#define MAX_USER_SPECIAL_KEY   0x80000000L

typedef struct _mComponentClass mComponentClass;
typedef struct _mComponent mComponent;

#define mComponentClassHeader(clss, superCls) \
	mObjectClassHeader(clss, superCls) \
	const char* className; \
	clss * (*createComponent)(mComponent* parent, ClassType(clss)*, DWORD addValue); \
	WNDPROC _window_proc ; \
	BOOL (*setProperty)(clss *self, int id, DWORD value); \
	DWORD (*getProperty)(clss *self, int id); \
	int (*setId)(clss *self, int id); \
	int (*getId)(clss *self); \
	mComponent* (*getReleated)(clss*self, int releated); \
	mComponent* (*setReleated)(clss *self, mComponent* comp,  int releated); \
	mComponent* (*getChild)(clss* self, int id);  \
	void        (*removeChild)(clss* self, mComponent* child);  \
	PUBLIC DWORD (*getPropertyByPath)(clss* self, int *id_path); \
	PUBLIC BOOL  (*setPropertyByPath)(clss* self, int *id_path, DWORD value); \
	PUBLIC BOOL  (*addEventListenerByPath)(clss* self, int *id_path, mObject* listener, NCS_CB_ONOBJEVENT on_event); \
	PUBLIC BOOL  (*addBindPropByPath)(clss* self, int *id_path, int event_id, int prop_type, int flags, mBindProp *prop, int bind_type); \
	PUBLIC BOOL  (*queryByPath)(clss* self, int *id_path, DWORD *pret, ...); \
	PUBLIC BOOL  (*vqueryByPath)(clss* self, int *id_path, DWORD* pret, va_list va); \
	PUBLIC BOOL  (*isMainObj)(clss* self);  \
	BOOL  (*setSpecificData)(clss* self, DWORD key, DWORD value, PFreeSpecificData freeSpecific); \
	DWORD (*getSpecificData)(clss* self, DWORD key, BOOL *pOk);

/**
 * \struct mComponentClass
 * \brief the basic class of components, windows. Drived by mObjectClass
 * 
 *  - const char* \b className - the class name of component, \sa NCSCLASSNAME
 *
 *  - mComponent * \b createComponent(\a mComponent* parent, \a mComponentClass * the_class, \a DWORD addData);\n
 *    create a mComponent instance, see Design Patterns : Factroy Method\n
 *    \param parent - the parent recieve the new created component
 *    \param the_class - the class of new created component
 *    \param addData - param to new compont's construct function: \a mObjectClass .construct\n
 *    \return mComponent * the new instance pointer of component
 *
 *  - WNDPROC \b _window_proc - window proc, used for registering, \sa ncsRegisterComponent
 *
 *  - BOOL \b setProperty(\a mComponent *self, \a int id, \a DWORD value);\n
 *    Set the property of component\n
 *    \param self - this pointer
 *    \param id - the id of property
 *    \param value - the value of property\n
 *    \return TRUE - sucessfull , FALSE - failed
 *
 *  - DWORD \b getProperty (\a mComponent * self, \a int id);\n
 *    Get the property value of component\n
 *    \param self - this pointer
 *    \param id - the id of property\n
 *    \return DWORD - value of property or (DWORD)-1 if failed
 *
 *  - int \b setId (\a mComponent *self, \a int id);\n
 *    Set the Id of this component\n
 *    \param self - this pointer
 *    \param id - the new id of component
 *    \return old id. 
 *
 *  - int \b getId (\a mComponent *self);\n
 *    Get the Id of this component\n
 *    \param self - this pointer
 *    \return the Id of component
 *
 *  - mComponent* \b getReleated(\a mComponent* self, \a int releated); \n
 *    Get Releated components, such as parent, children, next sliding, prev sliding\n
 *    \param self - this pointer
 *    \param releated - the releated type, \ref ncsComponentReleated
 *    \return the releated component pointer if found, NULL otherwise
 *
 *  - mComponent* \b setReleated(\a mComponent* self,\a mComponent *comp, \a int releated);\n
 *    Set Releated components\n
 *    \param self - this pointer
 *    \param releated - the releated type, \ref ncsComponentReleated
 *    \param comp - the component to set
 *    \return the param comp pointer if success, or NULL otherwise
 *
 *  - mComponent* \b getChild(\a mComponent* self, \a int id);\n
 *    Get a child by id\n
 *    \param self - this pointer
 *    \param id - the child id
 *    \return the pointer of children, or NULL if failed
 *  - BOOL \b setSpecificData (mObject* self, DWORD key, DWORD value, \a PFreeSpecificData freeSpecific = NULL)
 *    set the sepcific data to an object\n
 *    \param self - this pointer
 *    \param key  - the key of data
 *    \param value - the value of data
 *    \param freeSpecific - the free function to the value, called when object will be destroied
 *    \return TRUE/FALSE
 *
 *  - DWORD \b getSpecificData(mComponent* self, DWORD key, BOOL *pOk = NULL)
 *    get the specific data has been set
 *    \param self - this pointer
 *    \param key - the key of specific data
 *    \param pOk - get the data sucesses or not
 *    \return The data value or zero (when pOk is FALSE)
 
 *
 */

/**
 * \enum ncsComponentReleated
 * \brief The releation between components
 */
enum ncsComponentReleated{
	NCS_CMPT_NEXT = 0,
	NCS_CMPT_PREV,
	NCS_CMPT_PARENT,
	NCS_CMPT_CHILDREN
};
/**
 * \var NCS_CMPT_NEXT
 * \brief next sliding component
 */
/**
 * \var NCS_CMPT_PREV
 * \brief prev sliding component
 */
/**
 * \var NCS_CMPT_PARENT
 * \brief the parent component
 */
/**
 * \var NCS_CMPT_CHILDREN
 * \brief the first child component
 */

/**
 * \enum mComponentQueryCmd
 *
 * \brief queryByPath and vqueryByPath's cmd_id
 */
enum mComponentQueryCmd{
	/**
	 * setPropertyByPath
	 */
	QPC_SET_PROP = 0,
	/**
	 * getPropertyByPath
	 */
	QPC_GET_PROP,
	/**
	 * addEventListenerByPath
	 */
	QPC_ADD_EVENT,
	/**
	 * addBindPropByPath
	 */
	QPC_ADD_BIND,
	/**
	 * user
	 */
	QPC_USER
};

struct _mComponentClass {
	mComponentClassHeader(mComponent, mObject)
};

MGNCS_EXPORT mComponent *ncsCompGetMainParent(mComponent*self);

MGNCS_EXPORT mComponent *ncsCompGetRoot(mComponent* self);

#define IDPATH_END -1

#define IDPATH_ME     -2
#define IDPATH_PARENT -3
#define IDPATH_MAINPARENT -4
#define IDPATH_ROOT -5

#define ISVALID_IDPATH(id_path) \
	((id_path) && (id_path)[0] != IDPATH_END)

#define IDPATH_ISOBJ(id_path) \
	((id_path)[1] != IDPATH_END)

#define BYPATHFUNCEX(baseclass, base_check, err_ret, func, finaly_func_ex, ...) do{ \
	mComponent* cmp; \
	if(!ISVALID_IDPATH(id_path)) \
		return 0;  \
	while(IDPATH_ISOBJ(id_path)) { \
		if(id_path[0] == IDPATH_ME){ \
			id_path ++; \
			continue; \
		} else if(id_path[0] == IDPATH_PARENT) \
			cmp = _c(self)->getReleated(self, NCS_CMPT_PARENT); \
		else if(id_path[0] == IDPATH_MAINPARENT) \
			cmp = ncsCompGetMainParent((mComponent*)self); \
		else if(id_path[0] == IDPATH_ROOT) \
			cmp = ncsCompGetRoot((mComponent*)self); \
		else  \
			cmp = _c(self)->getChild(self, id_path[0]); \
		if(!cmp && base_check(cmp,baseclass)) return err_ret; \
		return _c(baseclass*)->func((baseclass*)cmp, ++id_path, ##__VA_ARGS__); \
	} \
	finaly_func_ex(self, id_path[0], ##__VA_ARGS__); \
}while(0)

#define BYPATHFUNC(baseclass, base_check,err_ret, func, final_func, ...) \
	BYPATHFUNCEX(baseclass, base_check,err_ret, func, return (_c(self)->final_func), ##__VA_ARGS__)

#define IDPATH_NOBASECHECK(cmp, baseclass)   1

/**
 * \var g_stmComponentCls
 * \brief the singleton of mComponentClass
 */
MGNCS_EXPORT extern mComponentClass g_stmComponentCls; //Class(mComponent);

/**
 * A struct include a event mapping node
 *
 * \note only used by \ref mComponent
 *
 */
typedef struct _NCS_EVENT_HANDLER_NODE {
	/**
	 * The event code 
	 */
	UINT message;
	/**
	 * A callback of event
	 * 
	 * \note diffrent event has diffrent callback,
	 * so we use void * to make all the pointer of callback
	 * can be saved. 
	 */
	void *handler;
	/**
	 * next node
	 */
	struct _NCS_EVENT_HANDLER_NODE *next;
}NCS_EVENT_HANDLER_NODE;

typedef struct _mSpecificDataNode {
	DWORD key;
	DWORD value;
	PFreeSpecificData freeSpecific;
	struct _mSpecificDataNode* next;
}mSpecificDataNode;

#define mComponentHeader(clss) \
	mObjectHeader(clss) \
	NCS_EVENT_HANDLER_NODE * event_handlers; \
	mSpecificDataNode* specificHead;

/**
 * \struct mComponent
 * \brief the component member define
 *
 *  - \a NCS_EVENT_HANDLER_NODE * \b event_handlers - the head of handler table
 *
 * \sa mComponentClass
 *
 */

struct _mComponent {
	mComponentHeader(mComponent)
};

/** @} end of Component */

/**
 * \addtogroup global_defines Global defines
 * @{
 */

/**
 * \defgroup global_component_functions Global Component Functions
 * @{
 */

//////////////////////////////////////////
/**
 * A struct of event-handler mapping
 *
 * \note only used for param
 *
 * \sa NCS_EVENT_HANDLER_NODE
 */
typedef struct _NCS_EVENT_HANDLER {
	/**
	 * The event code
	 */
	UINT message;
	/**
	 * The event callback pointer
	 */
	void *handler;
}NCS_EVENT_HANDLER;

/**
 * \fn void * ncsSetComponentHandler(mComponent* comp, UINT message, void *handler);
 * \brief set the component handler
 *
 * \param comp the compont to set
 * \param message the event code
 * \param handler the event callback pointer
 *
 * \return old event callback if it has been set
 *
 * \sa ncsSetComponentHandlers
 */
MGNCS_EXPORT void * ncsSetComponentHandler(mComponent* comp, UINT message, void *handler);

/**
 * \fn void ncsSetComponentHandlers(mComponent* comp, NCS_EVENT_HANDLER* handlers, int count);
 * \brief set an array of event handlers
 * 
 * \param comp - the component to set
 * \param handlers - the array of \ref NCS_EVENT_HANDLER
 * \param count - the count of array handlers. 
 *
 * \note if count == -1, handlers must end by {-1, NULL}; anywhere, ncsSetComponentHandlers would stop
 * if it find an element of array handlers is equal {-1, NULL}, whether count is equal -1 or not
 *
 * \sa ncsSetComponentHandler
 */
MGNCS_EXPORT void ncsSetComponentHandlers(mComponent* comp, NCS_EVENT_HANDLER* handlers, int count);

/**
 * \fn void* ncsGetComponentHandler(mComponent* comp, UINT message);
 * \brief get an event callback 
 *
 * \param comp 
 * \param message - event code
 * 
 * \return void * the handler of message, or NULL if not set
 */
MGNCS_EXPORT void* ncsGetComponentHandler(mComponent* comp, UINT message);

/**
 * A struct of connection map
 * \note used for \a ncsComponentConnectEvents, in resmanager
 *
 * \sa ncsComponentConnectEvents
 */
typedef struct _NCS_EVENT_CONNECT_INFO {
	/**
	 * The id of sender, must be a child or itself of a component
	 */
	int id_sender;
	/**
	 * The id of listener, must be a child or itself of a component
	 */
	int id_listener;
	/**
	 * The event id of sender
	 */
	int event_id;
	/**
	 * The callback for processing event
	 */
	NCS_CB_ONOBJEVENT event;
}NCS_EVENT_CONNECT_INFO;

/**
 * \fn BOOL ncsComponentConnectEvents(mComponent* comps, NCS_EVENT_CONNECT_INFO *connects, int counts=-1);
 * \brief connect the events between parent and its children, used by \ref ncsCreateMainWindowIndirectFromID
 *
 * \param comps the mComponent object pointer, it's the parent of the senders and listeners in \ref NCS_EVENT_CONNECT_INFO
 * \param connects the connection information array
 * \param counts the count of connects array, if it's -1, the array must end by {0, 0, -1, NULL}
 *
 * \sa NCS_EVENT_CONNECT_INFO, ncsComponentConnectEvents
 */
MGNCS_EXPORT BOOL ncsComponentConnectEvents(mComponent* comps, NCS_EVENT_CONNECT_INFO *connects, int counts/*=-1*/);

/**
 * \fn BOOL ncsIsClass(const char* class_name, mComponentClass* clss);
 * \brief check a class name is a special class type or not
 *
 * \param class_name  the class name to check, this is the component class Name, not the typeName of mObject.
 * \param clss the expected class type pointer
 *
 * \return TRUE - check success, FALSE -failed
 */
MGNCS_EXPORT BOOL ncsIsClass(const char* className, mComponentClass* clss);

/**
 * \def ISCLASS
 * \brief the wrapper of \ref ncsIsClass, test a one component class is special one. 
 * \note class_name is a string indicating the component class name, such as \a NCSCTRL_STATIC; the clssName is an identifier of a class, such as mStatic.
 * \sa ncsIsClass
 */
#define ISCLASS(class_name, clssName) ncsIsClass((const char*)(class_name), (mComponentClass*)(&Class(clssName)))

/**
 * \fn BOOL ncsIsChildClass(const char* childClassName, const char* parentClassName);
 * \brief check a class name is derived class of a special class
 *
 * \param childClassName - the class name to be checked, the component class name, not the typeName of mObject
 * \param parentClassName - the class name expected as child class's ancestor, the component class name, not the typeName of mObject
 *
 * \return TRUE - check sucess, FALSE -failed
 */
MGNCS_EXPORT BOOL ncsIsChildClass(const char* childClassName, const char* parentClassName);

/**
 * \fn BOOL ncsRegisterComponent(mComponentClass *compCls, DWORD dwStyle, DWORD dwExStyle, int idCursor, int idBkColor);
 * \brief register a component class into MiniGUI, so that \ref ncsCreateWindow and \ref ncsCreateWindow can find a \ref mComponentClass instance
 *
 * \param compCls the \ref mComponentClass to be registered
 * \param dwStyle the default style
 * \param dwExStyle the default extend style
 * \param idCursor the default cursor
 * \param idBkColor the default background color
 *
 * \return TRUE - success, FALSE - failed
 *
 * \sa ncsGetComponentClass, ncsCreateWindow, ncsCreateWindowIndirect, ncsCreateMainWindow, ncsCreateMainWindowIndirect
 */
MGNCS_EXPORT BOOL ncsRegisterComponent(mComponentClass *compCls, DWORD dwStyle, DWORD dwExStyle, int idCursor, int idBkColor);

/**
 * \fn mComponentClass * ncsGetComponentClass(const char* className, BOOL check);
 * \brief Get a \ref mComponentClass instance from MiniGUI
 *
 * \note the class_name must be registered into MiniGUI by calling ncsRegisterComponent
 *
 * \param className the class name to find
 * \param check check the class name with found mComponentClass instance, to ensure that we found the right class
 *
 * \return the mComponentClass pointer if sucess, NULL otherwise
 */
MGNCS_EXPORT mComponentClass * ncsGetComponentClass(const char* className, BOOL check);

MGNCS_EXPORT mComponentClass * ncsClassFromMagicNum(DWORD magic_num, BOOL check, const char* class_name);

/** @} end of global_component_functions */

#define MGNCS_REGISTER_COMPONENT_EX(className, dwStyle, dwExStyle, idCursor, idBkColor) \
	do {if(!ncsRegisterComponent((mComponentClass*)MGNCS_INIT_CLASS(className), \
                dwStyle, dwExStyle, idCursor, idBkColor)) \
		return FALSE;}while(0)

/* NOTE: use WE_BGC_WINDOW as the default background color instead of WE_MAINC_THREED_BODY */
#define MGNCS_REGISTER_COMPONENT(className) \
	MGNCS_REGISTER_COMPONENT_EX(className, \
                WS_NONE, WS_EX_NONE, IDC_ARROW, WE_BGC_WINDOW)

#define MGNCS_UNREG_COMPONENT(clssName) \
	UnregisterWindowClass(Class(clssName).className)	

#define NCS_SET_PROP(self, id, value) \
	_c(self)->setProperty(self, (id), (value))
#define NCS_GET_PROP(self, id)        \
	_c(self)->getProperty(self, (id))

/** @} end of global_defines */

#ifdef __cplusplus
}
#endif

#endif

