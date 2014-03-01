/*
 ===============================================================================
 Name        : interrupt.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/9/5
 Copyright   : 2012 (C) Embeda Technology Inc.
 License	 : CC BY-SA 3.0
 Description : extern interrupt
 History	 :

 ===============================================================================
 */
#include "arduino/interrupt.h"

CInterrupt::~CInterrupt() {
	m_int.end();	// disable the interrupt
}

void CInterrupt::run() {
	while(1){
		m_int.wait();	// wait interrupt
		m_func();		// call use's interrupt function
	}
}
