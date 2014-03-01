/*
 ===============================================================================
 Name        : sslm-k.h
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


#ifndef SSLM_K_H_
#define SSLM_K_H_

#include "class/thread.h"
#include "class/spi.h"
#include "class/pwm.h"
#include "class/pin.h"
#include "class/semaphore.h"

typedef enum {
	sslmNORMAL = 0,
	sslmREVERSE
}SSLM_TYPE;

class SSLM_K: public CThread {
public:
	SSLM_K(int digits, SPI_PORT_T port, PIN_NAME_T ssl, PWM_CH_T bri, PIN_NAME_T clr);
	virtual ~SSLM_K();
	virtual bool start();

	virtual void brightness(float percentage);
	virtual void clear();
	void setting(int index, SSLM_TYPE type);

	void write(int index, unsigned value);
	void symbol(int index, uint8_t value);
	void dot(int index, bool enable);
	void trigger();

	// for OTC LED module
	void colon(int index);
	void dach(int index);
	virtual void clear(int index);

protected:
	CSpiMaster	m_spi;
	CPin		m_ssl;
	CPin		m_clr;
	CPwm		m_bri;

	int			m_digits;
	uint8_t		*m_value;
	uint8_t		*m_flag;
	CSemaphore	m_trigger;

	virtual void run();
	inline bool isReverse(int index);
};

#endif /* SSLM_K_H_ */
