#include "XThread.h"
#include <assert.h>

#if defined(TARGET_OS_WINDOWS)

	DWORD XThread::_ThreadFunc(LPVOID pvThread)
	{
		if (XThread* pThis = (XThread*)pvThread) {
			pThis->mState = RUNNING;
			pThis->Run();
			pThis->mState = STOPPED;
		}
		return 0;
	}

	XThread::XThread(const XString &name)
		: mState (NOT_STARTED)
		, mHandle (NULL)
		, mId (0)
	{
	}

	XThread::~XThread()
	{
		if(mHandle != NULL) {
			CloseHandle(mHandle);
		}
	}

	bool XThread::Start()
	{
		if(mHandle != NULL) {
			return false;
		}
		mState = NOT_STARTED;
		mHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_ThreadFunc, this, 0, &mId);
		return (mHandle != NULL);
	}

	unsigned XThread::GetId() { return GetThreadId(mHandle); }

	bool XThread::Wait() { return WaitForSingleObject(mHandle, INFINITE) == WAIT_OBJECT_0; }

	void XThread::Suspend() { SuspendThread(mHandle); }

	void XThread::Resume() { ResumeThread(mHandle); }

	void XThread::SleepMs(unsigned ms) { Sleep(ms); }

	unsigned XThread::GetCurrentId() { return GetCurrentThreadId(); }

#elif defined(TARGET_OS_UNIX)

	#include <sys/syscall.h>
	#include <unistd.h>

	void* XThread::_ThreadFunc(void* aPtr)
	{
		if (XThread* pThis = (XThread*) aPtr) {
			pThis->mState = RUNNING;
			pThis->Run();
			pThis->mState = STOPPED;
		}
		return NULL;
	}

	XThread::XThread(const XString &name)
		: mState(NOT_STARTED)
		, mThread(0)
		, mName(name)
	{
	}

	XThread::~XThread()
	{
		if (mThread != 0) {
			pthread_detach(mThread);
		}
	}

	bool XThread::Start()
	{
		if (mThread != 0) {
			return false;
		}
		mState = NOT_STARTED;

		if (pthread_create(&mThread, NULL, _ThreadFunc, this) == 0) {
			if (mThread) {
				if (mName.Length()) {
					pthread_setname_np(mThread, mName);
				}
				return true;
			}
		}
		return false;
	}

	/*unsigned Thread::GetId() {

	}*/

	bool XThread::Wait() {
		return (pthread_join(mThread, NULL) == 0);
	}

	void XThread::Suspend() {

	}

	void XThread::Resume() {

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
