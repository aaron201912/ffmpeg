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

#ifndef _MI_VDEC_H_
#define _MI_VDEC_H_

#define MI_VDEC_GET_ESBUFFER_DIRECT

#include "mi_vdec_datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

MI_S32 MI_VDEC_CreateChn(MI_VDEC_CHN VdecChn, MI_VDEC_ChnAttr_t *pstChnAttr);
MI_S32 MI_VDEC_DestroyChn(MI_VDEC_CHN VdecChn);
MI_S32 MI_VDEC_GetChnAttr(MI_VDEC_CHN VdecChn, MI_VDEC_ChnAttr_t *pstChnAttr);
MI_S32 MI_VDEC_StartChn(MI_VDEC_CHN VdecChn);
MI_S32 MI_VDEC_StopChn(MI_VDEC_CHN VdecChn);
MI_S32 MI_VDEC_GetChnStat(MI_VDEC_CHN VdecChn, MI_VDEC_ChnStat_t *pstChnStat);
MI_S32 MI_VDEC_ResetChn(MI_VDEC_CHN VdecChn);
MI_S32 MI_VDEC_SetChnParam(MI_VDEC_CHN VdecChn, MI_VDEC_ChnParam_t *pstChnParam);
MI_S32 MI_VDEC_GetChnParam(MI_VDEC_CHN VdecChn, MI_VDEC_ChnParam_t *pstChnParam);
MI_S32 MI_VDEC_SendStream(MI_VDEC_CHN VdecChn, MI_VDEC_VideoStream_t *pstVideoStream, MI_S32 s32MilliSec);
MI_S32 MI_VDEC_GetUserData(MI_VDEC_CHN VdecChn, MI_VDEC_UserData_t *pstUserData, MI_S32 s32MilliSec);
MI_S32 MI_VDEC_ReleaseUserData(MI_VDEC_CHN VdecChn, MI_VDEC_UserData_t *pstUserData);
MI_S32 MI_VDEC_SetDisplayMode(MI_VDEC_CHN VdecChn, MI_VDEC_DisplayMode_e eDisplayMode);
MI_S32 MI_VDEC_GetDisplayMode(MI_VDEC_CHN VdecChn, MI_VDEC_DisplayMode_e *peDisplayMode);
MI_S32 MI_VDEC_SetOutputPortAttr(MI_VDEC_CHN VdecChn, MI_VDEC_OutputPortAttr_t *pstOutputPortAttr);
MI_S32 MI_VDEC_GetOutputPortAttr(MI_VDEC_CHN VdecChn, MI_VDEC_OutputPortAttr_t *pstOutputPortAttr);


#ifdef __cplusplus
}
#endif

#endif///_MI_VDEC_H_
