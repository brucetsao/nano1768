/*
 ===============================================================================
 Name        : Firmata.cpp
 Author      : Jason
 Version     :
 Date		 : 2012/5/13
 Description :
 History     :
 ===============================================================================
 */

/*
 Firmata.cpp - Firmata library
 Copyright (C) 2006-2008 Hans-Christoph Steiner.  All rights reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 See file LICENSE.txt for further informations on licensing terms.
 */

//******************************************************************************
//* Includes
//******************************************************************************

#include "class/stream.h"
#include "class/pin.h"
#include "class/bus.h"
#include "arduino\Firmata.h"

#include "debug.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

//******************************************************************************
//* Board Define
//******************************************************************************
#define VERSION_BLINK_PIN	LED4

//******************************************************************************
//* Support Functions
//******************************************************************************
void FirmataClass::sendValueAsTwo7bitBytes(int value) {
	stream->write(value & 0x7F);
	stream->write((value >> 7) & 0x7F);
} // */

void FirmataClass::startSysex() {
	stream->write(START_SYSEX);
}

void FirmataClass::endSysex() {
	stream->write(END_SYSEX);
}

//******************************************************************************
//* Constructors
//******************************************************************************

FirmataClass::FirmataClass(void) {
	firmwareVersionCount = 0;
	systemReset();
}

//******************************************************************************
//* Public Methods
//******************************************************************************

/* begin method for overriding default serial bitrate */
void FirmataClass::begin(CStream *s) {
	//  begin(57600);
	stream = s;
	blinkVersion();
	sleep(300);
	printVersion();
	printFirmwareVersion();
}

/* begin method for overriding default serial bitrate
 void FirmataClass::begin(long speed)
 {
 #if defined(__AVR_ATmega128__)  // Wiring
 Serial.begin((uint32_t)speed);
 #else
 Serial.begin(speed);
 #endif
 blinkVersion();
 sleep(300);
 printVersion();
 printFirmwareVersion();
 } // */

// output the protocol version message to the serial port
void FirmataClass::printVersion(void) {
	byte pk[] =
			{ REPORT_VERSION, FIRMATA_MAJOR_VERSION, FIRMATA_MINOR_VERSION };
	stream->write(pk, sizeof(pk));
}

void FirmataClass::blinkVersion(void) {
	// flash the pin with the protocol version
	//  pinMode(VERSION_BLINK_PIN,OUTPUT);
	pin13strobe(FIRMATA_MAJOR_VERSION, 200, 400);
	sleep(300);
	pin13strobe(2, 1, 4); // separator, a quick burst
	sleep(300);
	pin13strobe(FIRMATA_MINOR_VERSION, 200, 400);
}

void FirmataClass::printFirmwareVersion(void) {
	byte i;

	if (firmwareVersionCount) { // make sure that the name has been set before reporting
		startSysex();
		stream->write(REPORT_FIRMWARE);
		stream->write(firmwareVersionVector, 2);
		for (i = 2; i < firmwareVersionCount; ++i) {
			sendValueAsTwo7bitBytes(firmwareVersionVector[i]);
		}
		endSysex();
	}
}

void FirmataClass::setFirmwareNameAndVersion(const char *name, byte major,
		byte minor) {
	/*
	 const char *filename;
	 char *extension;

	 // parse out ".cpp" and "applet/" that comes from using __FILE__
	 extension = strstr(name, ".cpp");
	 filename = strrchr(name, '/') + 1; //points to slash, +1 gets to start of filename
	 // add two bytes for version numbers
	 if (extension && filename) {
	 firmwareVersionCount = extension - filename + 2;
	 } else {
	 firmwareVersionCount = strlen(name) + 2;
	 filename = name;
	 }  // */

	firmwareVersionCount = strlen(name) + 3;

	firmwareVersionVector = (byte *) malloc(firmwareVersionCount);
	firmwareVersionVector[firmwareVersionCount] = 0;
	firmwareVersionVector[0] = major;
	firmwareVersionVector[1] = minor;
	strncpy((char*) firmwareVersionVector + 2, name, firmwareVersionCount - 2);
	// alas, no snprintf on Arduino
	//    snprintf(firmwareVersionVector, MAX_DATA_BYTES, "%c%c%s",
	//             (char)major, (char)minor, firmwareVersionVector);
}

//------------------------------------------------------------------------------
// Serial Receive Handling

int FirmataClass::available(void) {
	return stream->readable();
}

void FirmataClass::processSysexMessage(void) {
	switch (storedInputData[0]) { //first byte in buffer is command
	case REPORT_FIRMWARE:
		printFirmwareVersion();
		break;
	case STRING_DATA: {
		byte bufferLength = (sysexBytesRead - 1) / 2;
		char *buffer = (char*) malloc(bufferLength * sizeof(char));
		byte i = 1;
		byte j = 0;
		while (j < bufferLength) {
			buffer[j] = (char) storedInputData[i];
			i++;
			buffer[j] += (char) (storedInputData[i] << 7);
			i++;
			j++;
		}
		currentString(buffer);
	}
		break;
	default:
		currentSysex(storedInputData[0], sysexBytesRead - 1,
				storedInputData + 1);
	}
}

