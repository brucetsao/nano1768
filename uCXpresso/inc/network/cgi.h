/*
 ===============================================================================
 Name        : cgi.h
 Description : customer port the tCGIHandler of lwIP
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/2/14	v1.0.0	First Edition.									
 */

#ifndef CGI_H_
#define CGI_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Function pointer for a CGI script handler.
 *
 * This function is called each time the HTTPD server is asked for a file
 * whose name was previously registered as a CGI function using a call to
 * http_set_cgi_handler. The iIndex parameter provides the index of the
 * CGI within the ppcURLs array passed to http_set_cgi_handler. Parameters
 * pcParam and pcValue provide access to the parameters provided along with
 * the URI. iNumParams provides a count of the entries in the pcParam and
 * pcValue arrays. Each entry in the pcParam array contains the name of a
 * parameter with the corresponding entry in the pcValue array containing the
 * value for that parameter. Note that pcParam may contain multiple elements
 * with the same name if, for example, a multi-selection list control is used
 * in the form generating the data.
 *
 * The function should return a pointer to a character string which is the
 * path and filename of the response that is to be sent to the connected
 * browser, for example "/thanks.htm" or "/response/error.ssi".
 *
 * The maximum number of parameters that will be passed to this function via
 * iNumParams is defined by LWIP_HTTPD_MAX_CGI_PARAMETERS. Any parameters in the incoming
 * HTTP request above this number will be discarded.
 *
 * Requests intended for use by this CGI mechanism must be sent using the GET
 * method (which encodes all parameters within the URI rather than in a block
 * later in the request). Attempts to use the POST method will result in the
 * request being ignored.
 *
 */
typedef int (*tCGIHandler)(int iIndex, int iNumParams, char *pcParam[], char *pcValue[], const char **response);

/*
 * Structure defining the base filename (URL) of a CGI and the associated
 * function which is to be called when that URL is requested.
 */
typedef struct
{
    const char *pcCGIName;
    tCGIHandler pfnCGIHandler;
} tCGI;

extern void http_set_cgi_handlers(const tCGI *pCGIs, int iNumHandlers);

extern int FindCGIParameter(const char *pcToFind, char *pcParam[], int iNumParams);
extern unsigned long DecodeFormString(const char *pcEncoded, char *pcDecoded,unsigned long ulLen);
extern unsigned long EncodeFormString(const char *pcDecoded, char *pcEncoded,unsigned long ulLen);

#ifdef __cplusplus
}
#endif
#endif /* SSI_H_ */
