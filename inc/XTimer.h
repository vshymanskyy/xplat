#ifndef _X_TIMER_H_
#define _X_TIMER_H_

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <XPlat.h>
#include <XDelegate.h>
#include <XList.h>
#include <XLocks.h>

#if defined (TARGET_OS_UNIX)

#include <XThread.h>

class XTimerContext : private XThread
{
public:
	typedef XDelegate< void (void*) > Handler;

public:
	XTimerContext()
		: mWaiting(mTimers.End())
	{
		if (0 != pipe(mPipe)) {
			X_FATAL("Could not initialize pipe (%d) %s", errno, strerror(errno));
		}
		X_ASSERT(mPipe[0]);
		X_ASSERT(mPipe[1]);

		Start();
	}

	virtual ~XTimerContext() {
		Stop();
		Wait();
		close(mPipe[0]);
		close(mPipe[1]);
	}

	static XTimerContext& Global() {
		static XTimerContext _instance;
		return _instance;
	}

	void SetTimer(Handler h, uint32_t ms, uint32_t repeat = 0, void* arg = NULL);

	void CancelTimer(Handler h, void* arg = NULL) {
		mLock.Lock();
		if ((mWaiting != mTimers.End()) && (mTimers[mWaiting].mHandler == h) && (mTimers[mWaiting].mArg == arg)) {
			mTimers.Remove(mWaiting);
			mWaiting = mTimers.End();
		} else {
			for (XList<TimerEntry>::It it = mTimers.First(); it != mTimers.End(); ++it) {
				if ((mTimers[it].mHandler == h) && (mTimers[it].mArg == arg)) {
					mTimers.Remove(it);
				}
			}
		}
		Wake();
		mLock.Unlock();
	}

private:

	struct TimerEntry {
		timeval		mExpires;
		uint32_t	mPeriod;
		Handler		mHandler;
		void*		mArg;
		int			mId;
	};

	virtual void Stop() {
		XThread::Stop();
		Wake();
	}

	virtual int Run();

	void Wake() {
		char ctrl = '.';
		if (write(mPipe[1], &ctrl, 1) != 1) {
			X_ASSERT(false);
		}
	}

private:
	XList <TimerEntry>	mTimers;
	XList<TimerEntry>::It mWaiting;
	int					mPipe[2];
	XMutexRecursive		mLock;
	fd_set				mReadSet;
	//fd_set			mXcptSet;
};

#elif defined (TARGET_OS_WINDOWS)

class XTimerContext
{
public:
	typedef XDelegate< void (void*) > Handler;

public:
	XTimerContext() {
		// Create the timer queue.
		hTimerQueue = CreateTimerQueue();
		if (NULL == hTimerQueue) {
			printf("CreateTimerQueue failed (%d)\n", GetLastError());
			return;
		}
	}

	virtual ~XTimerContext() {
		mLock.Lock();
		if (!DeleteTimerQueueEx(hTimerQueue, INVALID_HANDLE_VALUE)) {
			printf("DeleteTimerQueue failed (%d)\n", GetLastError());
		}
		//X_ASSERT_EQ(mTimers.Count(), 0, "%d");
		mLock.Unlock();
	}

	static XTimerContext& Global() {
		static XTimerContext _instance;
		return _instance;
	}

	void SetTimer(Handler h, uint32_t ms, uint32_t repeat = 0, void* arg = NULL) {
		TimerEntry* entry = new TimerEntry();
		entry->mContext = this;
		entry->mHandler = h;
		entry->mArg = arg;
		entry->mOnce = (repeat == 0);
		mLock.Lock();
		mTimers.Append(entry);

		// Set a timer to call the timer routine
		ULONG flags = WT_EXECUTEINTIMERTHREAD;
		if (repeat == 0)
			flags |= WT_EXECUTEONLYONCE;
		if (!CreateTimerQueueTimer( &entry->mTimer, hTimerQueue, &TimerRoutine, entry, ms, repeat, flags)) {
			printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
		}
		mLock.Unlock();
	}

	void CancelTimer(Handler h, void* arg = NULL) {
		mLock.Lock();
		for (XList<TimerEntry*>::It it = mTimers.First(); it != mTimers.End(); ++it) {
			if (mTimers[it]->mHandler == h && mTimers[it]->mArg == arg) {
				DeleteTimerQueueTimer (hTimerQueue, mTimers[it]->mTimer, NULL);
				delete mTimers[it];
				mTimers.Remove(it);
			}
		}
		mLock.Unlock();
	}

private:

	static VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
	{
		if (TimerEntry* entry = (TimerEntry*)lpParam) {
			entry->mHandler(entry->mArg);
			if (entry->mOnce) {
				//entry->mContext->mLock.Lock();
				//DeleteTimerQueueTimer (entry->mContext->hTimerQueue, entry->mTimer, NULL);
				//XList<TimerEntry*> &timers = entry->mContext->mTimers;
				//delete entry;
				//timers.Remove(timers.FindAfter(timers.First(), entry));
				//entry->mContext->mLock.Unlock();
			}
		}
	}

private:

	struct TimerEntry {
		XTimerContext* mContext;
		Handler mHandler;
		void* mArg;
		HANDLE mTimer;
		bool mOnce;
	};

	HANDLE hTimerQueue;
	XList<TimerEntry*>	mTimers;
	XMutexRecursive		mLock;
};

#endif

#endif /* _X_TIMER_H_ */
