/*
 ===============================================================================
 Name        : hcsr04.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2012/12/13
 Copyright   : 2012 (C) www.embeda.com.tw / www.ucxpresso.net
 License	 : CC BY-SA 3.0
 Description :
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+--------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+--------------
 
 ===============================================================================
 */

#ifndef HCSR04_H_
#define HCSR04_H_

#include "class/pin.h"
#include "class/thread.h"


class HCSR04: public CThread {
public:
	uint32_t m_distance;
	HCSR04(PIN_NAME_T trig, PIN_NAME_T echo);
	virtual bool start();

protected:
	CPin m_trig;
	CPin m_echo;
	virtual void run();
	uint32_t distance(uint32_t timeout=200);
};


#endif /* HCSR04_H_ */
