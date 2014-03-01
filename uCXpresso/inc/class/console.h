/*
 ===============================================================================
 Name        : console.h
 Author      : Jason
 Version     :
 Date		 : 2012/1/2
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :

 History     :
 ===============================================================================
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "class/object.h"
#include "class/smallprintf.h"
#include "class/stream.h"

/*! \enum CONSOLE_CTRL_T
 */
typedef enum {
	ends = 0,
	endl,
	dec,
	hex,
	oct,
	flush,
	ESC = 0x1b
}CONSOLE_CTRL_T;

/** \class Console console.h "class/console.h"
 * \brief The Console class provides a lightweight input/output stream to console.
 */
class Console: public CSmallPrintf {
public:
	/**Constructs a console object.
	 * \param io is a CStream object to provide the input/output stream.
	 *
	 * \code
	 * Example:
	 * 		CSerial cdc(USB);		// create an USB serial stream object.
	 * 		Console con(cdc);		// create a console by the USB stream.
	 *
	 * 		if ( con.isConnected() ) {
	 * 			con << "Hello World" << endl;
	 * 		}
	 * \endcode
	 */
	Console(CStream &io);

	/**Call the member function the close the console stream.
	 */
	virtual int  close();

	/**Call the member function to check the stream is ready to read (or write).
	 *
	 * \code
	 * Example:
	 * 		CSerial cdc(USB);		// create an USB serial stream object.
	 * 		Console con(cdc);		// create a console by the USB stream.
	 *
	 * 		if ( con.isConnected() ) {
	 * 			con << "Hello World" << endl;
	 * 		}
	 * \endcode
	 */
	virtual bool isConnected();

	/**Call the member function to enable or disable the echo.
	 * \param set is a boolean value, set true to enable the echo.
	 */
	virtual void echo(bool set);

	/**Call the member function to clear the screen of console.
	 */
	virtual void clear();

	/**Call the member function to put a character to the serial stream of console.
	 * \param ch is a integer value of character.
	 * \return the same value, if put a character successful;
	 */
	virtual int  putchar(int ch) { return putc(ch); }

	/**Call the member function to get a character from the serial stream of console.
	 * \return the received character.
	 */
	virtual int  getchar()		{ return getc(); }

	/**Call the member function to put a string to console.
	 * \param[in] str is a string pointer to be written.
	 * \return the how many character sent.
	 */
	virtual int  putstr(LPCTSTR str);

	/**Call the member function to get a string from the serial stream of console.
	 * \param[out] strbuf is a string pointer where the string stored.
	 * \param[in] size is a size_t to represent the size of strbuf to be read.
	 * \return the how many characters received.
	 */
	virtual int  getstr(LPTSTR strbuf, size_t size);

	/**Call the member function to write a block buffer to the serial stream of console.
	 * \param[in] buf is a pointer to a block data with the content to be written.
	 * \param[in] size is a integer value to specified the size of block to write.
	 * \return the how many bytes to write.
	 */
	virtual int  write(const void* buf, int size);

	/**Call the member function to read a block buffer from the serial stream of console.
	 * \param[out] buf is a pointer to a block data where the content read will be stored.
	 * \param[in] size is integer value to represent the size of block to be read.
	 * \return the how many byte to be read.
	 */
	virtual int	 read(void* buf, int size);

	/**A shorthand for read a character.
	 *
	 * \code
	 * Examle:
	 * 		char c;
	 * 		con >> c;
	 * \endcode
	 */
	virtual void operator >> (char &ch);

	/**A shorthand for read a byte value.
	 *
	 * \code
	 * Example:
	 * 		byte c;
	 * 		con >> c;
	 * \endcode
	 */
	virtual void operator >> (uint8_t &b);

	/**A shorthand for write a character to console.
	 * \return *this.
	 *
	 * \code
	 * Example:
	 * 		con << '?' << "Whate is it this year? " << 2012 << endl;
	 * \endcode
	 */
	virtual Console& operator << (TCHAR ch);

	/**A shorthand for write a string to console.
	 * \return *this.
	 *
	 * \code
	 * Example:
	 * 		con << '?' << "Whate is it this year? " << 2012 << endl;
	 * \endcode
	 */
	virtual Console& operator << (LPCTSTR str);

	/**A shorthand for write a integer value to console.
	 * \return *this.
	 *
	 * \code
	 * Example:
	 * 		con << '?' << "Whate is it this year? " << 2012 << endl;
	 * \endcode
	 */
	virtual inline Console& operator << (int val) {
		putv(val);
		return *this;
	}

	/**A shorthand for write a float value to console.
	 * \return *this.
	 */
	virtual inline Console& operator << (float val) {
		putv((double)val);
		return *this;
	}

	/**A shorthand for write a double value to console.
	 * \return *this.
	 */
	virtual inline Console& operator << (double val) {
		putv(val);
		return *this;
	}

	/**A shorthand for write a size_t value to console.
	 * \return *this;
	 */
	virtual inline Console& operator << (size_t val) {
		putv((unsigned)val);
		return *this;
	}

	/**A shorthand for write a unsigned integer 16 bits value to console.
	 * \return *this.
	 */
	virtual inline Console& operator << (uint16_t val) {
		putv((unsigned)val);
		return *this;
	}

	/*A shorthand for write a unsigned integer 32 bits value to console.
	 * \return *this.
	 */
	virtual inline Console& operator << (uint32_t val) {
		putv((unsigned)val);
		return *this;
	}

	/**A shorthand for write a unsigned integer 8 bits value to console.
	 * \return *this.
	 */
	virtual inline Console& operator << (uint8_t val) {
		putv((unsigned)val);
		return *this;
	}

	/**A shorthand for write a control operator to console.
	 * \return *this;
	 *
	 * \code
	 * Example:
	 * 		con << '?' << "Whate is it this year? " << 2012 << endl;
	 * \endcode
	 */
	virtual Console& operator << (CONSOLE_CTRL_T ctrl);

	/**Call the member function to retrieve the input stream.
	 * \return a CStream object for input.
	 */
	inline CStream* iStream() { return m_input; }

	/**Call the member function to retrieve the output stream.
	 * \return a CStrea object for output.
	 */
	inline CStream* oStream() { return m_output; }

	/*! \cond PRIVATE */
public:
	Console();
	Console(CStream &tx, CStream &rx);
	virtual void assign(CStream &tx, CStream &rx);
	virtual int putc(int ch);
	virtual int getc();
	virtual LPCTSTR gets();

protected:
	CStream *m_output;
	CStream *m_input;
	bool 	 m_echo;

	virtual void alloc(int bufsize=256);
	virtual void free();
	/*! \endcond */
};

#endif /* CONSOLE_H_ */
