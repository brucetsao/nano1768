/*
 ===============================================================================
 Name        : adxl345.cpp
 Author      : http://code.google.com/p/keyglove/
 Created on	 : 2012/4/8
  License	 : MIT
 Description : ANALOG DEVICES, Digital Accelerometer
 History	 :
 	 2012/4/8: Modify by Jason for uCRtm Demo
 ===============================================================================
 */
#include <math.h>
#include "adxl345.h"

#define DEVICE (0x53)    // ADXL345 device address
#define TO_READ (6)      // num of bytes we are going to read each time (two bytes for each axis)

CAdxl345::CAdxl345() {
}

CAdxl345::~CAdxl345() {
}

void CAdxl345::powerOn() {
	begin(); // enable I2C Bus

	//Turning on the ADXL345
	writeTo(DEVICE, ADXL345_POWER_CTL, 0);
	writeTo(DEVICE, ADXL345_POWER_CTL, 16);
	writeTo(DEVICE, ADXL345_POWER_CTL, 8);
}

// Reads the acceleration into three variable x, y and z
void CAdxl345::readAccel(int* x, int* y, int* z) {
	byte _buff[6];
	readFrom(DEVICE, ADXL345_DATAX0, TO_READ, _buff); //read the acceleration data from the ADXL345

	// each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
	// thus we are converting both bytes in to one int
	*x = (((int) _buff[1]) << 8) | _buff[0];
	*y = (((int) _buff[3]) << 8) | _buff[2];
	*z = (((int) _buff[5]) << 8) | _buff[4];
}

// Writes val to address register on device
int CAdxl345::writeTo(int device, byte address, byte val) {
	byte _buff[2];
	_buff[0] = address;
	_buff[1] = val;
	return write(device, _buff, 2);
}

// Reads num bytes starting from address register on device in to _buff array
int CAdxl345::readFrom(int device, byte address, int num, byte *buff) {
	write(device, &address, 1);
	return read(device, buff, num);
}

// Gets the range setting and return it into rangeSetting
// it can be 2, 4, 8 or 16
void CAdxl345::getRangeSetting(byte* rangeSetting) {
	byte _b;
	readFrom(DEVICE, ADXL345_DATA_FORMAT, 1, &_b);
	*rangeSetting = _b & 0x0003;
}

// Sets the range setting, possible values are: 2, 4, 8, 16
void CAdxl345::setRangeSetting(int val) {
	byte _s;
	byte _b;

	switch (val) {
	case 2:
		_s = 0x00;
		break;
	case 4:
		_s = 0x01;
		break;
	case 8:
		_s = 0x02;
		break;
	case 16:
		_s = 0x03;
		break;
	default:
		_s = 0x00;
	}
	readFrom(DEVICE, ADXL345_DATA_FORMAT, 1, &_b);
	_s |= (_b & 0x00EC);
	writeTo(DEVICE, ADXL345_DATA_FORMAT, _s);
}
// gets the state of the SELF_TEST bit
bool CAdxl345::getSelfTestBit() {
	return getRegisterBit(ADXL345_DATA_FORMAT, 7);
}

// Sets the SELF-TEST bit
// if set to 1 it applies a self-test force to the sensor causing a shift in the output data
// if set to 0 it disables the self-test force
void CAdxl345::setSelfTestBit(bool selfTestBit) {
	setRegisterBit(ADXL345_DATA_FORMAT, 7, selfTestBit);
}

// Gets the state of the SPI bit
bool CAdxl345::getSpiBit() {
	return getRegisterBit(ADXL345_DATA_FORMAT, 6);
}

// Sets the SPI bit
// if set to 1 it sets the device to 3-wire mode
// if set to 0 it sets the device to 4-wire SPI mode
void CAdxl345::setSpiBit(bool spiBit) {
	setRegisterBit(ADXL345_DATA_FORMAT, 6, spiBit);
}

// Gets the state of the INT_INVERT bit
bool CAdxl345::getInterruptLevelBit() {
	return getRegisterBit(ADXL345_DATA_FORMAT, 5);
}

