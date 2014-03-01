/*
 ===============================================================================
 Name        : mutex.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2012/2/13
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 History     :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/2/13	v1.0.0	First Edition									Jason
 ===============================================================================
 */
#ifndef MUTEX_H_
#define MUTEX_H_

#include "class/object.h"

/*! \class CMutex mutex.h "class/mutex.h"
 *  \brief Use the CMutex class to lock or unlock a resource.
 *  \note Mutexes and binary semaphores are very similar but have some subtle differences: Mutexes include a priority inheritance mechanism, binary semaphores do not. This makes binary semaphores the better choice for implementing synchronisation (between tasks or between tasks and an interrupt), and mutexes the better choice for implementing simple mutual exclusion.
 */
class CMutex: public CObject {
public:
	/**Call the member function to lock (take semaphore) a resource with block when resource in used.
	 *
	 * \code
	 * Example:
	 * 		CMutex mutex;
	 *
	 * 		CSerial cdc(USB);
	 * 		Console (cdc);
	 *
	 * 		class Task1: public CThread {
	 * 		protected:
	 * 			virtual void run() {
	 *				while(1) {
	 *					if ( cdc.isConnected() ) {
	 *						mutex.lock();
	 *						con << "1111111111" << endl;
	 *						mutex.unlock();
	 *					}
	 *				}
	 * 			}
	 * 		};
	 *
	 * 		class Task2: public CThread {
	 * 		protected:
	 * 			virtual void run() {
	 *				while(1) {
	 *					if ( cdc.isConnected() ) {
	 *						mutex.lock();
	 *						con << "2222222222" << endl;
	 *						mutex.unlock();
	 *					}
	 *				}
	 * 			}
	 * 		};
	 * \endcode
	 */
	virtual void lock();

	/**Call the member function to lock (take) a resource without block when resource in used.
	 * \return true if lock successful; otherwise, failed.
	 */
	virtual bool trylock();

	/**Call the member function to unlock (release) a resource.
	 *
	 * \code
	 * Example:
	 * 		CMutex mutex;
	 *
	 * 		CSerial cdc(USB);
	 * 		Console (cdc);
	 *
	 * 		class Task1: public CThread {
	 * 		protected:
	 * 			virtual void run() {
	 *				while(1) {
	 *					if ( cdc.isConnected() ) {
	 *						mutex.lock();
	 *						con << "1111111111" << endl;
	 *						mutex.unlock();
	 *					}
	 *				}
	 * 			}
	 * 		};
	 *
	 * 		class Task2: public CThread {
	 * 		protected:
	 * 			virtual void run() {
	 *				while(1) {
	 *					if ( cdc.isConnected() ) {
	 *						mutex.lock();
	 *						con << "2222222222" << endl;
	 *						mutex.unlock();
	 *					}
	 *				}
	 * 			}
	 * 		};
	 * \endcode
	 */
	virtual void unlock();

	/*! \cond PRIVATE */
public:
	CMutex();
	virtual ~CMutex();
protected:
	xHandle m_xHandle;
	/*! \endcond */
};

#endif
