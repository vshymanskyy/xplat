#include "XCmdShell.h"
#include "XPlat.h"

int XShell::Run() {
	while(!IsStopping()) {
		printf("%s> ", mLog.GetName()?(char*)mLog.GetName():"cli");

		if (fgets(mBuffer, 1024, stdin)) {
			int argc = 0;
			if (char** argv = CommandLineToArgv(mBuffer, &argc)) {
				if(!argc) continue;
				for(XList<Command>::It it = mCommands.First(); it != mCommands.End(); ++it) {
					if(!strcasecmp(argv[0], mCommands[it].command)) {
						LOG(mLog, "Executing:" << mBuffer);
						mCommands[it].handler(argc, argv);
						goto executed;
					}
				}
				printf("Command '%s' not found\n", argv[0]);
executed:
				free(argv);
			}
		} else {
			break;
		}
	}
	return 0;
}
