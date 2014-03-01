/*
===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.1
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : main definition
 History	 :
===============================================================================
*/
#include "demo_shell_net.cpp"
#include "demo_spi.cpp"
#include "demo_led.cpp"
#include "demo_pwm.cpp"
#include "demo_pinint.cpp"
#include "probe/probe.h"
#include "debug.h"

CDebug dbg(DBG_USB);

// Customer pool memory
static uint8_t pool[DEFAULT_POOL_SIZE];	// 16KB for pool

/* ===============================================================================
	main routine
   =============================================================================== */
int main(void) {

	pool_memadd((uint32_t)pool, sizeof(pool));

#ifdef DEBUG
	dbg.start();
	probe.start();
#endif

	CShellDemoNET netSH;	// CSocket shell server demo
	CLedDemo   led;		// nano11768 on board Debug LED

	netSH.start("telnet");
	led.start("led");

	//
	// for Embedded Artisis LPCXpresso Base board
	//
#ifdef ARTISIS
	CPwmDemo   pwm;
	CSpiDemo  spi;
	CAdcDemo  adc;
	CPinIntDemo pinInt;

	spi.start("spi");
	adc.start("adc");
	pwm.start("pwm");
	pinInt.start("pinIRQ");
#endif

	while(1){
 	 	 // do loop routine
	} // */
	return 0 ;
}
