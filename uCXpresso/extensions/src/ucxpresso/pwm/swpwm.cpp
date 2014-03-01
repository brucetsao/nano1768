/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/9/1
 Copyright   : 2012 (C) Embeda Technology Inc. (www.embeda.com.tw)
 License	 : CC BY-SA 3.0
 Description : 
 History	 :
 ===============================================================================
 */
#include "pwm/swpwm.h"
#include "class/timer.h"
#include "class/pin.h"
#include "arduino.h"

typedef struct {
	uint32_t	dutyCycle;
	uint32_t	flag;
	CPin 		pin;
}SWPWM_T;

SWPwm::SWPwm(TIMER_T t, float accuracy): m_timer(t) {
	m_cycle = 0;
	m_accuracy = accuracy;
}

SWPwm::~SWPwm() {
	SWPWM_T *ch;
	LIST_POS pos;
	pos = m_lstCH.getHeadPos();
	while(pos) {
		ch =(SWPWM_T *) m_lstCH.removeAt(pos);
		delete ch;
		pos = m_lstCH.getNext(pos);
	}
}

void SWPwm::period(float sec) {
	float match = ((sec * 1000000.0f) / m_accuracy);
	m_timer.setting(1, match);
}

bool SWPwm::start() {
	bool ret;
	m_cycle = 0;
	ret = CThread::start("swPWM", 120, PRI_SUPER);
	m_timer.begin();
	return ret;
}

void SWPwm::stop() {
	m_timer.end();
}

int SWPwm::add(PIN_NAME_T p, float dutyCycle) {
	SWPWM_T *ch = new SWPWM_T;
	if ( ch ) {
		ch->pin.assign(p);
		ch->pin.output();
		ch->dutyCycle = map(dutyCycle, 0.0f, 1.0f, 0.0f, m_accuracy);
		ch->flag = 0;
		if ( m_lstCH.addTail(ch) ) {
			return m_lstCH.count();
		}
	}
	return 0;
}

bool SWPwm::update(int index, float dutyCycle) {
	SWPWM_T *ch = (SWPWM_T*) m_lstCH[index];
	if ( ch ) {
		ch->dutyCycle = map(dutyCycle, 0.0f, 1.0f, 0.0f, m_accuracy);
		return true;
	}
	return false;
}

#define CH_STATE	31
#define CH_COUNT	(ch->flag & 0x00FFFF)

void SWPwm::run() {
	SWPWM_T *ch;
	LIST_POS pos;
	while(1) {
		// wait timer interrupt
		m_timer.wait();

		pos = m_lstCH.getHeadPos();							// get first channel
		while(pos) {
			ch = (SWPWM_T *)m_lstCH.getAt(pos);				// get channel from list
			if ( ch ) {
				ch->flag++;									// inc pwm count
				if (  ch->dutyCycle>0 ) {
					if ( bit_chk(ch->flag, CH_STATE) ) {
						if ( CH_COUNT>=ch->dutyCycle ) {		// count to dutyCycle?
							ch->pin = LOW;						// set output low
							bit_clr(ch->flag, CH_STATE);		// change state
						}
					} else if ( CH_COUNT>=m_accuracy ) {		// count to end ?
						ch->pin = HIGH;							// set output high
						ch->flag = bit(CH_STATE);				// change state & RESET count
					}
				} else {
					ch->pin = LOW;								// always LOW when dutycycle=0
				}
			}
			pos = m_lstCH.getNext(pos);						// next channel
		}
		m_cycle++;
	}
}
