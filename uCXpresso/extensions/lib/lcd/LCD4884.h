/*
 ===============================================================================
 Name        : lcd48x84 @ extensions
 Modify      : Jason
 Date		 : 2012/06/09
 Version     : 1.0.0
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 License	 : CC BY-SA
 Description :
 History	 :
 ===============================================================================
 */

/*
 Modified by Lauren
 version 0.3

 Any suggestions are welcome.
 E-mail: Lauran.pan@gmail.com

 Editor     : Lauren from DFRobot
 Date       : 06.01.2012

 =��=

 * Have the back light under control.
 * Update the library and sketch to compatible with IDE V1.0 and earlier

 */

#ifndef LCD4884_h
#define LCD4884_h

/*
 #if defined(ARDUINO) && ARDUINO >= 100
 #include "Arduino.h"
 #else
 #include "WProgram.h"
 #endif
 */
#include "arduino.h"
#include "class/bus.h"

#define SPI_SCK 	0	// UNO_2
#define SPI_MOSI 	1	// UNO_3
#define LCD_DC  	2	// UNO_4
#define SPI_CS  	3	// UNO_5
#define LCD_RST 	4	// UNO_6
#define LCD_BL  	5	// UNO_7

//display mode -- normal / highlight
#define LCD_NORMAL	0
#define LCD_HIGHLIGHT 1
/*
 #ifndef OFF
 #define OFF 0
 #endif
 #ifndef ON
 #define ON 1
 #endif */

class LCD4884 {
public:
	LCD4884();
	void LCD_init(void);
	void backlight(byte dat);
	void LCD_write_byte(byte dat, byte dat_type);
	void LCD_draw_bmp_pixel(byte X, byte Y, const byte *map, byte Pix_x, byte Pix_y);
	void LCD_write_string(byte X, byte Y, const char *s, char mode = LCD_NORMAL);
	void LCD_write_chinese(byte X, byte Y, const byte *c, byte ch_with, byte num, byte line, byte row);
	void LCD_write_string_big(byte X, byte Y, const char *string, char mode);

	void LCD_write_digital_num(byte x, byte y, const char *num, char mode);
	void LCD_write_digital_chr(byte x, byte y, byte ch, char mode);

	void LCD_write_char_big(byte X, byte Y, byte ch, char mode);
	void LCD_write_char(byte c, char mode);
	void LCD_set_XY(byte X, byte Y);
	void LCD_clear(void);

protected:
	CBus lcd; // LCD I/F
};

//extern LCD4884 lcd;

#endif   // 
