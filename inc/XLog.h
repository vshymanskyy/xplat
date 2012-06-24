#ifndef _X_LOG_H_
#define _X_LOG_H_

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "XList.h"
#include "XTime.h"
#include "XString.h"

#define __LOG_ENTRY_SIZE 1024

//#define LOG_STR(val) STRINGIFY(val)
//#define LOG_VAL(val) "(" LOG_STR(val) ")" << " = " << (val)
//#define LOG_LEVEL_MAX Log::WARN

#ifdef LOG_LEVEL_MAX
#define __LOG(log, level, ...) { const XLog& __l = log; if (LOG_LEVEL_MAX >= level && __l.GetLevel() >= level) { XLog::Stream logTrace(&__l, level, __FUNCTION__, __FILE__, __LINE__); logTrace << __VA_ARGS__; } }
#else
#define __LOG(log, level, ...) { const XLog& __l = log; if (__l.GetLevel() >= level) { XLog::Stream logTrace(&__l, level, __FUNCTION__, __FILE__, __LINE__); logTrace << __VA_ARGS__; } }
#endif

#define FMT(...) LogStub();logTrace(__VA_ARGS__)

#define LOG_DEEP(log, ...) __LOG(log, XLog::DEEP, __VA_ARGS__)
#define LOG_LOTS(log, ...) __LOG(log, XLog::LOTS, __VA_ARGS__)
#define LOG_MORE(log, ...) __LOG(log, XLog::MORE, __VA_ARGS__)
#define LOG(log, ...) __LOG(log, XLog::NORM, __VA_ARGS__)
#define LOG_WARN(log, ...) __LOG(log, XLog::WARN, __VA_ARGS__)
#define LOG_CRIT(log, ...) __LOG(log, XLog::CRIT, __VA_ARGS__)
#define LOG_FAIL(log, ...) __LOG(log, XLog::FAIL, __VA_ARGS__)

struct XLogger;

class LogStub {
};

class XLog
{
public:
	enum Level
	{
		FAIL,       // Shows always, terminates application
		CRIT,       // Shows always
		WARN,       // Shown only when log level != LOG_NONE
		INFO,       // Log level 0
		NORM,       // Log level 1
		MORE,       // Log level 2
		LOTS,       // Log level 3
		DEEP,       // Log level 4

		LOG_LEVEL_QTY
	};

	class Stream
	{
	public:
		struct Data {
			Data(const XLog* log, Level level, const char* func, const char* file, int line);
			~Data();

			void IncRef() { ++mRef; }
			unsigned DecRef() { return --mRef; }
			void IncPos(int qty) { if (qty > 0) { mPos += qty; } }
			char* GetPos() const { return mPos; }
			unsigned GetQty() const { return mMsg+X_COUNTOF(mMsg)-mPos-1; }
			unsigned GetMsgLen() const { return mPos-mMsg; }
		public:
			char mMsg[__LOG_ENTRY_SIZE];
			const char* mFunc;
			const char* mFile;
			const int mLine;
			const XLog* mLog;
			const Level mLevel;
			XPlatDateTime mTime;
			const unsigned mTid;
		private:
			unsigned mRef;
			char* mPos;
		};

		/// @brief Declared as private to prevent usage of assignment operator
		Stream& operator=(const Stream&);

	public:
		Stream(const XLog* log, Level level, const char* func, const char* file, int line)
			: mData(new Data(log, level, func, file, line))
		{}

		Stream(const Stream& log) : mData(log.mData) {
			mData->IncRef();
		}

		~Stream() {
			if (0 == mData->DecRef()) delete mData;
		}

		#define __LOG_APPEND(fmt, ...) { mData->IncPos(snprintf(mData->GetPos(), mData->GetQty(), (fmt), __VA_ARGS__)); }

		const Stream& operator << (bool v) const                { return (*this) << (v?"true":"false"); }
		const Stream& operator << (const void* v) const         { __LOG_APPEND("%p ", v) return *this; }

