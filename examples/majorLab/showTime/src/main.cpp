/*
 ===============================================================================
 Name        : main.cpp
 Author      :
 Version     : 1.0.0
 Date		 :
 Copyright   :
 License	 :
 Description : main definition
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
#include "led/seven_seg_ledx4.h"

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
	SevenLEDx4 leds(UNO_0, UNO_1, UNO_2, UNO_3, UNO_4, UNO_5, UNO_6, UNO_7, UNO_8, UNO_9, UNO_10, UNO_11);
	leds.start();

	// Enter an endless loop
	CTime tm;
	int sec, min;

	while (1) {
		tm = CTime::now();		// get system date/time
		sec = tm.getSecond();
		min = tm.getMinute();

		leds[3] = min / 10;		// display the high of minute
		leds[2] = min % 10;		// display the low of minute
		leds[1] = sec / 10;		// display the high of second
		leds[0] = sec % 10;		// display the low of seconde

		// Simple demo Code (removable)
		led = !led;
		sleep(500);
	}
	return 0;
}
