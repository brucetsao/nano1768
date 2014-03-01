/*
 ===============================================================================
 Name        : callback.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2013/12/27
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description : for nano11Uxx-BLE
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 
 ===============================================================================
 */

#include "MyFirmata.h"
#include "class/bus.h"
#include "class/pwm.h"
#include "class/adc.h"

//
// HARDWARE define for UNO.NET
//
#define TOTAL_ANALOG_PINS       6
#define TOTAL_PWM_PINS			6
#define TOTAL_PINS              20 // 14 digital + 6 analog
#ifndef TOTAL_PORTS
#define TOTAL_PORTS             ((TOTAL_PINS + 7) / 8)
#endif

#define IS_PIN_DIGITAL(p)       ((p) >= 2 && (p) <= 19)
#define IS_PIN_ANALOG(p)        ((p) >= 14 && (p) < 14 + TOTAL_ANALOG_PINS)
#define IS_PIN_PWM(p)           ((p) >=2 && (p)<=7)	// PWM1-PWM6
#define PIN_TO_DIGITAL(p)       (p)
#define PIN_TO_ANALOG(p)        ((p) - 14)
#define PIN_TO_PWM(p)           ((p)-2)

#define MAX_QUERIES 				8
#define MINIMUM_SAMPLING_INTERVAL 	10

static CBus unoBus(
		UNO_0, 	// 0 ,
		UNO_1, 	// 1 ,
		UNO_2, 	// 2 , PWM1
		UNO_3, 	// 3 , PWM2
		UNO_4, 	// 4 , PWM3
		UNO_5, 	// 5 , PWM4
		UNO_6, 	// 6 , PWM5
		UNO_7,	// 7 , PWM6
		UNO_8, 	// 8 , SDA
		UNO_9, 	// 9 , SCL
		UNO_10,	// 10, SSEL
		UNO_11, // 11, MOSI
		UNO_12, // 12, MISO
		UNO_13, // 13, SCK
		P15, 	// 14, A0
		P16, 	// 15, A1
		P17, 	// 16, A2
		P18, 	// 17, A3
		P19, 	// 18, A4
		P20, 	// 19, A5
		END);

CPwm pwm[TOTAL_PWM_PINS] = { PWM1, PWM2, PWM3, PWM4, PWM5, PWM6};

//
// Global MyFirmata Object
//
extern MyFirmataClass myFirmata;

void reportAnalogCallback(byte analogPin, int value);

/*==============================================================================
 * GLOBAL VARIABLES
 *============================================================================*/

/* analog inputs */
int analogInputsToReport = 0; // bitwise array to store pin reporting

/* digital input ports */
byte reportPINs[TOTAL_PORTS];       // 1 = report this port, 0 = silence
byte previousPINs[TOTAL_PORTS];     // previous 8 bits sent

/* pins configuration */
byte pinConfig[TOTAL_PINS];         // configuration of every pin
byte portConfigInputs[TOTAL_PORTS]; // each bit: 1 = pin in INPUT, 0 = anything else
int pinState[TOTAL_PINS];           // any value that has been written

int samplingInterval = 38;          // how often to run the main loop (in ms)

//
// Functions
//

void outputPort(byte portNumber, byte portValue, byte forceSend) {
	// pins not configured as INPUT are cleared to zeros
	portValue = portValue & portConfigInputs[portNumber];

	// only send if the value is different than previously sent
	if (forceSend || previousPINs[portNumber] != portValue) {
		myFirmata.sendDigitalPort(portNumber, portValue);
		previousPINs[portNumber] = portValue;
	}
}

