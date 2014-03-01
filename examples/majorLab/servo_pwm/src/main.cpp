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

#include "class/pwm.h"
#include "class/serial.h"
#include "class/console.h"

//
// Servo Specifications
//

#define SERVO_MAX_DEG	 	60.0f
#define SERVO_MIN_DEG	 	-60.0f

#define SERVO_MAX_PWM		0.0021f
#define SERVO_MIN_PWM		0.0009f
#define SERVO_CENTER_PWM	0.0015f

#define SERVO_PWM_PERIOD	0.02f	// 20ms
#define SERVO_PPD			((SERVO_MAX_PWM-SERVO_MIN_PWM)/(SERVO_MAX_DEG-SERVO_MIN_DEG))

//
// Servo Motor Class
//
class CServo: public CPwm {
public:
	CServo(PWM_CH_T ch) :
		CPwm(ch) {
		period(SERVO_PWM_PERIOD);
		start();
	}

	// set the Servo position
	void position(float degress) {
		if (degress >= SERVO_MIN_DEG && degress <= SERVO_MAX_DEG) {
			float pwm = SERVO_CENTER_PWM + (SERVO_PPD * degress);
			pulseWidth(pwm);
		}
	}
};

/* ==============================================
 main task routine
 ============================================== */
int main(void) {
	pool_memadd((uint32_t) pool, sizeof(pool));

	CSerial cdc(USB);
	Console con(cdc, cdc);

	CServo servo(PWM1);
	float degress = 0;

	servo.begin();
	servo.position(degress);

	// Enter an endless loop
	while (1) {
		if (cdc.isConnected()) {
			while (cdc.readable()) {
				switch (con.getchar()) {
				case '+':
					if ((degress + 10) <= SERVO_MAX_DEG) {
						degress += 5;
					}
					break;
				case '-':
					if ((degress - 10) >= SERVO_MIN_DEG) {
						degress -= 5;
					}
					break;
				case '0':
					degress = 0;
					break;
				}
			}
			servo.position(degress);
		}
	}
	return 0;
}
