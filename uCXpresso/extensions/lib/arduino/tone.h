/*
 ===============================================================================
 Name        : tone.h
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/9/5
 Copyright   : 2012 (C) Embeda Technology Inc.
 License	 : CC BY-SA 3.0
 Description : tone
 History	 :

 ===============================================================================
 */

#ifndef TONE_H_
#define TONE_H_

#include "class/thread.h"
#include "class/timer.h"

class CTone: public CThread {
public:
	CTone(int pin);
	virtual ~CTone();

	bool begin(uint32_t frequency, uint32_t duration=0);
	void end();

// internal use
public:
	int 	m_pin;
	CTimer	*m_timer;
	uint32_t m_duration;
protected:
	virtual void run();
};

#endif /* TONE_H_ */
