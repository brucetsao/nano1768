/*
 ===============================================================================
 Name        : probe.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2013/1/22
 License     : CC BY-SA 3.0
 Description : websocket_adc_mail
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/1/22	v1.0.0	First Edition.									Jason
 */

#ifndef PROBE_H_
#define PROBE_H_

#include "class/thread.h"

#define xMAIL_PROBE	"iPROBE"

class iProbe: public CThread {
public:
#ifdef DEBUG
	virtual bool start(int periodTime=1000);

protected:
	int m_periodTime;
	virtual void run();

#else
	virtual bool start(int periodTime=1000) { return false; }
#endif // DEBUG
};

#ifdef DEBUG
extern iProbe probe;
#endif

#endif /* PROBE_H_ */
