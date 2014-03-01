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

#include "class/pin.h"
#include "class/thread.h"
#include "demo_shell_usb.cpp"	// Shell for DEBUG

//
// LED1 Thread Demo
//
class CLed1: public CThread {
protected:
	virtual void run() {
		CPin led(LED1);
		while(1){
			led.invert();
			sleep(500);		// 500ms
		}
	}
};

//
// LED2 Thread Demo
//
class CLed2: public CThread {
protected:
	virtual void run() {
		CPin led(LED2);
		while(1) {
			led.invert();
			sleep(1000);	// 1000ms
		}
	}
};

/* ==============================================
	main task routine
   ============================================== */
int main(void) {
	pool_memadd((uint32_t)pool, sizeof(pool));

	//
	// for debug
	//
#ifdef DEBUG
	CShellDemoUSB usbSH;
	usbSH.start("Shell", 260); // Set shell task stack size = 260 bytes
#endif

	//
	// LED Demo
	//
	CLed1 led1;
	CLed2 led2;

	led1.start("LED1");			// Start LED1 demo thread
	led2.start("LED2");			// Start LED2 demo thread

	// Enter an infinite loop, just incrementing a counter
	while(1){
 	 	 // do something (your loop code here)
	}
	return 0 ;
}
