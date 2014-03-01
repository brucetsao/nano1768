/*
===============================================================================
 Name        : demo_spi.cpp
 Author      : Jason
 Version     : 1.0.1
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : CSpiMaster and CAdc + CMsgBox Demo
			 : The Demo is for Embedded Artisis base board
 History	 :
===============================================================================
*/
#include "class/thread.h"
#include "class/mailbox.h"
#include "class/spi.h"
#include "class/adc.h"

const uint8_t segLED[] = {
	0x24, // 0
	0x7D, // 1
	0xE0, // 2
	0x70, // 3
	0x39, // 4
	0x32, // 5
	0x22, // 6
	0x7C, // 7
	0x20, // 8
	0x30, // 9
};

#define SPI_MAILBOX	"DISPLAY"

class CSpiDemo: public CThread {
public:
	virtual void run() {
		int *val;
		CMailBox mail(SPI_MAILBOX, sizeof(val));
		CSpiMaster spi(SPI0, P24);
		spi.format(SPI_DATABIT_8, SPI_MODE_0);
		spi.frequency(MHZ(20));
		spi.begin();
		while(1) {
			val = (int *) mail.wait();
			if ( val ) {
				spi << segLED[*val];
			}
		}
	}
};

class CAdcDemo: public CThread {
public:
	virtual void run() {
		int  val, last=0;
		CAdc adc(AD0);
		adc.begin();
		while(1) {
			val = (int) (adc.read(0.2, 3) / 409.5);
			if ( val!=last ) {
				CMailBox::post(SPI_MAILBOX, &val);
				last = val;
			}
		}
	}
};
