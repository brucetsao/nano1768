/*
 ===============================================================================
 Name        : txtLcd.cpp
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2012/5/19
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : CC BY-SA 3.0
 Description : Text LCD Class
 History     :
 ===============================================================================
 */

#include "txtLcd.h"
#include "class/timeout.h"
//#include "utils/debug.h"

#define FLAG 		(m_dspFlag.Char[0])
#define MAX_ROW		(m_dspFlag.Char[2])
#define MAX_COL		(m_dspFlag.Char[3])
#define _X			(m_dspCB.Char[0])
#define _Y			(m_dspCB.Char[1])

#define LCD_FLAG_AUTOSCROLL		4
#define LCD_FLAG_RIGHT2LEFT		5
#define LCD_FLAG_SETCURSOR		7

CTxtLcd::CTxtLcd() {
	MAX_ROW = DEF_MAX_ROW; // default 16 x 2
	MAX_COL = DEF_MAX_COL;
	m_dspFlag.Long = 0;
	m_dspCB.Long = 0;
}

CTxtLcd::CTxtLcd(PIN_NAME_T RS, PIN_NAME_T RW, PIN_NAME_T E, PIN_NAME_T D0,
		PIN_NAME_T D1, PIN_NAME_T D2, PIN_NAME_T D3, PIN_NAME_T D4,
		PIN_NAME_T D5, PIN_NAME_T D6, PIN_NAME_T D7) {

	MAX_ROW = DEF_MAX_ROW; // default 16 x 2
	MAX_COL = DEF_MAX_COL;
	m_dspFlag.Long = 0;
	m_dspCB.Long = 0;

	assign(RS, RW, E, D0, D1, D2, D3, D4, D5, D6, D7);
}

CTxtLcd::~CTxtLcd() {
	// TODO Auto-generated destructor stub
}

void CTxtLcd::assign(PIN_NAME_T RS, PIN_NAME_T RW, PIN_NAME_T E, PIN_NAME_T D0,
		PIN_NAME_T D1, PIN_NAME_T D2, PIN_NAME_T D3, PIN_NAME_T D4,
		PIN_NAME_T D5, PIN_NAME_T D6, PIN_NAME_T D7) {

	m_pinRS.assign(RS);
	m_pinRS.output(NOT_OPEN);

	m_pinRW.assign(RW);
	m_pinRW.output(NOT_OPEN);

	m_pinEN.assign(E);
	m_pinEN.output(NOT_OPEN);

	m_dataBus.assign(D0, D1, D2, D3, D4, D5, D6, D7, END);
	m_dataBus.output(NOT_OPEN);
}

void CTxtLcd::begin(int maxCol, int maxRow) {
	CTick tm;

	MAX_ROW = maxRow;
	MAX_COL = maxCol;
	FLAG = 0x04; // display ON

	sleep(40);

	write(RS_INST, 0x38, false);
	tm.delay(40);
	write(RS_INST, 0x38, false);
	tm.delay(40);
	write(RS_INST, 0x38, false);
	tm.delay(40);

	send_cmd(0x06);
	send_cmd(0x0C);
	send_cmd(0x38);	// Function Set: DL=8, N=2Line, F=5x11

	clear();
	home();
}

bool CTxtLcd::isBusy() {
	return ((read(RS_INST) & 0x80) ? true : false);
}

void CTxtLcd::clear() {
	send_cmd(0x01);
	_X = _Y = 0;
}

void CTxtLcd::home() {
	send_cmd(0x2);
	_X = _Y = 0;
}

void CTxtLcd::setCursor(int col, int row) {
	byte addr;
	_X = col;
	_Y = row;

	switch(_Y)	{
		case 0:
			addr = _X;
			break;
		case 1:
			addr = 0x40 | _X;
		   	break;
		case 2:
			addr = 0x14 | _X;
			break;
		case 3:
			addr = 0x54 | _X;
			break;
		default:
			addr = 0;
	}
	send_cmd(0x80 | addr);
}

