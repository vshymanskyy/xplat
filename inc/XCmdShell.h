#ifndef _X_CMD_SHELL_H_
#define _X_CMD_SHELL_H_

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include "XThread.h"
#include "XDelegate.h"
#include "XList.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

class XShell : private XThread {

public:
	typedef XDelegate< int (int argc, char** argv) > Handler;

private:
	struct Command {
		const char* command;
		Handler handler;
	};

private:
	int Help(int, char**) {
		printf("Available commands:\n");
		for(XList<Command>::It it = mCommands.First(); it != mCommands.End(); ++it) {
			printf(" %s\n", mCommands[it].command);
		}
		return 0;
	}

	int Exit(int, char**) {
		Stop();
		return 0;
	}

public:
	XShell(const char* title)
		: XThread	(title)
	{
		RegisterCommand("Help", Handler(this, &XShell::Help));
		RegisterCommand("Exit", Handler(this, &XShell::Exit));
	}

	virtual ~XShell() {
		// TODO: Wake thread
	}

	void RegisterCommand(const char* command, Handler handler) {
		Command cmd = { command, handler };
		mCommands.Append(cmd);
	}

	virtual int Run();

private:
	XList<Command> mCommands;
	char mBuffer[1024];
};

#endif /* _X_CMD_SHELL_H_ */
