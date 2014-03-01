/*
===============================================================================
 Name        : main.cpp
 Author      : User
 Version     : 1.0.0
 Copyright   :
 Description : main definition
 History	 :
===============================================================================
*/
#include "uCXpresso.h"
// Customer pool memory
static uint8_t pool[DEFAULT_POOL_SIZE];	// 16KB for pool

#include "class/console.h"
#include "class/serial.h"
#include "class/pin.h"


/* ===============================================================================
	main routine
   =============================================================================== */
int main(void) {
	pool_memadd((uint32_t)pool, sizeof(pool));

	CSerial cdc(USB);
	Console con(cdc, cdc);
	CPin led(LED1);

// 2012/6/28 masked: on board LED don't need to set the mode, it always output with NOT_OPEN
//	led.output();

	while(1){
		if ( cdc.isConnected() ) {
			con << "Hello World" << endl;
			while( cdc.isConnected() ) {
				led.invert();
				sleep(500);
			}
		}
	} // */
	return 0 ;
}
