/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/12/8
 Copyright   :
 License	 :
 Description : main definition
 History	 :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------

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
#include "btHC05.h"
#include "class/bus.h"

//
// TODO: insert other definitions and declarations here
//


/* ==============================================
 main task routine
 ============================================== */
int main(void) {
	pool_memadd((uint32_t) pool, sizeof(pool));

#ifdef DEBUG
	dbg.start();
#endif

	// Simple demo Code (removable)
	CPin led(LED1);

	// TODO: insert setup code here
	btHC05 bt(UART2, UNO_2, UNO_3);

	CBus leds(LED2, LED3, END);
	CString res;

	char *cmd = new char[64];
	int  ch =0, len = 0;

	// Enter an endless loop
	while (1) {
		if ( dbg.isDebugMode() ) {

			if ( bt.readable() ) {
				bt.read(&ch, 1);
				dbg.printf("%c", ch);
				leds[1] = LED_ON;
			}

			if ( (ch=dbg.isAnyKey())!=0 ) {
				dbg.putc(ch);	// echo
				if ( len>64 || ch==0x0D ) {
					cmd[len] = 0x00;
					res = bt.ATcommand(cmd);
					dbg.printf(res);
					len = 0;
				} else {
					cmd[len++] = ch;
				}
				leds[0] = LED_ON;
			}
		}

		// Simple demo Code (removable)
		led = !led;
		if ( bt.readable()==0 ) {
			sleep(100);
			leds[0] = LED_OFF;
			leds[1] = LED_OFF;
		}
	}
	return 0;
}
