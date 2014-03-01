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
#include "class/mailbox.h"
#include "class/thread.h"
#include "class/bus.h"

//
// TODO: insert other definitions and declarations here
//
#define MAIL_LED	"LED"

class CLedTask: public CThread {
protected:
	virtual void run() {
		CBus 	 leds(LED1, LED2, LED3, LED4, END);
		CMailBox mail(MAIL_LED);
		int 	 *i;
		while(1) {
			i =(int *) mail.wait();
			if ( i ) {
				leds[*i] = !leds[*i];
			}
		}
	}
};

class Task1: public CThread {
protected:
	virtual void run() {
		int i=0;
		while(1) {
			CMailBox::post(MAIL_LED, &i);
			sleep(200);
		}
	}
};

class Task2: public CThread {
protected:
	virtual void run() {
		int i=1;
		while(1) {
			CMailBox::post(MAIL_LED, &i);
			sleep(300);
		}
	}
};


class Task3: public CThread {
protected:
	virtual void run() {
		int i=2;
		while(1) {
			CMailBox::post(MAIL_LED, &i);
			sleep(400);
		}
	}
};

class Task4: public CThread {
protected:
	virtual void run() {
		int i=3;
		while(1) {
			CMailBox::post(MAIL_LED, &i);
			sleep(500);
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
	CLedTask ledTask;
	ledTask.start();

	Task1 task1;
	task1.start();

	Task2 task2;
	task2.start();

	Task3 task3;
	task3.start();

	Task4 task4;
	task4.start();

	// Enter an endless loop
	while (1) {
		// Your loop code here
	}
	return 0;
}
