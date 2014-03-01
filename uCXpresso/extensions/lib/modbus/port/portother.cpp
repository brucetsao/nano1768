/*
 ===============================================================================
 Name        : portother.cpp
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2013/1/4
 License     : CC BY-SA 3.0
 Description : modbus_demo
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/1/4	v1.0.0	First Edition.									Jason
*/
#include "debug.h"

#include <stdio.h>
#include "port.h"
#include "mb.h"
#include "mbport.h"
#include "mbconfig.h"

//
// Defines
//
#define MB_FRAME_LOG_BUFSIZE    512
static const char *arszLevel2Str[] = { "DEBUG", "INFO", "WARN", "ERROR" };
static eMBPortLogLevel eLevelMax = MB_LOG_DEBUG;

//
// Start implementation
//
PR_BEGIN_EXTERN_C

void vMBPortLogLevel( eMBPortLogLevel eNewLevelMax ) {
    eLevelMax = eNewLevelMax;
}

#ifdef MB_TCP_DEBUG

void vMBPortLog( eMBPortLogLevel eLevel, const CHAR * szModule, const CHAR * szFmt, ... ) {
    va_list args;

    dbg.printf( "%s: %s: ", arszLevel2Str[eLevel], szModule);

    va_start( args, szFmt );
    dbg.printf(szFmt, args );
    va_end( args );
}


void
prvvMBTCPLogFrame( const CHAR * pucMsg, const UCHAR * pucFrame, USHORT usFrameLen )
{
    int             i;
    int             res = -1;
    int             iBufPos = 0;
    size_t          iBufLeft = MB_FRAME_LOG_BUFSIZE;
    CHAR     		*arcBuffer = new CHAR[MB_FRAME_LOG_BUFSIZE];

    assert( pucFrame != NULL );

    for( i = 0; i < usFrameLen; i++ )
    {
        /* Print some additional frame information. */
        switch ( i )
        {
        case 0:
            /* TID = Transaction Identifier. */
            res = snprintf( &arcBuffer[iBufPos], iBufLeft, "| TID = " );
            break;
        case 2:
            /* PID = Protocol Identifier. */
            res = snprintf( &arcBuffer[iBufPos], iBufLeft, " | PID = " );
            break;
        case 4:
            /* Length */
            res = snprintf( &arcBuffer[iBufPos], iBufLeft, " | LEN = " );
            break;
        case 6:
            /* UID = Unit Identifier. */
            res = snprintf( &arcBuffer[iBufPos], iBufLeft, " | UID = " );
            break;
        case 7:
            /* MB Function Code. */
            res = snprintf( &arcBuffer[iBufPos], iBufLeft, "|| FUNC = " );
            break;
        case 8:
            /* MB PDU rest. */
            res = snprintf( &arcBuffer[iBufPos], iBufLeft, " | DATA = " );
            break;
        default:
            res = 0;
            break;
        }
        if( res == -1 )
        {
            break;
        }
        else
        {
            iBufPos += res;
            iBufLeft -= res;
        }

        /* Print the data. */
        res = snprintf( &arcBuffer[iBufPos], iBufLeft, "%02X", pucFrame[i] );
        if( res == -1 )
        {
            break;
        }
        else
        {
            iBufPos += res;
            iBufLeft -= res;
        }
    }

    if( res != -1 )
    {
        /* Append an end of frame string. */
        res = snprintf( &arcBuffer[iBufPos], iBufLeft, " |\r\n" );
        if( res != -1 )
        {
            vMBPortLog( MB_LOG_DEBUG,  pucMsg, "%s", arcBuffer );
        }
    }

    delete arcBuffer;
}

#endif
PR_END_EXTERN_C
