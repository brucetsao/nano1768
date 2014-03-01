/*
===============================================================================
 Name        : main.cpp
 Author      : USER
 Version     : 1.0.1
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : main definition
 History	 :
===============================================================================
*/
#include "uCXpresso.h"

/* ==============================================
	user adjustable pool memory
   ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

// User include files here
#include "class/adc.h"
#include "servo.h"

#include "debug.h"
CDebug dbg(DBG_USB);

//
#define VR 10.0f	// VR=10K ohm
#define R1 10.0f	// R1=10K ohm
#define MAX_VALUE	((MAX_ADC_VALUE * VR)/(VR+R1))

//
// AD to Degrees
//
float AD2Degrees(int v) {
	float degress = v * (SERVO_DEGREES_RANGE / MAX_VALUE);
	degress += SERVO_MIN_DEG;
	return degress;
}

/* ==============================================
	main task routine
   ============================================== */
int main(void) {
	pool_memadd((uint32_t)pool, sizeof(pool));

#ifdef DEBUG
	dbg.start();
#endif
	int val;
	float degrees;

	CAdc vr(AD5);		// use A5 for VR
	CServo servo(PWM1);	// use PWM1 for Servo Motor

	vr.begin();
	servo.begin();

	// Enter an endless loop
	while(1){
 	 	 val = vr.read(0.1, 8);
 	 	 degrees = AD2Degrees(val);
 	 	 servo = degrees;

#ifdef DEBUG
 	 	 if ( dbg.isDebugMode() ) {
 	 		 dbg.println("ADC=%d, Degrees=%f\n", val, degrees);
 	 	 }
#endif
 	 	 sleep(100);
	}
	return 0 ;
}
