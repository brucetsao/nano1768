/*
 ===============================================================================
 Name        : tft_ili9325.cpp
 Author      : Jason
 Version     : v1.0.1
 Date		 : 2012/5/23
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : CC BY-SA 3.0
 Description : TFT-LCD (for ILI9325 Chip)
 History     :
 2012/5/26, add member wr_bmp for speed up
 ===============================================================================
 */
#include "lcd/tft_ili9325.h"
#include "class/thread.h"
#include "debug.h"

#define LCD_DEVICE_ID	0x9325

/*--------------- Graphic LCD interface hardware definitions -----------------*/
#define SSP_START   (0x70)              /* Start byte for SSP transfer        */
#define SSP_RD      (0x01)              /* WR bit 1 within start              */
#define SSP_WR      (0x00)              /* WR bit 0 within start              */
#define SSP_DATA    (0x02)              /* RS bit 1 within start byte         */
#define SSP_INDEX   (0x00)              /* RS bit 0 within start byte         */

TFT_ILI9325::TFT_ILI9325() {
}

TFT_ILI9325::TFT_ILI9325(SPI_PORT_T port, PIN_NAME_T CS, PIN_NAME_T BL) {
	assign(port, CS, BL);
}

void TFT_ILI9325::assign(SPI_PORT_T port, PIN_NAME_T CS, PIN_NAME_T BL) {
	pinBL.assign(BL);
	pinBL.output(NOT_OPEN);

	spi.assign(port, CS);
	spi.format(SPI_DATABIT_8, SPI_MODE_3);
	spi.frequency(MHZ(12));
	spi.begin();
}

TFT_ILI9325::~TFT_ILI9325() {
}

void TFT_ILI9325::begin() {
	uint16_t devCode;

	sleep(50);

	devCode = rd_reg(0x00);
	if (devCode == LCD_DEVICE_ID) {

		/* Start Initial Sequence --------------------------------------------------*/
		wr_reg(0xE3, 0x3008);
		wr_reg(0xE7, 0x0012);
		wr_reg(0xEF, 0x1231);

		//  wr_reg(0xE5, 0x8000);                 /* Set the internal vcore voltage     */
		wr_reg(0x00, 0x0001); /* Start internal OSC                 */
		wr_reg(0x01, 0x0100); /* Set SS and SM bit                  */
		wr_reg(0x02, 0x0700); /* Set 1 line inversion               */
		wr_reg(0x03, 0x1030); /* Set GRAM write direction and BGR=1 */
		wr_reg(0x04, 0x0000); /* Resize register                    */
		wr_reg(0x08, 0x0207); /* 2 lines each, back and front porch */
		wr_reg(0x09, 0x0000); /* Set non-disp area refresh cyc ISC  */
		wr_reg(0x0A, 0x0000); /* FMARK function                     */
		wr_reg(0x0C, 0x0000); /* RGB interface setting              */
		wr_reg(0x0D, 0x0000); /* Frame marker Position              */
		wr_reg(0x0F, 0x0000); /* RGB interface polarity             */

		/* Power On sequence -------------------------------------------------------*/
		wr_reg(0x10, 0x0000); /* Reset Power Control 1              */
		wr_reg(0x11, 0x0007); /* Reset Power Control 2              */
		wr_reg(0x12, 0x0000); /* Reset Power Control 3              */
		wr_reg(0x13, 0x0000); /* Reset Power Control 4              */

		sleep(200); /* Discharge cap power voltage (200ms)*/

		wr_reg(0x10, 0x1690); /* SAP, BT[3:0], AP, DSTB, SLP, STB   */
		wr_reg(0x11, 0x0227); /* DC1[2:0], DC0[2:0], VC[2:0]        */

		sleep(50); /* Delay 50 ms                        */

		wr_reg(0x12, 0x001B); /* VREG1OUT voltage                   */

		sleep(50); /* Delay 50 ms
		 */
		wr_reg(0x13, 0x1600); /* VDV[4:0] for VCOM amplitude        */
		wr_reg(0x29, 0x0018); /* VCM[4:0] for VCOMH                 */
		wr_reg(0x2B, 0x000C);

		sleep(50); /* Delay 50 ms                        */

		wr_reg(0x20, 0x0000); /* GRAM horizontal Address            */
		wr_reg(0x21, 0x0000); /* GRAM Vertical Address              */

		/* Adjust the Gamma Curve --------------------------------------------------*/
		wr_reg(0x30, 0x0000);
		wr_reg(0x31, 0x0404);
		wr_reg(0x32, 0x0304);
		wr_reg(0x35, 0x0005);
		wr_reg(0x36, 0x1604);
		wr_reg(0x37, 0x0304);
		wr_reg(0x38, 0x0303);
		wr_reg(0x39, 0x0707);
		wr_reg(0x3C, 0x0500);
		wr_reg(0x3D, 0x000F);

		/* Set GRAM area -----------------------------------------------------------*/
		wr_reg(0x50, 0x0000); 			/* Horizontal GRAM Start Address      */
		wr_reg(0x51, (LCD_HEIGHT - 1)); /* Horizontal GRAM End   Address      */
		wr_reg(0x52, 0x0000); 			/* Vertical   GRAM Start Address      */
		wr_reg(0x53, (LCD_WIDTH - 1)); 	/* Vertical   GRAM End   Address      */
		wr_reg(0x60, 0xA700); 			/* Gate Scan Line                     */
		wr_reg(0x61, 0x0001); 			/* NDL,VLE, REV                       */
		wr_reg(0x6A, 0x0000); 			/* Set scrolling line                 */

		/* Partial Display Control -------------------------------------------------*/
		wr_reg(0x80, 0x0000);
		wr_reg(0x81, 0x0000);
		wr_reg(0x82, 0x0000);
		wr_reg(0x83, 0x0000);
		wr_reg(0x84, 0x0000);
		wr_reg(0x85, 0x0000);

		/* Panel Control -----------------------------------------------------------*/
		wr_reg(0x90, 0x0010);
		wr_reg(0x92, 0x6000);

		wr_reg(0x03, 0x1038);
		wr_reg(0x07, 0x0133); /* 262K color and display ON          */

		/* Turn backlight on                  */
		pinBL = HIGH;
		pinBL = LOW;
		pinBL = HIGH;
	} else {
		dbg.println("TFT_ILI9325::begin! Unknow device id %Xh\n", devCode);
	}
}

