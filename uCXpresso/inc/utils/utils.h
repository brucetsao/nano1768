/*
 * utils.h
 *
 *  Created on: 2011/4/15
 *      Author: jason
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "uCXpresso.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void _swap(void *v, int size);

extern char *spn_malloc(unsigned long size, const char *format, ...);
extern char *safestr(char *str, unsigned long max_size);

extern int strincmp(LPCTSTR str1, LPCTSTR str2, int len);
extern int strifind(LPCTSTR str, LPCTSTR find);

#define CMPIC(a, b) (((a)>='A' && (a)<='Z' ? (a)-'A' + 'a' : (a)) == ((b)>='A' && (b)<='Z' ? (b)-'A' + 'a' : (b)))

extern uint32_t	gethex(LPCTSTR s);
extern double	fmodf(double f, double div);

#ifdef __cplusplus
}
#endif
#endif /* UTILS_H_ */
