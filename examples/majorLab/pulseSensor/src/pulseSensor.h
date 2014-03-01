/*
 ===============================================================================
 Name        : pulseSensor.h
 Modify      : Jason
 Version     : v1.0.0
 Date		 : 2012/6/28
 url		 : http://www.ucxpresso.net
 Description : 
 History     :
 ===============================================================================
 */

#ifndef PULSESENSOR_H_
#define PULSESENSOR_H_

#include "class/thread.h"
#include "class/adc.h"
#include "class/pin.h"

class CPulseSensor: public CThread {
public:
	CPulseSensor(ADC_CH_T ch, PIN_NAME_T led);
	virtual ~CPulseSensor();
	void start(LPCTSTR name="PSensor");

public:
	bool B;     		// becomes true when there is a heart pulse
	bool QS;      		// becomes true when pulse rate is determined. every 20 pulses
	int HRV;      		// holds the time between beats
	int BPM;      		// used to hold the pulse rate
	int Fade;

protected:
	CAdc adPulse;
	CPin ledPulse;

	int  Signal;
	int	 Peek;
	int	 Senvitive;
	uint32_t lastTime;
	bool Pulse;

	int	 hrvCount;
	uint32_t HRVs;

protected:
	virtual void run();

};

#define PulseB

#endif /* PULSESENSOR_H_ */
