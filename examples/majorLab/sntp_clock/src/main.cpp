/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/10/31
 Copyright   : Embeda International Inc.
 License	 : CC BY-SA 3.0
 Description : main definition
 History	 :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/10/31 v1.0.0	First Edition.									Jason
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
#include "class/time.h"

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
	CPin sec(UNO_12);
	sec.output();
	sec = HIGH;

	SevenLEDx4 leds(UNO_0, UNO_1, UNO_2, UNO_3, UNO_4, UNO_5, UNO_6, UNO_7, UNO_8, UNO_9, UNO_10, UNO_11);
	leds.clear();
	leds.start();

	for (int i=0; i<16; i++) {
		leds[0] = i;
		leds[1] = i;
		leds[2] = i;
		leds[3] = i;
		sleep(500);
	}
	sec = LOW;

	CTime tm;
	CPin sw(P20);
	sw.input(INTERNAL_PULL_UP);

	// Enter an endless loop
	while (1) {
		tm = CTime::now();

		if ( sw==HIGH ) {
			leds[0] = tm.getMinute() % 10;
			leds[1] = tm.getMinute() / 10;
			leds[2] = tm.getHour() % 10;
			leds[3] = tm.getHour() / 10;

			// Simple demo Code (removable)
			sec = HIGH;
			sleep(300);

			sec = LOW;
		} else {
			leds[0] = tm.getDay() % 10;
			leds[1] = tm.getDay() / 10;
			leds[2] = tm.getMonth() % 10;
			leds[3] = tm.getMonth() / 10;
		}
		sleep(700);
	}
	return 0;
}
