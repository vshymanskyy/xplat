#include "XThread.h"
#include <assert.h>

#if defined(TARGET_OS_WINDOWS)

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
		return (mThread != NULL);
	}

	unsigned XThread::GetId() { return GetThreadId(mThread); }

	bool XThread::Wait() { return WaitForSingleObject(mThread, INFINITE) == WAIT_OBJECT_0; }

	void XThread::Suspend() { SuspendThread(mThread); }

	void XThread::Resume() { ResumeThread(mThread); }

	void XThread::Close() { CloseHandle(mThread); }

	void XThread::SleepMs(unsigned ms) { Sleep(ms); }

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
		usleep(ms * 1000);
	}

	unsigned XThread::GetCurrentId() {
		return syscall(SYS_gettid);
	}

#else
#	error "Platform not supported"
#endif
