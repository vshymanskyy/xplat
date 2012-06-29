#include "XLog.h"
#include "XThread.h"

#if defined TARGET_OS_WINDOWS
	#include <Windows.h>
	void XLogger::AddEntry(const XLog::Stream::Data* data)
	{
		//TODO: lock
		const char* title = (data->mLog && data->mLog->GetName())?(data->mLog->GetName()):(data->mFunc);
		const XString str = XString::Format("%4d %02d:%02d:%02d.%03d %s: %s\n", data->mTid,
			data->mTime.hour,
			data->mTime.minute,
			data->mTime.second,
			data->mTime.msecond,
			title, data->mMsg);
		OutputDebugString((const char*)str);
	}
#else

	#define X_ANSI_FC_BLACK   "\x1b[22;30m"
	#define X_ANSI_FC_RED     "\x1b[22;31m"
	#define X_ANSI_FC_GREEN   "\x1b[22;32m"
	#define X_ANSI_FC_YELLOW  "\x1b[22;33m"
	#define X_ANSI_FC_BLUE    "\x1b[22;34m"
	#define X_ANSI_FC_MAGENTA "\x1b[22;35m"
	#define X_ANSI_FC_CYAN    "\x1b[22;36m"
	#define X_ANSI_FC_WHITE   "\x1b[22;37m"
	#define X_ANSI_FC_DEFAULT "\x1b[22;39m"

	#define X_ANSI_FC_BLACK_B   "\x1b[1;30m"
	#define X_ANSI_FC_RED_B     "\x1b[1;31m"
	#define X_ANSI_FC_GREEN_B   "\x1b[1;32m"
	#define X_ANSI_FC_YELLOW_B  "\x1b[1;33m"
	#define X_ANSI_FC_BLUE_B    "\x1b[1;34m"
	#define X_ANSI_FC_MAGENTA_B "\x1b[1;35m"
	#define X_ANSI_FC_CYAN_B    "\x1b[1;36m"
	#define X_ANSI_FC_WHITE_B   "\x1b[1;37m"
	#define X_ANSI_FC_DEFAULT_B "\x1b[1;39m"

	#define X_ANSI_FC_BLACK_D   "\x1b[2;30m"
	#define X_ANSI_FC_RED_D     "\x1b[2;31m"
	#define X_ANSI_FC_GREEN_D   "\x1b[2;32m"
	#define X_ANSI_FC_YELLOW_D  "\x1b[2;33m"
	#define X_ANSI_FC_BLUE_D    "\x1b[2;34m"
	#define X_ANSI_FC_MAGENTA_D "\x1b[2;35m"
	#define X_ANSI_FC_CYAN_D    "\x1b[2;36m"
	#define X_ANSI_FC_WHITE_D   "\x1b[2;37m"
	#define X_ANSI_FC_DEFAULT_D "\x1b[2;39m"

	#define X_ANSI_RESET "\x1b[0m"
	#define X_ANSI_DIM "\x1b[0;2m"

	#define X_ANSI_CLEAR "\x1b[2J"

	void XLogger::AddEntry(const XLog::Stream::Data* data)
	{
		//TODO: lock
		const char* title = (data->mLog && data->mLog->GetName().Length())?((char*)data->mLog->GetName()):(data->mFunc);

		const char* fmt =
				(data->mLevel <= XLog::CRIT) ? "%4d"X_ANSI_FC_GREEN" %02d:%02d:%02d.%03d"X_ANSI_FC_CYAN" %s:"X_ANSI_FC_RED" %s"X_ANSI_RESET"\n":
				(data->mLevel == XLog::WARN) ? "%4d"X_ANSI_FC_GREEN" %02d:%02d:%02d.%03d"X_ANSI_FC_CYAN" %s:"X_ANSI_FC_YELLOW" %s"X_ANSI_RESET"\n":
				(data->mLevel >= XLog::LOTS) ? "%4d"X_ANSI_FC_GREEN" %02d:%02d:%02d.%03d"X_ANSI_FC_CYAN" %s:"X_ANSI_DIM" %s"X_ANSI_RESET"\n":
				"%4d"X_ANSI_FC_GREEN" %02d:%02d:%02d.%03d"X_ANSI_FC_CYAN" %s:"X_ANSI_RESET" %s\n";

		fprintf((data->mLevel < XLog::WARN)?stderr:stdout, fmt, data->mTid,
			data->mTime.hour,
			data->mTime.minute,
			data->mTime.second,
			data->mTime.msecond,
			title, data->mMsg);
	}
#endif


XLog::Stream::Data::Data(const XLog* log, XLog::Level level, const char* func, const char* file, int line)
	: mFunc		(func)
	, mFile		(file)
	, mLine		(line)
	, mLog		(log)
	, mLevel	(level)
	, mTid		(XThread::GetCurrentId())
	, mRef		(1)
	, mPos		(mMsg)
{
	XPlatGetTime(&mTime, NULL);
}

XLog::Stream::Data::~Data() {
	if (mLog) {
		*mPos = '\0';

		((mLog->mLogger) ? (mLog->mLogger) :
			(XLogManager::Get().GetDefaultLogger()))->AddEntry(this);

		if (mLevel == FAIL) {
			abort();
		}
	}
}
