/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/10/08
 Copyright   :
 License	 : MIT
 Description : Serial Loop Test (Must short the TX and RX pin)
 History	 :
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
#include "class/serial.h"

//
// TODO: insert other definitions and declarations here
//
static const char message[] = "0123456789-abcdefghijklmnopqrstuvwxyz-ABCEDFGHIJKLMNOPQRSTUVWXYZ!\r\n";
static char buf[sizeof(message)+1];

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

	dbg.waitToDebugMode();

	// TODO: insert setup code here
	CSerial uart(UART2, 128);		// set FIFO size = 128 bytes
	uart.settings(19200);			// set baud-rate = 19200 bps
	uart.write(message, sizeof(message));

	int len, i=0;
	// Enter an endless loop
	while (1) {
		len = uart.read(buf+i, sizeof(buf)-i);
		if ( len ) {
			i += len;
			if ( i>=sizeof(message)) {
				dbg.printf((LPCTSTR) buf);
				uart.write(buf, i);
				i = 0;
			}
			led = !led;
		}
		sleep(100);
	}
	return 0;
}
