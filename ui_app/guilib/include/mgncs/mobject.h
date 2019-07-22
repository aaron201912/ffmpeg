/**
 * \file mobject.h
 * \author dongjunjie
 * \date 2009/04/16
 *
 * This file include the basic object defines
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

#ifndef _MGUI_NCSCTRL_OBJECT_H
#define _MGUI_NCSCTRL_OBJECT_H

#ifdef __cplusplus
extern "C"{
#endif

/**
 * \defgroup Object mObject
 * @{
 */

#define MGNCS_INIT_CLASS(clss) \
	Class(clss).classConstructor((mObjectClass*)(void*)(&(Class(clss))))
/**
 * \def _c
 * \brief the marco get the class of a object
 */
#define _c(ths)                 (ths)->_class
#define ClassType(className)    className##Class
#define Class(className)        g_st##className##Cls
#define ObjectType(className)   className

#ifdef  WIN32
#define _TYPEOF(x)  void* //__typeof(x)
#else 
#define _TYPEOF(x)  typeof(x)
#endif

#define BEGIN_MINI_CLASS(clss, superCls) \
static ClassType(clss) * clss##ClassConstructor(ClassType(clss)* _class); \
ClassType(clss) Class(clss) = { (PClassConstructor)clss##ClassConstructor }; \
static const char* clss##_type_name = #clss; \
static ClassType(clss) * clss##ClassConstructor(ClassType(clss)* _class) { \
	unsigned short * _pintfOffset = NULL; \
	_pintfOffset = (unsigned short *)((UINT_PTR)_pintfOffset ^ 0); /* VW: prevent unused-but-set-variable warning */ \
	_class = (ClassType(clss)*)((PClassConstructor)(Class(superCls).classConstructor))((mObjectClass*)_class); \
	_class->super = (superCls##Class*)&Class(superCls); \
	_class->typeName = clss##_type_name; \
	_class->objSize = sizeof(clss);      \
	_pintfOffset = &_class->intfOffset;

#define END_MINI_CLASS return _class; }

#define CLASS_METHOD_MAP(clss, name) \
		_class->name = (_TYPEOF(_class->name))(clss##_##name);


#define NEED_CHECK

#ifdef NEED_CHECK
#define CHECK_METHOD(cls, mth)  ((cls) && (cls)->mth)
#else
#define CHECK_METHOD(cls, mth)  (1)
#endif


#define PUBLIC
#define PRIVATE
#define PROTECTED
#define STATIC

#define _M(pobj, method, ...)  _c(pobj)->method((pobj), ##__VA_ARGS__)

#define DECLARE_CLASSEX(EXPORT, clss, clssSuper) \
	typedef struct _##clss##Class clss##Class; \
	typedef struct _##clss clss; \
	struct _##clss##Class{ \
		clss##ClassHeader(clss, clssSuper) \
	}; \
	struct _##clss { \
		clss##Header(clss) \
	}; \
	EXPORT extern clss##Class g_st##clss##Cls;

#define DECLARE_CLASS(clss, clssSuper) DECLARE_CLASSEX(MGNCS_EXPORT, clss, clssSuper)
#define NONE_EXPORT

#define _SUPER(super,self,method, ...)  Class(super).method((super*)(self), ##__VA_ARGS__)

#define DECLARE_OBJECT(Clss)  \
	typedef struct _##Clss##Class Clss##Class;  \
	typedef struct _##Clss Clss;  \
	MGNCS_EXPORT extern Clss##Class g_st##Clss##Cls;

#define DEFINE_OBJECT(Clss, ClssSuper)  \
	struct _##Clss##Class { Clss##ClassHeader(Clss, ClssSuper) }; \
	struct _##Clss { Clss##Header(Clss) };

////////////////////// interface support
#define INTERFACE(Interface)  \
	Interface##VTable * Interface##_; /*Interface vtable*/

#define DECLARE_INTERFACE(Interface)  \
	typedef struct _##Interface##VTable Interface##VTable; \
	typedef struct _##Interface Interface;

#define DEFINE_INTERFACE(Interface)          \
	struct _##Interface##VTable {                             \
		Interface##Header(Interface, Interface)         \
	};                                                        \
	struct _##Interface { Interface##VTable *_vtable; };

