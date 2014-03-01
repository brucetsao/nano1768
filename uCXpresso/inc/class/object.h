/*
 ===============================================================================
 Name        : object.h
 Author      : Jason
 Version     : v1.0.2
 Date		 : 2013/7/5
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2011/12/18	v1.0.0	First Edition									Jason
 2013/6/25	v1.0.1	remove onEvent() member function				Jason
 2013/7/5	v1.0.2	Add isThread() member function					Jason
 ===============================================================================
 */
#ifndef OBJECT_H_
#define OBJECT_H_

#include "uCXpresso.h"

class CObject {
public:
	CObject();
	virtual ~CObject();

	virtual inline bool isThread() {
		return false;
	}
};

#endif /* OBJECT_H_ */
