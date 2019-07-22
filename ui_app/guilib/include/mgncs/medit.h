/**
 * \file medit.h
 * \author Wangjian<wangjian@minigui.org>
 * \date 2009/07/13
 *
 * This file includes the definition of mEdit control.
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

#ifndef _MGUI_NCSCTRL_EDIT_H
#define _MGUI_NCSCTRL_EDIT_H
 
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/* structure to represent a text string */
typedef struct strbuffer_s
{
    unsigned char *string;     /* text string content */
    unsigned int  txtlen;      /* text len or buffer used size */
    unsigned int  blocksize;   /* block size, a block is an allocate unit */
    unsigned int  buffsize;    /* buffer size */
} StrBuffer;

/* ------------------------- memory and struct alloc ------------------------ */

#define te_alloc                         malloc
#define te_free                          free
#define te_realloc(ptr, size)   realloc(ptr, size)

/* alloc with a unit of blocksize */
static inline void* testr_alloc (StrBuffer *txtbuff, size_t len, size_t blocksize)
{
    txtbuff->buffsize = (len + 1) + (blocksize - (len + 1)%blocksize);
    txtbuff->string = (unsigned char *)te_alloc (txtbuff->buffsize);
    txtbuff->blocksize = blocksize;
    //txtbuff->txtlen = len;
    return txtbuff->string;
}

static inline void* testr_realloc (StrBuffer *txtbuff, size_t len)
{
    if (len + 1 > (size_t)txtbuff->buffsize || 
             len + 1 < (size_t)(txtbuff->buffsize - txtbuff->blocksize)) {
        /* really realloc */
        txtbuff->buffsize = (len + 1) + (txtbuff->blocksize - 
                            (len + 1)%txtbuff->blocksize);
        txtbuff->string = (unsigned char *)te_realloc (txtbuff->string, txtbuff->buffsize);
    }

    return txtbuff->string;
}

static inline void testr_free (StrBuffer *txtbuff)
{
    if (txtbuff) {
        te_free (txtbuff->string);
    }
}

static inline void testr_setstr (StrBuffer *txtbuff, const char *newstring, unsigned int len)
{
    int datalen;

    if (len < 0 && !newstring) return;

    datalen = MIN(len, txtbuff->buffsize-1);
    memcpy (txtbuff->string, newstring, datalen);
    txtbuff->string[datalen] = '\0';
    txtbuff->txtlen = datalen;
}

/* copy a string buffer */
static inline StrBuffer* testr_copy (StrBuffer *des, StrBuffer *src)
{
    if (!des || !src)
        return NULL;

    testr_free (des);
    testr_alloc (des, src->txtlen, des->blocksize);
    testr_setstr (des, (const char *)src->string, src->txtlen);
    return des;
}

/* duplicates a string buffer */
static inline StrBuffer* testr_dup (StrBuffer *s1)
{
    StrBuffer *s2;

    if (!s1)
        return NULL;

    s2 = (StrBuffer *)malloc (sizeof(s1));
    if (s2) {
        testr_alloc (s2, s1->txtlen, s1->blocksize);
        testr_setstr (s2, (const char *)s1->string, s1->txtlen);
    }
    return s2;
}

/* duplicates a string buffer */
static inline StrBuffer* testr_dup2 (StrBuffer *s2, StrBuffer *s1)
{
    if (!s1 || !s2)
        return NULL;

    testr_alloc (s2, s1->txtlen, s1->blocksize);
    testr_setstr (s2, (const char *)s1->string, s1->txtlen);
    return s2;
}

/* -------------------------------------------------------------------------- */


/**
 * \defgroup Control_Edit mEdit
 * @{
 */

/*
 * \def NCSCTRL_EDIT
 * \brief the name of edit control
*/
#define NCSCTRL_EDIT   NCSCLASSNAME("edit")

#ifdef _MGNCS_USE_DBEDIT
#define DOUBLE_DC HDC   db_dc;
#else
#define DOUBLE_DC 
#endif

typedef struct _mEdit mEdit;
typedef struct _mEditClass mEditClass;
typedef struct _mEditRenderer    mEditRenderer;

