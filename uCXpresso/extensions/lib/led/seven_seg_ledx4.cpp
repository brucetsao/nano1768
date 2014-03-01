/*
 ===============================================================================
 Name        : seven_seg_ledx4.cpp
 Author      : Jason
 Version     : v1.0.1
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

#include "seven_seg_ledx4.h"
#include "class/timer.h"

//
// TODO: insert other definitions and declarations here
//
#define	LED_FLAG_INVISIBLE	15	// invisible the leds
#define LED_FLAG_SYMBOL		14	// user's symbol

static const uint32_t seven_seg_digits[] = {
		//pgfedcba
		0b00111111, // 0
		0b00000110, // 1
		0b01011011, // 2
		0b01001111, // 3
		0b01100110, // 4
		0b01101101, // 5
		0b01111101, // 6
		0b00000111, // 7
		0b01111111, // 8
		0b01101111, // 9
		0b01110111, // a
		0b01111100,	// b
		0b00111001,	// c
		0b01011110,	// d
		0b01111001,	// e
		0b01110001,	// f
};

SevenLEDx4::SevenLEDx4(
		PIN_NAME_T a,
		PIN_NAME_T b,
		PIN_NAME_T c,
		PIN_NAME_T d,
		PIN_NAME_T e,
		PIN_NAME_T f,
		PIN_NAME_T g,
		PIN_NAME_T dp,
		PIN_NAME_T d0, PIN_NAME_T d1, PIN_NAME_T d2, PIN_NAME_T d3) :
	m_leds(a, b, c, d, e, f, g, dp, END),
	m_scan(d0, d1, d2, d3, END)
{
	m_leds.output(); 			// High Active
	m_scan.output(OPEN_DRAIN);	// Low Active
}

uint16_t& SevenLEDx4::operator [](int index) {
	return m_value[index];
}

void SevenLEDx4::run() {
	CTimer t(TIMER0);
	t.setting(1, 250);	// interval 250us
	t.begin();
	int i = 0;
	while (1) {
		if ( !bit_chk(m_value[i],LED_FLAG_INVISIBLE )  ) {
			if ( bit_chk(m_value[i], LED_FLAG_SYMBOL) )
				 m_leds = m_value[i] & 0x0FF; 				// update 7s led value (user's symbol)
			else m_leds = seven_seg_digits[m_value[i]]; 	// update 7s led value
			m_scan = ~(1 << i); 							// turn-on current 7s led (to low)
		}
		i = (i + 1) % 4; 	// next 7s
		t.wait();			// wait for next
		m_scan = 0xFF;		// turn all leds off
	}
}

bool SevenLEDx4::start() {
#ifdef DEBUG
	return CThread::start("7sLEDx4", 128, PRI_HIGH);
#else
	return CThread::start("7sLEDx4", 96, PRI_HIGH);
#endif
}

void SevenLEDx4::symbol(int index, byte c) {
	if ( index<4 ) {
		m_value[index] = (1<<LED_FLAG_SYMBOL) | c;
	}
}

void SevenLEDx4::clear() {
	for (int i=0; i<4; i++) {
		m_value[i] = (1<<LED_FLAG_INVISIBLE);
	}
}