void setPinModeCallback(byte pin, int mode) {
	if (IS_PIN_ANALOG(pin)) {
		reportAnalogCallback(PIN_TO_ANALOG(pin), mode == ANALOG ? 1 : 0); // turn on/off reporting
	}

	if (IS_PIN_DIGITAL(pin)) {
		if (mode == INPUT) {
			portConfigInputs[pin / 8] |= (1 << (pin & 7));
		} else {
			portConfigInputs[pin / 8] &= ~(1 << (pin & 7));
		}
	}

	pinState[pin] = 0;
	switch (mode) {
	case ANALOG:
		if (IS_PIN_ANALOG(pin)) {
			if (IS_PIN_DIGITAL(pin)) {
				unoBus[pin].input(NEITHER); // set pin as input & disable internal pull-ups
			}
			pinConfig[pin] = ANALOG;
		}
		break;
	case INPUT:
		if (IS_PIN_DIGITAL(pin)) {
			unoBus[pin].input(INTERNAL_PULL_UP);
			pinConfig[pin] = INPUT;
			// hack it only
			reportPINs[pin / 8] |= (1 << (pin & 7));
		}
		break;
	case OUTPUT:
		if (IS_PIN_DIGITAL(pin)) {
			if ( IS_PIN_PWM(pin) ) {// disable PWM
				pwm[PIN_TO_PWM(pin)].disable();
			}
			unoBus[pin].output();
			pinConfig[pin] = OUTPUT;
		}
		break;
	case PWM:
		if (IS_PIN_PWM(pin)) {
			pwm[PIN_TO_PWM(pin)].enable();
			pwm[PIN_TO_PWM(pin)].pulseWidth(0.0f);
			pinConfig[pin] = PWM;
		}
		break;
	default:
		myFirmata.sendString("Unknown pin mode"); // TODO: put error msgs in EEPROM
	}
	// TODO: save status to EEPROM here, if changed
}

/* check all the active digital inputs for change of state, then add any events
 * to the Serial output queue using Serial.print() */
void checkDigitalInputs(void) {
	/* Using non-looping code allows constants to be given to readPort().
	 * The compiler will apply substantial optimizations if the inputs
	 * to readPort() are compile-time constants. */
	uint32_t value = unoBus.read();

	if (TOTAL_PORTS > 0 && reportPINs[0]) {
		outputPort(0, value & 0xff, false);
	}
	if (TOTAL_PORTS > 1 && reportPINs[1]) {
		outputPort(1, (value>>8) & 0xfF, false);
	}
	if (TOTAL_PORTS > 2 && reportPINs[2]) {
		outputPort(2, (value>>16) & 0xff, false);
	}
}

void analogWriteCallback(byte pin, int value) {
	if (pin < TOTAL_PINS) {
		switch (pinConfig[pin]) {
		case SERVO:
			break;
		case PWM:
			if (IS_PIN_PWM(pin))
				pwm[PIN_TO_PWM(pin)].dutyCycle( map((float)value, 0.0f, 255.0f, 0.0f, 1.0f) );
			pinState[pin] = value;
			break;
		}
	}
}

void digitalWriteCallback(byte port, int value) {
	uint32_t last = unoBus.read();
	switch (port) {
	case 0:
		unoBus = (last & 0xFFFF00) | value;
		break;
	case 1:
		unoBus = (last & 0xFF00FF) | (value<<8);
		break;
	case 2:
		unoBus = (last & 0x00FFFF) | (value<<16);
		break;
	default:
		break;
	}
}

// -----------------------------------------------------------------------------
/* sets bits in a bit array (int) to toggle the reporting of the analogIns
 */
//void FirmataClass::setAnalogPinReporting(byte pin, byte state) {
//}
void reportAnalogCallback(byte analogPin, int value) {
	if (analogPin < TOTAL_ANALOG_PINS) {
		if (value == 0) {
			analogInputsToReport = analogInputsToReport & ~(1 << analogPin);
		} else {
			analogInputsToReport = analogInputsToReport | (1 << analogPin);
		}
	}
	// TODO: save status to EEPROM here, if changed
}

void systemResetCallback() {

	for (byte i = 0; i < TOTAL_PORTS; i++) {
		reportPINs[i] = false;      // by default, reporting off
		portConfigInputs[i] = 0;	// until activated
		previousPINs[i] = 0;
	}

	// pins with analog capability default to analog input
	// otherwise, pins default to digital output
	for (byte i = 0; i < TOTAL_PINS; i++) {

		// skip pin 8, 9 for BLE Shield
		if (i >= 8 && i <= 13)
			continue;

		setPinModeCallback(i, OUTPUT);
	}

	// Enable PWM power
	CPwm::period(0.005); // Set PWM period time = 5ms

	// by default, do not report any analog inputs
	analogInputsToReport = 0;

	/* send digital inputs to set the initial state on the host computer,
	 * since once in the loop(), this firmware will only send on change */
}

/*==============================================================================
 * SYSEX-BASED commands
 *============================================================================*/

