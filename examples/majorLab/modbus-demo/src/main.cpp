/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2013/1/4
 Copyright   : embeda international inc.
 License	 : CC BY-SA 3.0
 Description : MODBUS-TCP demo
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/1/4 	v1.0.0 b1	Beta 1											Jason
 ===============================================================================
 */
#include "uCXpresso.h"
#include "arduino.h"
#include "class/pin.h"
#include "debug.h"

CDebug dbg(DBG_USB);

/* ==============================================
 user adjustable pool memory
 ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

//
// TODO: insert other include files here
//
#include "mb.h"
#include "mbconfig.h"
#include "class/timeout.h"

//
// TODO: insert other definitions and declarations here
//
#define REG_INPUT_START 		1000
#define REG_INPUT_NREGS 		4
#define REG_HOLDING_START       2000
#define REG_HOLDING_NREGS       130

#define SLAVE_ID 				0x0A

//
// Static variables
//
static USHORT usRegInputStart = REG_INPUT_START;
static USHORT usRegInputBuf[REG_INPUT_NREGS];
static USHORT usRegHoldingStart = REG_HOLDING_START;
static USHORT usRegHoldingBuf[REG_HOLDING_NREGS];

/* ==============================================
 main task routine
 ============================================== */
int main(void) {
	pool_memadd((uint32_t) pool, sizeof(pool));

#ifdef DEBUG
	dbg.start();
#endif

	eMBErrorCode eStatus;
//	dbg.waitToDebugMode();

#if MB_TCP_ENABLED == 1
	eStatus = eMBTCPInit( MB_TCP_PORT_USE_DEFAULT );
#endif

	if (eStatus != MB_ENOERR)
		dbg.println("can't initialize modbus stack!");

	/* Enable the Modbus Protocol Stack. */
	eStatus = eMBEnable();
	if (eStatus != MB_ENOERR)
		dbg.println("can't enable modbus stack!");

	// Initialise some registers
	usRegInputBuf[1] = 0x1234;
	usRegInputBuf[2] = 0x5678;
	usRegInputBuf[3] = 0x9abc;

	// debug LED
	CPin led(LED1);
	CTimeout tm;

	// Enter an endless loop
	while (1) {
		if ( tm.read()>0.5 ) {
			led = !led;
			tm.reset();
		}

	    eStatus = eMBPoll(  );

	    /* Here we simply count the number of poll cycles. */
	    usRegInputBuf[0]++;
	}
	return 0;
}

//
// Start implementation (call by MODBUS stack)
//
PR_BEGIN_EXTERN_C

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_HOLDING_START ) &&
        ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        switch ( eMode )
        {
            /* Pass current register values to the protocol stack. */
        case MB_REG_READ:
            while( usNRegs > 0 )
            {
                *pucRegBuffer++ = ( UCHAR ) ( usRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( UCHAR ) ( usRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;

            /* Update current register values with new values from the
             * protocol stack. */
        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}

PR_END_EXTERN_C
