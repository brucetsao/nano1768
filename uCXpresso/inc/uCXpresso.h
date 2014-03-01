/*! \headerfile ucxpresso.h "uCXpresso.h"
 *
 */
#ifndef UCXPRESSO_H
#define UCXPRESSO_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// VERSION
#define RELEASED				255

/*! \cond PRIVATE */
#define uCXpresso_VER_MAJOR		1
#define uCXpresso_VER_MINOR		4
#define uCXpresso_VER_REV		0
#define uCXpresso_VER_RC		0
#define uCXpresso_VER_BUILD		"2014/2/8"
#define uCXpresso_VER_STR		"V1.4.0"

#define FATFS_VERSION			"R0.09b"

//
// device
//
#define NANO176X

//
//	defined
//
#define DEFAULT_POOL_SIZE		23000

#if __GNUC__				// GCC
	#if __bool_true_false_are_defined==0
		typedef enum BOOLEAN
		{
			false = 0,
			true = !false
		}bool;
	#endif
	#define TRUE	true
	#define FALSE	false
	#define BOOL	bool
	#define PACK_STRUCT __attribute__ ((__packed__))
#endif

typedef const char*			LPCTSTR;
typedef char*				LPTSTR;

/* These types must be 16-bit, 32-bit or larger integer */
typedef int					INT;
typedef unsigned int		UINT;

/* These types must be 8-bit integer  */
#ifndef CHAR
typedef char				CHAR;
#endif

#ifndef TCHAR
typedef char TCHAR;
#endif

typedef unsigned char		UCHAR;
typedef unsigned char		BYTE;

/* These types must be 16-bit integer */
typedef short				SHORT;
typedef unsigned short		USHORT;
typedef unsigned short		WORD;
typedef unsigned short		WCHAR;

/* These types must be 32-bit integer */
typedef long				LONG;
typedef unsigned long		ULONG;
typedef unsigned long		DWORD;

typedef unsigned char		byte;
typedef unsigned short		word;
typedef unsigned long		dword;
typedef bool 				boolean;



/*! \cond PRIVATE */
/*! \union _u16_u ucxpresso.h "uCXpresso.h"
 */
typedef union _u16_u
{
	uint16_t   Int;
	uint8_t	Char[2];
}u16_u;

/*! \union _u132_u ucxpresso.h "uCXpresso.h"
 */
typedef union _u32_u
{
	uint32_t	Long;
	uint16_t	Int[2];
	uint8_t	Char[4];
}u32_u;

/*! \union _u64_u ucxpresso.h "uCXpresso.h"
 */
typedef union _u64_u
{
	uint64_t	Int64;
    uint32_t   	Long[2];
    uint16_t   	Int[4];
	uint8_t		Char[8];
}u64_u;
/*! \endcond */

#define null_str			((const char *)0)
#define PURE_VIRTUAL_FUNC	0
typedef void* 				xHandle;

//
// Core
//
extern LPCTSTR getPartID();

//
// RTOS
//
#define MAX_DELAY_TIME	0xffffffff
extern void sleep(int ms);
extern void sysReboot(void);

//
//	LWIP
//
/*! \union IP_ADDR_T socket.h "class/socket.h"
 */
typedef union {
	uint32_t	addr;		///< a 32bits unsigned long integer type address
	uint8_t		addr8[4];	///< a 4 bytes array address
}IP_ADDR_T;

// lwip error code
#ifndef ERR_OK
#define ERR_OK          0    /* No error, everything OK. */
#define ERR_MEM        -1    /* Out of memory error.     */
#define ERR_BUF        -2    /* Buffer error.            */
#define ERR_TIMEOUT    -3    /* Timeout.                 */
#define ERR_RTE        -4    /* Routing problem.         */
#define ERR_INPROGRESS -5    /* Operation in progress    */
#define ERR_VAL        -6    /* Illegal value.           */
#define ERR_WOULDBLOCK -7    /* Operation would block.   */
#define ERR_USE        -8    /* Address in use.          */
#define ERR_ISCONN     -9    /* Already connected.       */
#define ERR_ABRT       -10   /* Connection aborted.      */
#define ERR_RST        -11   /* Connection reset.        */
#define ERR_CLSD       -12   /* Connection closed.       */
#define ERR_CONN       -13   /* Not connected.           */
#define ERR_ARG        -14   /* Illegal argument.        */
#define ERR_IF         -15   /* Low-level netif error    */
#define ERR_IS_FATAL(e) ((e) < ERR_ISCONN)
#endif						// ERROR_CODE
typedef int				ERR_T;

