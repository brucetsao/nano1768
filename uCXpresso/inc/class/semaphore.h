/*
 ===============================================================================
 Name        : semaphore.h
 Author      : Jason
 Version     : v1.0.3
 Date		 : 2013/12/20
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/2/18	v1.0.0	First Edition									Jason
 2012/3/13  v1.0.1	Separate the Semaphore and Mutex class			Jason
 2012/9/24	v1.0.2	Add binary mode									Jason
 	 	 	 	 	Remove waiting count
 2013/12/20 v1.0.3	Add waiting() member to return the number of 	Jason
 	 	 	 	 	waiting task.
  ===============================================================================
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "class/object.h"

/*! \class CSemaphore semaphore.h "class/semaphore.h"
 * \brief The CSemaphore class provides two kinds semaphore which counting and binary.
 * \note CSemaphore cannot be used before a call to member counting or binary.
 */
class CSemaphore: public CObject {
public:
	/**Call the member function to creates a counting semaphore.
	 * \param count is a integer value to specified the maximum count thate can be reached.
	 * \param init is a integer value to assigned to the semaphore when it is created.
	 * \return true if create semaphore successful; otherwise, failed.
	 */
	virtual bool counting(int count, int init=0);

	/**Call the member function to creates a binary semaphore.
	 * \return true if create semaphore successful; otherwise, failed.
	 * \note Binary semaphores and mutexes are very similar but have some subtle differences: Mutexes include a priority inheritance mechanism, binary semaphores do not. This makes binary semaphores the better choice for implementing synchronisation (between tasks or between tasks and an interrupt), and mutexes the better choice for implementing simple mutual exclusion.
	 */
	virtual bool binary();

	/**Call the member function to wait a semaphore available.
	 * \param timeout is a integer value to specified the millisecond to wait for the semaphore to become available.
	 * \return true if create semaphore successful; otherwise, failed.
	 *
	 * \code
	 * Example:
	 * 		CSemaphore semLED;
	 *
	 * 		// resource
	 * 		CBus leds(LED1, LED2, LED3, LED4, END);
	 * 		int  index;
	 *
	 * 		// task1
	 * 		class Task1: public CThread {
	 * 		protected:
	 * 			virtual void run() {
	 * 				while(1) {
	 * 					index = rand() % 4;		// get a random value in 0~3.
	 * 					semLED.release();		// release resource.
	 * 					sleep(500);
	 * 				}
	 * 			}
	 * 		};
	 *
	 * 		// task2
	 * 		class Task2: public CThread {
	 *		protected:
	 * 			virtual void run() {
	 * 				while(1) {
	 * 					semLED.wait();			// wait resource available.
	 * 					leds[index] = !leds[index];
	 * 				}
	 * 			}
	 * 		}
	 *
	 * 		void main() {
	 * 			...
	 * 			semLED.binary();	// set the semaphore in binary mode
	 * 			...
	 * 		}
	 * \endcode
	 */
	virtual bool wait(int timeout=MAX_DELAY_TIME);

	/**Call the member function to release a semaphore.
	 * \param fromISR is a boolean to specified the release occur from interrupt routine. (internal used)
	 * \return a integer value to identify the context switch wake. (internal used)
	 *
	 * \code
	 * Example:
	 * 		CSemaphore semLED;
	 *
	 * 		// resource
	 * 		CBus leds(LED1, LED2, LED3, LED4, END);
	 * 		int  index;
	 *
	 * 		// task1
	 * 		class Task1: public CThread {
	 * 		protected:
	 * 			virtual void run() {
	 * 				while(1) {
	 * 					index = rand() % 4;		// get a random value in 0~3.
	 * 					semLED.release();		// release resource.
	 * 					sleep(500);
	 * 				}
	 * 			}
	 * 		};
	 *
	 * 		// task2
	 * 		class Task2: public CThread {
	 *		protected:
	 * 			virtual void run() {
	 * 				while(1) {
	 * 					semLED.wait();			// wait resource available.
	 * 					leds[index] = !leds[index];
	 * 				}
	 * 			}
	 * 		}
	 *
	 * 		void main() {
	 * 			...
	 * 			semLED.binary();	// set the semaphore in binary mode
	 * 			...
	 * 		}
	 * \endcode
	 */
	virtual int  release(bool fromISR=false);

	/**Number of task in waiting
	 */
	virtual int waiting();

	/*! \cond PRIVATE */
public:
	CSemaphore();
	virtual ~CSemaphore();

protected:
	xHandle m_xHandle;
	int		m_waiting;
	/*! \endcond */
};

#endif /* SEMAPHORE_H_ */
