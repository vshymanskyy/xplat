#include "XLog.h"
#include "XCmdShell.h"

namespace XLogCmd {
	static int ShowLogs(int, char**) {
		XLogManager::Get().ShowLogs();
		return 0;
	}

	static int SetLog(int argc, char** argv) {
		return 0;
	}

	void Init(XShell& shell) {
		shell.RegisterCommand("logs", &ShowLogs);
		shell.RegisterCommand("log", &SetLog);
	}
}