//
// Bit Control
//
#define bit(x) 		(1<<x)
#define bitmask(x) 	(_bit(x)-1)

#define bit_set(f,b)	f |= bit(b)
#define bit_clr(f,b)	f &= ~bit(b)
#define bit_chk(f,b)    ((f & bit(b))? true : false)

//
// pool memory functions
//
#ifndef _POOL_MEM_FUNCS_
#define _POOL_MEM_FUNCS_
extern void *pool_memcpy(void *dest, const void *sour, size_t size);
extern void *pool_memset(void *dest, int val, size_t size);
#endif

extern void pool_memadd(unsigned long base, size_t size);
#define memcpy(x,y,z)	pool_memcpy(x,y,z)
#define memset(x,y,z)	pool_memset(x,y,z)

extern size_t heapAvailableSize();		// return the available size of heap memory
extern void *tryMalloc(size_t size);	// try to malloc a memory with

//
// Miscellaneous
//
#define MHZ(x)				(x*1000000ul)
#define KHZ(x)				(x*1000ul)
#define MAX(a, b)			(((a) > (b)) ? (a) : (b))
#define MIN(a, b)			(((a) < (b)) ? (a) : (b))
#define ABS(a)				(((a) < 0) ? -(a) : (a))
#define OFFSET(a, b, max)	(((a) >= (b)) ? (a-b):(max-b+a+1))		// offset
#define constrain(x,a,b)	((x<a)?a:(x>b)?b:x)

#define MAX_UINT32			0xffffffff
#define CRLF				"\r\n"
#define NO_EFFECT			{}

//
// nano1768 to UNO.Net PIN name convertor
//
#define UNO_A0	AD0
#define UNO_A1	AD1
#define UNO_A2	AD2
#define UNO_A3	AD3
#define UNO_A4	AD4
#define UNO_A5	AD5

#define UNO_0	P27 	// 0
#define UNO_1	P28 	// 1
#define UNO_2	P26 	// 2 , PWM1
#define UNO_3	P25 	// 3 , PWM2
#define UNO_4	P24 	// 4 , PWM3
#define UNO_5	P23 	// 5 , PWM4
#define UNO_6	P22 	// 6 , PWM5
#define UNO_7	P21 	// 7 , PWM6

#define UNO_8	P9		// 8
#define UNO_9	P10 	// 9
#define UNO_10	P8 		// 10
#define UNO_11	P5 		// 11
#define UNO_12	P6 		// 12
#define UNO_13	P7 		// 13

#define	D0	UNO_0
#define D1	UNO_1
#define D2	UNO_2
#define D3	UNO_3
#define D4	UNO_4
#define D5	UNO_5
#define D6	UNO_6
#define D7	UNO_7
#define D8	UNO_8
#define D9	UNO_9
#define D10 UNO_10
#define D11	UNO_11
#define D12	UNO_12
#define D13 UNO_13
#define D14	P11
#define D15	P12
#define D16 P13
#define D17 P14
#define D18	P30
#define D19	P29
#define A0	AD0
#define A1	AD1
#define A2	AD2
#define A3	AD3
#define A4	AD4
#define A5	AD5

extern const unsigned char _zero_[];

//
// for GCC/G++ (newlib)
//
#ifndef __IMPORT
#define __IMPORT
#endif

#ifndef _READ_WRITE_RETURN_TYPE
#define _READ_WRITE_RETURN_TYPE _ssize_t
#endif

#ifdef __cplusplus
}
#endif

//
// C++ Library
//
#ifdef __cplusplus
extern "C++" {
	float map(float x, float in_min, float in_max, float out_min, float out_max);
	int   map(int x, int in_min, int in_max, int out_min, int out_max);
}
#endif

#endif // NANO_H
