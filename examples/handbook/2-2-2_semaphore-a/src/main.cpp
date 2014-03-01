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
#include <stdlib.h>
#include <time.h>

#include "class/semaphore.h"
#include "class/bus.h"

CSemaphore semLED;
CBus leds(LED1, LED2, LED3, LED4, END);
int	 index;

class Task1: public CThread {
protected:
	virtual void run() {
		while(1) {
			index = rand() % 4;	// get a random value in 0~3.
			semLED.release();	// release the index.
			sleep(500);
		}
	}
};

class Task2: public CThread {
protected:
	virtual void run() {
		while(1) {
			semLED.wait();
			leds[index] = !leds[index];
		}
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

	// TODO: insert setup code here
	srand( time(NULL) );	// initialize random seed:
	semLED.binary();		// set the semaphore in binary mode

	Task1 task1;
	task1.start("t1");

	Task2 task2;
	task2.start("t2");

	// Enter an endless loop
	while (1) {
		// nothing
	}
	return 0;
}
