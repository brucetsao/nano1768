/*
 ===============================================================================
 Name        : inifile.h
 Author      : Jason
 Version     :
 Date		 : 2011/12/29
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : 

 History     :
 ===============================================================================
 */

#ifndef INIFILE_H_
#define INIFILE_H_

#include "class/object.h"
#include "class/file.h"

#define MAX_INI_LINE_STRLEN	64

class CIniFile: public CObject {
protected:
	CFile 	m_file;
	LPCTSTR	m_filename;
	LPCTSTR m_section;
	LPTSTR	m_linebuf;
	int	  	m_offset;

public:
	CIniFile(LPCTSTR filename, LPCTSTR dir=DEF_USER_FILE_PATH);
	virtual ~CIniFile();

	virtual bool 	section(LPCTSTR name);
	virtual int  	getValue(LPCTSTR key, int def=0);
	virtual LPCTSTR	getValue(LPCTSTR key, LPCTSTR def="");
	virtual bool	getValue(LPCTSTR key, bool def=false);

};

#endif /* INIFILE_H_ */
