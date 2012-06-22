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
	typedef XDelegate< void () > Handler;

public:
	XTimerContext()
		: mWaiting(mTimers.End())
	{
		if (pipe(mPipe) == -1) {
			X_ASSERT(false);
		}

		Start();
	}

	virtual ~XTimerContext() {
		Stop();
		Wait();
		close(mPipe[0]);
		close(mPipe[1]);
	}

	void SetTimer(Handler h, uint32_t ms, uint32_t repeat = 0);

	void CancelTimer(Handler h) {
		mLock.Lock();
		if ((mWaiting != mTimers.End()) && (mTimers[mWaiting].mHandler == h)) {
			mTimers.Remove(mWaiting);
			mWaiting = mTimers.End();
		} else {
			for (XList<TimerEntry>::It it = mTimers.First(); it != mTimers.End(); ++it) {
				if (mTimers[it].mHandler == h) {
					mTimers.Remove(it);
				}
			}
		}
		Wake();
		mLock.Unlock();
	}

private:

	struct TimerEntry {
		timeval mExpires;
		uint32_t mPeriod;
		Handler mHandler;
		int mId;
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
};

#elif defined (TARGET_OS_WINDOWS)

class XTimerContext
{
public:
	typedef XDelegate< void () > Handler;

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

	void SetTimer(Handler h, uint32_t ms, uint32_t repeat = 0) {
		TimerEntry* entry = new TimerEntry();
		entry->mContext = this;
		entry->mHandler = h;
		entry->mOnce = (repeat == 0);
		mLock.Lock();
		mTimers.Append(entry);

		// Set a timer to call the timer routine in 10 seconds.
		ULONG flags = WT_EXECUTEINTIMERTHREAD;
		if (repeat == 0)
			flags |= WT_EXECUTEONLYONCE;
		if (!CreateTimerQueueTimer( &entry->mTimer, hTimerQueue, &TimerRoutine, entry, ms, repeat, flags)) {
			printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
		}
		mLock.Unlock();
	}

	void CancelTimer(Handler h) {
		mLock.Lock();
		for (XList<TimerEntry*>::It it = mTimers.First(); it != mTimers.End(); ++it) {
			if (mTimers[it]->mHandler == h) {
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
			entry->mHandler();
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
		HANDLE mTimer;
		bool mOnce;
	};

	HANDLE hTimerQueue;
	XList<TimerEntry*>	mTimers;
	XMutexRecursive		mLock;
};

#endif

#endif /* _X_TIMER_H_ */
