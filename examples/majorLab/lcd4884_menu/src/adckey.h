/*
 ===============================================================================
 Name        : adckey.h
 Author      : Jason
 Version     :
 Date		 : 2012/6/11
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : CC BY-SA 3.0
 Description :
 History     :
 ===============================================================================
 */

#ifndef ADCKEY_H_
#define ADCKEY_H_

#include "class/adc.h"
#include "class/thread.h"

#define MAX_KEYS	5

typedef enum {
	KEY_LEFT = 0,
	KEY_CENTER,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_UP
}ADKEY_T;

class CAdcKey: public CThread {
public:
	CAdcKey(ADC_CH_T ch);
	virtual ~CAdcKey();
	virtual void start(LPCTSTR name="key");
	virtual bool isPressed(ADKEY_T key, bool clearFlag=true);

protected:
	CAdc 	 m_keyAd;
	uint32_t m_flag;
	CMutex	 m_mutex;
	virtual int get_key();
	virtual void run() ;
};

#endif /* ADCKEY_H_ */
