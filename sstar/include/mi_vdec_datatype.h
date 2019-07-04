//<Sigmastar Software>
//******************************************************************************
// Sigmastar Software
// Copyright (c) 2010 - 2012 Sigmastar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("Sigmastar Software") are
// intellectual property of Sigmastar Semiconductor, Inc. ("Sigmastar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of Sigmastar Software is expressly prohibited, unless prior written
// permission has been granted by Sigmastar.
//
// By accessing, browsing and/or using Sigmastar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. Sigmastar shall retain any and all right, ownership and interest to Sigmastar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to Sigmastar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that Sigmastar Software might include, incorporate or be
//    supplied together with third party`s software and the use of Sigmastar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. Sigmastar Software and any modification/derivatives thereof shall be deemed as
//    Sigmastar`s confidential information and you agree to keep Sigmastar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. Sigmastar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by Sigmastar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against Sigmastar for any loss, damage, cost or expense that you may
//    incur related to your use of Sigmastar Software.
//    In no event shall Sigmastar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if Sigmastar Software has been modified by Sigmastar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, Sigmastar may from time to time provide technical supports or
//    services in relation with Sigmastar Software to you for your use of
//    Sigmastar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use Sigmastar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating Sigmastar or any of its affiliates to furnish any person,
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
//<Sigmastar Software>

#ifndef _MI_VDEC_DATATYPE_H_
#define _MI_VDEC_DATATYPE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "mi_common.h"

#define MI_VDEC_CHN MI_U32

#define MI_DEF_VDEC_ERR(err) MI_DEF_ERR(E_MI_MODULE_ID_VDEC, E_MI_ERR_LEVEL_ERROR, err)

#define MI_ERR_VDEC_INVALID_DEVID           MI_DEF_VDEC_ERR(E_MI_ERR_INVALID_DEVID)
#define MI_ERR_VDEC_INVALID_CHNID           MI_DEF_VDEC_ERR(E_MI_ERR_INVALID_CHNID)
#define MI_ERR_VDEC_ILLEGAL_PARAM           MI_DEF_VDEC_ERR(E_MI_ERR_ILLEGAL_PARAM)
#define MI_ERR_VDEC_CHN_EXIST               MI_DEF_VDEC_ERR(E_MI_ERR_EXIST)
#define MI_ERR_VDEC_CHN_UNEXIST             MI_DEF_VDEC_ERR(E_MI_ERR_UNEXIST)
#define MI_ERR_VDEC_NULL_PTR                MI_DEF_VDEC_ERR(E_MI_ERR_NULL_PTR)
#define MI_ERR_VDEC_NOT_CONFIG              MI_DEF_VDEC_ERR(E_MI_ERR_NOT_CONFIG)
#define MI_ERR_VDEC_NOT_SUPPORT             MI_DEF_VDEC_ERR(E_MI_ERR_NOT_SUPPORT)
#define MI_ERR_VDEC_NOT_PERM                MI_DEF_VDEC_ERR(E_MI_ERR_NOT_PERM)
#define MI_ERR_VDEC_NOMEM                   MI_DEF_VDEC_ERR(E_MI_ERR_NOMEM)
#define MI_ERR_VDEC_NOBUF                   MI_DEF_VDEC_ERR(E_MI_ERR_NOBUF)
#define MI_ERR_VDEC_BUF_EMPTY               MI_DEF_VDEC_ERR(E_MI_ERR_BUF_EMPTY)
#define MI_ERR_VDEC_BUF_FULL                MI_DEF_VDEC_ERR(E_MI_ERR_BUF_FULL)
#define MI_ERR_VDEC_SYS_NOTREADY            MI_DEF_VDEC_ERR(E_MI_ERR_SYS_NOTREADY)
#define MI_ERR_VDEC_BADADDR                 MI_DEF_VDEC_ERR(E_MI_ERR_BADADDR)
#define MI_ERR_VDEC_BUSY                    MI_DEF_VDEC_ERR(E_MI_ERR_BUSY)
#define MI_ERR_VDEC_CHN_NOT_START           MI_DEF_VDEC_ERR(E_MI_ERR_CHN_NOT_STARTED)
#define MI_ERR_VDEC_CHN_NOT_STOP            MI_DEF_VDEC_ERR(E_MI_ERR_CHN_NOT_STOPED)
#define MI_ERR_VDEC_NOT_INIT                MI_DEF_VDEC_ERR(E_MI_ERR_NOT_INIT)
#define MI_ERR_VDEC_INITED                  MI_DEF_VDEC_ERR(E_MI_ERR_INITED)
#define MI_ERR_VDEC_NOT_ENABLE              MI_DEF_VDEC_ERR(E_MI_ERR_NOT_ENABLE)
#define MI_ERR_VDEC_NOT_DISABLE             MI_DEF_VDEC_ERR(E_MI_ERR_NOT_DISABLE)
#define MI_ERR_VDEC_SYS_TIMEOUT             MI_DEF_VDEC_ERR(E_MI_ERR_SYS_TIMEOUT)
#define MI_ERR_VDEC_NOT_STARTED             MI_DEF_VDEC_ERR(E_MI_ERR_DEV_NOT_STARTED)
#define MI_ERR_VDEC_NOT_STOPED              MI_DEF_VDEC_ERR(E_MI_ERR_DEV_NOT_STOPED)
#define MI_ERR_VDEC_CHN_NO_CONTENT          MI_DEF_VDEC_ERR(E_MI_ERR_CHN_NO_CONTENT)
#define MI_ERR_VDEC_FAILED                  MI_DEF_VDEC_ERR(E_MI_ERR_FAILED)


