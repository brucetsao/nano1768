/*
 ===============================================================================
 Name        : dac.h
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/4/11
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : 

 History     :
 ===============================================================================
 */

#ifndef DAC_H_
#define DAC_H_

#include "class/peripheral.h"

typedef enum {
	DAC0	= 0
}DAC_CH_T;


class CDac: public CPeripheral {
public:
	CDac();
	CDac(DAC_CH_T ch);
	virtual ~CDac();

	void assign(DAC_CH_T ch);

	bool write(uint32_t value);		// 3.3V: value=65535, 0V: value=0
	bool write(float percentage);	// Analog Out = 3.3V * percentage
protected:
	int m_ch;
};

#endif /* DAC_H_ */
