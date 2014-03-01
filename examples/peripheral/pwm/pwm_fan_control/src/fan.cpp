/*
 ===============================================================================
 Name        : fan.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2013/8/4
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description :
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 
 ===============================================================================
 */

#include "fan.h"

#define FAN_RPM_RESOLUTION	30
#define FAN_RPM_TIMEOUT		300		// 300ms

CFan::CFan(PWM_CH_T ctrl, PIN_NAME_T sense) :
m_ctrl(ctrl),
m_sense(sense)
{
	m_rpmcount = 0;
	m_rpm = 0;
}

CFan::~CFan() {
}

bool CFan::start(LPCTSTR name) {

	m_ctrl.dutyCycle(0.0);	// turn off FAN
	m_ctrl.begin();			// start PWM
	m_sense.begin();		// start Interrupt

	return CThread::start(name, 128, PRI_HARDWARE);
}

void CFan::write (float percentage) {
	m_ctrl.dutyCycle(percentage);
}

void CFan::run() {
	CTimeout tm;			// time count for RPM
	tm.reset();
	while(isAlive()) {				// check thread alive

		//
		// wait for sense pin trigger (interrupt)
		//
		if ( m_sense.wait(FAN_RPM_TIMEOUT) ) {
			if ( m_sense==LOW ) {	// filter noise (for Falling only)
				//
				// RPM calculate
				//
				m_rpmcount++;
				if ( m_rpmcount>=20 ) {
					// Update RPM every 20 counts, increase this for better RPM resolution,
					// decrease for faster update
					m_rpm = (FAN_RPM_RESOLUTION * 1000 * m_rpmcount) / tm.elapsed();
					tm.reset();
					m_rpmcount = 0;

				} // end of if ( m_rpmcount>=20 )
			}

		} else {
			//
			// timeout
			//
			m_ctrl.dutyCycle(0.0f);	// turn off fan
			m_rpm = 0;
		}
	}
}