#define IMPLEMENT(Clss,Interface) \
	_class->_##Interface##_obj_offset = (UINT_PTR)(void*)&(((Clss*)0)->Interface##_); \
	_class->_##Interface##_next_offset = 0;                                                \
	*_pintfOffset =  (unsigned short)(UINT_PTR)(void*)&(((ClassType(Clss)*)0)->_##Interface##_obj_offset); \
	_pintfOffset = &_class->_##Interface##_next_offset;

#define INTERFACE_CAST(Interface, pobj)  \
	((Interface*)((unsigned char*)(pobj) + (_c(pobj)->_##Interface##_obj_offset)))

//the basic interface
DECLARE_INTERFACE(IInterface)
struct _IInterfaceVTable{
	unsigned short _obj_offset;
	unsigned short _next_offset;
};

struct _IInterface {
	IInterfaceVTable * _vtable;
};

#define IInterfaceHeader(Interface, ClssImpl)  \
	unsigned short _##Interface##_obj_offset; \
	unsigned short _##Interface##_next_offset; \

/**
 * \def INTEFACE_ADJUST
 * \brief adjust the pointer as the real pointer
 */
#define INTEFACE_ADJUST(piobj)  (_TYPEOF(piobj))((unsigned char*)(piobj) -  \
		((IInterfaceVTable*)((piobj))->_vtable)->_obj_offset)

#define INTERFACE2OBJECT(type, piobj)  (type*)((unsigned char*)(piobj) - ((IInterfaceVTable*)((piobj))->_vtable)->_obj_offset)

/**
 * \def _IM 
 * \brief call the interface object
 */
#define _IM(piobj, method, ...) \
	(piobj)->_vtable->method(INTEFACE_ADJUST(piobj), ##__VA_ARGS__)
#define _I(piobj)  (piobj)->_vtable



typedef struct _mObjectClass mObjectClass;
typedef struct _mObject mObject;

/**
 * \typedef mObjectClass* (*PClassConstructor)(mObjectClass * cls);
 * \brief the Class initialize callback
 *
 * \param cls - the class pointer to initialize
 *
 * \return mObjectClass* - the initialized class pointer, equal param cls
 */
typedef mObjectClass* (*PClassConstructor)(mObjectClass *);

/**
 * We support two kind of construct: one is the old construct, uses DWORD to pass the
 * user params; the other is new construct, use the va_list to pass the params
 *
 * mgncs don't known which construct of an object use, we just pass them by diffrent wrapper
 *
 * the caller must known which wrapper should be used. 
 *
 * if you use the wrong wrapper, the system may be CRASH 
 */
typedef void (*PCONSTRUCT_DWORD)(void* self, DWORD addData);
#include <stdarg.h>
typedef void (*PCONSTRUCT_VALIST)(void* self, va_list va);

#define mObjectClassHeader(clss, superCls) \
	PClassConstructor classConstructor; \
	ClassType(superCls) * super; \
	const char* typeName; \
	unsigned short objSize; \
	unsigned short intfOffset; /** define the interface offset*/ \
	void (*construct)(clss *self, DWORD addData); \
	void (*destroy)(clss *self); \
	DWORD (*hash)(clss *self); \
	const char* (*toString)(clss *self, char* str, int max);  \
    int (*addRef)(clss*); \
    int (*release)(clss*);




/**
 * \struct mObjectClass
 * \brief the basic object class of NCS
 *
 *  - PClassConstruct \b classConstructor \n
 *    the class intialize callback
 *
 *  - mObjectClass * \b super \n
 *    the super class pointer
 *
 *  - const char* \b typeName \n
 *    the class name, for example, "Object"
 *
 *  - unsigned int \b objSize \n
 *    the size of corresponding object, used to create a object
 *
 *  - void \b construct(mObject *self, DWORD addData);\n
 *    the construct method of mObject\n
 *    \param self - this pointer
 *    \param addData - the initialize data, depend on derived class
 *  - void \b destroy (mObject* self);\n
 *    Destroy a mObject object\n
 *    \param self - this pointer
 *
 *
 * \sa mObject
 *
 */

struct _mObjectClass {
	mObjectClassHeader(mObject, mObject)
};

/**
 * \var g_stmObjectCls
 * \brief the singleton of mObjectClass, represent a class object
 */
MGNCS_EXPORT extern mObjectClass g_stmObjectCls; //Class(mObject);

enum mObjectStatus
{
    NCSF_OBJECT_TODEL = 0x1000,
};
#define NCS_OBJ_TODEL(self) (self->objStatus & NCSF_OBJECT_TODEL)

#define mObjectHeader(clss) \
	ClassType(clss) * _class; \
    short objRefCount; \
    short objStatus;

/**
 * \struct mObject
 * \brief the Object struct
 *
 *  - mObjectClass * \a _class  \n
 *   the class pointer of this Object
 *
 *  \sa mObjectClass
 */

struct _mObject {
	mObjectHeader(mObject)
};


/**
 * \fn void ncsRefObjFreeSpecificData(DWORD key, DWORD value)
 */
MGNCS_EXPORT void ncsRefObjFreeSpecificData(DWORD key, DWORD value);

MGNCS_EXPORT mObject* initObject(mObject* pobj, mObjectClass* _class);

/**
 * init an object with DWORD param
 */
static inline mObject* _initObject(mObject* pobj, mObjectClass* _class,DWORD param) {
	memset(pobj, 0, _class->objSize);
	initObject(pobj, _class);
	((PCONSTRUCT_DWORD)(_class->construct))(pobj, param);
	return pobj;
}

/**
 * init an object with va_list params
 * first param is the construct param count
 */
static inline mObject* initObjectArgs(mObject* pobj, mObjectClass* _class, ...)
{
	va_list va;
	va_start(va, _class);
	memset(pobj, 0, _class->objSize);
	initObject(pobj, _class);
	((PCONSTRUCT_VALIST)(_class->construct))(pobj, va);
	va_end(va);
	return pobj;
}

#define INIT_OBJEX(Clss, pobj, param)  ((Clss* )_initObject((mObject*)(pobj), (mObjectClass*)&Class(Clss), (DWORD)(param)))
#define INIT_OBJ(Clss, pobj)  INIT_OBJEX(Clss, pobj, 0)
#define INIT_OBJV(Clss, pobj, ...) ((Clss* )initObjectArgs((mObject*)(pobj), (mObjectClass*)&Class(Clss), ##__VA_ARGS__))
#define UNIT_OBJ(pobj)  (_c(pobj)->destroy(pobj))

/** @} end of Object define */

/**
 * \addtogroup global_defines Global defines
 * @{
 */

 /**
  * \defgroup gobal_object_functions Global Object Functions
  * @{
  */

/**
* \fn mObject * newObject(mObjectClass *_class);
* \brief new a object instance, like \b new operator in C++
*
* \param _class - the class of the object
*
* \return the new pointer of object
*
*/
MGNCS_EXPORT mObject * newObject(mObjectClass *_class);
static inline mObject * ncsNewObject(mObjectClass *_class,DWORD addData){
	mObject * obj = newObject(_class);
	if(!obj)
		return NULL;

	_class->construct(obj, addData);
	return obj;
}

#define NEWEX(classType, addData)  (classType*)(void*)ncsNewObject((mObjectClass*)(void *)(&Class(classType)), (DWORD)(addData))
#define NEW(classType)  NEWEX(classType, 0)
static inline mObject * _newobject_args(mObjectClass* _class, ...)
{
	va_list va;
	mObject * obj;
	va_start(va, _class);
	obj = newObject(_class);
	if(obj)
		((PCONSTRUCT_VALIST)(_class->construct))(obj, va);
	va_end(va);
	return obj;
}
#define NEWV(Clss, ...)  ((Clss*)_newobject_args((mObjectClass*)&(Class(Clss)), ##__VA_ARGS__))

/**
 * \fn void deleteObject(mObject *obj);
 * \brief delete a object intance, like \b delete operator in C++
 *
 * \param obj - the object want to delete
 *
 */
MGNCS_EXPORT void deleteObject(mObject *obj);
#define DELETE(obj)   deleteObject((mObject*)(obj))

/**
 * \def TYPENAME
 * \brief Get the class name form a Object pointer
 */
#define TYPENAME(obj)  ((obj)?(((obj)->_class)?((obj)->_class)->typeName:""):"")

///////////////////////

/**
 * \fn BOOL ncsInstanceOf(mObject* object, mObjectClass* clss);
 * \brief check an object is the class instance or not, same as \b instanceof operator in Java
 *
 * \param object - the pointer of object being to test
 * \param clss - the pointer of class for test
 *
 * \return TRUE - object is instance of clss, FALSE - not
 */
MGNCS_EXPORT BOOL ncsInstanceOf(mObject* object, mObjectClass* clss);

/**
 * \def INSTANCEOF
 * \brief A wrapper of \ref ncsInstanceOf, just like java's instanceof
 *
 * Example:
 * 	Test a object is mComponentClass:
 * 	INSTANCEOF(obj, mComponent)
 */
#define INSTANCEOF(obj, clss)  ncsInstanceOf((mObject*)(obj), (mObjectClass*)(void*)(&Class(clss)))

/**
 * \fn static inline mObject*  ncsIsValidObj(mObject* obj);
 * \brief Check a pointer is a valid mObject or not
 *
 * \param obj - the excpeted object pointer
 *
 * \return mObject * the pointer of obj or other NULL if obj is an invalid mObject pointer
 */
static inline mObject*  ncsIsValidObj(mObject* obj){
	return (INSTANCEOF(obj, mObject)?(obj):NULL);
}

/**
 * \def CHECKOBJ
 * \brief the wrapper of ncsIsValidObj
 *
 * \sa ncsIsValidObj
 */
#define CHECKOBJ(obj)  ncsIsValidObj((mObject*)obj)

/**
 * \fn mObject* ncsSafeCast(mObject* obj, mObjectClass *clss);
 * \brief safe type cast function, like the \b dynamic_cast operator in C++
 *
 * \param obj - the mObject pointer being casted
 * \param clss - the target type to cast
 *
 * \return mObject * - the object pointer if cast safe, NULL otherwise
 *
 * \sa ncsInstanceOf
 */
MGNCS_EXPORT mObject* ncsSafeCast(mObject* obj, mObjectClass *clss);

/**
 * \def TYPE_CAST
 * \brief unsafe cast. donot check the type of class
 */
#define TYPE_CAST(Type, obj)  ((Type*)(obj))
/**
 * \def SAFE_CAST
 * \brief wrapper of ncsSafeCast, check the class type before cast.
 *
 * \note this macro is same as \b dynamic_cast in C++
 *
 * \sa ncsSafeCast
 */
#define SAFE_CAST(Type, obj)  TYPE_CAST(Type, ncsSafeCast((mObject*)obj,(mObjectClass*)(void*)(&(Class(Type)))))

/**
 * \fn mObjectClass *ncsSafeCastClass(mObjectClass* clss, mObjectClass* castCls);
 * \brief cast from a supper class to derived class type safely
 *
 * \param clss - the supper type class pointer
 * \param castCls - the derived class type pointer
 *
 * \note use \ref SAFE_CAST_CLASS
 *
 * \return mObjectClass * - the pointer of clss if cast safe, otherwise NULL
 */
MGNCS_EXPORT mObjectClass *ncsSafeCastClass(mObjectClass* clss, mObjectClass* castCls);
/**
 * \def SAFE_CAST_CLASS
 * \brief the wrapper of ncsSafeCastClass
 */
#define SAFE_CAST_CLASS(Clss, ClssCast) ((ClassType(ClssCast)*)(ncsSafeCastClass((mObjectClass*)Clss, (mObjectClass*)(void*)(&Class(ClssCast)))))

int ncsParseConstructParams(va_list args, const char* signature, ...);

/**
 * \typedef typedef BOOL (*NCS_CB_ONOBJEVENT)(mObject* self, mObject *sender, int eventId, DWORD param);
 * \brief The Callback of connection
 *
 * \note For widgets, only support notification
 *
 * \param self  The listener pointer
 * \param sender The sender of event
 * \param eventId the id of event
 * \param param the param of event
 *
 * \return TRUE - continue dispatch the event; FALSE - stop the event.
 */
typedef BOOL (*NCS_CB_ONOBJEVENT)(mObject* self, mObject *sender, int eventId, DWORD param);

#define NCS_CB_ONPIECEEVENT NCS_CB_ONOBJEVENT

/**
 * \fn BOOL ncsAddEventListener(mObject *sender, \
 * 					mObject* listener, \
 * 					NCS_CB_ONOBJEVENT eventProc, \
 * 					int eventId);
 * \brief connect sender object to listener object
 * 
 * \param sender The sender of event
 * \param listener The listener of event
 * \param eventProc The processing callback of event
 * \param eventId The id of event
 *
 * \return TRUE - Sucessed, FALSE - Failed
 * 
 * \sa ncsAddEventListener, ncsAddEventListeners, NCS_CB_ONOBJEVENT
 */
MGNCS_EXPORT BOOL ncsAddEventListener(mObject *sender,
	mObject* listener,
	NCS_CB_ONOBJEVENT eventProc,
	int eventId);

/**
 * BOOL ncsAddEventListeners(mObject *sender, \
 * 					mObject* listener, \
 * 					NCS_CB_ONOBJEVENT eventProc, \
 * 					int* eventIds);
 * \brief connect a group of events from sender to listener
 *
 * \param sender The sender of events
 * \param listener The listener of events
 * \param eventProc The processing callback of events
 * \param eventIds The id array of events, end by 0
 *
 * \return TRUE - Sucessed, FALSE - Failed
 * 
 * \sa ncsAddEventListener, ncsAddEventListeners, NCS_CB_ONOBJEVENT
 */
MGNCS_EXPORT BOOL ncsAddEventListeners(mObject *sender,
	mObject* listener,
	NCS_CB_ONOBJEVENT eventProc,
	int* eventIds);

/**
 * \fn BOOL ncsRemoveEventListener(mObject * listener);
 * \brief remove the connections which are listened by the object
 *
 * \note this function is called when a listener is destroied
 *
 * \param listener the object pointer of listener
 *
 * \return TRUE - sucessed; FALSE - failed
 */
MGNCS_EXPORT BOOL ncsRemoveEventListener(mObject * listener);

/**
 * \fn BOOL ncsRemoveEventConnect(mObject* listener, mObject* source, int event);
 * \brief remove a special event connect
 *
 * \param listener the listener object
 * \param source the source object
 * \param event the special event want to remove
 *
 */
MGNCS_EXPORT BOOL ncsRemoveEventConnect(mObject* listener, mObject* source, int eventId);

/**
 * \fn BOOL ncsRemoveEventSource(mObject *sender);
 * \brief remove the connections when a sender is destroied
 *
 * \note this function is called when a sender is destroied
 *
 * \param sender the sender of events
 *
 * \return TRUE - sucessed; FALSE - failed
 */
MGNCS_EXPORT BOOL ncsRemoveEventSource(mObject *sender);

/**
 * \fn void ncsRaiseEvent(mObject *sender, int event_id, DWORD param);
 * \brief raise an event to listeners
 *
 * \note it will call the callback of \ref NCS_CB_ONOBJEVENT add by \ref ncsAddEventListeners, or \ref ncsAddEventListener
 *
 * \param sender event sender
 * \param event_id the id of event
 * \param param the event param
 *
 * \sa NCS_CB_ONOBJEVENT, ncsAddEventListener, ncsAddEventListeners
 */
MGNCS_EXPORT void ncsRaiseEvent(mObject *sender, int eventId, DWORD param);


/** @} end of global_object_funtions */

/** @} endof global_defines */

#ifdef __cplusplus
}
#endif


#endif

