/*
===============================================================================
 Name        : sysusage.h
 Author      : Jason
 Version     : 1.0.0
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 License 	 : CC BY-SA 3.0
 Description : check CPU usage
 History	 :
===============================================================================
*/

#ifndef SYSUSAGE_H_
#define SYSUSAGE_H_

#include "class/thread.h"

class SysUsage: public CThread {
public:
	int	m_cpu;
	int	m_net;
	virtual bool start();

protected:
	uint32_t	m_lstCPU;
	uint32_t	m_lstNET;
	virtual void run();
};

#endif /* CPUSAGE_H_ */