#define mEditHeader(Class)   \
    mScrollViewHeader(Class) \
	DOUBLE_DC

/** 
 * \struct mEdit
 * \brief the edit struct of edit control, derived from \ref mScrollView.
 *
 *  \sa mEditClass
 */
struct _mEdit
{
    mEditHeader(mEdit)
};


typedef struct _TextAttribute
{
    struct _TextAttribute* (*clone)(void);
    void (*destroy)(void);
}TextAttribute;

typedef struct _PasteInfo {
    char          *str;
    int            len;
    TextAttribute *attr;
    void          *user_info;
    struct _PasteInfo *next;
}PasteInfo;

typedef struct _TextCopyPaste
{
    void (*addCopyInfo)(const char* str, int len, TextAttribute *atrr, void *user_info);
    void (*endCopyInfo)(void);
    BOOL (*isEmpty)(void);
    PasteInfo* (*getPasteInfo)(void);
}TextCopyPaste;

typedef struct _UndoRedoInfo
{
    char           *text;
    int             start;
    int             end;
    TextAttribute  *attr;
    void           *user_data;
    struct _UndoRedoInfo * next;
} UndoRedoInfo;

typedef struct _TextUndoRedoInfo
{
    BOOL (*pushText)(const char* text, int start, int end, TextAttribute *attr, void *user_data);
    BOOL (*endPush)(void);

    UndoRedoInfo* (*getUndoTop)(void);
    UndoRedoInfo* (*getRedoTop)(void);

    void (*popUndo)(void);
    void (*popRedo)(void);

    BOOL (*isUndoEmpty)(void);
    BOOL (*isRedoEmpty)(void);

    void (*empty)(void);
}TextUndoRedoInfo;

#define mEditClassHeader(clsName, parentClass) \
    mScrollViewClassHeader(clsName, parentClass)    \
    int (*onChar)(clsName* self, int asciiCode, DWORD keyFlags);           \
    int (*setContent)(clsName *self, const char* str, int start, int len);   \
    void (*replaceText)(clsName *self, const char* str, int start, int len, int replaceStart, int replaceEnd);    \
    void (*insert)(clsName *self, const char* str, int start, int len, int insertStart);    \
    void (*append)(clsName *self, const char* str, int start, int len);            \
    int (*getTextLength)(clsName *self);                                           \
    int (*getContent)(clsName *self, char *strBuff, int bufLen, int start, int end); \
    int (*setTextAttr)(clsName *self, int start, int end, TextAttribute *attr);    \
    TextAttribute* (*getTextAttr)(clsName *self, int start, int end, int *pEnd);   \
    int (*setSel)(clsName *self, int start, int end);                              \
    int (*getSel)(clsName *self, int *pStart, int *pEnd);                          \
    void (*setMargin)(clsName *self, int left, int top, int right, int bottom);    \
    void (*copy)(clsName *self);                                                   \
    void (*cut)(clsName *self);                                                    \
    void (*paste)(clsName *self);                                                  \
    TextCopyPaste * (*setCopyPaste)(clsName *self, TextCopyPaste* cp);             \
    void (*undo)(clsName *self);                                                   \
    void (*redo)(clsName *self);                                                   \
    TextUndoRedoInfo * (*setUndoRedo)(clsName *self, TextUndoRedoInfo* ur);        \
    BOOL (*makevisible)(clsName *self, int pos);