// Sets the INT_INVERT bit
// if set to 0 sets the interrupts to active high
// if set to 1 sets the interrupts to active low
void CAdxl345::setInterruptLevelBit(bool interruptLevelBit) {
	setRegisterBit(ADXL345_DATA_FORMAT, 5, interruptLevelBit);
}

// Gets the state of the FULL_RES bit
bool CAdxl345::getFullResBit() {
	return getRegisterBit(ADXL345_DATA_FORMAT, 3);
}

// Sets the FULL_RES bit
// if set to 1, the device is in full resolution mode, where the output resolution increases with the
//   g range set by the range bits to maintain a 4mg/LSB scal factor
// if set to 0, the device is in 10-bit mode, and the range buts determine the maximum g range
//   and scale factor
void CAdxl345::setFullResBit(bool fullResBit) {
	setRegisterBit(ADXL345_DATA_FORMAT, 3, fullResBit);
}

// Gets the state of the justify bit
bool CAdxl345::getJustifyBit() {
	return getRegisterBit(ADXL345_DATA_FORMAT, 2);
}

// Sets the JUSTIFY bit
// if sets to 1 selects the left justified mode
// if sets to 0 selects right justified mode with sign extension
void CAdxl345::setJustifyBit(bool justifyBit) {
	setRegisterBit(ADXL345_DATA_FORMAT, 2, justifyBit);
}

// Sets the THRESH_TAP byte value
// it should be between 0 and 255
// the scale factor is 62.5 mg/LSB
// A value of 0 may result in undesirable behavior
void CAdxl345::setTapThreshold(int tapThreshold) {
	tapThreshold = MIN(MAX(tapThreshold,0),255);
	byte _b = byte(tapThreshold);
	writeTo(DEVICE, ADXL345_THRESH_TAP, _b);
}

// Gets the THRESH_TAP byte value
// return value is comprised between 0 and 255
// the scale factor is 62.5 mg/LSB
int CAdxl345::getTapThreshold() {
	byte _b;
	readFrom(DEVICE, ADXL345_THRESH_TAP, 1, &_b);
	return int(_b);
}

// Sets the OFSX, OFSY and OFSZ bytes
// OFSX, OFSY and OFSZ are user offset adjustments in twos complement format with
// a scale factor of 15,6mg/LSB
// OFSX, OFSY and OFSZ should be comprised between
void CAdxl345::setAxisOffset(int x, int y, int z) {
	writeTo(DEVICE, ADXL345_OFSX, byte(x));
	writeTo(DEVICE, ADXL345_OFSY, byte(y));
	writeTo(DEVICE, ADXL345_OFSZ, byte(z));
}

// Gets the OFSX, OFSY and OFSZ bytes
void CAdxl345::getAxisOffset(int* x, int* y, int*z) {
	byte _b;
	readFrom(DEVICE, ADXL345_OFSX, 1, &_b);
	*x = int(_b);
	readFrom(DEVICE, ADXL345_OFSY, 1, &_b);
	*y = int(_b);
	readFrom(DEVICE, ADXL345_OFSZ, 1, &_b);
	*z = int(_b);
}

// Sets the DUR byte
// The DUR byte contains an unsigned time value representing the maximum time
// that an event must be above THRESH_TAP threshold to qualify as a tap event
// The scale factor is 625µs/LSB
// A value of 0 disables the tap/double tap funcitons. Max value is 255.
void CAdxl345::setTapDuration(int tapDuration) {
	tapDuration = MIN(MAX(tapDuration,0),255);
	byte _b = byte(tapDuration);
	writeTo(DEVICE, ADXL345_DUR, _b);
}

// Gets the DUR byte
int CAdxl345::getTapDuration() {
	byte _b;
	readFrom(DEVICE, ADXL345_DUR, 1, &_b);
	return int(_b);
}

// Sets the latency (latent register) which contains an unsigned time value
// representing the wait time from the detection of a tap event to the start
// of the time window, during which a possible second tap can be detected.
// The scale factor is 1.25ms/LSB. A value of 0 disables the double tap function.
// It accepts a maximum value of 255.
void CAdxl345::setDoubleTapLatency(int doubleTapLatency) {
	byte _b = byte(doubleTapLatency);
	writeTo(DEVICE, ADXL345_LATENT, _b);
}

