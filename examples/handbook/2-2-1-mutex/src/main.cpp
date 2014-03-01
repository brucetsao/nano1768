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
#include "class/console.h"
#include "class/serial.h"
#include "class/mutex.h"

// Defaine the global mutex for Console
CMutex m_mutex;

//
// Define the global Console resource
//
CSerial cdc(USB);
Console con(cdc,cdc);

//
// Demo Task 1
//
class CTask1: public CThread {
protected:
	virtual void run() {
		while(1) {
			if ( cdc.isConnected() ) {
				m_mutex.lock();
				con << "1111111111111111111111111111" << endl;
				m_mutex.unlock();
				sleep(500);
			}
		}
	}
};

//
// Demo Task 2
//
class CTask2: public CThread {
protected:
	virtual void run() {
		while(1) {
			if ( cdc.isConnected() ) {
				m_mutex.lock();
				con << "2222222222222222222222222222" << endl;
				m_mutex.unlock();
				sleep(500);
			}
		}
	}
};

/* ==============================================
	main task routine
   ============================================== */
int main(void) {
	pool_memadd((uint32_t)pool, sizeof(pool));

	// Define the Task1 and Task2 objects
	CTask1 t1;
	CTask2 t2;

	t1.start("Task1");
	t2.start("Task2");

	// Enter an endless loop
	while(1){
 	 	 // do something (your loop code here)
	}
	return 0 ;
}
