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
#include <stdlib.h>

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
	int i;

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// debug a exception
		i = (rand() % 100);

		if ( i>80 ) {	// occur exception when i>80
			dbg.println("i=%d", i);
			dbg.breakpoint();
		}
	}
	return 0;
}
