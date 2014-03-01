/*
 ===============================================================================
 Name        : menu.cpp
 Author      : Jason
 Version     :
 Date		 : 2012/6/12
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : CC BY-SA 3.0
 Description : Menu Demo (with LCD4884)
 History     :
 ===============================================================================
 */
#include "class/string.h"
#include "class/time.h"
#include "class/timeout.h"
#include "lcd/lcd4884.h"
#include "adckey.h"
#include "ucxpresso_logo.h"
#include "menu.h"

#define MENU_X	10		// 0-83
#define MENU_Y	0		// 0-5

LCD4884 lcd;
CAdcKey m_key(AD0);

//
// menu items & functions
//
#define NUM_MENU_ITEM 6
static const char *menu_items[NUM_MENU_ITEM] = {
		"Backlight",
		"Date & Time",
		"Temperature",
		"Char Map",
		"Bitmap",
		"About"
};

CMenuDemo::CMenuDemo() {
}

CMenuDemo::~CMenuDemo() {
}

void CMenuDemo::init_MENU() {
	lcd.LCD_clear();
	for (int i = 0; i < NUM_MENU_ITEM; i++) {
		lcd.LCD_write_string(MENU_X, MENU_Y + i, menu_items[i], (i==m_current_menu_item) ? LCD_HIGHLIGHT : LCD_NORMAL);
	}
}

void CMenuDemo::setup() {
	m_key.start();

	lcd.LCD_init();
	lcd.LCD_clear();

	m_current_menu_item = 0;
	init_MENU();

	lcd.backlight(LED_OFF);
	m_bl = false;
}

void CMenuDemo::loop() {
	int i;
	for (i = 0; i < MAX_KEYS; i++) {
		if (m_key.isPressed((ADKEY_T)i)) {
			switch (i) {

			case KEY_UP:
				// current item to normal display
				lcd.LCD_write_string(MENU_X, MENU_Y + m_current_menu_item,
						menu_items[m_current_menu_item], LCD_NORMAL);
				m_current_menu_item -= 1;
				if (m_current_menu_item < 0)
					m_current_menu_item = NUM_MENU_ITEM - 1;
				// next item to highlight display
				lcd.LCD_write_string(MENU_X, MENU_Y + m_current_menu_item,
						menu_items[m_current_menu_item], LCD_HIGHLIGHT);
				break;

			case KEY_DOWN:
				// current item to normal display
				lcd.LCD_write_string(MENU_X, MENU_Y + m_current_menu_item,
						menu_items[m_current_menu_item], LCD_NORMAL);
				m_current_menu_item += 1;
				if (m_current_menu_item > (NUM_MENU_ITEM - 1))
					m_current_menu_item = 0;
				// next item to highlight display
				lcd.LCD_write_string(MENU_X, MENU_Y + m_current_menu_item,
						menu_items[m_current_menu_item], LCD_HIGHLIGHT);
				break;

			case KEY_LEFT:
				init_MENU();
				m_current_menu_item = 0;
				break;

			case KEY_CENTER:
				lcd.LCD_clear();
				execute(m_current_menu_item);
				lcd.LCD_clear();
				init_MENU();
				break;
			}

		}
	}
}

void CMenuDemo::execute(int item) {
	switch(item) {
	case 0:	// backlight
		backlight();
		break;
	case 1:
		datetime();
		break;
	case 2:
		temperature();
		break;
	case 3:
		charmap();
		break;
	case 4:
		bitmap();
		break;
	case 5:
		about();
		break;
	}
}

// waiting for center key press
void CMenuDemo::waitfor_OKkey() {
	while ( !m_key.isPressed(KEY_CENTER) ) {
		sleep(10);
	}
}

#include "..\sht1x_task.h"
extern CSHT1xTask sht1x;
void CMenuDemo::temperature() {
	CString str;
	do {
		str.printf("Temp: %0.2f C", sht1x.temp);
		lcd.LCD_write_string(0, 1, *str);

		str.printf("Humi: %0.2f %c", sht1x.humi, '%');
		lcd.LCD_write_string(0, 3, *str);

		sleep(1000);
		lcd.LCD_clear();
	} while( !m_key.isPressed(KEY_CENTER) );
}

void CMenuDemo::charmap() {
	char i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 14; j++) {
			lcd.LCD_set_XY(j * 6, i);
			lcd.LCD_write_char(i * 14 + j + 32, LCD_NORMAL);
		}
	}

	lcd.LCD_write_string(38, 5, "OK", LCD_HIGHLIGHT);
	waitfor_OKkey();
}

void CMenuDemo::bitmap() {
	lcd.LCD_draw_bmp_pixel(5, 1, ucxpresso_logo, 73, 15);
	lcd.LCD_write_string(38, 5, "OK", LCD_HIGHLIGHT);
	waitfor_OKkey();
}

void CMenuDemo::about() {

	lcd.LCD_write_string(0, 1, "LCD4884 Demo", LCD_NORMAL);
	lcd.LCD_write_string(0, 3, "uCXpresso.NET", LCD_NORMAL);
	lcd.LCD_write_string(38, 5, "OK", LCD_HIGHLIGHT);
	waitfor_OKkey();
}

void CMenuDemo::datetime() {
	CTime tm;
	CString date, time;
	bool secdot = true;
	CTimeout to;
	do {
		to.reset();
		secdot = !secdot;
		// show date
		tm = CTime::now();
		date.printf("%04d/%02d/%02d", tm.getYear(), tm.getMonth(), tm.getDay());
		lcd.LCD_write_string(0, 0, *date, LCD_HIGHLIGHT);

		// show time
		if ( secdot )
			 time.printf("%02d:%02d", tm.getHour(), tm.getMinute());
		else time.printf("%02d %02d", tm.getHour(), tm.getMinute());
		lcd.LCD_write_digital_num(2, 2, *time, LCD_NORMAL);

		to.wait(500);
	}while ( !m_key.isPressed(KEY_CENTER) );
}

void CMenuDemo::backlight() {
	m_bl = !m_bl;
	lcd.backlight(m_bl);
}

