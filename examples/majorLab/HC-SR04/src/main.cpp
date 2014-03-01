/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/12/13
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
#include "hcsr04.h"
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
	HCSR04 hcsr(P15, P16);
	hcsr.start();

    SevenLEDx4 leds(UNO_0, UNO_1, UNO_2, UNO_3, UNO_4, UNO_5, UNO_6, UNO_7, UNO_8, UNO_9, UNO_10, UNO_11);
    leds.start();

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// display the distance to 7-segment 4 digits display
		if ( hcsr.m_distance>0 ) {
			dbg.println(hcsr.m_distance);
			leds[0] = (hcsr.m_distance % 10);
			leds[1] = (hcsr.m_distance / 10) % 10;
			leds[2] = (hcsr.m_distance / 100) % 10;
			leds[3] = (hcsr.m_distance / 1000) % 10;
		}
	}
	return 0;
}
