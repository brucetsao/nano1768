/*
===============================================================================
 Name        : demo_echo.cpp
 Author      : Jason
 Version     : 1.0.1
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : Console + USB CDC Serial stream demo
 History	 :
===============================================================================
*/
#include "class/thread.h"
#include "class/serial.h"
#include "class/console.h"

class CEchoDemo: public CThread {
protected:
	virtual void run(){
		char ch;
		CSerial cdc(USB);
		Console con(cdc, cdc);

		while(cdc.isConnected()==false){
			sleep(100);
		}
		con << "Hello World!!" << endl;

		while(1){
			con >> ch;
			con << ch;
		} // */
	}
};
