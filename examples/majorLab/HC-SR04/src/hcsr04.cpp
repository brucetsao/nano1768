/*
 ===============================================================================
 Name        : hcsr04.cpp
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

#include "hcsr04.h"

#include "class/pin.h"
#include "class/thread.h"
#include "class/timeout.h"

HCSR04::HCSR04(PIN_NAME_T trig, PIN_NAME_T echo) :
	m_trig(trig), m_echo(echo) {
	m_echo.input(INTERNAL_PULL_DOWN);
	m_trig.output();
	m_trig = LOW;
	m_distance = 0;
}

//
//
//
void HCSR04::run() {
	while (1) {
		m_distance = distance();
		sleep(500);
	}
}

bool HCSR04::start() {
	return CThread::start("HCSR04", 96, PRI_HIGH);
}

//
// 	Calculate Echo Pulse width (
//	return: centimeter (CM)
//
uint32_t HCSR04::distance(uint32_t timeout) {
	CTick tick;
	CTimeout to;
	to.reset();

	// trigger
	m_trig = HIGH;
	tick.delay(10); // 10us
	m_trig = LOW;

	// wait echo LOW->HIGH
	while (m_echo != HIGH) {
		if (to.isExpired(timeout))
			return 0;
	}

	// wait echo HIGH->LOW
	tick.reset();
	while (m_echo == HIGH) {
		if (to.isExpired(timeout))
			return 0;
	}

	return (tick.elapsed() * 343.2 / 20000);
}

