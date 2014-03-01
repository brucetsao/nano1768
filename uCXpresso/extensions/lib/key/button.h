/*
 ===============================================================================
 Name        : button.h
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2013/5/29
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/05/29	v1.0.0	First Edition									 Jason
  ===============================================================================
 */
#ifndef BUTTON_H_
#define BUTTON_H_

#include <cstdarg>
#include "class/thread.h"
#include "class/bus.h"

#define KEY_FLAG_DOWN	0

class CButton: public CThread {
public:
	CButton(PIN_NAME_T pin);
	virtual bool start(LPCTSTR name);

	virtual void onKeyDown() {};
	virtual void onKeyUp() {};

	inline  bool isKeyDown() {
		return bit_chk(m_flag, KEY_FLAG_DOWN);
	}
protected:
	virtual ~CButton();
	CPin		m_pin;
	uint32_t	m_flag;
	virtual void run();
};

//
// Button Group
//
class CButtons: public CThread {
public:
	CButtons(PIN_NAME_T pin, ...);
	~CButtons();

	virtual bool start(LPCTSTR name);

	virtual void onKeyDown(int i) {};
	virtual void onKeyUp(int i) {};
	virtual int  isKeyDown();

	inline int count() {
		return m_pins.count();
	}
protected:
	CBus 	 m_pins;
	uint32_t m_flag;
	uint32_t m_down;
	virtual void run();
};

#endif /* BUTTON_H_ */
