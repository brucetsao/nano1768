/*
 ===============================================================================
 Name        : pinint.h
 Author      : Jason
 Version     : 1.0.1
 Created on	 : 2012/3/20
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : GPIO interrupt service class
 History	 :
 ===============================================================================
 */

#ifndef PININT_H_
#define PININT_H_

#include "class/object.h"
#include "class/semaphore.h"
#include "class/pin.h"

typedef enum {
	RISING = 0,
	FALLING
}EDGE_STATE_T;

class CPinINT: public CPin {
public:
	CPinINT();
	CPinINT(PIN_NAME_T pin, EDGE_STATE_T edge=FALLING, PIN_INPUT_MODE_T mode=INTERNAL_PULL_UP);
	virtual ~CPinINT();
	virtual void assign(PIN_NAME_T pin, EDGE_STATE_T edge=FALLING, PIN_INPUT_MODE_T mode=INTERNAL_PULL_UP);

	// enable / disable
	virtual void enable();
	virtual void disable();
	inline void begin() { enable(); }
	inline void end() { disable(); }

	virtual bool wait(uint32_t tm=MAX_DELAY_TIME);

	// trigger by software
	virtual void release();

protected:
	xHandle	m_gpio;
};

#endif /* PININT_H_ */
