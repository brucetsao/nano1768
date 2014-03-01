/*
 ===============================================================================
 Name        : MyFirmata.cpp
 Author      : Jason
 Date		 : 2012/5/13
 Version     : 1.0.0
 Copyright   : Embeda Technology Inc.
 License	 : CC BY-SA 3.0
 Description : FirmataClass Extension
 History	 :
 ===============================================================================
 */
#include "class/adc.h"
#include "class/pwm.h"
#include "class/bus.h"
#include "servo/servo.h"
#include "MyFirmata.h"
#include "debug.h"

//
// Global Peripheral
//
static ADCBus *adcs = NULL;
static CPwm *pwms = NULL;
static CServo *servo = NULL;


static CBus port0(
		P27, 	// 0
		P28, 	// 1
		P26, 	// 2 , PWM1
		P25, 	// 3 , PWM2
		P24, 	// 4 , PWM3
		P23, 	// 5 , PWM4
		P22, 	// 6 , PWM5
		P21, 	// 7 , PWM6
		END);

static CBus port1(
		P9, 	// 8
		P10, 	// 9
		P8, 	// 10
		P5, 	// 11
		P6, 	// 12
		P7, 	// 13
		END);

static CBus port2(
		P15, 	// A0
		P16, 	// A1
		P17, 	// A2
		P18, 	// A3
		P19, 	// A4
		P20, 	// A5
		END);


CMyFirmata::CMyFirmata() {
	int i;

	setFirmwareNameAndVersion("MY-FIRMATA", uCXpresso_VER_MAJOR,
			uCXpresso_VER_MINOR);

	analogInputsToReport = 0;
	for (i = 0; i < TOTAL_PORTS; i++) {
		portStatus[i] = 0;
		reportPINs[i] = false;
	}

	// initlalize global A/D Channel
	if ( adcs==NULL ) {
		adcs = new ADCBus(AD0, AD1, AD2, AD3, AD4, AD5, END);
		adcs->enable();
	}

	// initlailze global PWM channel
	if (pwms == NULL) {
		pwms = new CPwm[TOTAL_PWM_PINS];
		CPwm::period(PWM_PEIROD); // set PWM period = 20ms
		CPwm::start();
	}

	if ( servo == NULL ) {
		servo = new CServo[2];
		servo[0].assign(PWM5, &SERVO_MG_995);
		servo[0].begin();
		servo[1].assign(PWM6, &SERVO_RB_421);
		servo[1].begin();
	}
}

void CMyFirmata::chkLoop() {
	if ( available() > 0) {
		processInput();
	}
}


void CMyFirmata::outLoop() {
	for (int analogPin = 0; analogPin < TOTAL_ANALOG_PINS; analogPin++) {
		if (analogInputsToReport & (1 << analogPin)) {
			sendAnalog(analogPin, adcs->value(analogPin) / 4);
		}
	}
	checkDigitalInputs();
}

void CMyFirmata::outputPort(byte port, uint32_t val) {
	if ( port<TOTAL_PORTS ) {
		val = val & ~portStatus[port];
		if (previousPINs[port] != val ) {
			sendDigitalPort(port, val);
			previousPINs[port] = val;
		}
	}
}

/* -----------------------------------------------------------------------------
 * check all the active digital inputs for change of state, then add any events
 * to the Serial output queue using Serial.print() */
void CMyFirmata::checkDigitalInputs(void) {

	for (int i = 0; i < TOTAL_PORTS; i++) {
		if (reportPINs[i]) {
			switch (i) {
			case 0:
				outputPort(0, port0.read());
				break;
			case 1:
				outputPort(1, port1.read());
				break;
			case ANALOG_PORT:
				outputPort(ANALOG_PORT, port2.read());
				break;
			}
		}
	}
}

/* override virtual callback functions */
void CMyFirmata::currentAnalog(byte pin, int val) {
	float tmp;

	if ( pin>=PWM_PIN_START && pin<=7 ) {
		switch(pin) {
		case 6:	// servo motor
			tmp = ((val / 255.0f) * servo[0]->total_degrees) + servo[0]->min_degrees;
			servo[0] = tmp;
			break;
		case 7:	// servo motor
			tmp = ((val / 255.0f) * servo[1]->total_degrees) + servo[1]->min_degrees;
			servo[1] = tmp;
			break;
		default: // PWM1-5
			// change val (0~255)to duty cycle 0%~100%
			tmp = val / 255.0f;
			pwms[pin-2].dutyCycle(tmp);
		}
	}
}

void CMyFirmata::currentDigital(byte port, int val) {
	switch (port) {
	case 0: // port 0 = 2~7
		port0 = val;
		break;
	case 1: // port 1 = 8~13
		port1 = val;
		break;
	case ANALOG_PORT:
		port2 = val;
		break;
	}
}

void CMyFirmata::currentReportAnalog(byte pin, int val) {
	if (val == 0) {
		analogInputsToReport = analogInputsToReport & ~(1 << pin);
	} else { // everything but 0 enables reporting of that pin
		analogInputsToReport = analogInputsToReport | (1 << pin);
	}
}

void CMyFirmata::currentReportDigital(byte port, int val) {
	if ( port<TOTAL_PORTS ) {
		reportPINs[port] = (byte) val;

		/* turn off analog reporting when used as digital
		if (port == ANALOG_PORT) {
			analogInputsToReport = 0;
		} // */
	}
}

void CMyFirmata::currentPinMode(byte pin, int val) {
	byte port = 0;
	byte offset = 0;
	CPin *Pin = NULL;

	if (pin < 8) {
		port = 0;
		offset = 0;
		Pin = &port0[pin];
	} else if (pin < 14) {
		port = 1;
		offset = 8;
		Pin = &port1[pin - offset];

	} else if (pin < 22) {
		port = 2;
		offset = 14;
		Pin = &port2[pin - offset];
	}

	switch (val) {

	case FM_INPUT:
		if ( Pin ) {
			Pin->input(); 	// set as input mode
			portStatus[port] = portStatus[port] &~ (1 << (pin - offset));
		}
		break;

	case FM_OUTPUT:
		if ( Pin ) {
			Pin->output(NOT_OPEN); // set as output mode
			portStatus[port] = portStatus[port] | (1 << (pin - offset));
		}
		break;

	case FM_PWM:
		if ( pin >= PWM_PIN_START && pin <= PWM_PIN_END ) {
			pwms[pin - 2].assign((PWM_CH_T) (pin - 1));
			pwms[pin - 2].begin();
		}
		break;

	case FM_ANALOG:
	case FM_SERVO:
	case FM_SHIFT:
	case FM_I2C:
	default:
		sendString("");
		break;
	}
}

void CMyFirmata::currentSystemReset() {
}

void CMyFirmata::currentString(LPTSTR str) {
}

void CMyFirmata::currentSysex(byte command, byte argc, byte*argv) {
}
