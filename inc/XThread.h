#ifndef THREAD_H
#define THREAD_H

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include "XPlat.h"
#include "XString.h"

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

	virtual ~XThread();

	virtual bool Start();

	virtual void Stop() {
		mState = STOPPING;
	}

	unsigned GetId();

	bool Wait();
	void Suspend();
	void Resume();

	bool IsStopping() const {
		return mState == STOPPING;
	}

	static void SleepMs(unsigned ms);

	static unsigned GetCurrentId();

	virtual int Run() = 0;

private:
	EState mState;

#if defined (TARGET_OS_WINDOWS)
	static DWORD WINAPI _ThreadFunc(LPVOID pvThread);
	HANDLE mHandle;
	DWORD mId;
#elif defined (TARGET_OS_UNIX)
	static void* _ThreadFunc(void* aPtr);
	pthread_t mThread;
	XString mName;
#else
#	error "Platform not supported"
#endif

};

#endif /* THREAD_H */
