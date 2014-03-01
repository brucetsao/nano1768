/*
 ===============================================================================
 Name        : timer.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2012/5/16
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : 
 History     :
 ===============================================================================
 */

#ifndef _OS_TIMER_H_
#define _OS_TIMER_H_

#include "class/object.h"
#include "class/thread.h"

class osTimer: public CObject {
public:
	osTimer();
	osTimer(long period, bool autoReload=true);
	virtual ~osTimer();

	virtual bool setting(long period, bool autoReload=true);

	virtual bool start(long blockTime=0);
	virtual bool reset(long blockTime=0);
	virtual bool stop(long blockTime=0);
	virtual bool isActive();

protected:
	virtual void onTimer() = PURE_VIRTUAL_FUNC;

private:
	xHandle m_handle;
	static void xTimerHandler(void *handle);
};

#endif /* TIMER_H_ */
