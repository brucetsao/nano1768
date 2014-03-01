/*
 ===============================================================================
 Name        : adckey.h
 Author      : Jason
 Version     : V1.0.0
 Date		 : 2012/6/11
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : CC BY-SA 3.0
 Description :
 History     :
 ===============================================================================
 */

#include "class/adc.h"
#include "class/thread.h"
#include "class/timeout.h"
#include "class/mutex.h"
#include "debug.h"
#include "adckey.h"

#define DBG_ADC_VALUE

// R1=10K Ohm (for 3.3V)
static const int adc_key_val[MAX_KEYS] = { 150, 300, 600, 1200, 2400 };

#ifdef DBG_ADC_VALUE
int last = 0;
#endif

CAdcKey::CAdcKey(ADC_CH_T ch): m_keyAd(ch) {
	m_flag = 0;
}

CAdcKey::~CAdcKey() {
}

void CAdcKey::run() {
	CTick bounce;
	int key, lastK = -1;
	while (1) {
		key = get_key();
		if (key >= 0) { // is any key press
			if (key != lastK) {
				bounce.reset();
				lastK = key;
			} else if (bounce.isExpired(10000) ) { // bounce time > 10ms
				m_mutex.lock();
				bit_set(m_flag, key); 			// update key flag (bit)
				m_mutex.unlock();
				while(get_key()>=0);			// wait key release
			}
		} else {
			lastK = -1;
		}
	}
}

int CAdcKey::get_key() {
	int val, k;
	val = m_keyAd.read(0.2, 3); // read A/D value with filter (20% & try count=3)

#ifdef DBG_ADC_VALUE
	if (val != last) {
		dbg.println("ad key val=%d", val);
		last = val;
	}
#endif

	for (k = 0; k < MAX_KEYS; k++) {
		if (val < adc_key_val[k]) {
			return k;
		}
	}
	return -1;
}

void CAdcKey::start(LPCTSTR name) {
	m_keyAd.begin();
#ifdef DBG_ADC_VALUE
	CThread::start(name); // default stack size (128) for debug
#else
	CThread::start(name, 64);
#endif
}

bool CAdcKey::isPressed(ADKEY_T key, bool clearFlag) {
	bool res = bit_chk(m_flag, key);
	if (res && clearFlag) {
		m_mutex.lock();
		bit_clr(m_flag, key);
		m_mutex.unlock();
	}
	return res;
}
