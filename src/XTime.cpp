#include "XTime.h"

#if defined (TARGET_OS_WINDOWS)
	#include <windows.h>
	#include <sys/timeb.h>
	#include <time.h>

	bool XPlatGetTime(XPlatDateTime_t *time, XPlatTimeZone_t *tzone)
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
				time->usecond = ((tick.QuadPart % tickPerSecond.QuadPart) % 1000) & 0x3FF;
				time->msecond = ((tick.QuadPart % tickPerSecond.QuadPart) / 1000) & 0x3FF;
			} else {
				time->usecond = 0;
				time->msecond = tstruct.millitm & 0x3FF;
			}
			time->second = ltm.tm_sec & 0x3F;
			time->minute = ltm.tm_min & 0x3F;
			time->hour = ltm.tm_hour & 0x1F;
			time->mday = ltm.tm_mday & 0x1F;
			time->month = ltm.tm_mon & 0xF;
			time->year = ltm.tm_year & 0x3FF;
			time->wday = ltm.tm_wday & 0x7;
			time->yday = ltm.tm_yday & 0x1F;
		}

		if (NULL != tzone) {
			tzone->minuteswest = (tstruct.timezone / 60) & 0x7FF;
			tzone->dsttime = tstruct.dstflag;
			tzone->dstnowset = 0;
			tzone->dstnow = 0;
		}
		return true;
	}

#elif defined (TARGET_OS_UNIX)
	#include <time.h>
	#include <sys/time.h>
	#include <sys/types.h>

	bool XPlatGetTime(XPlatDateTime_t *time, XPlatTimeZone_t *tzone)
	{
		struct timeval tv;
		struct timezone tz;
		struct tm lt;

		if (gettimeofday(&tv, &tz) != 0) {
			return false;
		}

		struct tm* ptm = localtime_r(&tv.tv_sec, &lt);
		if (NULL != ptm && NULL != time) {
			time->usecond = (tv.tv_usec % 1000) & 0x3FF;
			time->msecond = (tv.tv_usec / 1000) & 0x3FF;
			time->second = ptm->tm_sec & 0x3F;
			time->minute = ptm->tm_min & 0x3F;
			time->hour = ptm->tm_hour & 0x1F;
			time->mday = ptm->tm_mday & 0x1F;
			time->month = ptm->tm_mon & 0xF;
			time->year = ptm->tm_year & 0x3FF;
			time->wday = ptm->tm_wday & 0x7;
			time->yday = ptm->tm_yday & 0x1F;
		}

		if (NULL != tzone) {
			tzone->minuteswest = tz.tz_minuteswest & 0x7FF;
			tzone->dsttime = tz.tz_dsttime != 0;
			if (NULL != ptm) {
				tzone->dstnowset = ptm->tm_isdst >= 0;
				tzone->dstnow = ptm->tm_isdst > 0;
			}
		}
		return true;
	}

#endif
