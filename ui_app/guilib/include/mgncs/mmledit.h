/**
 * \file mmledit.h
 * \author Wangjian<wangjian@minigui.org>
 * \date 2009/07/13
 *
 * This file includes the definition of mMlEdit control.
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


#ifndef _MGUI_NCSCTRL_MLEDIT_H
#define _MGUI_NCSCTRL_MLEDIT_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/* ------------------------------- text document/buffer ------------------------ */

/* content type, reserved */
typedef enum {
    CT_TEXT,
    CT_BMP,
    CT_ICON
} ContentType;

/* text format, reserved */
typedef enum {
    TF_PLAINTEXT,
    TF_RICHTEXT,
    TF_MEDIATEXT
} TextFormat;

/* one text line (end with '\n') is a scrollview item */ 
/* structure of text node/line */
typedef struct _textnode
{
    list_t    list;            /* list element */
    StrBuffer content;
    DWORD     addData;         /* for storing scrollview item handle */
} TextNode;

/* structure of a text mark, recording insert position or selection range */
typedef struct _textmark
{
    int       pos_lnOff;       /* mark offset in the text node*/
    TextNode  *curNode;        /* text node containing the mark*/
} TextMark;

/* structure of text document data */
typedef struct _textdoc
{
    list_t    queue;           /* text line/node head */

    /* setup field */
    unsigned char lnsep;       /* line seperator (default is "\n") */
    int       nDefLineSize;    /* default line buffer size*/
    int       nBlockSize;      /* line buffer block size */

    /* node init function executed when creating node */
    void      (*init_fn)     (struct _textdoc *, TextNode *, TextNode *);
    /* node change function executed when changing current insertion/selection node */
    void      (*change_fn)   (struct _textdoc *, BOOL bSel);
    /* node content change function executed when string content of a node is changed */
    void      (*change_cont) (struct _textdoc *, TextNode *node);
    /* node destroy function */
    void      (*del_fn)      (struct _textdoc *, TextNode *);
    void       *fn_data;       /* data passed to init_fn */

    /* status field */
    TextMark  insert;          /* cursor/insertion mark */
    TextMark  selection;       /* selection mark */

} TextDoc;

/* -------------------------------------------------------------------------- */

/* backup data struct */
typedef struct _status_data
{
    DWORD     flags;        /* editor status */

    int       desCaretX;  /* the desired caret x position, changed by mouse and <- , -> */

    int       caretX;      /* caret x position in the virtual content window */
    int       caretY;      /* caret y position in the virtual content window */
    int       selX;        /* selection point x position in the virtual content window */
    int       selY;        /* selection point y position in the virtual content window */

    TextNode  *maxNode;     /* node with the max length */
    int       maxLen;       /* max line length */
    TextNode  *secNode;     /* node with the second max length */
    int       secLen;       /* seconde max line length */

    int       curItemY;     /* y position of the current insertion node */
    int       selItemY;     /* y position of the selection node */
} STATDATA;
typedef STATDATA* PSTATDATA;

/* structure of backup text node/line */
typedef struct _bk_textnode
{
    list_t    list;         /* list element */
    StrBuffer content;      /* text content */
} BkNode;

typedef struct _backup_data
{
    STATDATA  statData;
    TextMark  bkIns;        /* backup insertion point */
    TextMark  bkSel;        /* backup selection point */
    int       opType;       /* operation type */
    list_t    bkQueue;      /* backup text nodes */
} BKDATA;
typedef BKDATA* PBKDATA;

/**
 * \defgroup Control_MlEdit mMlEdit
 * @{
 */

/*
 * \def NCSCTRL_MLEDIT
 * \brief the name of mledit control
*/
#define NCSCTRL_MLEDIT   NCSCLASSNAME("mledit")

typedef struct _mMlEdit         mMlEdit;
typedef struct _mMlEditClass    mMlEditClass;
typedef struct _mMlEditRenderer mMlEditRenderer;


#define mMlEditHeader(Class)        \
        mEditHeader(Class)          \
        TextDoc   txtDoc;           \
        StrBuffer teBuff;           \
        int       nrDiffLine;       \
        int       wNoChanged;       \
        int       wLastLine;        \
        int       hardLimit;        \
        int       curLen;           \
        int       nLineHeight;      \
        int       nLineAboveH;      \
        int       nLineBaseH;       \
        unsigned char lnChar;       \
        unsigned char caretShape;   \
        char      *title;           \
        int       titleIndent;      \
        DWORD     flags;            \
        DWORD     exFlags;          \
        BOOL      contDirty;        \
        int       desCaretX;        \
        int       caretX;           \
        int       caretY;           \
        int       selX;             \
        int       selY;             \
        TextNode  *maxNode;         \
        int       maxLen;           \
        TextNode  *secNode;         \
        int       secLen;           \
        int       curItemY;         \
        int       selItemY;         \
        TextCopyPaste *cp;

