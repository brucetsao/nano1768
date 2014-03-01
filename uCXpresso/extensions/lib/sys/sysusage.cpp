/*
===============================================================================
 Name        : SysUsage.cpp
 Author      : Jason
 Version     : 1.0.1
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : Ajxa (XHTML request) demo
 History	 :
===============================================================================
*/

#include "sys/sysusage.h"
#include "class/socket.h"
#include "class/timeout.h"

bool SysUsage::start() {
	return CThread::start("CPU", 64, PRI_HIGH);
}

void SysUsage::run() {
	CTimeout to;
	m_cpu = 0;
	m_net = 0;

	while(1) {
		to.wait(1000);	// update m_usage at every 1 second
		to.reset();
		m_cpu = constrain((1000 - OFFSET(CThread::getIdleTickCount(), m_lstCPU, MAX_UINT32))/10, 0, 100);
		m_net = constrain(OFFSET(CSocket::getRxPackageCount(), m_lstNET, MAX_UINT32), 0, 100);

		m_lstCPU = CThread::getIdleTickCount();
		m_lstNET = CSocket::getRxPackageCount();
	}
}
