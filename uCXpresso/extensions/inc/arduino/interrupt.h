/*
 ===============================================================================
 Name        : interrupt.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/9/5
 Copyright   : 2012 (C) Embeda Technology Inc.
 License	 : CC BY-SA 3.0
 Description : extern interrupt
 History	 :

 ===============================================================================
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "arduino.h"
#include "class/thread.h"
#include "class/pinint.h"

class CInterrupt: public CThread {
// internal use
public:
	virtual ~CInterrupt();
	int		  m_pin;
	CPinINT	  m_int;
	INT_FUNC  m_func;

protected:
	virtual void run();
};
#endif /* INTERRUPT_H_ */
