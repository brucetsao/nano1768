/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/10/08
 Copyright   :
 License	 : MIT
 Description : A simple EMF detector for Analog-To-Digital converter
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
#include "class/adc.h"
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
	CBus leds(LED1, LED2, LED3, LED4, END);

	// TODO: insert setup code here
	CAdc ad(AD5);
	ad.enable();

	// Enter an endless loop
	int 	i, value;
	float	graduate = MAX_ADC_VALUE/(leds.count()+1);
	while (1) {
		value = ad.read();
		for (i=0; i<leds.count(); i++) {
			if ( value>(graduate*(leds.count()-i)) ) {
				leds[leds.count()-i-1] = LED_ON;
			} else {
				leds[leds.count()-i-1] = LED_OFF;
			}
		}
		sleep(100);
	}
	return 0;
}
