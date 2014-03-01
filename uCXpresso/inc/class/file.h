/*
 ===============================================================================
 Name        : file.h
 Author      : Jason
 Version     :
 Date		 : 2012/2/18
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 History     :
 ===============================================================================
 */

#ifndef FILE_H_
#define FILE_H_

#include "class/stream.h"
#include "class/list.h"
#include "class/string.h"
#include "class/time.h"

///////////////////////////////////////////////////////////////////////////////
class CFileInfo: public CObject {
public:
	/* file attribute bits */
	enum attrib {
		fa_readonly = 0x01,
		fa_hidden = 0x02,
		fa_system = 0x04,
		fa_volume = 0x08,
		fa_dir = 0x10,
		fa_archive = 0x20
	};

public:
	CString		name;
	CTime		time;
	uint32_t	size;		/* File size */
	uint32_t	attrib;		/* Attribute */

public:
	inline bool isDir()	{ return ((attrib & fa_dir)!=0); }
	inline bool isReadOnly() { return ((attrib & fa_readonly)!=0); }
	inline bool isHidden() { return ((attrib & fa_hidden)!=0); }
};

///////////////////////////////////////////////////////////////////////////////
#define DEF_USER_FILE_PATH	"\\home"

// CFile Class
class CFile: public CStream {
public:
	// file open flags
	typedef enum {
		READ			= 0x01,
		OPEN_EXISTING	= 0x00,
		WRITE			= 0x02,
		CREATE_NEW		= 0x04,
		CREATE_ALWAYS	= 0x08,
		OPEN_ALWAYS		= 0x10
	}FILE_FLAG_T;

protected:
	xHandle		m_hFile;
	int			m_offset;
	FILE_FLAG_T	m_flag;
	LPCTSTR 	m_szLocalPath;

public:
	CFile();
	CFile(LPCTSTR filename, FILE_FLAG_T flag=CFile::READ, LPCTSTR dir=DEF_USER_FILE_PATH);
	virtual ~CFile();

	virtual void	 path(LPCTSTR dir);
	inline void 	 path(CString &s) { path((LPCTSTR)s); }

	virtual bool 	 open(LPCTSTR filename, FILE_FLAG_T flag=CFile::READ);
	virtual int	 	 close();

	virtual bool 	 seek(uint32_t offset);
	virtual void	 flush();
	virtual uint32_t size();
	virtual bool	 isEOF();

public:
	// stream virtual functions
	virtual int	readable();
	virtual int	writeable();
	virtual int read(void *buf, int len, bool block=true);
	virtual int write(const void *buf, int len, bool block=true);
	virtual bool isConnected();

public:
	virtual int readln(char *str, int len);
	virtual int offset();	// return current file offset
	virtual LPCTSTR gets();

public:
	// File System static functions
	void remove(LPCTSTR filename);
	void rename(LPCTSTR oldName, LPCTSTR newName);
	static	uint32_t diskFreeSize();
};

///////////////////////////////////////////////////////////////////////////////

class CDir: public CObject {
protected:
	xHandle m_dir;
	CList 	m_list;
public:
	CDir();
	CDir(LPCTSTR path);
	virtual ~CDir();
	virtual int assign(LPCTSTR path);
	virtual CList* operator -> ();
};

#endif /* FILE_H_ */
