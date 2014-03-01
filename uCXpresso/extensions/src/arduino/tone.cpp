/*
 ===============================================================================
 Name        : tone.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/9/5
 Copyright   : 2012 (C) Embeda Technology Inc.
 License	 : CC BY-SA 3.0
 Description : tone
 History	 :

 ===============================================================================
 */
#include "arduino.h"
#include "class/pin.h"
#include "class/timeout.h"
#include "arduino/tone.h"

CTone::CTone(int pin) {
	m_pin = pin;
	m_timer = NULL;
	m_duration = 0;
}

CTone::~CTone() {
	end();
}

void CTone::run() {
	CPin pin(_PIN_(m_pin));
	pin.output(NOT_OPEN);
	m_timer->begin();
	CTimeout to;
	while(1) {
		pin = !pin;
		m_timer->wait();
		if ( m_duration ) {
			if ( to.isExpired(m_duration) ) break;
		}
	}
}

bool CTone::begin(uint32_t frequency, uint32_t duration) {
	int t = CTimer::getUnused();
	if ( t>=0 && frequency>0 ) {
		if ( m_timer==NULL ) {
			m_timer = new CTimer((TIMER_T)t);
		}
		if ( m_timer ) {
			uint32_t match = (1000000/(frequency*2));
			m_timer->setting(1, match);
			m_duration = duration;
			start("tone", 90, PRI_SUPER);
		}
	}
	return false;
}

void CTone::end() {
	if ( m_timer ) {
		delete m_timer;
		m_timer = NULL;
	}
}
