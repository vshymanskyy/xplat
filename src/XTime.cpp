#include "XTime.h"

#if defined (TARGET_OS_WINDOWS)
	#include <windows.h>
	#include <sys/timeb.h>
	#include <time.h>

	bool XPlatGetTime(XPlatDateTime_t *time, XPlatTimeZone_t *timezone)
	{
		struct _timeb tstruct;
		LARGE_INTEGER tickPerSecond;
		LARGE_INTEGER tick;

		// get the high resolution counter's accuracy
		BOOL hasPC = QueryPerformanceFrequency(&tickPerSecond);

		// what time is it ?
		if (_ftime_s(&tstruct) != 0) {
			return false;
		}
		hasPC &= QueryPerformanceCounter(&tick);

		struct tm ltm;
		if (NULL != time && localtime_s(&ltm, &tstruct.time) == 0) {
			if (hasPC) {
				time->usecond = (tick.QuadPart % tickPerSecond.QuadPart) % 1000;
				time->msecond = (tick.QuadPart % tickPerSecond.QuadPart) / 1000;
			} else {
				time->usecond = 0;
				time->msecond = tstruct.millitm;
			}
			time->second = ltm.tm_sec;
			time->minute = ltm.tm_min;
			time->hour = ltm.tm_hour;
			time->mday = ltm.tm_mday;
			time->month = ltm.tm_mon;
			time->year = ltm.tm_year;
			time->wday = ltm.tm_wday;
			time->yday = ltm.tm_yday;
		}

		if (NULL != timezone) {
			timezone->minuteswest = tstruct.timezone / 60;
			timezone->dsttime = tstruct.dstflag;
			timezone->dstnowset = 0;
			timezone->dstnow = 0;
		}
		return true;
	}

#elif defined (TARGET_OS_UNIX)
	#include <time.h>
	#include <sys/time.h>
	#include <sys/types.h>

	bool XPlatGetTime(XPlatDateTime_t *time, XPlatTimeZone_t *timezone)
	{
		struct timeval tv;
		struct timezone tz;
		struct tm lt;

		if (gettimeofday(&tv, &tz) != 0) {
			return false;
		}

		struct tm* ptm = localtime_r(&tv.tv_sec, &lt);
		if (NULL != ptm && NULL != time) {
			time->usecond = tv.tv_usec % 1000;
			time->msecond = tv.tv_usec / 1000;
			time->second = ptm->tm_sec;
			time->minute = ptm->tm_min;
			time->hour = ptm->tm_hour;
			time->mday = ptm->tm_mday;
			time->month = ptm->tm_mon;
			time->year = ptm->tm_year;
			time->wday = ptm->tm_wday;
			time->yday = ptm->tm_yday;
		}

		if (NULL != timezone) {
			timezone->minuteswest = tz.tz_minuteswest;
			timezone->dsttime = tz.tz_dsttime != 0;
			if (NULL != ptm) {
				timezone->dstnowset = ptm->tm_isdst >= 0;
				timezone->dstnow = ptm->tm_isdst > 0;
			}
		}
		return true;
	}

#endif
