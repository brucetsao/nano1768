/*
 * CButton.cpp
 *
 *  Created on: 2013/5/29
 *      Author: Jason
 */

#include "button.h"
#include "class/timeout.h"
#include "debug.h"

//
//
//

CButton::CButton(PIN_NAME_T pin) : m_pin(pin) {
	m_pin.input(INTERNAL_PULL_UP);
	m_flag = 0;
}

CButton::~CButton() {

}

bool CButton::start(LPCTSTR name) {
	return CThread::start(name);
}

void CButton::run() {
	CTimeout bounce;
	while(1) {
		if ( bit_chk(m_flag, KEY_FLAG_DOWN) ) {
			if ( m_pin==HIGH ){
				if ( bounce.isExpired(10)  ) {
					bit_clr(m_flag, KEY_FLAG_DOWN);
					onKeyUp();		// call virtual function "onKeyUp()"
				}
			} else {
				bounce.reset();
			}
		} else {
			if ( m_pin==LOW ){
				if ( bounce.isExpired(5)  ) {
					bit_set(m_flag, KEY_FLAG_DOWN);
					onKeyDown();	// call virtual function "onKeyDown()"
				}
			} else {
				bounce.reset();
			}
		}
	}
}

//
//
//

CButtons::CButtons(PIN_NAME_T pin, ...) {
	// assign pins
	va_list pins;
	va_start(pins, pin);
	m_pins.assign(pin, pins);
	va_end(pins);

	m_pins.input(INTERNAL_PULL_UP);
	m_flag = m_pins;
	m_down = m_pins;
}

CButtons::~CButtons() {

}

bool CButtons::start(LPCTSTR name) {
	return CThread::start(name);
}

int CButtons::isKeyDown() {
	int pins=0;
	for (int i=0; i<m_pins.count(); i++) {
		if ( bit_chk(m_down, i)==false ) {	// LOW is down
			pins |= bit(i);
		}
	}
	return pins;
}

void CButtons::run() {
	CTimeout tm;
	uint32_t newval;
	while(1) {
		newval = m_pins;
		if ( newval != m_flag ) {
			if ( tm.isExpired(10) ) {						// wait for bounce time
				m_down = newval;
				for (int i=0; i<m_pins.count(); i++) {
					if ( bit_chk((m_flag ^ newval), i)  ) {	// is different? Key Even caused...
						if ( bit_chk(newval, i) ) { 		// HIGH is key up
							onKeyUp(i);
						} else {							// LOW is key down
							onKeyDown(i);
						}
					}
				}
				m_flag = newval;
			}
		} else {
			tm.reset();
		}
	}
}

