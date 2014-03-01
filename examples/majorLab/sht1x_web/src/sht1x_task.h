/*
 ===============================================================================
 Name        : sht1x_task.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2012/6/26
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : CC BY-SA 3.0
 Description : 
 History     :
 ===============================================================================
 */

#ifndef SHT1X_TASK_H_
#define SHT1X_TASK_H_

#include "class/thread.h"
#include "class/pin.h"
#include "sensor/sht1x.h"

class CSHT1xTask: public CThread {
public:
	float	temp;
	float	humi;

	CSHT1xTask(int dat, int clk);
	virtual ~CSHT1xTask();

protected:
	SHT1x   sht1x;
	virtual void run();
};

#endif /* SHT1X_TASK_H_ */
