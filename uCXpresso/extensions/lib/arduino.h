/*
 ===============================================================================
 Name        : arduino.h
 Author      : Jason
 Version     : 1.0.1
 Date		 : 2012/9/8
 Copyright   : 2012 (C) Embeda Technology Inc.
 License	 : CC BY-SA 3.0
 Description : arduino core library
 History	 :
 	 2012/9/4: remove #define DEFAULT
 ===============================================================================
 */
#ifndef _ARDUINO_H
#define _ARDUINO_H

//#include <stdlib.h>
//#include <math.h>

#include "class/pin.h"
#include "class/adc.h"
#include "class/pinint.h"

#define MAX_DIGITAL_PIN	20
//
// Digital I/O
//
extern PIN_NAME_T _PIN_(int pin);

typedef enum {
	INPUT = 0,
	OUTPUT
}PIN_DIR_T;

extern void pinMode(int pin, PIN_DIR_T mode);
extern void digitalWrite(int pin, int value);
extern int digitalRead(int pin);

//
// Analog I/O
//
#define A0	AD0
#define A1	AD1
#define A2	AD2
#define A3	AD3
#define A4	AD4
#define A5	AD5
#define analogReference(x)			// no effect (only internal 3.3V)
extern int analogRead(ADC_CH_T pin);
extern void analogWrite(int pin, byte value, uint32_t frequency=490);

//
// Advanced I/O
//
typedef enum {
	LSBFIRST = 0,
	MSBFIRST,
}BITORDER_T;

extern void tone(int pin, uint32_t frequency, long duration=0);
extern void noTone(int pin);
extern void shiftOut(int dataPin, int clockPin, BITORDER_T bitOrder, byte value);
extern byte shiftIn(int dataPin, int clockPin, BITORDER_T bitOrder);
extern uint32_t pulseIn(int pin, PIN_LEVEL_T value, uint32_t timeout=0);

//
// Time
//
#include "class/time.h"
#define millis()				CTime::getTickCount(MS)
#define micros()				CTime::getTickCount(US)
extern void delayMicroseconds(uint32_t us);
#define delay(ms)				sleep(ms)

//
// Math
//
#define min(x,y)				MIN(x,y)
#define max(x,y)				MAX(x,y)
#define abs(n)					ABS(n)

#include "arduino/constrain.h"
//#define constrain(x,a,b)	((x<a)?a:(x>b)?b:x)

//
// Random Numbers
//
#define randomSeed(seed)	srand(speed)
#define random(n)			((rand() % n) + 1)

//
// Bits and Bytes
//
#define lowByte(x)			(x & 0x00FF)
#define highByte(x)			((x>>8) & 0x00FF)
#define word(h, l)			((uint16_t)((h<<8) | l))
#define bitRead(x,n)		(bit_chk(x,n))
#define bitWrite(x,n,b)		((b?bit_set(x,n):bit_clr(x,n)))
#define bitSet(x,n)			bit_set(x,n)
#define bitClear(x,n)		bit_clr(x,n)

//
// External Interrupts
//
typedef void(*INT_FUNC)(void);
extern void attachInterrupt(int pin, INT_FUNC function, EDGE_STATE_T edge, int stack=80);
extern void detachInterrupt(int pin);

//
// Debug
//
#include "debug.h"
#define Serial dbg

#endif