// Gets the Latent value
int CAdxl345::getDoubleTapLatency() {
	byte _b;
	readFrom(DEVICE, ADXL345_LATENT, 1, &_b);
	return int(_b);
}

// Sets the Window register, which contains an unsigned time value representing
// the amount of time after the expiration of the latency time (Latent register)
// during which a second valud tap can begin. The scale factor is 1.25ms/LSB. A
// value of 0 disables the double tap function. The maximum value is 255.
void CAdxl345::setDoubleTapWindow(int doubleTapWindow) {
	doubleTapWindow = MIN(MAX(doubleTapWindow,0),255);
	byte _b = byte(doubleTapWindow);
	writeTo(DEVICE, ADXL345_WINDOW, _b);
}

// Gets the Window register
int CAdxl345::getDoubleTapWindow() {
	byte _b;
	readFrom(DEVICE, ADXL345_WINDOW, 1, &_b);
	return int(_b);
}

// Sets the THRESH_ACT byte which holds the threshold value for detecting activity.
// The data format is unsigned, so the magnitude of the activity event is compared
// with the value is compared with the value in the THRESH_ACT register. The scale
// factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the
// activity interrupt is enabled. The maximum value is 255.
void CAdxl345::setActivityThreshold(int activityThreshold) {
	activityThreshold = MIN(MAX(activityThreshold,0),255);
	byte _b = byte(activityThreshold);
	writeTo(DEVICE, ADXL345_THRESH_ACT, _b);
}

// Gets the THRESH_ACT byte
int CAdxl345::getActivityThreshold() {
	byte _b;
	readFrom(DEVICE, ADXL345_THRESH_ACT, 1, &_b);
	return int(_b);
}

// Sets the THRESH_INACT byte which holds the threshold value for detecting inactivity.
// The data format is unsigned, so the magnitude of the inactivity event is compared
// with the value is compared with the value in the THRESH_INACT register. The scale
// factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the
// inactivity interrupt is enabled. The maximum value is 255.
void CAdxl345::setInactivityThreshold(int inactivityThreshold) {
	inactivityThreshold = MIN(MAX(inactivityThreshold,0),255);
	byte _b = byte(inactivityThreshold);
	writeTo(DEVICE, ADXL345_THRESH_INACT, _b);
}

// Gets the THRESH_INACT byte
int CAdxl345::getInactivityThreshold() {
	byte _b;
	readFrom(DEVICE, ADXL345_THRESH_INACT, 1, &_b);
	return int(_b);
}

// Sets the TIME_INACT register, which contains an unsigned time value representing the
// amount of time that acceleration must be less thant the value in the THRESH_INACT
// register for inactivity to be declared. The scale factor is 1sec/LSB. The value must
// be between 0 and 255.
void CAdxl345::setTimeInactivity(int timeInactivity) {
	timeInactivity = MIN(MAX(timeInactivity,0),255);
	byte _b = byte(timeInactivity);
	writeTo(DEVICE, ADXL345_TIME_INACT, _b);
}

// Gets the TIME_INACT register
int CAdxl345::getTimeInactivity() {
	byte _b;
	readFrom(DEVICE, ADXL345_TIME_INACT, 1, &_b);
	return int(_b);
}

// Sets the THRESH_FF register which holds the threshold value, in an unsigned format, for
// free-fall detection. The root-sum-square (RSS) value of all axes is calculated and
// compared whith the value in THRESH_FF to determine if a free-fall event occured. The
// scale factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the free-fall
// interrupt is enabled. The maximum value is 255.
void CAdxl345::setFreeFallThreshold(int freeFallThreshold) {
	freeFallThreshold = MIN(MAX(freeFallThreshold,0),255);
	byte _b = byte(freeFallThreshold);
	writeTo(DEVICE, ADXL345_THRESH_FF, _b);
}

