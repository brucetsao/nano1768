/*
 ===============================================================================
 Name        : peripheral.h
 Author      : Jason Liao
 Version     : 1.0.1
 Created on	 : 2012/3/5
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : 
 History	 :
 ===============================================================================
 */

#ifndef PERIPHERAL_H_
#define PERIPHERAL_H_

#include "class/object.h"

class CPeripheral: virtual public CObject {
	/*! \cond PRIVATE */
public:
	CPeripheral();
	virtual ~CPeripheral();
	/*! \endcond */
};

#endif /* PERIPHERAL_H_ */