/** 
 * \struct mMlEdit
 * \brief the multiline edit control, derived from \ref mEdit.
 *
 *   - \b txtDoc \n
 *      text document object.
 *   - \b teBuff \n
 *      tmp text content.
 *   - \b nrDiffLine \n
 *      first different line.
 *   - \b wNoChanged \n
 *      width not changed of first different line.
 *   - \b wLastLine \n  
 *      width of last line. 
 *   - \b hardLimit \n 
 *      hard limit.
 *   - \b curLen \n 
 *      current tedata's len, does not include char '\\n'.
 *   - \b nLineHeight \n 
 *      line height.
 *   - \b nLineAboveH \n 
 *      height of above-line area.
 *   - \b nLineBaseH \n 
 *      height of base-line area.
 *   - \b lnChar \n
 *      the char used to represent line seperator.
 *   - \b caretShape \n 
 *      shape of the caret.
 *   - \b title \n 
 *      title text displayed before content text.
 *   - \b titleIndent \n 
 *      title indent.
 *   - \b flags \n 
 *      editor status.
 *   - \b exFlags \n 
 *      editor extends status.
 *   - \b contDirty \n 
 *      content changed flags in text field.
 *   - \b desCaretX \n
 *      the desired caret x position, changed by mouse and <- , ->
 *   - \b caretX \n 
 *      caret x position in the virtual content window.
 *   - \b caretY \n  
 *      caret y position in the virtual content window.
 *   - \b selX \n 
 *      selection point x position in the virtual content window.
 *   - \b selY \n 
 *      selection point y position in the virtual content window.
 *   - \b maxNode \n 
 *      node with the max length.
 *   - \b maxLen \n  
 *      max line length.
 *   - \b secNode \n  
 *      node with the second max length.
 *   - \b secLen \n 
 *      seconde max line length
 *   - \b curItemY \n  
 *      y position of the current insertion node.
 *   - \b selItemY \n
 *      y position of the selection node.
 *   - \b cp \n
 *      Copy & Paste operators
 * 
 *  \sa mMlEditClass
 */
struct _mMlEdit
{
    mMlEditHeader(mMlEdit)
};

#define mMlEditClassHeader(clsName, parentClass)                   \
    mEditClassHeader (clsName, parentClass)                        \
    int (*setTitle)(clsName *self, const char *title, int len);    \
    int (*getTitle)(clsName *self, char *buffer, int len); 

/** 
 * \struct mMlEditClass
 * \brief the VTable of \a mMlEdit, derived from \ref mEditClass.
 * 
 * - int (* \b setTitle)(mMlEdit* self, const char* title, int len); \n
 *   Set the title of mMlEdit
 *   \param title - the string of title
 *   \param len - the length of title
 *   \return length has been set or -1 if faield
 *
 * - int (* \b getTitle)(mMlEdit* self, char* buffer, int len); \n
 *   Get the title text
 *   \param buffer - buffer to recieve the titile string
 *   \param len - the length of buffer
 *   \return the length of string saved in buffer
 *
 * \sa mEditClass
 */
struct _mMlEditClass
{
    mMlEditClassHeader(mMlEdit, mEdit)
};

/**
 * \var g_stmMlEditCls
 * \brief the instance of mMlEditClass
 *
 * \sa mMlEditClass
 */
MGNCS_EXPORT extern mMlEditClass g_stmMlEditCls;


#define mMlEditRendererHeader(clsName, parentClass) \
	mEditRendererHeader(clsName, parentClass)       \

/**
 * \struct mMlEditRenderer
 * \brief MlEdit class's Renderer interface, derived from \ref mEditRenderer.
 *
 * \sa mMlEdit, mMlEditClass
 */
struct _mMlEditRenderer
{
	mMlEditRendererHeader(mMlEdit, mEdit)
};

/**
 * \def NCSS_MLEDIT_AUTOWRAP
 * \brief Automatically wraps against border when inputting.
 */
#define NCSS_MLEDIT_AUTOWRAP   (0x0001<<NCSS_EDIT_SHIFT)

/**
 * \def NCSS_MLEDIT_TITLE
 * \brief Shows specified title texts
 */
#define NCSS_MLEDIT_TITLE      (0x0002<<NCSS_EDIT_SHIFT)

#define NCSS_MLEDIT_SHIFT  (NCSS_EDIT_SHIFT+2)

/** MlEdit Property define :*/
enum mMlEditProp 
{
    NCSP_MLEDIT_LINECOUNT = NCSP_EDIT_MAX + 1,   /**< Count of line, Type: int, Read-Only*/
    NCSP_MLEDIT_LINEHEIGHT,                    /**< Height of line, Type: int */
    NCSP_MLEDIT_LINEFEEDISPCHAR,               /**< Line feed display charater, Type : unsigned char */
    NCSP_MLEDIT_LINESEP,                       /**< Line separater, Type : unsigned char */
    NCSP_MLEDIT_CARETSHAPE,                    /**< Caret shape, Type : \ref ncsCaretShape */
    NCSP_MLEDIT_NUMOFPARAGRAPHS,               /**< Count of paragraphs Type : int , Read-Only*/
    NCSP_MLEDIT_MAX
};

enum mMlEditNotify
{
    NCSN_MLEDIT_MAX = NCSN_EDIT_MAX + 1,
};

/** Caret Shapes define: */
enum ncsCaretShape
{
    NCS_CARETSHAPE_LINE = 0, /**< Line-shaped caret */
    NCS_CARETSHAPE_BLOCK     /**< Block-shaped caret */
};


/* internal status flags */
#define NCSF_MLEDIT_SELECT         0x0001
#define NCSF_MLEDIT_FOCUSED        0x0002
#define NCSF_MLEDIT_TMP            0x0004
#define NCSF_MLEDIT_REPLACE        0x0008
#define NCSF_MLEDIT_MOVE           0x0010
#define NCSF_MLEDIT_NOCARET        0x0020
#define NCSF_MLEDIT_EX_SETFOCUS    0x0040
#define NCSF_MLEDIT_EX_KILLFOCUS   0x0080

    /** @} end of Control_MlEdit */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_MLEDIT_H */
