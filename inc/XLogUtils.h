#ifndef _X_LOG_UTILS_H_
#define _X_LOG_UTILS_H_

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <XLog.h>

#include <ctype.h>

class XFileLogger: public XLogger
{
public:
	XFileLogger(const char* fn) { mFile = fopen(fn, "a+"); if (!mFile) { X_FATAL("Cannot open file: %s", fn); } }
	virtual ~XFileLogger() { fclose(mFile); }

	virtual void AddEntry(const XLog::Stream::Data* data) {
		fprintf(mFile, "%4d %02d:%02d:%02d.%03d %s: %s\n", data->mTid,
			data->mTime.hour,
			data->mTime.minute,
			data->mTime.second,
			data->mTime.msecond,
			(data->mLog && data->mLog->GetName().Length())?
			((char*)data->mLog->GetName()):(data->mFunc),
			data->mMsg);
		fflush(mFile);
	}
private:
	FILE* mFile;
};

class XMultiLogger: public XLogger, public XList<XLogger*>
{
public:
	XMultiLogger(XLogger* l0, XLogger* l1, XLogger* l2 = NULL, XLogger* l3 = NULL) {
		if (l0) Append(l0);
		if (l1)	Append(l1);
		if (l2)	Append(l2);
		if (l3)	Append(l3);
	}

	~XMultiLogger() {
		for (It it = First(); it != End(); ++it)
			delete Get(it);
	}

	virtual void AddEntry(const XLog::Stream::Data* data) {
		for (It it = First(); it != End(); ++it)
			Get(it)->AddEntry(data);
	}
};

class XLogDump
{
	friend const XLog::Stream& operator << (const XLog::Stream& str, const XLogDump& d);
public:
	explicit XLogDump(const void* ptr, size_t len)
			: mInPtr(ptr), mInLen(len), mOutAddr(true), mOutWidth(16)
	{}
private:
	const void* mInPtr;
	size_t mInLen;
	bool mOutAddr;
	unsigned mOutWidth;
};

inline
const XLog::Stream& operator << (const XLog::Stream& str, const XLogDump& d) {
	const unsigned char* b = static_cast<const unsigned char*>(d.mInPtr);
	const unsigned char* const e = b + d.mInLen;

	str << '\n';

	while (b < e) {
		const unsigned char* const b2 = b;
		const unsigned char* const e2 = b + d.mOutWidth;

		if(d.mOutAddr) {
			str("%p: ", b);
		}
		while (b < e2) {
			if (b < e) {
				str("%02X ", *b & 0xFF);
			} else {
				str << "   ";
			}
			b++;
		}

		str << "| ";
		b = b2;
		while (b < e2) {
			char c = *b & 0xFF;
			if (b >= e) {
				break;
			} else if (!isprint(c)) {
				c = '.';
			}
			str << c;
			b++;
		}

		if (b < e) {
			str << '\n';
		}
	}

	return str;
}

struct XLogAutoTrace
{
	XLogAutoTrace(XLog& log, const char* name)
			: mLog(log), mName(name)
	{
		LOG(mLog, "-> " << mName)
	}

	~XLogAutoTrace() {
		LOG(mLog, "<- " << mName)
	}

private:
	XLog& mLog;
	const char* mName;
};

#endif /* _X_LOG_UTILS_H_ */
