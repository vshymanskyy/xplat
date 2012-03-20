#ifndef PERFTIMER_H_
#define PERFTIMER_H_

#if defined(WIN32)

	#include <windows.h>

	class TimeCounter {
	public:
		TimeCounter() {
			QueryPerformanceFrequency(&mFreq);
			QueryPerformanceCounter(&mStart);
		}

		int Reset() {
			LARGE_INTEGER prev = mStart;
			QueryPerformanceCounter(&mStart);
			return ((mStart.QuadPart-prev.QuadPart)*1000)/mFreq.QuadPart;
		}

		int Elapsed() {
			LARGE_INTEGER curr;
			QueryPerformanceCounter(&curr);
			return ((curr.QuadPart-mStart.QuadPart)*1000)/mFreq.QuadPart;
		}

	private:
		LARGE_INTEGER mStart;
		LARGE_INTEGER mFreq;
	};

#elif defined(__GNUC__)

	#include <sys/time.h>

	class TimeCounter {
	public:
		TimeCounter() {
			gettimeofday(&mStart, NULL);
		}

		int Reset() {
			timeval prev = mStart;
			gettimeofday(&mStart, NULL);
			return timevaldiff(mStart, prev);
		}

		int Elapsed() {
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

#endif /* PERFTIMER_H_ */
