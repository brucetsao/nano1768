/*
 ===============================================================================
 Name        : sht1x_task.cpp
 Author      : 
 Version     : v1.0.0
 Date		 : 2012/6/26
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : CC BY-SA 3.0
 Description : 
 History     :
 ===============================================================================
 */

#include "sht1x_task.h"

CSHT1xTask::CSHT1xTask(int dat, int clk) : sht1x(dat, clk) {

}

CSHT1xTask::~CSHT1xTask() {
}

void CSHT1xTask::run() {
	while(1) {
		sht1x.update();
		temp = sht1x.readTemperatureC();
		humi = sht1x.readHumidity();
	}
}
