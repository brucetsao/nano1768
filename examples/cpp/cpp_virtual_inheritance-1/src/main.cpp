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

//
// TODO: insert other definitions and declarations here
//
class A {
public:
	A() {
		dbg.println("A Constructor");
	}
};

class B1: virtual public A {
public:
	B1() {
		dbg.println("B1 Constructor");
	}
};

class B2: virtual public A {
public:
	B2() {
		dbg.println("B2 Constructor");
	}
};


class C:public B1, B2 {
public:
	C() {
		dbg.println("C Constructor");
	}
};


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
	dbg.waitToDebugMode();
	C c;

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// Your loop code here

	}
	return 0;
}
