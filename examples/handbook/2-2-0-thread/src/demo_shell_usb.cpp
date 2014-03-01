/*
 ===============================================================================
 Name        : demo_shell.cpp
 Author      : Jason
 Version     : 1.0.1
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : CSocket server Demo
 History	 :
 ===============================================================================
 */
#include "class/thread.h"
#include "class/serial.h"
#include "class/shell.h"

class CShellDemoUSB: public CThread {
protected:
	virtual void run() {
		CSerial cdc(USB);
		CShell  shell(cdc);

		while (1) {
			if ( cdc.isConnected() ) {
				shell.run();
			} else {
				sleep(100);
			}
		}
	}
};