void TFT_ILI9325::wr_cmd(uint8_t c) {
	uint8_t cmd[3];

	cmd[0] = SSP_START | SSP_WR | SSP_INDEX;
	cmd[1] = 0;
	cmd[2] = c;
	spi.readwrite(cmd, NULL, 3);
}

void TFT_ILI9325::wr_dat(uint16_t d) {
	uint8_t dat[3];
	dat[0] = SSP_START | SSP_WR | SSP_DATA;
	dat[1] = d >> 8;
	dat[2] = d & 0xFF;
	spi.readwrite(dat, NULL, 3);
}

#define TFT_BLK_SIZE 	2048
void TFT_ILI9325::wr_bmp(uint8_t *bmp, int size) {
	int i;
	uint8_t *blk;
	uint8_t *p = bmp;
	;

	blk = new uint8_t[TFT_BLK_SIZE + 1];
	blk[0] = SSP_START | SSP_WR | SSP_DATA;

	CThread::suspendAll();
	do {
		for (i = 0; i < TFT_BLK_SIZE && i < size; i += 2) {
			blk[i + 1] = p[i + 1];
			blk[i + 2] = p[i];
		}
		spi.readwrite(blk, NULL, i + 1);
		size -= i;
		p += i;
	} while (size > 0);
	CThread::resumeAll();
	delete blk;
}

uint16_t TFT_ILI9325::rd_dat() {
	uint8_t dat[4];

	dat[0] = SSP_START | SSP_RD | SSP_DATA;
	dat[1] = 0;
	dat[2] = 0;
	dat[3] = 0;

	spi.readwrite(dat, dat, 4);
	return (uint16_t) ((dat[2] << 8) | dat[3]);
}

void TFT_ILI9325::wr_reg(uint8_t reg, uint16_t val) {
	wr_cmd(reg);
	wr_dat(val);
}

uint16_t TFT_ILI9325::rd_reg(uint8_t reg) {
	wr_cmd(reg);
	return rd_dat();
}

void TFT_ILI9325::setWindow(int x, int y, int w, int h) {
	wr_reg(0x50, y); 			/* Horizontal GRAM Start Address      */
	wr_reg(0x51, y + h - 1); 	/* Horizontal GRAM End   Address (-1) */
	wr_reg(0x52, x); 			/* Vertical   GRAM Start Address      */
	wr_reg(0x53, x + w - 1); 	/* Vertical   GRAM End   Address (-1) */

	wr_reg(0x20, y);
	wr_reg(0x21, x);
}

void TFT_ILI9325::fill(int x, int y, int w, int h, uint16_t color) {
	uint32_t i, size;
	uint8_t *blk;

	// alloc memory
	blk = new uint8_t[TFT_BLK_SIZE + 1];
	size = (LCD_WIDTH * LCD_HEIGHT) * 2;

	// fill color
	blk[0] = SSP_START | SSP_WR | SSP_DATA;
	for (i = 0; i < TFT_BLK_SIZE && i < size; i += 2) {
		blk[i + 1] = color >> 8;
		blk[i + 2] = color & 0xFF;
	}

	CThread::suspendAll();

	// write to tft-lcd
	setWindow(x, y, w, h);

	wr_cmd(0x22);
	do {
		spi.readwrite(blk, NULL, TFT_BLK_SIZE + 1);
		size -= TFT_BLK_SIZE;
	} while (size > 0);

	CThread::resumeAll();
	delete blk;
}

void TFT_ILI9325::bitmap(int x, int y, int w, int h, const void *bmp) {
	uint32_t len = w * h;

	setWindow(x, y, w, h);

	wr_cmd(0x22);
	wr_bmp((uint8_t *) bmp, len * 2);
}