		const Stream& operator << (short v) const               { __LOG_APPEND("%hi ", v) return *this; }
		const Stream& operator << (int v) const                 { __LOG_APPEND("%i ", v) return *this; }
		const Stream& operator << (long v) const                { __LOG_APPEND("%li ", v) return *this; }
		const Stream& operator << (long long v) const           { __LOG_APPEND("%lli ", v) return *this; }

		const Stream& operator << (unsigned short v) const      { __LOG_APPEND("%hu ", v) return *this; }
		const Stream& operator << (unsigned int v) const        { __LOG_APPEND("%u ", v) return *this; }
		const Stream& operator << (unsigned long v) const       { __LOG_APPEND("%lu ", v) return *this; }
		const Stream& operator << (unsigned long long v) const  { __LOG_APPEND("%llu ", v) return *this; }

		const Stream& operator << (float v) const               { __LOG_APPEND("%.3f ", v) return *this; }
		const Stream& operator << (double v) const              { __LOG_APPEND("%.3f ", v) return *this; }
		const Stream& operator << (long double v) const         { __LOG_APPEND("%.3Lf ", v) return *this; }

		#undef __LOG_APPEND

		const Stream& operator << (char c) const {
			if (mData->GetQty()) {
				*mData->GetPos() = c;
				mData->IncPos(1);
			}
			return *this;
		}

		const Stream& operator << (const char* s) const {
			if (!s) {
				s = "<null>";
			}
			size_t len = strlen(s);
			if (len > mData->GetQty())
				len = mData->GetQty();
			memcpy(mData->GetPos(), s, len);
			mData->IncPos(len);
			return *this;
		}

		const Stream& operator << (const LogStub& str) const {
			(void)str;
			return *this;
		}

		const Stream& operator () (const char* fmt, ...) const GCC_SPECIFIC(__attribute__((format(printf,2,3)))) {
			va_list args;
			va_start(args, fmt);
			mData->IncPos(vsnprintf(mData->GetPos(), mData->GetQty(), fmt, args));
			va_end(args);
			return *this;
		}

	private:
		Data* mData;
	};

public:

	XLog(const char* name = NULL, Level level = NORM, XLogger* logger = NULL);

	~XLog();

	XString GetName() const { return mName; }
	void SetName(const XString& name) { mName = name; }
	Level GetLevel() const { return mLevel; }
	void SetLevel(const Level l) { mLevel = l; }

private:
	XString mName;
	Level mLevel;
	XLogger* mLogger;
};

struct XLogger {
	XLogger() {}
	virtual ~XLogger() {}
	virtual void AddEntry(const XLog::Stream::Data* data);
};

class XLogManager: public Singleton<XLogManager>
{
	friend class XLog;
	friend class Singleton<XLogManager>;
public:
	void SetDefaultLogger(XLogger* log) {
		if (mDefaultLogger) {
			delete mDefaultLogger;
		}
		mDefaultLogger = log;
	}

	XLogger* GetDefaultLogger() {
		return mDefaultLogger;
	}

	void ShowLogs() const {
		printf("Registered logs:\n");
		for (XList<XLog*>::It i = mLogs.First(); i != mLogs.End(); ++i) {
			printf(" %s\n", (char*)mLogs[i]->GetName());
		}
	}

private:
	XLogManager()
		: mDefaultLogger(new XLogger())
	{
	}

	~XLogManager() {
		if (mDefaultLogger)
			delete mDefaultLogger;
	}

	void AddLog(XLog* log) {
		mLogs.Append(log);
	}

	void RemoveLog(XLog* log) {
		XList<XLog*>::It node = mLogs.FindAfter(mLogs.First(), log);
		mLogs.Remove(node);
	}

private:
	XLogger* mDefaultLogger;
	XList<XLog*> mLogs;
};

inline
XLog::XLog(const char* name, Level level, XLogger* logger)
		: mName(name), mLevel(level), mLogger(logger)
{
	XLogManager::Get().AddLog(this);
}

inline
XLog::~XLog()
{
	XLogManager::Get().RemoveLog(this);
}

#endif /* _X_LOG_H_ */
