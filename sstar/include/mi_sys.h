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
#ifndef _MI_SYS_H_
#define _MI_SYS_H_

#include "mi_common.h"
#include "mi_sys_datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

MI_S32 MI_SYS_Init(void);

MI_S32 MI_SYS_Exit (void);

MI_S32 MI_SYS_BindChnPort(MI_SYS_ChnPort_t *pstSrcChnPort, MI_SYS_ChnPort_t *pstDstChnPort , MI_U32 u32SrcFrmrate,  MI_U32 u32DstFrmrate);

MI_S32 MI_SYS_UnBindChnPort(MI_SYS_ChnPort_t *pstSrcChnPort, MI_SYS_ChnPort_t *pstDstChnPort);

MI_S32 MI_SYS_GetBindbyDest (MI_SYS_ChnPort_t *pstDstChnPort, MI_SYS_ChnPort_t *pstSrcChnPort);

MI_S32 MI_SYS_GetVersion (MI_SYS_Version_t *pstVersion);

MI_S32 MI_SYS_GetCurPts (MI_U64 *pu64Pts);

MI_S32 MI_SYS_InitPtsBase (MI_U64 u64PtsBase);

MI_S32 MI_SYS_SyncPts (MI_U64 u64Pts);

MI_S32 MI_SYS_SetReg (MI_U32 u32RegAddr, MI_U16 u16Value, MI_U16 u16Mask);

MI_S32 MI_SYS_GetReg (MI_U32 u32RegAddr, MI_U16 *pu16Value);

MI_S32 MI_SYS_SetChnMMAConf (MI_ModuleId_e eModId, MI_U32 u32DevId, MI_U32 u32ChnId, MI_U8 *pu8MMAHeapName);

MI_S32 MI_SYS_GetChnMMAConf (MI_ModuleId_e eModId,  MI_U32 u32DevId, MI_U32 u32ChnId, void  *data, MI_U32 u32Length);

MI_S32 MI_SYS_ChnInputPortGetBuf (MI_SYS_ChnPort_t *pstChnPort ,MI_SYS_BufConf_t *pstBufConf, MI_SYS_BufInfo_t *pstBufInfo , MI_SYS_BUF_HANDLE *bufHandle , MI_S32 s32TimeOutMs);

MI_S32 MI_SYS_ChnInputPortPutBuf (MI_SYS_BUF_HANDLE bufHandle  ,MI_SYS_BufInfo_t *pstBufInfo , MI_BOOL bDropBuf);

MI_S32 MI_SYS_ChnOutputPortGetBuf (MI_SYS_ChnPort_t *pstChnPort,MI_SYS_BufInfo_t *pstBufInfo, MI_SYS_BUF_HANDLE *bufHandle);

MI_S32 MI_SYS_ChnOutputPortPutBuf (MI_SYS_BUF_HANDLE hBufHandle);

MI_S32 MI_SYS_SetChnOutputPortDepth(MI_SYS_ChnPort_t *pstChnPort , MI_U32 u32UserFrameDepth , MI_U32 u32BufQueueDepth);

MI_S32 MI_SYS_ChnPortInjectBuf (MI_SYS_BUF_HANDLE handle ,MI_SYS_ChnPort_t *pstChnInputPort);

MI_S32 MI_SYS_GetFd(MI_SYS_ChnPort_t *pstChnPort , MI_S32 *s32Fd);

MI_S32 MI_SYS_CloseFd(MI_S32 s32ChnPortFd);

MI_S32 MI_SYS_ConfDevPubPools(MI_ModuleId_e  eModule, MI_U32 u32DevId, MI_VB_PoolListConf_t  stPoolListConf);

MI_S32 MI_SYS_ReleaseDevPubPools(MI_ModuleId_e  eModule, MI_U32 u32DevId);

MI_S32 MI_SYS_ConfGloPubPools(MI_VB_PoolListConf_t  stPoolListConf);

MI_S32 MI_SYS_ReleaseGloPubPools(void);

MI_S32 MI_SYS_Set_Vdec_UseVbPool_Flag(MI_U32 u32Value);
/*
N.B.
below MMAHeapName can only be NULL or real mma heap name, do not set it with random character string.
you can get mma heap name xxx from "mma_heap=xxx," of cat /proc/cmdline.
*/
MI_S32 MI_SYS_MMA_Alloc(MI_U8 *pstMMAHeapName, MI_U32 u32BlkSize ,MI_PHY *phyAddr);

MI_S32 MI_SYS_MMA_Free(MI_PHY  phyAddr);
MI_S32 MI_SYS_Mmap(MI_U64 phyAddr, MI_U32 u32Size , void **ppVirtualAddress , MI_BOOL bCache);

MI_S32 MI_SYS_Munmap(void *pVirtualAddress, MI_U32 u32Size);
MI_S32 MI_SYS_FlushInvCache(void *pVirtualAddress, MI_U32 u32Length);

#ifndef __KERNEL__
#define DEBUG_YUV_USER_API
#ifdef DEBUG_YUV_USER_API
typedef  FILE* FILE_HANDLE;
//return value of the following API is not standard file operation return value.

/*
return :
FILE_HANDLE type in fact is a pointer,
NULL:fail
not NULL:success
*/
FILE_HANDLE open_yuv_file(const char *pathname, int open_for_write);

/*
return value:
0:success
-1:fail
*/
int read_yuv_file(FILE_HANDLE filehandle, MI_SYS_FrameData_t framedata);

/*
return value:
0:success
-1:fail
*/
int write_yuv_file(FILE_HANDLE filehandle, MI_SYS_FrameData_t framedata);

void close_yuv_file(FILE_HANDLE filehandle);

/*
return value:
0:success
-1:fail
*/
int reset_yuv_file(FILE_HANDLE filehandle);

/*
return value:
0:success
-1:fail
*/
int is_in_yuv_file_end(FILE_HANDLE filehandle);
#endif
#else
/*
do nothing,kernel space debug API is in mi_sys_internal.h/mi_sys_internal.c
*/
#endif

#ifdef __cplusplus
}
#endif

#endif ///_MI_SYS_H_
