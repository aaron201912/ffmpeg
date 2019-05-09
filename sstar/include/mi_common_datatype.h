//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file    mi_common_datatype.h
/// @brief The common interface definition
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MI_COMMON_DATATYPE_H_
#define _MI_COMMON_DATATYPE_H_

//-------------------------------------------------------------------------------------------------
//  System Data Type
//-------------------------------------------------------------------------------------------------
/// data type unsigned char, data length 1 byte
typedef unsigned char                            MI_U8;         // 1 byte
/// data type unsigned short, data length 2 byte
typedef unsigned short                           MI_U16;        // 2 bytes
/// data type unsigned int, data length 4 byte
typedef unsigned int                             MI_U32;        // 4 bytes
/// data type unsigned int, data length 8 byte
typedef unsigned long long                       MI_U64;        // 8 bytes
/// data type signed char, data length 1 byte
typedef signed char                              MI_S8;         // 1 byte
/// data type signed short, data length 2 byte
typedef signed short                             MI_S16;        // 2 bytes
/// data type signed int, data length 4 byte
typedef signed int                               MI_S32;        // 4 bytes
/// data type signed int, data length 8 byte
typedef signed long long                         MI_S64;        // 8 bytes
/// data type float, data length 4 byte
typedef float                                    MI_FLOAT;      // 4 bytes
/// data type 64bit physical address
typedef unsigned long long                       MI_PHY;        // 8 bytes
/// data type pointer content
typedef unsigned long                            MI_VIRT;       // 4 bytes when 32bit toolchain, 8 bytes when 64bit toolchain.

typedef unsigned char                            MI_BOOL;

//-------------------------------------------------------------------------------------------------
//  Moudle common  type
//-------------------------------------------------------------------------------------------------
typedef MI_S32 MI_AI_CHN;
typedef MI_S32 MI_AO_CHN;
typedef MI_S32 MI_AUDIO_DEV;
typedef MI_S32 MI_VIF_DEV;
typedef MI_S32 MI_DISP_DEV;
typedef MI_S32 MI_DISP_LAYER;
typedef MI_S32 MI_DISP_CHN;
typedef MI_S32 MI_VENC_CHN;
typedef MI_S32 MI_VDEC_CHN;
typedef MI_S32 MI_IVE_HANDLE;
typedef MI_S32 MI_VPE_DEV;
typedef MI_S32 MI_DIVP_CHN;
typedef MI_U32 MI_RGN_HANDLE;
typedef MI_S32  MI_HANDLE;
//-------------------------------------------------------------------------------------------------
//  Macros
//-------------------------------------------------------------------------------------------------
#define INIT_ST(st) do { \
    memset(&st, 0, sizeof(st));\
    st.u16SizeofStructure = sizeof(st);\
}while(0)



//-------------------------------------------------------------------------------------------------
//  Defines
//-------------------------------------------------------------------------------------------------
#define MI_HANDLE_NULL                           (0)
#define MI_SUCCESS    (0)
#define MI_BIT(_bit_)                    (1 << (_bit_))
#define MI_ERR_ID            (0x80000000L + 0x20000000L)
#define MI_IVE_INITIAL_ERROR_CODE       (0x80)
#define MI_VDF_INITIAL_ERROR_CODE      (0xA0)
#define MI_VENC_INITIAL_ERROR_CODE    (0xC0)
#define MI_RGN_INITIAL_ERROR_CODE      (0xE0)
#define MI_AI_INITIAL_ERROR_CODE         (0x100)
#define MI_AO_INITIAL_ERROR_CODE        (0x120)
#define MI_VIF_INITIAL_ERROR_CODE       (0x140)
#define MI_VPE_INITIAL_ERROR_CODE       (0x160)
#define MI_VDEC_INITIAL_ERROR_CODE    (0x180)
#define MI_SYS_INITIAL_ERROR_CODE       (0x1A0)
#define MI_FB_INITIAL_ERROR_CODE         (0x1C0)
#define MI_HDMI_INITIAL_ERROR_CODE     (0x1E0)
#define MI_DIVP_INITIAL_ERROR_CODE     (0x200)
#define MI_GFX_INITIAL_ERROR_CODE       (0x220)
#define MI_VDISP_INITIAL_ERROR_CODE   (0x240)
#define MI_DISP_INITIAL_ERROR_CODE     (0x260)
#define MI_WARP_INITIAL_ERROR_CODE     (0x280)
#define MI_CV_INITIAL_ERROR_CODE       (0x2A0)
#define MI_CEVA_INITIAL_ERROR_CODE     (0x2C0)
#define MI_UAC_INITIAL_ERROR_CODE      (0x2E0)
#define MI_LDC_INITIAL_ERROR_CODE     (0x300)


/******************************************************************************
|----------------------------------------------------------------|
| 1 |   APP_ID   |   MOD_ID    | ERR_LEVEL |   ERR_ID            |
|----------------------------------------------------------------|
|<--><--7bits----><----8bits---><--4bits---><------12bits------->|
******************************************************************************/

#define MI_DEF_ERR( module, level, errid) \
    ((MI_S32)( (MI_ERR_ID) | ((module) << 16 ) | ((level)<<12) | (errid) ))


#ifndef __KERNEL__
/// data type null pointer
#ifdef NULL
#undef NULL
#endif
#define NULL                                     0