void FirmataClass::processInput(void) {
	uint8_t inputData = stream->read(); // this is 'int' to handle -1 when no data
	int command;

	// TODO make sure it handles -1 properly

	if (parsingSysex) {
		if (inputData == END_SYSEX) {
			//stop sysex byte
			parsingSysex = false;
			//fire off handler function
			processSysexMessage();
		} else {
			//normal data byte - add to buffer
			storedInputData[sysexBytesRead] = inputData;
			sysexBytesRead++;
		}
	} else if ((waitForData > 0) && (inputData < 128)) {
		waitForData--;
		storedInputData[waitForData] = inputData;
		if ((waitForData == 0) && executeMultiByteCommand) { // got the whole message
dbg.println(executeMultiByteCommand);
			switch (executeMultiByteCommand) {
			case ANALOG_MESSAGE:
				currentAnalog(multiByteChannel,	(storedInputData[0] << 7) + storedInputData[1]);
				break;

			case DIGITAL_MESSAGE:
				currentDigital(multiByteChannel, (storedInputData[0] << 7) + storedInputData[1]);
				break;

			case SET_PIN_MODE:
				currentPinMode(storedInputData[1], storedInputData[0]);
				break;
			case REPORT_ANALOG:
				currentReportAnalog(multiByteChannel,storedInputData[0]);
				break;
			case REPORT_DIGITAL:
				currentReportDigital(multiByteChannel,storedInputData[0]);
				break;
			}
			executeMultiByteCommand = 0;
		}
	} else {
		// remove channel info from command byte if less than 0xF0
		if (inputData < 0xF0) {
			command = inputData & 0xF0;
			multiByteChannel = inputData & 0x0F;
		} else {
			command = inputData;
			// commands in the 0xF* range don't use channel data
		}

		switch (command) {
		case ANALOG_MESSAGE:
		case DIGITAL_MESSAGE:
		case SET_PIN_MODE:
			waitForData = 2; // two data bytes needed
			executeMultiByteCommand = command;
			break;
		case REPORT_ANALOG:
		case REPORT_DIGITAL:
			waitForData = 1; // two data bytes needed
			executeMultiByteCommand = command;
			break;
		case START_SYSEX:
			parsingSysex = true;
			sysexBytesRead = 0;
			break;
		case SYSTEM_RESET:
			systemReset();
			break;
		case REPORT_VERSION:
			printVersion();
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Serial Send Handling

// send an analog message
void FirmataClass::sendAnalog(byte pin, int value) {
	byte data[3];

	data[0] = (ANALOG_MESSAGE | (pin & 0xF));
	data[1] = value & 0x7F;
	data[2] = (value >> 7) & 0x7F;

	// pin can only be 0-15, so chop higher bits
	stream->write(data, sizeof(data));
}

// send a single digital pin in a digital message
void FirmataClass::sendDigital(byte pin, int value) {
	/* TODO add single pin digital messages to the protocol, this needs to
	 * track the last digital data sent so that it can be sure to change just
	 * one bit in the packet.  This is complicated by the fact that the
	 * numbering of the pins will probably differ on Arduino, Wiring, and
	 * other boards.  The DIGITAL_MESSAGE sends 14 bits at a time, but it is
	 * probably easier to send 8 bit ports for any board with more than 14
	 * digital pins.
	 */

	// TODO: the digital message should not be sent on the serial port every
	// time sendDigital() is called.  Instead, it should add it to an int
	// which will be sent on a schedule.  If a pin changes more than once
	// before the digital message is sent on the serial port, it should send a
	// digital message for each change.

	//    if(value == 0)
	//        sendDigitalPortPair();
}

// send 14-bits in a single digital message (protocol v1)
// send an 8-bit port in a single digital message (protocol v2)
void FirmataClass::sendDigitalPort(byte portNumber, int portData) {
	byte data[3];

	data[0] = (DIGITAL_MESSAGE | (portNumber & 0xF));
	data[1] =  portData & 0x7F;
	data[2] = (portData >> 7) & 0x7F;

	stream->write(data, sizeof(data));
}

void FirmataClass::sendSysex(byte command, byte bytec, byte* bytev) {
	byte i;
	startSysex();
	stream->write(command);
	for (i = 0; i < bytec; i++) {
		sendValueAsTwo7bitBytes(bytev[i]);
	}
	endSysex();
}

void FirmataClass::sendString(byte command, const char* string) {
	sendSysex(command, strlen(string), (byte *) string);
}

// send a string as the protocol string type
void FirmataClass::sendString(const char* string) {
	sendString(STRING_DATA, string);
}

//******************************************************************************
//* Private Methods
//******************************************************************************


// resets the system state upon a SYSTEM_RESET message from the host software
void FirmataClass::systemReset(void) {
	byte i;

	waitForData = 0; // this flag says the next serial input will be data
	executeMultiByteCommand = 0; // execute this after getting multi-byte data
	multiByteChannel = 0; // channel data for multiByteCommands


	for (i = 0; i < MAX_DATA_BYTES; i++) {
		storedInputData[i] = 0;
	}

	parsingSysex = false;
	sysexBytesRead = 0;

	currentSystemReset();
	//flush(); //TODO uncomment when Firmata is a subclass of HardwareSerial
}

// =============================================================================
// used for flashing the pin for the version number
void FirmataClass::pin13strobe(int count, int onInterval, int offInterval) {
	byte i;
	//  pinMode(VERSION_BLINK_PIN, OUTPUT);
	CPin led(VERSION_BLINK_PIN);

	for (i = 0; i < count; i++) {
		sleep(offInterval);
		led = HIGH;
		sleep(onInterval);
		led = LOW;
	}
}

// make one instance for the user to use
//FirmataClass Firmata;

