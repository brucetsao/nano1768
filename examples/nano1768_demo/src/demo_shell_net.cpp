/*
 ===============================================================================
 Name        : demo_shell.cpp
 Author      : Jason
 Version     : 1.0.1
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : CSocket server Demo
 History	 :
 ===============================================================================
 */
#include "class/thread.h"
#include "class/socket.h"
#include "class/shell.h"

#define TELNET_PORT		23

class CShellDemoNET: public CThread {
protected:
	virtual void run() {
		CSocket server;
		CSocket *client;
		CShell  *shell;

		server.create(SOCK_STREAM);
		server.bind(TELNET_PORT);
		server.listen();

		while (1) {
			if ((client = server.accept()) != NULL) {
				shell = new CShell;
				if (shell) {
					shell->assign(client);
					shell->start();
				} else {
					delete client;
				}
			}
		}
	}
};