void sysexCallback(byte command, byte argc, byte *argv) {
/*	byte mode;
	byte slaveAddress;
	byte slaveRegister;
	byte data;
	unsigned int delayTime; // */

	switch (command) {
	case I2C_REQUEST:
/*
		mode = argv[1] & I2C_READ_WRITE_MODE_MASK;
		if (argv[1] & I2C_10BIT_ADDRESS_MODE_MASK) {
			BleFirmata.sendString(
					"10-bit addressing mode is not yet supported");
			return;
		} else {
			slaveAddress = argv[0];
		}

		switch (mode) {
		case I2C_WRITE:
			Wire.beginTransmission(slaveAddress);
			for (byte i = 2; i < argc; i += 2) {
				data = argv[i] + (argv[i + 1] << 7);
#if ARDUINO >= 100
				Wire.write(data);
#else
				Wire.send(data);
#endif
			}
			Wire.endTransmission();
			delayMicroseconds(70);
			break;
		case I2C_READ:
			if (argc == 6) {
				// a slave register is specified
				slaveRegister = argv[2] + (argv[3] << 7);
				data = argv[4] + (argv[5] << 7);  // bytes to read
				readAndReportData(slaveAddress, (int) slaveRegister, data);
			} else {
				// a slave register is NOT specified
				data = argv[2] + (argv[3] << 7);  // bytes to read
				readAndReportData(slaveAddress, (int) REGISTER_NOT_SPECIFIED,
						data);
			}
			break;
		case I2C_READ_CONTINUOUSLY:
			if ((queryIndex + 1) >= MAX_QUERIES) {
				// too many queries, just ignore
				BleFirmata.sendString("too many queries");
				break;
			}
			queryIndex++;
			query[queryIndex].addr = slaveAddress;
			query[queryIndex].reg = argv[2] + (argv[3] << 7);
			query[queryIndex].bytes = argv[4] + (argv[5] << 7);
			break;
		case I2C_STOP_READING:
			byte queryIndexToSkip;
			// if read continuous mode is enabled for only 1 i2c device, disable
			// read continuous reporting for that device
			if (queryIndex <= 0) {
				queryIndex = -1;
			} else {
				// if read continuous mode is enabled for multiple devices,
				// determine which device to stop reading and remove it's data from
				// the array, shifiting other array data to fill the space
				for (byte i = 0; i < queryIndex + 1; i++) {
					if (query[i].addr = slaveAddress) {
						queryIndexToSkip = i;
						break;
					}
				}

				for (byte i = queryIndexToSkip; i < queryIndex + 1; i++) {
					if (i < MAX_QUERIES) {
						query[i].addr = query[i + 1].addr;
						query[i].reg = query[i + 1].addr;
						query[i].bytes = query[i + 1].bytes;
					}
				}
				queryIndex--;
			}
			break;
		default:
			break;
		}
		break;
	case I2C_CONFIG:
		delayTime = (argv[0] + (argv[1] << 7));

		if (delayTime > 0) {
			i2cReadDelayTime = delayTime;
		}

		if (!isI2CEnabled) {
			enableI2CPins();
		}	// */
		break;

	case SERVO_CONFIG:
/*		if (argc > 4) {
			// these vars are here for clarity, they'll optimized away by the compiler
			byte pin = argv[0];
			int minPulse = argv[1] + (argv[2] << 7);
			int maxPulse = argv[3] + (argv[4] << 7);

			if (IS_PIN_SERVO(pin)) {
				if (servos[PIN_TO_SERVO(pin)].attached())
					servos[PIN_TO_SERVO(pin)].detach();
				servos[PIN_TO_SERVO(pin)].attach(PIN_TO_DIGITAL(pin), minPulse,
						maxPulse);
				setPinModeCallback(pin, SERVO);
			}
		} // */
		break;

	case SAMPLING_INTERVAL:
		if (argc > 1) {
			samplingInterval = argv[0] + (argv[1] << 7);
			if (samplingInterval < MINIMUM_SAMPLING_INTERVAL) {
				samplingInterval = MINIMUM_SAMPLING_INTERVAL;
			}
		} else {
			//Firmata.sendString("Not enough data");
		}
		break;

	case EXTENDED_ANALOG:
		if (argc > 1) {
			int val = argv[1];
			if (argc > 2)
				val |= (argv[2] << 7);
			if (argc > 3)
				val |= (argv[3] << 14);
			analogWriteCallback(argv[0], val);
		}
		break;
	case CAPABILITY_QUERY:
		myFirmata.serial()->write(START_SYSEX);
		myFirmata.serial()->write(CAPABILITY_RESPONSE);
		for (byte pin = 0; pin < TOTAL_PINS; pin++) {
			if (IS_PIN_DIGITAL(pin)) {
				myFirmata.serial()->write((byte) INPUT);
				myFirmata.serial()->write(1);
				myFirmata.serial()->write((byte) OUTPUT);
				myFirmata.serial()->write(1);
			}
			if (IS_PIN_ANALOG(pin)) {
				myFirmata.serial()->write(ANALOG);
				myFirmata.serial()->write(10);
			}
			if (IS_PIN_PWM(pin)) {
				myFirmata.serial()->write(PWM);
				myFirmata.serial()->write(8);
			}
			/*
			if (IS_PIN_SERVO(pin)) {
				myFirmata.serial()->write(SERVO);
				myFirmata.serial()->write(14);
			}
			if (IS_PIN_I2C(pin)) {
				myFirmata.serial()->write(I2C);
				myFirmata.serial()->write(1);  // to do: determine appropriate value
			} // */
			myFirmata.serial()->write(127);
		}
		myFirmata.serial()->write(END_SYSEX);
		break;
	case PIN_STATE_QUERY:
		if (argc > 0) {
			byte pin = argv[0];
			myFirmata.serial()->write(START_SYSEX);
			myFirmata.serial()->write(PIN_STATE_RESPONSE);
			myFirmata.serial()->write(pin);
			if (pin < TOTAL_PINS) {
				myFirmata.serial()->write((byte) pinConfig[pin]);
				myFirmata.serial()->write((byte) pinState[pin] & 0x7F);
				if (pinState[pin] & 0xFF80)
					myFirmata.serial()->write((byte) (pinState[pin] >> 7) & 0x7F);
				if (pinState[pin] & 0xC000)
					myFirmata.serial()->write((byte) (pinState[pin] >> 14) & 0x7F);
			}
			myFirmata.serial()->write(END_SYSEX);
		}
		break;
	case ANALOG_MAPPING_QUERY:
		myFirmata.serial()->write(START_SYSEX);
		myFirmata.serial()->write(ANALOG_MAPPING_RESPONSE);
		for (byte pin = 0; pin < TOTAL_PINS; pin++) {
			myFirmata.serial()->write(IS_PIN_ANALOG(pin) ? PIN_TO_ANALOG(pin) : 127);
		}
		myFirmata.serial()->write(END_SYSEX);
		break;
	}
}

