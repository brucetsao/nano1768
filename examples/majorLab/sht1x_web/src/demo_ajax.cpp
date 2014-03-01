/*
===============================================================================
 Name        : demo_ajax.cpp
 Author      : Jason
 Version     : 1.0.1
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : Ajxa (XHTML request) demo
 History	 :
===============================================================================
*/
#include "class/ajax.h"
#include "class/string.h"
#include "class/time.h"
#include "sht1x_task.h"

#include "sys/sysusage.h"

#define CGI_INDEX_TEMP		0

#define SSI_INDEX_DATE		0x20
#define SSI_INDEX_TIME		0x21

#define MAX_USER_POOL		38000

extern CSHT1xTask sht1x;

// Ajax for Temperature & Humidity
class CAjaxDemo: public CAjax {
public:
	CAjaxDemo() {
		// CGI Demo
		add("/ajax/temp", CGI_INDEX_TEMP);

		// SSI Demo
		add("ssiDATE", SSI_INDEX_DATE);
		add("ssiTIME", SSI_INDEX_TIME);

		// start Sys Usage trace
		sys.start();
	}

	//
	// On Ajax XHTML request Event
	//
	virtual void onRequest(const int index, CString &result) {
		CTime tm;

		switch(index) {
		case CGI_INDEX_TEMP:
			result.printf("%0.2f, %0.2f, %d, %d, %d",
					sht1x.temp,
					sht1x.humi,
					(MAX_USER_POOL-heapAvailableSize())*100/MAX_USER_POOL,	// MAX user's memory=35KB
					sys.m_cpu,
					sys.m_net);
			break;

		case SSI_INDEX_DATE:
			tm = CTime::now();
			result.printf("%04d/%02d/%02d",
					tm.getYear(),
					tm.getMonth(),
					tm.getDay()
					);
			break;

		case SSI_INDEX_TIME:
			tm = CTime::now();
			result.printf("%02d:%02d:%02d",
					tm.getHour(),
					tm.getMinute(),
					tm.getSecond()
					);
			break;
		}
	}

protected:
	SysUsage sys;
};

