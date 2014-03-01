/*
 ===============================================================================
 Name        : sslm-k.cpp
 Author      : Jason
 Version     : v1.0.5
 Date		 : 2013/5/4
 Copyright   : 2012 (C) www.embeda.com.tw / www.ucxpresso.net
 License	 : CC BY-SA 3.0
 Description : Serial 7-Segment LED Module
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+--------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+--------------
 2012/12/29 v1.0.0	first edition									Jason
 2012/12/31 v1.0.1	Fixed symbol member function					Jason
 	 	 	 	 	Remove Reverse Font (don't need...)
 2013/01/01 v1.0.2	Change PWM frequency to 1KHz (1ms)				Jason
 2013/01/02 v1.0.3	set brightness to 50% in construct				Jason
 2013/03/07 v1.0.4  Add OTC Colon/Dach LED Module					Jason
 2013/05/04 v1.0.5	Slow down the SPI speed to 10KHz				Jason
 ===============================================================================
 */

#include "led/sslm-k.h"
#include "arduino.h"

#ifndef MAX_SSLM_PWM
#define MAX_SSLM_PWM	0.3f
#endif

static const uint8_t SSLM_FONT[] = {		// SSLM Normal
		//PGFEDCBA
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

#define SSLM_FLAG_REVERSE	1
#define SSLM_FLAG_DOT		5
#define SSLM_FLAG_SYMBOL	6

SSLM_K::SSLM_K(int digits, SPI_PORT_T port, PIN_NAME_T ssl, PWM_CH_T bri, PIN_NAME_T clr) {
	//
	// setup SPI port
	//
	m_spi.assign(port, ssl);
	m_spi.format(SPI_DATABIT_8, SPI_MODE_0);
	m_spi.frequency(KHZ(10));
	m_spi.begin();

	//
	// setup PWM channel for brightness
	//
	CPwm::period(0.001);		// PWM core period = 1ms
	m_bri.assign(bri);
	brightness(0.5);		// set brightness = 50%
	m_bri.begin();			// start PWM channel
	CPwm::start();			// start PWM core

	//
	// setup clear pin
	//
	m_clr.assign(clr);
	m_clr.output();			// clr pin as an output pin
	m_clr = HIGH;

	//
	// setup data buffer
	//
	m_digits = digits;
	if ( m_digits ) {
		m_value = new uint8_t[m_digits];
		m_flag = new uint8_t[m_digits];
		memset(m_value, 0, m_digits);
		memset(m_flag, 0, m_digits);
	}

	m_trigger.binary();
}

SSLM_K::~SSLM_K() {
	if ( m_digits ) {
		delete m_value;
		delete m_flag;
	}
}

bool SSLM_K::start() {
	return CThread::start("SSLM-K", 90, PRI_HIGH);
}

void SSLM_K::brightness(float percentage) {
	// set brightness range to protected LED
	percentage = map(percentage, 0.0f, 1.0f, 1.0f, MAX_SSLM_PWM);

	// output PWM
	m_bri.dutyCycle(percentage);
}

void SSLM_K::clear() {
	memset(m_value, 0, m_digits);
	for (int i=0; i<m_digits; i++) {
		m_flag[i] &= 0x0F;
	}
	trigger();
}

void SSLM_K::setting(int index, SSLM_TYPE type) {
	if ( index<m_digits ) {
		if ( type==sslmREVERSE ) {
			bit_set(m_flag[index], SSLM_FLAG_REVERSE);
		} else {
			bit_set(m_flag[index], SSLM_FLAG_REVERSE);
		}
	}
}

inline bool SSLM_K::isReverse(int index) {
	return bit_chk(m_flag[index], SSLM_FLAG_REVERSE);
}

void SSLM_K::write(int index, unsigned val) {
	if ( val<sizeof(SSLM_FONT) ) {
		symbol(index, SSLM_FONT[val]);
	}
}

void SSLM_K::symbol(int index, uint8_t value) {
	if ( index<m_digits ) {
		if ( isReverse(index) ) {
			m_value[index] = (value & 0x3F) | ((value & 0x40)<<1) | ((value & 0x80)>>1);	// swap DP & G
		} else {
			m_value[index] = value;
		}
	}
}

void SSLM_K::dot(int index, bool enable) {
	if ( index<m_digits ) {
		if ( enable ) {
			bit_set(m_flag[index], SSLM_FLAG_DOT);
		} else {
			bit_clr(m_flag[index], SSLM_FLAG_DOT);
		}
	}
}

void SSLM_K::trigger() {
	m_trigger.release();
}

void SSLM_K::run() {
	while(1) {
		m_trigger.wait();
		//
		// check flag
		//
		for (int i=(m_digits-1); i>=0; i--) {
			if ( bit_chk(m_flag[i], SSLM_FLAG_DOT) ) {
				m_value[i] |= (isReverse(i) ? 0x40 : 0x80);
			} else {
				m_value[i] &= ~(isReverse(i) ? 0x40 : 0x80);
			}
		}
		//
		// push to SPI
		//
		m_spi.readwrite(m_value, NULL, m_digits);
	}
}

//
// for OTC LED Module
//
static const uint8_t SSLM_COLON[] = {		// SSLM Normal
		//50921467
		//PGFEDCBA
		0b10001000, // colon
		0b00010100, // dach
};

void SSLM_K::colon(int index) {
	m_value[index] = SSLM_COLON[0];
	bit_set(m_flag[index], SSLM_FLAG_DOT);
}

void SSLM_K::dach(int index) {
	m_value[index] = SSLM_COLON[1];
	bit_clr(m_flag[index], SSLM_FLAG_DOT);
}

void SSLM_K::clear(int index) {
	if ( index<m_digits ) {
		m_value[index] = 0x00;
		bit_clr(m_flag[index], SSLM_FLAG_DOT);
	}
}