void CTxtLcd::cursor() {
	FLAG |= 0x02;
	send_cmd(0x08 | (FLAG & 0x07));
}
void CTxtLcd::noCursor() {
	FLAG &= 0x0D;
	send_cmd(0x08 | (FLAG & 0x07));
}
void CTxtLcd::blink() {
	FLAG |= 0x01;
	send_cmd(0x08 | (FLAG & 0x07));
}

void CTxtLcd::noBlink() {
	FLAG &= 0x0E;
	send_cmd(0x08 | (FLAG & 0x07));
}

void CTxtLcd::display() {
	FLAG |= 0x04;
	send_cmd(0x08 | (FLAG & 0x07));
}

void CTxtLcd::noDisplay() {
	FLAG &= 0x0B;
	send_cmd(0x08 | (FLAG & 0x07));
}

void CTxtLcd::cursorLeft() {
	send_cmd(0x10);
}

void CTxtLcd::cursorRight() {
	send_cmd(0x14);
}

void CTxtLcd::customerChar(byte addr, const byte *pattern) {
	send_cmd(0x40 + (addr << 3));
	for (int i = 0; i < 8; i++) {
		write(RS_DATA, *pattern++);
	}
}

void CTxtLcd::autoscroll() {
	bit_set(FLAG, LCD_FLAG_AUTOSCROLL);
	send_cmd(0x04 | ((FLAG>>4) & 0x03) );
}

void CTxtLcd::noAutoscroll() {
	bit_clr(FLAG, LCD_FLAG_AUTOSCROLL);
	send_cmd(0x04 | ((FLAG>>4) & 0x03) );
}

void CTxtLcd::leftToRight() {
	bit_set(FLAG, LCD_FLAG_RIGHT2LEFT);
	send_cmd(0x04 | ((FLAG>>4) & 0x03) );
}

void CTxtLcd::rightToLeft() {
	bit_clr(FLAG, LCD_FLAG_RIGHT2LEFT);
	send_cmd(0x04 | ((FLAG>>4) & 0x03) );
}

byte CTxtLcd::read(RS_LEVEL_T RS) {
	CTick tm;
	uint32_t val;

	m_dataBus.input();

	// set control pin
	m_pinRS = (RS ? HIGH : LOW);
	m_pinRW = HIGH;
	m_pinEN = HIGH;
	tm.delay(5); // 5 us


	val = m_dataBus;

	// release control pin
	m_pinEN = LOW;
	tm.delay(2); // 2us

	return val;
}

void CTxtLcd::write(RS_LEVEL_T RS, byte value, bool chkBusy) {
	CTick tm;

	while (chkBusy && isBusy()) {
		// LCD busy
	}

	m_dataBus.output(NOT_OPEN);
	m_dataBus = value;

	m_pinRS = (RS ? HIGH : LOW);
	m_pinRW = LOW;
	m_pinEN = HIGH;

	tm.delay(2);
	m_pinEN = LOW;
	tm.delay(2);
}

// @Override the pure virtual functions of CStream
int CTxtLcd::write(const void *buf, int len, bool block) {
	byte *p = (byte *) buf;

	if ( bit_chk(FLAG, LCD_FLAG_SETCURSOR) ) {
		setCursor(_X, _Y);
	}

	bit_clr(FLAG, LCD_FLAG_SETCURSOR);

	for (int i = 0; i < len; i++) {
		switch(p[i]) {
		case '\r':
			_X = 0;
			bit_set(FLAG, LCD_FLAG_SETCURSOR);
			break;
		case '\n':
			_Y = (_Y+1)<MAX_ROW ? _Y+1 : 0;
			bit_set(FLAG, LCD_FLAG_SETCURSOR);
			break;
		default:
			if ( p[i]>=0x20 ) {
				write(RS_DATA, p[i]);
				_X++;
				if ( _X>=MAX_COL ) {
					_X = 0;
					_Y++;
				}
			}
		}
	}

	if ( _Y>=MAX_ROW ) {
		_Y = 0;
	}
	return len;
}

int CTxtLcd::readable() {
	return 0;
}

int CTxtLcd::writeable() {
	return 1;
}

int CTxtLcd::read(void *buf, int len, bool block) {
	return 0;
}

int CTxtLcd::close() {
	return 0;
}

bool CTxtLcd::isConnected() {
	return true;
}
