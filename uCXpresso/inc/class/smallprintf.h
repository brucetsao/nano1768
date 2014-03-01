/*
 ===============================================================================
 Name        : smallprintf.h
 Author      : Jason
 Version     :
 Date		 : 2011/12/18
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 History     :
 ===============================================================================
 */

#ifndef SMALLPRINTF_H
#define SMALLPRINTF_H

#include <cstdarg>
#include "class/stream.h"

class CSmallPrintf: public CObject {
public:
	CSmallPrintf();

	int printf(LPCTSTR format, ...);
	int printf(LPCTSTR format, va_list args);

	int scanf(LPCTSTR format, ...);

	int putv(int val);
	int putv(unsigned val);
	int putv(double val);

	/*! \cond PRIVATE */
public:
	virtual int putc(int ch) = PURE_VIRTUAL_FUNC;
	virtual int getc() = PURE_VIRTUAL_FUNC;
	virtual LPCTSTR gets() = PURE_VIRTUAL_FUNC;
//	int scanf(LPCTSTR format, va_list args);

protected:
	virtual void alloc(int bufsize=256) = PURE_VIRTUAL_FUNC;
	virtual void free() = PURE_VIRTUAL_FUNC;
	int		 m_base;
	/*! \endcond */
};

#endif /* SMALLPRINTF_H */
