#pragma once

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#define LOCKABLE class LOCK = XLockStub

class XLockStub {
public:
	XLockStub()		{}
	~XLockStub()		{}
	bool Lock(const uint32_t timeout = 0) { return true; }
	bool TryLock()	{ return true; }
	void Unlock()	{}
};

template < class LOCK >
class XAutoLock {
public:
	XAutoLock(LOCK& lock) : mLock(lock) { mLock.lock(); }
	~XAutoLock()	{ mLock.unlock(); }

private:
	LOCK& mLock;
};

#if defined(__GNUC__)

	#include <pthread.h>
	#include <time.h>

	class XMutex {
	public:
		XMutex()		{ pthread_mutex_init(&mLock, NULL); }
		~XMutex()		{ pthread_mutex_destroy(&mLock); }
		bool Lock(const uint32_t timeout = 0) {
			if (timeout) {
				timespec ts;
			    ts.tv_sec = (timeout / 1000);
			    ts.tv_nsec = ((timeout % 1000) * 1000000);
				return (pthread_mutex_timedlock(&mLock, &ts) == 0);
			} else {
				return (pthread_mutex_lock(&mLock) == 0);
			}
		}
		bool TryLock()	{ return (pthread_mutex_trylock(&mLock) == 0); }
		void Unlock()	{ pthread_mutex_unlock(&mLock); }

	private:
		pthread_mutex_t mLock;
	};

	class XMutexRecursive {
	public:
		XMutexRecursive() {
			pthread_mutexattr_t attr;
			pthread_mutexattr_init(&attr);
			pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
			pthread_mutex_init(&mLock, &attr);
			pthread_mutexattr_destroy(&attr);
		}
		~XMutexRecursive() { pthread_mutex_destroy(&mLock); }
		bool Lock(const uint32_t timeout = 0) {
			if (timeout) {
				timespec ts;
			    ts.tv_sec = (timeout / 1000);
			    ts.tv_nsec = ((timeout % 1000) * 1000000);
				return (pthread_mutex_timedlock(&mLock, &ts) == 0);
			} else {
				return (pthread_mutex_lock(&mLock) == 0);
			}
		}
		bool TryLock()	{ return (pthread_mutex_trylock(&mLock) == 0); }
		void Unlock()	{ pthread_mutex_unlock(&mLock); }

	private:
		pthread_mutex_t mLock;
	};

#elif defined (WIN32)

	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>

	class XMutexRecursive {
	public:
		XMutexRecursive()	{ InitializeCriticalSection(&mLock); }
		~XMutexRecursive()	{ DeleteCriticalSection(&mLock); }
		bool Lock(const uint32_t timeout = 0) {
			EnterCriticalSection(&mLock);
			return true;
		}
		bool TryLock()	{ return TryEnterCriticalSection(&mLock); }
		void Unlock()	{ LeaveCriticalSection(&mLock) }

	private:
		pthread_mutex_t mLock;
	};

#else
#error "Platform not supported"
#endif
