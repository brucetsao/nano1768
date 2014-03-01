/*
 ===============================================================================
 Name        : seven_seg_ledx4.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2012/10/15
 Copyright   : 2012 (C) www.embeda.com.tw / www.ucxpresso.net
 License	 : CC BY-SA 3.0
 Description :
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+--------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+--------------
 2012/10/15 v1.0.0	First Edition.									Jason
 2012/10/16 v1.0.1	Add symbol() and clear() member functions.		Jason
 ===============================================================================
 */

#ifndef SEVEN_SEG_LEDX4_H_
#define SEVEN_SEG_LEDX4_H_

#include "class/thread.h"
#include "class/bus.h"

class SevenLEDx4: public CThread {
public:
	SevenLEDx4(PIN_NAME_T a, PIN_NAME_T b, PIN_NAME_T c, PIN_NAME_T d, PIN_NAME_T e, PIN_NAME_T f, PIN_NAME_T g, PIN_NAME_T dp, PIN_NAME_T d0, PIN_NAME_T d1, PIN_NAME_T d2, PIN_NAME_T d3);
	virtual bool start();
	virtual uint16_t& operator [] (int index);
	virtual void symbol(int index, byte c);
	virtual void clear();

protected:
	CBus 		m_leds;
	CBus 		m_scan;
	uint16_t	m_value[4];
	virtual void run();
};

#endif /* SEVEN_SEG_LEDX4_H_ */