#if !defined(__cplusplus)
#ifndef true
/// definition for true
#define true                                     1
/// definition for false
#define false                                    0
#endif
#endif

#if !defined(TRUE) && !defined(FALSE)
/// definition for TRUE
#define TRUE                                     1
/// definition for FALSE
#define FALSE                                    0
#endif
#endif


//-------------------------------------------------------------------------------------------------
//  Structures
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//	Enum
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_MI_MODULE_ID_IVE    = 0,
    E_MI_MODULE_ID_VDF      = 1,
    E_MI_MODULE_ID_VENC     = 2,
    E_MI_MODULE_ID_RGN     = 3,
    E_MI_MODULE_ID_AI    = 4,
    E_MI_MODULE_ID_AO    = 5,
    E_MI_MODULE_ID_VIF   = 6,
    E_MI_MODULE_ID_VPE    = 7,
    E_MI_MODULE_ID_VDEC    = 8,
    E_MI_MODULE_ID_SYS     = 9,
    E_MI_MODULE_ID_FB   = 10,
    E_MI_MODULE_ID_HDMI  = 11,
    E_MI_MODULE_ID_DIVP  = 12,
    E_MI_MODULE_ID_GFX   = 13,
    E_MI_MODULE_ID_VDISP   = 14,
    E_MI_MODULE_ID_DISP     = 15,
    E_MI_MODULE_ID_OS     = 16,
    E_MI_MODULE_ID_IAE = 17,
    E_MI_MODULE_ID_MD = 18,
    E_MI_MODULE_ID_OD = 19,
    E_MI_MODULE_ID_SHADOW = 20,
    E_MI_MODULE_ID_WARP = 21,
	E_MI_MODULE_ID_UAC = 22,
	E_MI_MODULE_ID_LDC = 23,
	E_MI_MODULE_ID_SD = 24,
    E_MI_MODULE_ID_PANEL = 25,
	E_MI_MODULE_ID_SNR = 26,
    E_MI_MODULE_ID_MAX,
} MI_ModuleId_e;

typedef enum
{
    E_MI_ERR_LEVEL_INFO,       /* informational                                */
    E_MI_ERR_LEVEL_WARNING,    /* warning conditions                           */
    E_MI_ERR_LEVEL_ERROR,      /* error conditions                             */
    E_MI_ERR_LEVEL_BUTT
}MI_ErrLevel_e;

typedef enum
{
    E_MI_ERR_INVALID_DEVID = 1, /* invlalid device ID                           */
    E_MI_ERR_INVALID_CHNID = 2, /* invlalid channel ID                          */
    E_MI_ERR_ILLEGAL_PARAM = 3, /* at lease one parameter is illagal
                               * eg, an illegal enumeration value             */
    E_MI_ERR_EXIST         = 4, /* resource exists                              */
    E_MI_ERR_UNEXIST       = 5, /* resource unexists                            */
    E_MI_ERR_NULL_PTR      = 6, /* using a NULL point                           */
    E_MI_ERR_NOT_CONFIG    = 7, /* try to enable or initialize system, device
                              ** or channel, before configing attribute       */
    E_MI_ERR_NOT_SUPPORT   = 8, /* operation or type is not supported by NOW    */
    E_MI_ERR_NOT_PERM      = 9, /* operation is not permitted
                              ** eg, try to change static attribute           */
    E_MI_ERR_NOMEM         = 12,/* failure caused by malloc memory              */
    E_MI_ERR_NOBUF         = 13,/* failure caused by malloc buffer              */
    E_MI_ERR_BUF_EMPTY     = 14,/* no data in buffer                            */
    E_MI_ERR_BUF_FULL      = 15,/* no buffer for new data                       */
    E_MI_ERR_SYS_NOTREADY  = 16,/* System is not ready,maybe not initialed or
                              ** loaded. Returning the error code when opening
                              ** a device file failed.                        */
    E_MI_ERR_BADADDR       = 17,/* bad address,
                              ** eg. used for copy_from_user & copy_to_user   */
    E_MI_ERR_BUSY          = 18,/* resource is busy,
                              ** eg. destroy a venc chn without unregister it */
    E_MI_ERR_CHN_NOT_STARTED          = 19,/* channel not start*/
    E_MI_ERR_CHN_NOT_STOPED          = 20,/* channel not stop*/
    E_MI_ERR_NOT_INIT          = 21,/* module not init before use it*/
    E_MI_ERR_INITED          = 22,/* module already init*/
    E_MI_ERR_NOT_ENABLE          = 23,/* device  channel or port  not  enable*/
    E_MI_ERR_NOT_DISABLE          = 24,/* device  channel or port  not  disable*/
    E_MI_ERR_SYS_TIMEOUT          = 25,/* sys timeout*/
    E_MI_ERR_DEV_NOT_STARTED          = 26,/* device  not started*/
    E_MI_ERR_DEV_NOT_STOPED          = 27,/* device  not stoped*/
    E_MI_ERR_CHN_NO_CONTENT          = 28,/*there is no content in the channel.*/
    E_MI_ERR_FAILED          ,/* unexpected error */

    E_MI_ERR_MAX          = 127,/* maxium code, private error code of all modules
                              ** must be greater than it                      */
}MI_ErrCode_e;


#endif///_MI_COMMON_DATATYPE_H_
