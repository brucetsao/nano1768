/*
 ===============================================================================
 Name        : ssi.h
 Description : customer port the tSSIHandler of lwIP
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/1/25	v1.0.0	First Edition.									
 */

#ifndef SSI_H_
#define SSI_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Function pointer for the SSI tag handler callback.
 *
 * This function will be called each time the HTTPD server detects a tag of the
 * form <!--#name--> in a .shtml, .ssi or .shtm file where "name" appears as
 * one of the tags supplied to http_set_ssi_handler in the ppcTags array.  The
 * returned insert string, which will be appended after the the string
 * "<!--#name-->" in file sent back to the client,should be written to pointer
 * pcInsert.  iInsertLen contains the size of the buffer pointed to by
 * pcInsert.  The iIndex parameter provides the zero-based index of the tag as
 * found in the ppcTags array and identifies the tag that is to be processed.
 *
 * The handler returns the number of characters written to pcInsert excluding
 * any terminating NULL or a negative number to indicate a failure (tag not
 * recognized, for example).
 *
 * Note that the behavior of this SSI mechanism is somewhat different from the
 * "normal" SSI processing as found in, for example, the Apache web server.  In
 * this case, the inserted text is appended following the SSI tag rather than
 * replacing the tag entirely.  This allows for an implementation that does not
 * require significant additional buffering of output data yet which will still
 * offer usable SSI functionality.  One downside to this approach is when
 * attempting to use SSI within JavaScript.  The SSI tag is structured to
 * resemble an HTML comment but this syntax does not constitute a comment
 * within JavaScript and, hence, leaving the tag in place will result in
 * problems in these cases.  To work around this, any SSI tag which needs to
 * output JavaScript code must do so in an encapsulated way, sending the whole
 * HTML <script>...</script> section as a single include.
 */
typedef uint16_t (*tSSIHandler)(int iIndex, char *pcInsert, int iInsertLen);

void http_set_ssi_handler(tSSIHandler pfnSSIHandler,
                          const char **ppcTags, int iNumTags);


#ifdef __cplusplus
}
#endif
#endif /* SSI_H_ */