/** 
 * \struct mEditClass
 * \brief the VTable of \a mEdit, derived from \ref mScrollViewClass.
 *
 * \sa mScrollViewClass
 *   
 *   - void \b setContent (mEdit *self, const char *str, int start, int len);\n
 *       set the text content of the edit.
 *       \param str - the text to be shown on edit. 
 *       \param start - the start of the text to be shown on edit. 
 *       \param len - length of the text to be shown on edit. 
 *       \return void 
 *
 *   - void \b replaceText (mEdit *self, const char* str, int start, int len, int replace_start, int replace_end);\n
 *      replace text.
 *      \param str - the new text
 *      \param start - the start of the str to be used
 *      \param len - the length of the str to be used
 *      \param replace_start - the start of the text to be replaced in the edit.
 *      \param replace_end - the end of the text to be repalced in the edit.
 *      \return void 
 *
 *   - void \b insert (mEdit *self, const char* str, int start, int len, int start_at);\n
 *      insert text.
 *      \param str - the new text
 *      \param start - the start of the str to be inserted
 *      \param len - the length of the str to be inserted
 *      \param start_at - insert position.
 *      \return void 
 *
 *   - void \b append (mEdit *self, const char* str, int start, int len);\n
 *      append the str into the edit text.
 *      \param str - The new text
 *      \param start - The start of the str to be appended
 *      \param len - The length of the str to be appended
 *      \return void 
 *
 *   - int \b getTextLength (mEdit *self);\n
 *      get the count of the edit text.
 *      \return The count of the text 
 *
 *   - int \b getContent (mEdit *self, char *strbuff, int buf_len, int start, int end); \n
 *      get the text of the edit.
 *      \param strbuff - The string buffer to save the return string
 *      \param buf_len - The size of strbuff
 *      \param start - Start position to get text
 *      \param end - End position to get text
 *      \return The real count of text in strbuff.  
 *      
 *   - int \b setTextAttr (mEdit *self, int start, int end, TextAttribute *attr); \n
 *      unimplemented.
 *
 *   - TextAttribute* \b getTextAttr (mEdit *self, int start, int end, int *p_end); \n
 *      unimplemented.
 *
 *   - int \b setSel (mEdit *self, int start, int end); \n
 *      set the selected position.
 *      \param start - The start of the selected position
 *      \param end - The end of the selected position
 *      \return The length of selected text , -1 for Failed.
 *
 *   - int \b getSel (mEdit *self, int *pstart, int *pend); \n
 *      get the selected position.
 *      \param pstart - The start of the selected position
 *      \param pend - The end of the selected position
 *      \return 0 for Succeed , -1 for Failed.
 *
 *   - void \b setMargin (mEdit *self, int left, int top, int right, int bottom); \n 
 *      set the margin
 *      \param left - the new margin left value
 *      \param top - the new margin top value
 *      \param right - the new margin right value
 *      \param bottom - the new margin bottom value
 *
 *   - void \b copy (mEdit *self); \n  
 *      copy the selected text to clip board.
 *
 *   - void \b cut (mEdit *self);  \n
 *      cut the selected text to clip board.
 *    
 *   - void \b paste (mEdit *self); \n
 *      paste the text in clip board into edit.
 *
 *   - TextCopyPaste* \b setCopyPaste (mEdit *self, TextCopyPaste* cp); \n
 *      set a new copyPaste instance create by user.
 *      \param cp - Pointer of the new copyPaste instance
 *      \return The old one.
 *
 *   - void \b undo (mEdit *self); \n
 *      unimplemented 
 *
 *   - void \b redo (mEdit *self); \n
 *      unimplemented
 *
 *   - TextUndoRedoInfo* \b setUndoRedo (mEdit *self, TextUndoRedoInfo* ur); \n
 *      unimplemented
 *
 *   - BOOL \b makevisible (mEdit *self, int pos); \n
 *      make the position visible
 *      \param pos - the position to be visible.
 *      \return TRUE for succeed, FALSE for failed.
 *      
 */ 
struct _mEditClass
{
    mEditClassHeader(mEdit, mScrollView)
};

/**
 * \var g_stmEditCls
 * \brief the instance of mEditClass
 *
 * \sa mEditClass
 */
MGNCS_EXPORT extern mEditClass g_stmEditCls;


#define mEditRendererHeader(clsName, parentClass) \
	mScrollViewRendererHeader(clsName, parentClass)

/**
 * \struct mEditRenderer
 * \brief Edit class's Renderer interface, derived from \ref mScrollViewRenderer.
 *
 * \sa mEdit, mEditClass
 */
struct _mEditRenderer
{
	mEditRendererHeader(mEdit, mScrollView)
};

/**
 * \def NCSS_EDIT_LEFT 
 * \brief left align edit
 */
#define NCSS_EDIT_LEFT        (0x0000L<<NCSS_SCRLV_SHIFT)

