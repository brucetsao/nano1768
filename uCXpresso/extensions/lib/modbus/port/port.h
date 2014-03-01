/*
 * FreeModbus Libary: MCF5235 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 * Parts of crt0.S Copyright (c) 1995, 1996, 1998 Cygnus Support
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: port.h,v 1.2 2006/09/04 14:39:20 wolti Exp $
 *
 * 2013/1/5, Modify for uCXpresso
 */

#ifndef _PORT_H
#define _PORT_H

/* ----------------------- Platform includes --------------------------------*/
#include "uCXpresso.h"

/* ----------------------- Defines ------------------------------------------*/
#undef INLINE
#define INLINE                  inline

#define assert( x )             //LWIP_ASSERT( #x, x );
#define PR_BEGIN_EXTERN_C       extern "C" {
#define PR_END_EXTERN_C         }

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

#ifdef DEBUG
#define MB_TCP_DEBUG            1       /* Debug output in TCP module. */

#endif // DEBUG

/* ----------------------- Type definitions ---------------------------------*/
#ifdef MB_TCP_DEBUG
typedef enum {
	MB_LOG_DEBUG = 0,
	MB_LOG_INFO,
	MB_LOG_WARN,
	MB_LOG_ERROR
} eMBPortLogLevel;

/* ----------------------- Function prototypes ------------------------------*/

void vMBPortLog(eMBPortLogLevel eLevel, const CHAR * szModule,
		const CHAR * szFmt, ...);
void prvvMBTCPLogFrame(const CHAR * pucMsg, const UCHAR * pucFrame,
		USHORT usFrameLen);
#endif // MB_TCP_DEBUG

//
// freeRTOS
//
extern void vPortEnterCritical();
extern void vPortExitCritical();

#define ENTER_CRITICAL_SECTION() 	vPortEnterCritical()
#define EXIT_CRITICAL_SECTION()		vPortExitCritical()

#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
