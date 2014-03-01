/*
 ===============================================================================
 Name        : main.cpp
 Author      : USER
 Version     : 1.0.1
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : main definition
 History	 :
 ===============================================================================
 */
#include "uCXpresso.h"

/* ==============================================
 user adjustable pool memory
 ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

#include "class/adc.h"
#include "class/bus.h"
#include "class/timeout.h"
#include "debug.h"

CDebug dbg(DBG_USB);

// Declare EMF A/D input level
static const int emf_level[] = {
		256,
		384,
		512,
		2048
};

/* ==============================================
 main task routine
 ============================================== */
int main(void) {
	pool_memadd((uint32_t) pool, sizeof(pool));

#ifdef DEBUG
	dbg.start();
#endif

	int val;

	// declare an emf to use the ADC5
	CAdc emf(AD5);
	emf.begin();

	// declare a bus to use the LED1~LED4
	CBus leds(LED1, LED2, LED3, LED4, END);

	// Enter an endless loop
	while (1) {

		val = emf.read(0.2, 8);

#ifdef DEBUG
		// Show the emf value when console is connected
		if ( dbg.isDebugMode() ) {
			dbg.println("EMF:%d\n\r", val);
		}
#endif
		// LED display for EMF
		for (int i = 0; i < 4; i++) {
			if (val > emf_level[i]) {
				leds[i] = LED_ON;
			} else {
				leds[i] = LED_OFF;
			}
		}
		sleep(5);
	}
	return 0;
}