typedef enum
{
    E_MI_VDEC_CODEC_TYPE_H264 = 0x0,
    E_MI_VDEC_CODEC_TYPE_H265,
    E_MI_VDEC_CODEC_TYPE_JPEG,
    E_MI_VDEC_CODEC_TYPE_MAX
} MI_VDEC_CodecType_e;

typedef enum
{
    E_MI_VDEC_JPEG_FORMAT_YCBCR400 = 0x0,
    E_MI_VDEC_JPEG_FORMAT_YCBCR420,
    E_MI_VDEC_JPEG_FORMAT_YCBCR422,
    E_MI_VDEC_JPEG_FORMAT_YCBCR444,
    E_MI_VDEC_JPEG_FORMAT_MAX
} MI_VDEC_JpegFormat_e;

typedef enum
{
    E_MI_VDEC_VIDEO_MODE_STREAM = 0x0,
    E_MI_VDEC_VIDEO_MODE_FRAME,
    E_MI_VDEC_VIDEO_MODE_MAX
} MI_VDEC_VideoMode_e;

typedef enum
{
    E_MI_VDEC_ERR_CODE_UNKNOW = 0x0,
    E_MI_VDEC_ERR_CODE_ILLEGAL_ACCESS,
    E_MI_VDEC_ERR_CODE_FRMRATE_UNSUPPORT,
    E_MI_VDEC_ERR_CODE_DEC_TIMEOUT,
    E_MI_VDEC_ERR_CODE_OUT_OF_MEMORY,
    E_MI_VDEC_ERR_CODE_CODEC_TYPE_UNSUPPORT,
    E_MI_VDEC_ERR_CODE_ERR_SPS_UNSUPPORT,
    E_MI_VDEC_ERR_CODE_ERR_PPS_UNSUPPORT,
    E_MI_VDEC_ERR_CODE_REF_LIST_ERR,
    E_MI_VDEC_ERR_CODE_MAX
} MI_VDEC_ErrCode_e;

typedef enum
{
    E_MI_VDEC_DECODE_MODE_ALL = 0x0,
    E_MI_VDEC_DECODE_MODE_I,
    E_MI_VDEC_DECODE_MODE_IP,
    E_MI_VDEC_DECODE_MODE_MAX
} MI_VDEC_DecodeMode_e;

typedef enum
{
    E_MI_VDEC_OUTPUT_ORDER_DISPLAY = 0x0,
    E_MI_VDEC_OUTPUT_ORDER_DECODE,
    E_MI_VDEC_OUTPUT_ORDER_MAX,
} MI_VDEC_OutputOrder_e;

typedef enum
{
    E_MI_VDEC_VIDEO_FORMAT_TILE = 0x0,
    E_MI_VDEC_VIDEO_FORMAT_REDUCE,
    E_MI_VDEC_VIDEO_FORMAT_MAX
} MI_VDEC_VideoFormat_e;

