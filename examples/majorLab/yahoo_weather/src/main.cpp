/*
 ===============================================================================
 Name        : main.cpp
 Author      :
 Version     : 1.0.0
 Date		 :
 Copyright   :
 License	 :
 Description : main definition
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
#include "probe/probe.h"
#include "debug.h"

CDebug dbg(DBG_USB);

/* ==============================================
 user adjustable pool memory
 ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

//
// TODO: insert other include files here
//
#include "net/api/weather.h"

#define WOEID_NEWYORK	"2459115"
#define WOEID_TAIPEI	"2306179"

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
	probe.start();
#endif

	// Simple demo Code (removable)
	CPin led(LED1);

	// TODO: insert setup code here
	Weather w;

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(3000);

		// Your loop code here
		if ( w.request(WOEID_NEWYORK) ) {
			dbg.printf("NewYork\t: temp=%d, humi=%d\n", w.temp(), w.humi());
		}

		if ( w.request(WOEID_TAIPEI) ) {
			dbg.printf("Taipei\t: temp=%d, humi=%d\n", w.temp(), w.humi());
		}
	}
	return 0;
}