void reportDigitalCallback(byte port, int value) {
	if (port < TOTAL_PORTS) {
		reportPINs[port] = (byte) value;
	}
	// do not disable analog reporting on these 8 pins, to allow some
	// pins used for digital, others analog.  Instead, allow both types
	// of reporting to be enabled, but check if the pin is configured
	// as analog when sampling the analog inputs.  Likewise, while
	// scanning digital pins, portConfigInputs will mask off values from any
	// pins configured as analog
}

void callback_init() {
	myFirmata.attach(ANALOG_MESSAGE, analogWriteCallback);
	myFirmata.attach(DIGITAL_MESSAGE, digitalWriteCallback);
	myFirmata.attach(REPORT_ANALOG, reportAnalogCallback);
	myFirmata.attach(REPORT_DIGITAL, reportDigitalCallback);
	myFirmata.attach(SET_PIN_MODE, setPinModeCallback);
	myFirmata.attach(START_SYSEX, sysexCallback);
	myFirmata.attach(SYSTEM_RESET, systemResetCallback);
	systemResetCallback();
}

//
void checkAnalogInputs() {
	int ch;
	for (int pin=0; pin<TOTAL_PINS; pin++) {
		if ( IS_PIN_ANALOG(pin) && pinConfig[pin]==ANALOG ) {
			ch = PIN_TO_ANALOG(pin);
			if ( analogInputsToReport & (1<<ch) ) {
				CAdc ad((ADC_CH_T)ch);
				ad.enable();
				myFirmata.sendAnalog(ch, ad.read());
			}
		}
	}
}

void reportDigital() {

}
