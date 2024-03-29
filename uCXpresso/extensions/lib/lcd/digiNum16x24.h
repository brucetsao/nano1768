/*
 ===============================================================================
 Name        : digiNum16x24.h
 Modify      : Jason
 Date		 : 2012/06/09
 Version     : 1.0.0
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 License	 : CC BY-SA
 Description : digital Number (Font) 16x24
 History	 :
 ===============================================================================
 */

#define MAX_DIGITAL_FONTS 13
static const unsigned char digatalFont[MAX_DIGITAL_FONTS][48] =
{
  {0x00, 0xFC, 0xFA, 0xF6, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0xF6, 0xFA, 0xFC, 0x00, 0x00, 0xEF, 0xC7, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0xC7, 0xEF, 0x00, 0x00, 0x7F, 0xBF, 0xDF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xDF, 0xBF, 0x7F, 0x00},     // 1	 ASCII 0
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF8, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0xC7, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x3F, 0x7F, 0x00},     // 2	 ASCII 1
  {0x00, 0x00, 0x02, 0x06, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0xF6, 0xFA, 0xFC, 0x00, 0x00, 0xE0, 0xC0, 0x90, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x13, 0x07, 0x0F, 0x00, 0x00, 0x7F, 0xBF, 0xDF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0x80, 0x00, 0x00},     // 3	 ASCII 2
  {0x00, 0x00, 0x02, 0x06, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0xF6, 0xFA, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x93, 0xC7, 0xEF, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xDF, 0xBF, 0x7F, 0x00},     // 4	 ASCII 3
  {0x00, 0xFC, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF8, 0xFC, 0x00, 0x00, 0x0F, 0x07, 0x13, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x93, 0xC7, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x3F, 0x7F, 0x00},     // 5	 ASCII 4
  {0x00, 0xFC, 0xFA, 0xF6, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x06, 0x02, 0x00, 0x00, 0x00, 0x0F, 0x07, 0x13, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x90, 0xC0, 0xE0, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xDF, 0xBF, 0x7F, 0x00},     // 6	 ASCII 5
  {0x00, 0xFC, 0xFA, 0xF6, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x06, 0x02, 0x00, 0x00, 0x00, 0xEF, 0xC7, 0x93, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x90, 0xC0, 0xE0, 0x00, 0x00, 0x7F, 0xBF, 0xDF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xDF, 0xBF, 0x7F, 0x00},     // 7	 ASCII 6
  {0x00, 0x00, 0x02, 0x06, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0xF6, 0xFA, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0xC7, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x3F, 0x7F, 0x00},     // 8	 ASCII 7
  {0x00, 0xFC, 0xFA, 0xF6, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0xF6, 0xFA, 0xFC, 0x00, 0x00, 0xEF, 0xC7, 0x93, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x93, 0xC7, 0xEF, 0x00, 0x00, 0x7F, 0xBF, 0xDF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xDF, 0xBF, 0x7F, 0x00},     // 9	 ASCII 8
  {0x00, 0xFC, 0xFA, 0xF6, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0xF6, 0xFA, 0xFC, 0x00, 0x00, 0x0F, 0x07, 0x13, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x93, 0xC7, 0xEF, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xDF, 0xBF, 0x7F, 0x00},     // 10	 ASCII 9
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0x87, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},     // 2	 ASCII :
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},     // 1	 ASCII .
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}      // space
};

#define DSP_DIGIT_SIZE_WIDTH    16
#define DSP_DIGIT_SIZE_HIGH     24
