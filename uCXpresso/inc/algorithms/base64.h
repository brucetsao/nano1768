#ifndef _BASE64_H
#define _BASE64_H

#include "class/string.h"

extern CString base64_encode(unsigned char const* , unsigned int len);
extern CString base64_decode(CString &encoded_string);

#endif
