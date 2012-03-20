#include "XLog.h"
#include "XThread.h"

void XLogger::AddEntry(const XLog::Stream::Data* data)
{
	//TODO: lock
	const char* title = (data->mLog && data->mLog->GetName())?(data->mLog->GetName()):(data->mFunc);
	printf("%4d %02d:%02d:%02d.%03d %s: %s\n", data->mTid,
		data->mTime.hour,
		data->mTime.minute,
		data->mTime.second,
		data->mTime.msecond,
		title, data->mMsg);
}

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
