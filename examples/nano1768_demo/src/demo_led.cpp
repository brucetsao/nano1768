/*
===============================================================================
 Name        : demo_led.cpp
 Author      : Jason
 Version     : 1.0.2
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : On board debug LED demo
 History	 :
 	 2012/3/29,  Change the LED show for CTime,
 	 	 	 	 to show current seconds of system-time
===============================================================================
*/

#include "class/bus.h"
#include "class/time.h"
#include "class/thread.h"
#include "class/socket.h"
#include "class/mailbox.h"

class CLedDemo: public CThread {
protected:
	virtual void run() {
		CBus leds(LED1, LED2, LED3, LED4, END);
		CMailBox mail("LED", sizeof(int));
		int val=0, *pMailVal, remoteVal=0;
		while (1) {
			pMailVal = (int *) mail.wait(200);
			if ( pMailVal!=NULL ) {
				remoteVal = *pMailVal;
			}

			if ( remoteVal==0 ) {
				//  has local IP address be assigned ?
				if ( CSocket::getLinkStatus() ) {	// if network connected?
					val = CTime::now() % 10;	// get current second (0~9)
					leds = val;
				} else {
					val = (val+1) & 0x0F;
					leds = val;
				}
			// remote control the DBG LED
			} else {
				for (int j=0; j<leds.count(); j++) {
					if ( bit_chk( remoteVal, j) ) {
						leds[j] = LED_ON;
					} else {
						leds[j] = LED_OFF;
					}
				}
			}
		}
	}
};

