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

#include <stdlib.h>
#include <time.h>
#include "class/thread.h"
#include "class/semaphore.h"

#include "class/serial.h"
#include "class/console.h"
#include "class/list.h"
#include "debug.h"

CDebug dbg(DBG_USB);

#define MAX_STORE_BUFFER	3

//
// A Store to save and sell the products
//
class CStore: public CObject {
protected:
	CList m_lstProd;
	CSemaphore m_semProd, m_semCons;
public:
	CStore() {
		// using counting semaphore mode
		m_semProd.binary();
		m_semCons.binary();
	}

	void deposit(int *val) {
		// is Full?
		if (m_lstProd.count() >= MAX_STORE_BUFFER) {
			dbg.println("[Store Is Full]\n\r");
			m_semProd.wait();
		}
		m_lstProd.addTail(val);

		// notify to Consumer
//		if ( m_semCons.wait()>0 ) {
			dbg.println("[Notify to Consumer]\n\r");
			m_semCons.release();
//		}
	}

	int* retrieve() {
		int *val;
		// is Empty?
		if (m_lstProd.count() == 0) {
			dbg.println("[Store Is Empty]\n\r");
			m_semCons.wait(); // TODO: wait fail!!
		}
		val = (int *) m_lstProd.removeHead();

		// notify to Producer
//		if ( m_semProd.waiting()>0 ) {
			dbg.println("[Notify to Producer]\n\r");
			m_semProd.release();
//		}
		return val;
	}
};

static CStore store;

//
// A factory create product to store
//
class CProducer: public CThread {
protected:
	virtual void run() {
		int *val;
		for (int i = 1; i <= 10; i++) {
			val = new int; 					// create a new product
			*val = i; 						// set product S/N
			store.deposit(val); 			// add to store
			dbg.println("Produce:%d\n\r", *val); 	// display information
			sleep((rand()%2000)); 			// random time to make next product
		}
	}
};

//
// A Consumer buy product from store
//
class Consumer: public CThread {
protected:
	virtual void run() {
		int *val;
		for (int i = 1; i <= 10; i++) {
			val = store.retrieve(); 		// buy product from store
			dbg.println("Consume:%d\n\r", *val); 	// display product S/N
			delete val; 					// Consume the product
			sleep((rand()%2000));			// random time to shopping again
		}
	}
};

/* ==============================================
 main task routine
 ============================================== */
int main(void) {
	pool_memadd((uint32_t) pool, sizeof(pool));

//#ifdef DEBUG
	dbg.start();
//#endif

	// initialize random seed:
	srand(time(NULL));

	// Declare the producer & consumer pointer
	CProducer *producer;
	Consumer  *consumer;

	// Enter an endless loop
	while (1) {
		// wait for connection
		if ( dbg.isDebugMode() ) {
			// start producer & consumer tasks
			dbg.println("[Start Producer & Consumer Demo]\n\r");

			producer = new CProducer;	// alloc new producer object
			consumer = new Consumer;	// alloc new consumer object

			producer->start("Prod");	// start the producer task
			consumer->start("Cons");	// start the consumer task

			while( dbg.isDebugMode() ){
				// wait exit debug mode
			}

			delete producer;			// delete producer task & object
			delete consumer;			// delete consumer task & object
		}
	}
	return 0;
}
