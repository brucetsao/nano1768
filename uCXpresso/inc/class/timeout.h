/*
 ===============================================================================
 Name        : timeout.h
 Author      : Jason
 Version     : 1.0.1
 Created on	 : 2012/3/18
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : A microsecond (us) timeout/expired checker
 History	 :
 ===============================================================================
 */

#ifndef TIMEOUT_H_
#define TIMEOUT_H_

#include "class/object.h"

// Unit: us

class CTick: public CObject {
public:
	CTick();

	virtual void reset();
	virtual uint32_t elapsed();
	virtual inline bool isExpired(uint32_t to) {
		return ((elapsed()>to )? true : false);
	}
	virtual inline void delay(uint32_t us) {
		while(isExpired(us)==false);	// a busy loop (:
	}
protected:
	uint32_t m_tick;
};

// Unit: ms
class CTimeout: public CObject {
public:
	CTimeout();
	virtual void reset();
	virtual void wait(int ms);			// timeout unit:ms
	virtual inline void wait(float sec){ 	// 0.1=100ms 0.02=20ms...
		wait((int)(sec * 1000));
	}
	virtual uint32_t elapsed();

	virtual inline bool isExpired(uint32_t to) {
		return ((elapsed()>to )? true : false);
	}

	/**Reterive timeout value
	 * \return float second
	 */
	virtual inline float read() {
		return (elapsed() / 1000.0f);
	}
protected:
	uint32_t m_tick;
};

#endif /* TIMEOUT_H_ */