/**
 * \def NCSS_EDIT_CENTER
 * \brief center align edit
 */
#define NCSS_EDIT_CENTER      (0x0001L<<NCSS_SCRLV_SHIFT)

/**
 * \def NCSS_EDIT_RIGHT
 * \brief right align edit
 */
#define NCSS_EDIT_RIGHT       (0x0002L<<NCSS_SCRLV_SHIFT)

/**
 * \def NCSS_EDIT_UPPERCASE
 * \brief uppercase edit
 */
#define NCSS_EDIT_UPPERCASE   (0x0004L<<NCSS_SCRLV_SHIFT)

/**
 * \def NCSS_EDIT_LOWERCASE
 * \brief lowercase edit
 */
#define NCSS_EDIT_LOWERCASE   (0x0008L<<NCSS_SCRLV_SHIFT)

/**
 * \def NCSS_EDIT_NOHIDESEL
 * \brief When lose the focus, it is still selected.
 */
#define NCSS_EDIT_NOHIDESEL   (0x0010L<<NCSS_SCRLV_SHIFT)

/**
 * \def NCSS_EDIT_READONLY
 * \brief readonly
 */
#define NCSS_EDIT_READONLY    (0x0020L<<NCSS_SCRLV_SHIFT)

/**
 * \def NCSS_EDIT_BASELINE
 * \brief show text with base line.
 */ 
#define NCSS_EDIT_BASELINE    (0x0040L<<NCSS_SCRLV_SHIFT)

#define NCSS_EDIT_SHIFT  (NCSS_SCRLV_SHIFT+8)



/**
 * \enum mEditProp
 * \brief the Property of edit
 */
enum mEditProp 
{
	/**
	 * \brief Get/Set the maximum length of text 
	 *  - Type int
	 *  - Read Write
	 *  - the max length of text
	 */
    NCSP_EDIT_LIMITTEXT = NCSP_SCRLV_MAX + 1, 
	/**
	 * \brief Get/Set the position of charactor
	 *  - Type int
	 *  - Read Write
	 *  - The postion begin from first charactor , include the CR
	 */
    NCSP_EDIT_CARETPOS ,
    NCSP_EDIT_MAX
};


/** Edit Notify Code : */
enum mEditNotify
{
    NCSN_EDIT_SETFOCUS = NCSN_ITEMV_SETFOCUS,                   /**< control have gaint the input focus */
    NCSN_EDIT_KILLFOCUS = NCSN_ITEMV_KILLFOCUS,                  /**< control have lost the input focus */
    NCSN_EDIT_CHANGE = NCSN_SCRLV_MAX + 1, /**< content of text is changed by inputing user */
    NCSN_EDIT_CONTCHANGED,               /**< content of text changed when kill focus*/
    NCSN_EDIT_UPDATE,                     /**< text update */
    NCSN_EDIT_SELCHANGED,                 /**< selection changed */
    NCSN_EDIT_MAXTEXT,                    /**< the count of text reach maximum length */
    NCSN_EDIT_MAX
};

#ifdef _MGNCS_USE_DBEDIT
static inline HDC mEdit_getDC(mEdit *self)
{
    if (self->db_dc) {
        SelectClipRect(self->db_dc, NULL);
		SelectFont(self->db_dc, GetWindowFont(self->hwnd));
        return self->db_dc;
    }
    else 
        return GetClientDC(self->hwnd);
}

static inline void mEdit_releaseDC(mEdit *self, HDC hdc) 
{
    if (self->db_dc == 0 || self->db_dc != hdc)
        ReleaseDC(hdc);
}
#else
static inline HDC mEdit_getDC(mEdit *self)
{
    return GetClientDC(self->hwnd);
}

static inline void mEdit_releaseDC(mEdit *self, HDC hdc) 
{
    ReleaseDC(hdc);
}
#endif
#define GETDC(self)             mEdit_getDC((mEdit*)self)
#define RELEASEDC(self, hdc)    mEdit_releaseDC((mEdit*)self, hdc)

    /** @} end of Control_Edit */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_EDIT_H */


