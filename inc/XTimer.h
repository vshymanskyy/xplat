#ifndef TIMER_H_
#define TIMER_H_

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <XThread.h>
#include <XLocks.h>
#include <XList.h>
#include <XDelegate.h>

#if defined (TARGET_OS_UNIX)

class XTimerContext : private XThread
{
public:
	typedef XDelegate< void () > Handler;

public:
	XTimerContext()
		: mWaiting(mTimers.End())
	{
		if (pipe(mPipe) == -1) {
			XASSERT(false);
		}

		Start();
	}

	virtual ~XTimerContext() {
		Stop();
		Wait();
		close(mPipe[0]);
		close(mPipe[1]);
	}

	void SetTimer(Handler h, uint32_t ms, uint32_t repeat = 0) {
		TimerEntry entry;
		gettimeofday(&entry.mExpires, NULL);
		TimevalAddMs(&entry.mExpires, ms);
		entry.mPeriod = repeat;
		entry.mHandler = h;

		mLock.Lock();
		mTimers.Append(entry);
		Wake();
		mLock.Unlock();
	}

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

	static
	void TimevalAddMs(timeval *result, unsigned long ms) {
	    result->tv_sec += (ms / 1000);
	    result->tv_usec += ((ms % 1000) * 1000);
	    if (result->tv_usec > 1000000) {
	        result->tv_usec -= 1000000;
	        result->tv_sec++;
	    }
	}

	/* return an integer greater than, equal to, or less than 0,
	   according as the timeval a is greater than,
	   equal to, or less than the timeval b. */
	static
	int TimevalCmp(const timeval& a, const timeval& b) {
	    if (a.tv_sec > b.tv_sec)
	        return 1;
	    else if (a.tv_sec < b.tv_sec)
	        return -1;
	    else if (a.tv_usec > b.tv_usec)
	        return 1;
	    else if (a.tv_usec < b.tv_usec)
	        return -1;
	    return 0;
	}

	static
	uint32_t TimevalToMs(const timeval& a) {
	    return ((a.tv_sec * 1000) + (a.tv_usec / 1000));
	}

	static
	void TimevalFromMs(timeval* result, uint32_t ms) {
	    result->tv_sec = (ms / 1000);
	    result->tv_usec = ((ms % 1000) * 1000);
	}

	static
	int TimevalSub(const timeval& to, const timeval& from, timeval* result) {
	    if (TimevalCmp(to, from) < 0)
	        return -1;
	    result->tv_sec = to.tv_sec - from.tv_sec;
	    result->tv_usec = to.tv_usec - from.tv_usec;
	    if (result->tv_usec < 0)    {
	        result->tv_sec--;
	        result->tv_usec += 1000000;
	    }
	    return 0;
	}

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

	virtual int Run() {
		timeval now, wait;
		fd_set readSet;
		//fd_set xcptSet;

		while (!IsStopping()) {
			gettimeofday(&now, NULL);

			// find first timer that expires
			// TODO: maintain priority queue
			mLock.Lock();
			mWaiting = mTimers.First();
			for (XList<TimerEntry>::It it = mTimers.First()+1; it != mTimers.End(); ++it) {
				if (TimevalCmp(mTimers[mWaiting].mExpires, mTimers[it].mExpires) > 0) {
					mWaiting = it;
				}
			}
			if (mWaiting == mTimers.End()) {
				// Wait until waked
				mLock.Unlock();
				FD_ZERO(&readSet);
				FD_SET(mPipe[0], &readSet);
				//FD_ZERO(&xcptSet);
				//FD_SET(mPipe[0], &xcptSet);
				const int qty = select(mPipe[0]+1, &readSet, NULL, NULL, NULL);
				if (qty == 1 && FD_ISSET(mPipe[0], &readSet)) {
					// Waked, clear pipe
					char buff[16];
					while (read(mPipe[0], &buff, 16) == 16) {
					}
				} /* else if (qty == 1 && FD_ISSET(mPipe[0], &xcptSet)) {
					// Error
					return 0;
				} */
				continue;
			}

			TimevalSub(mTimers[mWaiting].mExpires, now, &wait);

			mLock.Unlock();

			FD_ZERO(&readSet);
			FD_SET(mPipe[0], &readSet);
			//FD_ZERO(&xcptSet);
			//FD_SET(mPipe[0], &xcptSet);
			const int qty = select(mPipe[0]+1, &readSet, NULL, NULL, &wait);

			if (qty == 0) {
				// Timeout
				mLock.Lock();
				if (mWaiting != mTimers.End()) { // The entry may be deleted during sleep
					mTimers[mWaiting].mHandler();
					if (mWaiting != mTimers.End()) { // The entry may be deleted in handler
						if (mTimers[mWaiting].mPeriod) {
							TimevalAddMs(&mTimers[mWaiting].mExpires, mTimers[mWaiting].mPeriod);
						} else {
							mTimers.Remove(mWaiting);
						}
					}
				}
				mLock.Unlock();
			} else if (qty == 1 && FD_ISSET(mPipe[0], &readSet)) {
				// Waked, clear pipe
				char buff[16];
				while (read(mPipe[0], &buff, 16) == 16) {
				}
				continue;
			} /*else if (qty == 1 && FD_ISSET(mPipe[0], &xcptSet)) {
				// Error
				return 0;
			}*/ else {
				// Error
				XASSERT(false);
			}
		}
		return 0;
	}

	void Wake() {
		char ctrl = '.';
		if (write(mPipe[1], &ctrl, 1) != 1) {
			XASSERT(false);
		}
	}

private:
	XList <TimerEntry>	mTimers;
	XList<TimerEntry>::It mWaiting;
	int					mPipe[2];
	XMutexRecursive		mLock;
};

#else
#	error "Platform not supported"
#endif

#endif /* TIMER_H_ */
