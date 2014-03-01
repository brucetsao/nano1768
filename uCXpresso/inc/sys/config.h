#ifndef CONFIG_H_
#define CONFIG_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "uCXpresso.h"

#define MAX_SNTP_SERVER	3

typedef struct _upnp_ {
	char type[32];
	char manufacturer[32];
	char manufacturerURL[64];
	char modeName[32];
	char modeNumber[32];
	char modeDescription[64];
	char modeURL[64];
	char server[32];
	int  localPort;
}UPNP_T;


typedef struct _config_ {
	// SYSTEM
	uint32_t 	SN[5];		// CPU Serial Number
	bool		watchdog;
	int			main_stack;
	int			tcpipStack;

	// NETWORK
	bool		network;
	char		hostname[32];
	bool		autoIP;
	uint32_t	ipaddr;
	uint32_t	netmask;
	uint32_t	gwaddr;
	uint32_t	dnsIP;
	bool		ftpd;
	bool 		httpd;
	bool		sntp;

	// FTPD
	char		ftp_root[32];
	char		ftp_user[16];
	char		ftp_pswd[16];

	// HTTPD
	char		http_root[32];
	bool		ssi;
	bool		cgi;

	// SNTP
	uint32_t	sntp_periodtime;
	uint32_t	sntp_timezone;
	char		sntp_server[MAX_SNTP_SERVER][32];

	// UPNP
	UPNP_T		*upnp;
}CONFIG_T;

extern CONFIG_T	config;

#ifdef __cplusplus
}
#endif
#endif /* CONFIG_H_ */
