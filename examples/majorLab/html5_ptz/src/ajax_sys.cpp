/*
===============================================================================
 Name        : ajax_ptz.cpp
 Author      : Jason
 Version     : 1.0.0
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 License 	 : CC BY-SA 3.0
 Description : Ajxa Demo
 History	 :
===============================================================================
*/
#include "class/ajax.h"
#include "class/string.h"
#include "sys/sysusage.h"

#define CGI_INDEX_SYS		0

// Ajax for Temperature & Humidity
class CAjaxSYS: public CAjax {
public:
	CAjaxSYS() {
		// CGI Demo
		add("/ajax/sysinfo", CGI_INDEX_SYS);

		// start Sys Usage trace
		sys.start();
	}

	//
	// On Ajax XHTML request Event
	//
	virtual void onRequest(const int index, CString &result) {

		switch(index) {
		case CGI_INDEX_SYS:
			result.printf("%d, %d, %d",
					(35000-heapAvailableSize())*100/35000,	// MAX user's memory=35KB
					sys.m_cpu,
					sys.m_net);
			break;

		default:
			result = ajaxNG;
		}
	}

protected:
	SysUsage sys;
};

