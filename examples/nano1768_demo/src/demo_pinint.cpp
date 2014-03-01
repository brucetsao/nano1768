/*
 ===============================================================================
 Name        : demo_pinint.cpp
 Author      : Jason
 Version     : 1.0.1
 Created on	 : 2012/3/20
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : 
 History	 :
 ===============================================================================
 */

#include "class/thread.h"
#include "class/pinint.h"
#include "class/bus.h"

class CPinIntDemo: public CThread {
protected:
	virtual void run() {
		CPinINT irq1(P13);
		CPinINT irq2(P14);

		CPin led1(LED1);
		CPin led2(LED2);

		irq1.begin();
		irq2.begin();

		while(1) {
			if ( irq1.wait(0) ) {
				led1.invert();
			}

			if ( irq2.wait(0) ) {
				led2.invert();
			}
		}
	}
};
