/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2013/5/31
 Copyright   :
 License	 :
 Description : Thread's alive and resource test
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
#include "class/thread.h"

//
// A Class
//
class A {
public:
	A() {
		dbg.println("A Constructed");
	}
	~A() {
		dbg.println("A Destructed");
	}
};

//
// T thread
//
class T: public CThread {
public:
	T() {
		dbg.println("T Constructed");
	}

	~T() {
		dbg.println("T Destructed");
	}

protected:
	virtual void run() {
		dbg.println("T enters the run-loop:");
		A a;
		CPin led(LED2);
		while (isAlive()) {
			led = !led;
			sleep(200);
		}
		dbg.println("T exits the run-loop:");
	}
};

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
	dbg.waitToDebugMode();

	dbg.println("\nStart T:");

	T *t1 = new T;
	t1->start("T");

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// Your loop code here
		if (dbg.isAnyKey() && t1 != NULL) {
			dbg.println("\nKill T:");
			t1->kill();
		}
	}
	return 0;
}
