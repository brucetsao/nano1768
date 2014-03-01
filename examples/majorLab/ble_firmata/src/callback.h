/*
 ===============================================================================
 Name        : callback.h
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2013/9/30
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 
  ===============================================================================
 */


#ifndef CALLBACK_H_
#define CALLBACK_H_

extern int samplingInterval;

void callback_init(void);
void systemResetCallback(void);
void checkAnalogInputs(void);
void checkDigitalInputs(void);

#endif /* CALLBACK_H_ */
