#ifndef CMD_SHELL_H_
#define CMD_SHELL_H_

#include "XThread.h"
#include "XDelegate.h"
#include "XList.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

class XShell : public XThread {

public:
	typedef XDelegate< int (int argc, char** argv) > Handler;

private:
	struct Command {
		const char* command;
		Handler handler;
	};

	XList<Command> _commands;
	char mBuffer[1024];
	const char* mTitle;

	int Help(int argc, char** argv) {
		printf("Available commands:\n");
		for(XList<Command>::It it = _commands.First(); it != _commands.End(); ++it) {
			printf(" %s\n", _commands[it].command);
		}
		return 0;
	}

	int Exit(int argc, char** argv) {
		Stop();
		return 0;
	}

public:
	XShell(const char* title)
		: mTitle (title)
	{
		RegisterCommand("Help", Handler(this, &XShell::Help));
		RegisterCommand("Exit", Handler(this, &XShell::Exit));
	}

	virtual ~XShell() {
	}

	void RegisterCommand(const char* command, Handler handler) {
		Command cmd = { command, handler };
		_commands.Append(cmd);
	}

	virtual int Run();
};

#endif /* CMD_SHELL_H_ */
