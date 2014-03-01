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

#include "class/thread.h"
#include "class/semaphore.h"

#include "class/bus.h"
#include "class/serial.h"
#include "class/console.h"

//
// Global resource
//
CSemaphore m_semLED;
char num;

//
// A Task responses for console (terminal) input
//
class CInput: public CThread {
protected:
	virtual void run() {
		CSerial cdc(USB);
		Console con(cdc, cdc);
		while(1) {
			if ( con.isConnected() ) {
				con << "Input Num (1~4):" << flush;
				con >> num;
				if ( num>'0' && num<'5' ) {		// check number range valid?
					con << num << endl;			// echo the valid number
					num -= '0';
					m_semLED.release();
				} else {
					con << "Invalid" << endl;	// invalid
				}
			}
		}
	}
};

//
// A Task responses for LED1~4 output
//
class CLed: public CThread {
protected:
	virtual void run() {
		CBus leds(LED1, LED2, LED3, LED4);
		while(1) {
			if ( m_semLED.wait() ) {				// wait the resource (num) ready
				if ( num>0 && num<5 ) {
					leds[num-1].invert();			// invert the specify LED
				}
			}
		}
	}
};

/* ==============================================
	main task routine
   ============================================== */
int main(void) {
	pool_memadd((uint32_t)pool, sizeof(pool));

	CInput in;
	CLed   led;

	in.start("Input");
	led.start("LED");

	// Enter an endless loop
	while(1){
 	 	 // do something (your loop code here)
	}
	return 0 ;
}
