/*
 ===============================================================================
 Name        : txtLcd.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2012/5/19
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : CC BY-SA 3.0
 Description : Text LCD Class
 History     :
 ===============================================================================
 */

#ifndef TXTLCD_H_
#define TXTLCD_H_

#include "class/stream.h"
#include "class/bus.h"

#define DEF_MAX_COL	16
#define DEF_MAX_ROW	2

typedef enum {
	RS_INST = LOW, 	// Instruction
	RS_DATA = HIGH	// Data
}RS_LEVEL_T;

class CTxtLcd: public CStream {
public:
	CTxtLcd();
	CTxtLcd(PIN_NAME_T RS,
			PIN_NAME_T RW,
			PIN_NAME_T E,
			PIN_NAME_T D0,
			PIN_NAME_T D1,
			PIN_NAME_T D2,
			PIN_NAME_T D3,
			PIN_NAME_T D4,
			PIN_NAME_T D5,
			PIN_NAME_T D6,
			PIN_NAME_T D7);

	virtual ~CTxtLcd();

	virtual void assign(
						PIN_NAME_T RS,
						PIN_NAME_T RW,
						PIN_NAME_T E,
						PIN_NAME_T D0,
						PIN_NAME_T D1,
						PIN_NAME_T D2,
						PIN_NAME_T D3,
						PIN_NAME_T D4,
						PIN_NAME_T D5,
						PIN_NAME_T D6,
						PIN_NAME_T D7);

	virtual void begin(int maxCol=DEF_MAX_COL, int maxRow=DEF_MAX_ROW);	// default 16x2

	virtual void clear();
	virtual void home();

	virtual void setCursor(int col, int row);
	virtual void cursor();
	virtual void noCursor();
	virtual void blink();
	virtual void noBlink();
	virtual void display();
	virtual void noDisplay();
	virtual void cursorLeft();
	virtual void cursorRight();
	virtual void customerChar(byte addr, const byte *pattern);

	virtual void autoscroll();
	virtual void noAutoscroll();
	virtual void leftToRight();
	virtual void rightToLeft();

	inline void gotoXY(int x, int y) {
		setCursor(x, y);
	}

	//
	// @Override functions for CStream
	//
	virtual int	readable();
	virtual int	writeable();
	virtual int read(void *buf, int len, bool block=true);
	virtual int write(const void *buf, int len, bool block=true);
	virtual int close();
	virtual bool isConnected();

protected:
	CPin  m_pinRS, m_pinRW, m_pinEN;
	CBus  m_dataBus;
	u32_u m_dspFlag;
	u32_u m_dspCB;
	virtual byte read(RS_LEVEL_T RS);
	virtual void write(RS_LEVEL_T RS, byte value, bool chkBusy=true);
	virtual bool isBusy();

	inline void send_cmd(byte value) {
		write(RS_INST, value, true);
	}
};

#endif /* TXTLCD_H_ */
