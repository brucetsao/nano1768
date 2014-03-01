/*
 ===============================================================================
 Name        : mutex.h
 Author      : Jason
 Version     : v1.0.5
 Date		 : 2013/6/12
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/2/13	v1.0.0	First Edition									Jason
 2012/9/24	v1.0.1	Change message for MESSAGE_T (void*) only.		Jason
 2012/9/29	(cancel)Change mailbox name to integer value type.		Jason
 2013/1/03	v1.0.2	Change DEF_MAIL_COUNT to 1						Jason
 2013/4/19	v1.0.3	Add CMailBox::reset() member function			Jason
 2013/4/26	v1.0.4  Modify CMailBox::post default timeout to zero.  Jason
 2013/6/12	v1.0.5	Add CMailBox::peek() member function			Jason
 ===============================================================================
 */
#ifndef MAILBOX_H_
#define MAILBOX_H_

#include "class/object.h"
#define DEF_MAIL_COUNT	1

#define MESSAGE_T	xHandle

/*! \class CMailBox mailbox.h "class/mailbox.h"
 * \brief Use the CMailbox to receive a message pointer from sender.
 */
class CMailBox: public CObject {
public:
	/**Constructs a CMailBox object.
	 * \param name is a string to point to describe the name of mailbox.
	 * \param count is a integer value to specified the maximum number of message can be contain.
	 *
	 * \code
	 * Example:
	 * 		class CLedTask: public CThread {
	 * 		protected:
	 * 			virtual void run() {
	 *				CBus leds(LED2, LED3, LED4, END);
	 *				CMailBox mail("LED");
	 *				int *i;
	 *				while(1) {
	 *					i = (int *) mail.wait();
	 *					if ( i ) {
	 *						leds[*i] = !leds[*i];
	 *					}
	 *				}
	 * 			}
	 *		};
	 * \endcode
	 */
	CMailBox(LPCTSTR name, int count=DEF_MAIL_COUNT);

	/**Call the member function to wait a message from sender.
	 * \param timeout is an amount of time the task should block for a message to receive.
	 * \return MESSAGE_T
	 */
	virtual MESSAGE_T wait(uint32_t timeout=MAX_DELAY_TIME);

	/**Receive a message from mail box without remove
	 */
	virtual MESSAGE_T peek(uint32_t timeout=MAX_DELAY_TIME);

	/**Call the member function to retrieve the number of message stored in the mailbox.
	 * \return number of message.
	 */
	virtual int  count();	// how many mail in query

	/**Call the member function to send a message to receiver which the same mailbox name.
	 * \param name is a string to point to which the same name of mailbox to be received the message.
	 * \param msg is a pointer to the message (void*).
	 * \param timeout is a time to wait for post. (Default zero)
	 * \return true if post the message successful; otherwise, pose failed.
	 *
	 * \code
	 * Example:
	 * 	class Task1: public CThread {
	 * 	protected:
	 *		virtual void run() {
	 *			int i=0;
	 *			while(1) {
	 *				CMailBox::post(MAIL_LED, &i);
	 *				sleep(500);
	 *			}
	 *		}
	 *	};
	 * \endcode
	 * \remark The CMailBox::post is a static function.
	 */
	static  bool post(LPCTSTR name, MESSAGE_T msg, uint32_t timeout=0);

	/**Reset the mail box to original empty state.
	 *
	 */
	void reset();

	/*! \cond PRIVATE */
public:
	CMailBox();
	virtual ~CMailBox();
	virtual bool create(LPCTSTR name, size_t size=sizeof(MESSAGE_T), int count=DEF_MAIL_COUNT);
protected:
	xHandle m_handle;
	/*! \endcond */
};

#endif /* MSGBOX_H_ */
