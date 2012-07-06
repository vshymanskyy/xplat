#include "XThread.h"
#include <assert.h>

#if defined(TARGET_OS_WINDOWS)

	const DWORD MS_VC_EXCEPTION=0x406D1388;

	#pragma pack(push,8)
	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType; // Must be 0x1000.
		LPCSTR szName; // Pointer to name (in user addr space).
		DWORD dwThreadID; // Thread ID (-1=caller thread).
		DWORD dwFlags; // Reserved for future use, must be zero.
	} THREADNAME_INFO;
	#pragma pack(pop)

	static void SetThreadName(DWORD dwThreadID, char* threadName)
	{
		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = threadName;
		info.dwThreadID = dwThreadID;
		info.dwFlags = 0;

		__try
		{
			RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}

	DWORD XThread::_ThreadFunc(LPVOID pvThread)
	{
		if (XThread* pThis = (XThread*)pvThread) {
			LOG_MORE(pThis->mLog, "Thread started");
			pThis->mState = RUNNING;
			pThis->Run();
			pThis->mState = STOPPED;
		}
		return 0;
	}

	XThread::XThread(const XString &name)
		: mState	(NOT_STARTED)
		, mThread	(NULL)
		, mId		(0)
		, mLog		(name)
	{
	}

	bool XThread::Start()
	{
		if(mThread != NULL) {
			return false;
		}
		mState = NOT_STARTED;
		mThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_ThreadFunc, this, 0, &mId);
		if (mThread != NULL) {
			if (mLog.GetName()) {
				SetThreadName(mId, mLog.GetName());
			}
			return true;
		} else {
			return false;
		}
	}

	unsigned XThread::GetId() { return GetThreadId(mThread); }

	bool XThread::Wait() { return WaitForSingleObject(mThread, INFINITE) == WAIT_OBJECT_0; }

	void XThread::Suspend() { SuspendThread(mThread); }

	void XThread::Resume() { ResumeThread(mThread); }

	void XThread::Close() { CloseHandle(mThread); }

	void XThread::SleepMs(unsigned ms) {
		if (!ms) return;
		Sleep(ms);
	}

	unsigned XThread::GetCurrentId() { return GetCurrentThreadId(); }

#elif defined(TARGET_OS_UNIX)

	#include <sys/syscall.h>
	#include <unistd.h>

	void* XThread::_ThreadFunc(void* aPtr)
	{
		if (XThread* pThis = (XThread*) aPtr) {
			LOG_MORE(pThis->mLog, "Thread started");
			pThis->mTid = GetCurrentId();
			pThis->mState = RUNNING;
			pThis->Run();
			pThis->mState = STOPPED;
		}
		return NULL;
	}

	XThread::XThread(const XString &name)
		: mLog		(name)
		, mState	(NOT_STARTED)
		, mThread	(0)
		, mTid		(0)
	{
	}

	bool XThread::Start()
	{
		if (mThread != 0) {
			return false;
		}
		mState = NOT_STARTED;

		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, 32768);
		if (0 == pthread_create(&mThread, &attr, _ThreadFunc, this)) {
			if (mThread) {
				if (mLog.GetName()) {
					pthread_setname_np(mThread, mLog.GetName());
				}
				return true;
			}
		} else {
			LOG_CRIT(mLog, "Could not start thread (" << errno << "):" << strerror(errno) );
		}
		pthread_attr_destroy(&attr);

		return false;
	}

	unsigned XThread::GetId() {
		return mTid;
	}

	bool XThread::Wait() { return (pthread_join(mThread, NULL) == 0); }

	void XThread::Suspend() {
		//pthread_kill(mThread, SIGSTOP);
	}

	void XThread::Resume() {
		//pthread_kill(mThread, SIGCONT);
	}

	void XThread::Close() {
		pthread_detach(mThread);
	}

	void XThread::SleepMs(unsigned ms) {
		if (!ms) return;
		usleep(ms * 1000);
	}

	unsigned XThread::GetCurrentId() {
		return (unsigned)syscall(SYS_gettid);
	}

#else
#	error "Platform not supported"
#endif
