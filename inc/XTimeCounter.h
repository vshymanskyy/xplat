#ifndef _X_TIME_COUNTER_H_
#define _X_TIME_COUNTER_H_

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <XPlat.h>

#if defined(TARGET_OS_WINDOWS)

	#include <windows.h>

	class XTimeCounter {
	public:
		XTimeCounter() {
			QueryPerformanceFrequency(&mFreq);
			QueryPerformanceCounter(&mStart);
		}

		long Reset() {
			LARGE_INTEGER prev = mStart;
			QueryPerformanceCounter(&mStart);
			return ((mStart.QuadPart-prev.QuadPart)*1000)/mFreq.QuadPart;
		}

		long Elapsed() const {
			LARGE_INTEGER curr;
			QueryPerformanceCounter(&curr);
			return ((curr.QuadPart-mStart.QuadPart)*1000)/mFreq.QuadPart;
		}

	private:
		LARGE_INTEGER mStart;
		LARGE_INTEGER mFreq;
	};

#elif defined(TARGET_OS_UNIX)

	#include <sys/time.h>

	class XTimeCounter {
	public:
		XTimeCounter() {
			gettimeofday(&mStart, NULL);
		}

		long Reset() {
			timeval prev = mStart;
			gettimeofday(&mStart, NULL);
			return timevaldiff(mStart, prev);
		}

		long Elapsed() const {
			timeval curr;
			gettimeofday(&curr, NULL);
			return timevaldiff(curr, mStart);
		}

	private:
		static long timevaldiff(const timeval& finishtime, const timeval& starttime) {
			long msec = (finishtime.tv_sec-starttime.tv_sec)*1000;
			msec += (finishtime.tv_usec-starttime.tv_usec)/1000;
			return msec;
		}

		timeval mStart;
	};

#else
#error "Platform not supported"
#endif

#endif /* _X_TIME_COUNTER_H_ */
