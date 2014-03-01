/*
===============================================================================
 Name        : mutex.h
 Author      : Jason
 Version     : v1.0.3
 Date		 : 2014/1/1
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/4/19	v1.0.1 	rename PIN_LEVEL_T: LO to LOW, HI to HIGH		Jason
 2012/9/18	v1.0.2 	set output default to NOT_OPEN (provide power)
 2014/1/1	v1.0.3  Add END in PIN_NAME_T enumeration				Jason
 ===============================================================================
 */

#ifndef PIN_H_
#define PIN_H_

#include "class/peripheral.h"

/*	\enum PIN_NAME_T
 */
typedef enum {
	END		= 0,
	LED1 	= 1,
	LED2 	= 2,
	LED3 	= 3,
	LED4 	= 4,
	P5 		= 5,
	P6 		= 6,
	P7 		= 7,
	P8		= 8,
	P9		= 9,
	P10		= 10,
	P11		= 11,
	P12		= 12,
	P13		= 13,
	P14		= 14,
	P15		= 15,
	P16		= 16,
	P17		= 17,
	P18		= 18,
	P19		= 19,
	P20		= 20,
	P21		= 21,
	P22		= 22,
	P23		= 23,
	P24		= 24,
	P25		= 25,
	P26		= 26,
	P27		= 27,
	P28		= 28,
	P29		= 29,
	P30		= 30,
}PIN_NAME_T;

/*
When PIN as an input, the PIN_MODE control the input mode of all ports.
This includes the use of the on-chip pull-up/pull-down resistor feature
and a special open drain operating mode.
*/
typedef enum PIN_INPUT_MODE {
	// Pin has an on-chip pull-up resistor enabled
	INTERNAL_PULL_UP = 0,
	// enables the pull-up resistor if the pin is at a logic high
	// enables the pull-down resistor if the pin is at a logic low
	REPEATER = 1,
	// Pin has neither pull-up nor pull-down resistor enabled
	NEITHER	= 2,
	// Pin has an on-chip pull-down resistor enabled
	INTERNAL_PULL_DOWN = 3
}PIN_INPUT_MODE_T;

/*
When pin as an output, the open drain mode causes the pin to be pulled low normally
if it is configured as an output and the data value is 0.
If the data value is 1, the output drive of the pin is turned off
 */
typedef enum PIN_OUTPUT_MODE {
	// Pin is in the normal (not open drain) mode
	NOT_OPEN = 0,
	// Pin is in the open drain mode.
	OPEN_DRAIN = 1
}PIN_OUTPUT_MODE_T;

typedef enum PIN_LEVEL {
	LOW = 0,
	HIGH = 1,
	LED_OFF = LOW,
	LED_ON = HIGH
}PIN_LEVEL_T;

/*
 *  CPin,
*Constructs:
 *  	p = Pin Number = 5~30
 *  	d = PIN_DIR = OUTPUT or INPUT, Default is OUTPUT
 */
/*!	\class CPin pin.h "class/pin.h"
 *	\brief pin output/input control.
 */
class CPin: public CPeripheral {
public:
	/**Constructs a CPin object to connect to the specified pin.
	 * \param p ... are PIN_NAME_T to specified a pin to the object.
	 *
	 * \code
	 * Example:
	 * 		CPin key(P21);		// Create a key object to connect to pin 21.
	 * 		key.input();		// set the key object as a input.
	 *
	 * 		if ( key==LOW ) {	// check the pin level
	 *
	 * 		}
	 * \endcode
	 *
	 * \remark to 'END' of the pin arguments is MUST!!
	 */
	CPin(PIN_NAME_T p);

	virtual void output(PIN_OUTPUT_MODE_T mode=NOT_OPEN);		// set as an output pin
	virtual void input(PIN_INPUT_MODE_T mode=INTERNAL_PULL_UP);	// set as an input pin

	virtual void write(PIN_LEVEL_T val);	// write new pin value (for output)
	virtual PIN_LEVEL_T  read();			// read current pin value (for input)
	virtual void invert();					// invert current pin output value (for output)

	// a shorthand operator
	inline void operator =(PIN_LEVEL_T val) { this->write(val); }
	inline void operator =(CPin &pin) { this->write(pin.read());}
	inline operator PIN_LEVEL_T() { return read(); }
	inline PIN_LEVEL_T  operator !() { return (read()==HIGH? LOW : HIGH); }
	inline bool operator ==(PIN_LEVEL_T val) { return (this->read()==val ? true : false); }
	inline bool operator !=(PIN_LEVEL_T val) { return (this->read()!=val ? true : false); }

	/*! \cond PRIVATE */
	CPin();
	virtual ~CPin();
	virtual void assign(PIN_NAME_T p);		// assign pin number to CPin

protected:
	int m_nPin;
	/*! \endcond */
};

#endif /* PIN_H_ */