// Gets the THRESH_FF register.
int CAdxl345::getFreeFallThreshold() {
	byte _b;
	readFrom(DEVICE, ADXL345_THRESH_FF, 1, &_b);
	return int(_b);
}

// Sets the TIME_FF register, which holds an unsigned time value representing the minimum
// time that the RSS value of all axes must be less than THRESH_FF to generate a free-fall
// interrupt. The scale factor is 5ms/LSB. A value of 0 may result in undesirable behavior if
// the free-fall interrupt is enabled. The maximum value is 255.
void CAdxl345::setFreeFallDuration(int freeFallDuration) {
	freeFallDuration = MIN(MAX(freeFallDuration,0),255);
	byte _b = byte(freeFallDuration);
	writeTo(DEVICE, ADXL345_TIME_FF, _b);
}

// Gets the TIME_FF register.
int CAdxl345::getFreeFallDuration() {
	byte _b;
	readFrom(DEVICE, ADXL345_TIME_FF, 1, &_b);
	return int(_b);
}

bool CAdxl345::isActivityXEnabled() {
	return getRegisterBit(ADXL345_ACT_INACT_CTL, 6);
}
bool CAdxl345::isActivityYEnabled() {
	return getRegisterBit(ADXL345_ACT_INACT_CTL, 5);
}
bool CAdxl345::isActivityZEnabled() {
	return getRegisterBit(ADXL345_ACT_INACT_CTL, 4);
}
bool CAdxl345::isInactivityXEnabled() {
	return getRegisterBit(ADXL345_ACT_INACT_CTL, 2);
}
bool CAdxl345::isInactivityYEnabled() {
	return getRegisterBit(ADXL345_ACT_INACT_CTL, 1);
}
bool CAdxl345::isInactivityZEnabled() {
	return getRegisterBit(ADXL345_ACT_INACT_CTL, 0);
}

void CAdxl345::setActivityX(bool state) {
	setRegisterBit(ADXL345_ACT_INACT_CTL, 6, state);
}
void CAdxl345::setActivityY(bool state) {
	setRegisterBit(ADXL345_ACT_INACT_CTL, 5, state);
}
void CAdxl345::setActivityZ(bool state) {
	setRegisterBit(ADXL345_ACT_INACT_CTL, 4, state);
}
void CAdxl345::setInactivityX(bool state) {
	setRegisterBit(ADXL345_ACT_INACT_CTL, 2, state);
}
void CAdxl345::setInactivityY(bool state) {
	setRegisterBit(ADXL345_ACT_INACT_CTL, 1, state);
}
void CAdxl345::setInactivityZ(bool state) {
	setRegisterBit(ADXL345_ACT_INACT_CTL, 0, state);
}

bool CAdxl345::isActivityAc() {
	return getRegisterBit(ADXL345_ACT_INACT_CTL, 7);
}
bool CAdxl345::isInactivityAc() {
	return getRegisterBit(ADXL345_ACT_INACT_CTL, 3);
}

void CAdxl345::setActivityAc(bool state) {
	setRegisterBit(ADXL345_ACT_INACT_CTL, 7, state);
}
void CAdxl345::setInactivityAc(bool state) {
	setRegisterBit(ADXL345_ACT_INACT_CTL, 3, state);
}

bool CAdxl345::getSuppressBit() {
	return getRegisterBit(ADXL345_TAP_AXES, 3);
}
void CAdxl345::setSuppressBit(bool state) {
	setRegisterBit(ADXL345_TAP_AXES, 3, state);
}

bool CAdxl345::isTapDetectionOnX() {
	return getRegisterBit(ADXL345_TAP_AXES, 2);
}
void CAdxl345::setTapDetectionOnX(bool state) {
	setRegisterBit(ADXL345_TAP_AXES, 2, state);
}
bool CAdxl345::isTapDetectionOnY() {
	return getRegisterBit(ADXL345_TAP_AXES, 1);
}
void CAdxl345::setTapDetectionOnY(bool state) {
	setRegisterBit(ADXL345_TAP_AXES, 1, state);
}
bool CAdxl345::isTapDetectionOnZ() {
	return getRegisterBit(ADXL345_TAP_AXES, 0);
}
void CAdxl345::setTapDetectionOnZ(bool state) {
	setRegisterBit(ADXL345_TAP_AXES, 0, state);
}

