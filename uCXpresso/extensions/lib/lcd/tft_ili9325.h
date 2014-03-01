/*
 ===============================================================================
 Name        : tft_ili9325.h
 Author      : Jason
 Version     : v1.0.1
 Date		 : 2012/5/26
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : CC BY-SA 3.0
 Description : TFT-LCD (for ILI9325 Chip)
 History     :
 ===============================================================================
 */

#ifndef TFT_ILI9325_H_
#define TFT_ILI9325_H_

#include "class/spi.h"

#define LCD_WIDTH       320                 /* Screen Width (in pixels)           */
#define LCD_HEIGHT      240                 /* Screen Hight (in pixels)           */

class TFT_ILI9325: public CObject {
public:
	TFT_ILI9325();
	TFT_ILI9325(SPI_PORT_T port, PIN_NAME_T CS, PIN_NAME_T BL);
	virtual ~TFT_ILI9325();

	virtual void assign(SPI_PORT_T port, PIN_NAME_T CS, PIN_NAME_T BL);
	virtual void begin();

	virtual void fill(int x, int y, int w, int h, uint16_t color);
	virtual void bitmap(int x, int y, int w, int h, const void *bmp);

	inline virtual void clear() {
		fill(0, 0, LCD_WIDTH, LCD_HEIGHT, 0xFFFF);
	}

	inline virtual void backlight(PIN_LEVEL_T p){
		pinBL = p;
	}

protected:
	CPin 		pinBL;
	CSpiMaster	spi;

	virtual void 	 wr_cmd(uint8_t c);
	virtual void 	 wr_dat(uint16_t d);
	virtual uint16_t rd_dat();
	virtual void 	 wr_reg(uint8_t reg, uint16_t val);
	virtual uint16_t rd_reg(uint8_t reg);
	virtual void 	 wr_bmp(uint8_t *buf, int size);
	virtual void	 setWindow(int x, int y, int w, int h);
};

#endif /* TFT_ILI9325_H_ */
