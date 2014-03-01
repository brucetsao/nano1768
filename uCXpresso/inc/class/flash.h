/*
 ===============================================================================
 Name        : flash.h
 Author      : Jason
 Version     : v1.0.1
 Date		 : 2013/7/4
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : user's data flash memory
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/8/29	v1.0.0	first edition,									Json
 2013/7/4	v1.0.1	add safe-to-write protected.					Jason
 ===============================================================================
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "class/object.h"

#define MAX_FLASH_MEM_SIZE	(32*1024)	// 32KB users flash memory

class CFlash: public CObject {
public:
	CFlash(uint32_t addr=0);
	virtual ~CFlash();

	virtual int write(const void *buf, int size, uint32_t offset=0);
	virtual void* read(int offset=0);
	virtual inline void* operator * () {
		return read();
	}
protected:
	uint32_t m_addr;
	uint32_t m_last;
};

#endif /* FLASH_H_ */
