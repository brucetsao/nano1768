/*
 ===============================================================================
 Name        : menu.h
 Author      : Jason
 Version     :
 Date		 : 2012/6/12
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : CC BY-SA 3.0
 Description : Menu Demo (with LCD4884)
 History     :
 ===============================================================================
 */

#ifndef MENU_H_
#define MENU_H_

#include "class/object.h"

class CMenuDemo: public CObject {
public:
	CMenuDemo();
	virtual ~CMenuDemo();
	virtual void init_MENU();
	virtual void setup();
	virtual void loop();

protected:
	int  m_current_menu_item;
	bool m_bl;

	void execute(int item);
	void backlight();
	void datetime();
	void temperature();
	void charmap();
	void bitmap();
	void about();
	void waitfor_OKkey();
};

#endif /* MENU_H_ */
