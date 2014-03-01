/*
 ===============================================================================
 Name        : pwm.h
 Author      : Jason
 Version     : 1.0.1
 Created on	 : 2012/1/21
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : 
 History	 :
 ===============================================================================
 */

#ifndef PWM_H_
#define PWM_H_

#include "class/peripheral.h"

/*	\enum PWM_CH_T
 *
 */
typedef enum {
	PWM1 = 1,
	PWM2,
	PWM3,
	PWM4,
	PWM5,
	PWM6
}PWM_CH_T;

/*!	\class CPwm pwm.h "class/pwm.h"
 *	\brief Pulse-width modulated output.
 */
class CPwm: public CPeripheral {
public:
	/**Constructs a CPwm object to connect to the specified PWM channel.
	 * \param ch ... are PWM_CH_T to specified a PWM channel to the object.
	 *
	 * \code
	 * Example:
	 * 		CPwm::period(0.02);	// Set PWM period time = 20ms
	 * 		CPwm::start();		// Start the core PWM peripheral
	 *
	 * 		CPwm servo(PWM1);	// create a servo objecj
	 * 		servo.dutyCycle(80);// set servo object to 80% dutyCycle
	 * 		servo.begin();		// begin the servo PWM output
	 * \endcode
	 *
	 * \remark to 'END' of the pin arguments is MUST!!
	 */
	CPwm(PWM_CH_T ch);

	/**Begin the PWM channel output
	 */
	virtual void begin();
	inline void enable() { begin(); }

	/**End of the PWM channel output
	 */
	virtual void end();
	inline void disable() { end(); }

	/**Set the output duty-cycle, specified as a percentage (float)
	 */
	virtual void dutyCycle(float percentage);  	// 1.0=100% 0.5=50% ...

	/**Set the PWM pulse-width, specified in seconds (float)
	 */
	virtual void pulseWidth(float sec);			// 0.5=500ms 0.0005=500us ...

	/*Return the current output duty-cycle setting, measured as a percentage (float)
	 */
	virtual float read();

	/**Set output duty-cycle; inline function call to the member dutyCycle()
	 */
	inline void write(float percentage) {
		dutyCycle(percentage);
	}

	/**A shorthand to call the member pulseWidth()
	 */
	inline void operator = (float sec) {
		pulseWidth(sec);
	}

	/*A shorthand to retrieve the current output duty-cycle.
	 */
	inline operator float() {
		return read();
	}

	/**A static member function.
	 * Set the PWM period, specified in seconds (float).
	 */
	static void period(float sec);

	/**Power On the core PWM peripheral.
	 * 2014/1/1, the PWM of core will start automatically after set the period time.
	 */
	static void start();

	/**Power Off the core PWM peripheral.
	 */
	static void stop();

	/*! \cond PRIVATE */
	CPwm();
	virtual ~CPwm();
	virtual void assign(PWM_CH_T ch);
	static void frequency(uint32_t f);
protected:
	PWM_CH_T m_ch;
	uint32_t m_nPeriod;
	/*! \endcond */
};

#endif /* PWM_H_ */