typedef enum
{
    E_MI_VDEC_DISPLAY_MODE_PREVIEW = 0x0,
    E_MI_VDEC_DISPLAY_MODE_PLAYBACK,
    E_MI_VDEC_DISPLAY_MODE_MAX,
} MI_VDEC_DisplayMode_e;

typedef enum
{
    E_MI_VDEC_TILEMODE_OFF = 0x0,
    E_MI_VDEC_TILEMODE_ON,
    E_MI_VDEC_TILEMODE_MAX,
}MI_VDEC_TileMode_Status_e;

typedef enum
{
    E_MI_VDEC_INPLACE_MODE_OFF = 0x0,
    E_MI_VDEC_INPLACE_MODE_SHORTTERM,
    E_MI_VDEC_INPLACE_MODE_LONGTERM,
    E_MI_VDEC_INPLACE_MODE_MAX
}MI_VDEC_InplaceMode_e;

typedef struct MI_VDEC_OutputPortAttr_s
{
    MI_U16 u16Width;                         // Width of target image
    MI_U16 u16Height;                        // Height of target image
}MI_VDEC_OutputPortAttr_t;

typedef struct MI_VDEC_JpegAttr_s
{
    MI_VDEC_JpegFormat_e    eJpegFormat;
}MI_VDEC_JpegAttr_t;

typedef struct MI_VDEC_VideoAttr_s
{
    MI_U32  u32RefFrameNum;
}MI_VDEC_VideoAttr_t;


typedef struct MI_VDEC_ChnAttr_s
{
    MI_VDEC_CodecType_e eCodecType;
    MI_U32 u32BufSize;
    MI_U32 u32Priority;
    MI_U32 u32PicWidth;
    MI_U32 u32PicHeight;
    MI_VDEC_VideoMode_e eVideoMode;
    MI_VDEC_InplaceMode_e eInplaceMode;
    union
    {
        MI_VDEC_JpegAttr_t stVdecJpegAttr;
        MI_VDEC_VideoAttr_t stVdecVideoAttr;
    };
} MI_VDEC_ChnAttr_t;

typedef struct MI_VDEC_ChnStat_s
{
    MI_VDEC_CodecType_e eCodecType;
    MI_U32 u32LeftStreamBytes;
    MI_U32 u32LeftStreamFrames;
    MI_U32 u32LeftPics;
    MI_U32 u32RecvStreamFrames;
    MI_U32 u32DecodeStreamFrames;
    MI_BOOL bChnStart;
    MI_VDEC_ErrCode_e eErrCode;
} MI_VDEC_ChnStat_t;

typedef struct MI_VDEC_ChnParam_s
{
    MI_VDEC_DecodeMode_e eDecMode;
    MI_VDEC_OutputOrder_e eOutputOrder;
    MI_VDEC_VideoFormat_e eVideoFormat;
} MI_VDEC_ChnParam_t;

typedef struct MI_VDEC_VideoStream_s
{
    MI_U8 *pu8Addr;
    MI_U32 u32Len;
    MI_U64 u64PTS;
    MI_BOOL bEndOfFrame;
    MI_BOOL bEndOfStream;
}MI_VDEC_VideoStream_t;

typedef struct MI_VDEC_UserData_s
{
    MI_U8 *pu8Addr;
    MI_U32 u32Len;
    MI_BOOL bValid;
} MI_VDEC_UserData_t;

typedef struct MI_VDEC_BufConf_s
{
    MI_U64 u64TargetPts;
    MI_U32 u32BufSize;
    MI_U8 *pu8UsrAddr;
} MI_VDEC_BufConf_t;

typedef struct MI_VDEC_BufInfo_s
{
    MI_U8 *pu8Addr;         // kernl vir addr.
    MI_PHY phyAddr;
    MI_U32 u32BufSize;
    MI_U64 u64Pts;
    MI_BOOL bEndOfStream;

    MI_BOOL bPictureStart;
    MI_BOOL bBrokenByUs;
} MI_VDEC_BufInfo_t;

#ifdef __cplusplus
}
#endif

#endif///_MI_VDEC_DATATYPE_H_
