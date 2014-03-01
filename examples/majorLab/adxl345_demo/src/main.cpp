/*
===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.1
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : main definition
 History	 :
===============================================================================
*/
#include "uCXpresso.h"
#include "debug.h"

CDebug dbg(DBG_USB);

/* ==============================================
	user adjustable pool memory
   ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

//
// TODO: insert other include files here
#include "adxl345.h"
#include "class/pin.h"

//
// TODO: insert other definitions and declarations here


/* ==============================================
	main task routine
   ============================================== */
int main(void) {
	pool_memadd((uint32_t)pool, sizeof(pool));

#ifdef DEBUG
	dbg.start();
#endif

	// TODO: insert code here
	CAdxl345 adxl;
	adxl.assign(I2C0, KHZ(400));
	adxl.powerOn();

	CPin led(P7);
	led.output(NOT_OPEN);

	int x,y,z;

	// Enter an endless loop
	while(1){
		adxl.readAccel(&x, &y, &z);
#ifdef DEBUG
		if ( dbg.isDebugMode() ) {
			dbg.println("X=%d, Y=%d, Z=%d\n", x, y, z);
		}
#endif
		led = !led;
		sleep(200);
	}
	return 0 ;
}
