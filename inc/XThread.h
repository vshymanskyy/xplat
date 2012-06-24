#ifndef _X_THREAD_H_
#define _X_THREAD_H_

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include "XPlat.h"
#include "XString.h"
#include "XLog.h"

#if defined (TARGET_OS_WINDOWS)
#	include <windows.h>
#elif defined (TARGET_OS_UNIX)
#	include <pthread.h>
#endif

#ifdef COMPILER_MSVC
class Event
{
public:
	Event () : mHandle (CreateEvent(0, FALSE, FALSE, 0)) {}
	~Event () {	CloseHandle(mHandle); }
	void Release () { SetEvent(mHandle); }
	void Wait () { WaitForSingleObject(mHandle, INFINITE); }
private:
	HANDLE mHandle;
};
#endif

class XThread
{
public:
	enum EState {
		NOT_STARTED, RUNNING, STOPPING, STOPPED
	};

public:
	XThread(const XString& name = "");

	virtual ~XThread() {
		if (Exists()) {
			LOG_MORE(mLog, "Waiting for thread to exit...");
			Stop();
			Wait();
			Close();
		}
	}

	virtual bool Start();

	virtual void Stop() { mState = STOPPING; }
	bool IsStopping() const { return mState == STOPPING; }
	bool Exists() const { return mThread != 0; }

	bool Wait();
	void Suspend();
	void Resume();
	void Close();

	unsigned GetId();
	
	static void SleepMs(unsigned ms);
	static unsigned GetCurrentId();
protected:
	virtual int Run() = 0;

protected:
	XLog mLog;

private:
	EState mState;

#if defined (TARGET_OS_WINDOWS)
	static DWORD WINAPI _ThreadFunc(LPVOID pvThread);
	HANDLE mThread;
	DWORD mId;
#elif defined (TARGET_OS_UNIX)
	static void* _ThreadFunc(void* aPtr);
	pthread_t mThread;
	unsigned mTid;
#else
#	error "Platform not supported"
#endif

};

#endif /* _X_THREAD_H_ */