bool CAdxl345::isActivitySourceOnX() {
	return getRegisterBit(ADXL345_ACT_TAP_STATUS, 6);
}
bool CAdxl345::isActivitySourceOnY() {
	return getRegisterBit(ADXL345_ACT_TAP_STATUS, 5);
}
bool CAdxl345::isActivitySourceOnZ() {
	return getRegisterBit(ADXL345_ACT_TAP_STATUS, 4);
}

bool CAdxl345::isTapSourceOnX() {
	return getRegisterBit(ADXL345_ACT_TAP_STATUS, 2);
}
bool CAdxl345::isTapSourceOnY() {
	return getRegisterBit(ADXL345_ACT_TAP_STATUS, 1);
}
bool CAdxl345::isTapSourceOnZ() {
	return getRegisterBit(ADXL345_ACT_TAP_STATUS, 0);
}

bool CAdxl345::isAsleep() {
	return getRegisterBit(ADXL345_ACT_TAP_STATUS, 3);
}

bool CAdxl345::isLowPower() {
	return getRegisterBit(ADXL345_BW_RATE, 4);
}
void CAdxl345::setLowPower(bool state) {
	setRegisterBit(ADXL345_BW_RATE, 4, state);
}

float CAdxl345::getRate() {
	byte _b;
	readFrom(DEVICE, ADXL345_BW_RATE, 1, &_b);
	_b &= 0x000F;
	return (pow(2, ((int) _b) - 6)) * 6.25;
}

void CAdxl345::setRate(float rate) {
	byte _b, _s;
	int v = (int) (rate / 6.25);
	int r = 0;
	while (v >>= 1) {
		r++;
	}
	if (r <= 9) {
		readFrom(DEVICE, ADXL345_BW_RATE, 1, &_b);
		_s = (byte)(r + 6) | (_b & 0x00F0);
		writeTo(DEVICE, ADXL345_BW_RATE, _s);
	}
}

byte CAdxl345::getInterruptSource() {
	byte _b;
	readFrom(DEVICE, ADXL345_INT_SOURCE, 1, &_b);
	return _b;
}

bool CAdxl345::getInterruptSource(byte interruptBit) {
	return getRegisterBit(ADXL345_INT_SOURCE, interruptBit);
}

bool CAdxl345::getInterruptMapping(byte interruptBit) {
	return getRegisterBit(ADXL345_INT_MAP, interruptBit);
}

// Set the mapping of an interrupt to pin1 or pin2
// eg: setInterruptMapping(ADXL345_INT_DOUBLE_TAP_BIT,ADXL345_INT2_PIN);
void CAdxl345::setInterruptMapping(byte interruptBit, bool interruptPin) {
	setRegisterBit(ADXL345_INT_MAP, interruptBit, interruptPin);
}

bool CAdxl345::isInterruptEnabled(byte interruptBit) {
	return getRegisterBit(ADXL345_INT_ENABLE, interruptBit);
}

void CAdxl345::setInterrupt(byte interruptBit, bool state) {
	setRegisterBit(ADXL345_INT_ENABLE, interruptBit, state);
}

void CAdxl345::setRegisterBit(byte regAdress, int bitPos, bool state) {
	byte _b;
	readFrom(DEVICE, regAdress, 1, &_b);
	if (state) {
		_b |= (1 << bitPos); // forces nth bit of _b to be 1.  all other bits left alone.
	} else {
		_b &= ~(1 << bitPos); // forces nth bit of _b to be 0.  all other bits left alone.
	}
	writeTo(DEVICE, regAdress, _b);
}

bool CAdxl345::getRegisterBit(byte regAdress, int bitPos) {
	byte _b;
	readFrom(DEVICE, regAdress, 1, &_b);
	return ((_b >> bitPos) & 1);
}

