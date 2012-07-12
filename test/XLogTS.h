#include "XLog.h"
#include <XTimeCounter.h>

#include <cxxtest/TestSuite.h>

#define _USE_MATH_DEFINES
#include <math.h>

class LogTS: public CxxTest::TestSuite
{
public:

	struct VoidLogger : public XLogger {
		VoidLogger() {}
		virtual ~VoidLogger() {}
		virtual void AddEntry(const XLog::Stream::Data*) { }
	};

	class LoggerForTesting : public XLogger {
	public:
		LoggerForTesting() : mLastMsgLen(0) {}

		virtual void AddEntry(const XLog::Stream::Data* data) {
			mLastMsgLen = data->GetMsgLen();
			memcpy(mLastMsgBuff, data->mMsg, mLastMsgLen);
		}

		const char* LastMsg() const { return mLastMsgBuff; }
		unsigned LastMsgLen() const { return mLastMsgLen; }

	private:
		char mLastMsgBuff[__LOG_ENTRY_SIZE];
		unsigned mLastMsgLen;
	};

	void testLog(void)
	{
		LoggerForTesting tl;
		XLog l("Mixed output", XLog::NORM, &tl);
		LOG(l, FMT("hello %s", "world ") << 2+3 << FMT("1") << FMT("2"));
		TS_ASSERT_SAME_DATA(tl.LastMsg(), "hello world 5 12", tl.LastMsgLen());
		LOG(l, 128 << FMT("0x%08X ", 8716348) << FMT("%3.8f",M_PI));
		TS_ASSERT_SAME_DATA(tl.LastMsg(), "128 0x0085003C 3.14159265", tl.LastMsgLen());
	}

	void LogBench1(void)
	{
		LoggerForTesting tl;
		XLog l("Test", XLog::NORM, &tl);
		XTimeCounter tc;
		for(int i=0; i<100000; i++) {
			LOG(l, FMT("hello %s", "world "));
		}
		LOG(NULL, "100000 Normal prints:" << tc.Elapsed() << "ms, avg:" << double(tc.Elapsed())/100000 << "ms");
	}

	void testLogDump(void)
	{
		//Log l("Dump Test");
		//char buffer[100];
		//LOG(l, LogDump(buffer, sizeof(buffer)));
	}


};
