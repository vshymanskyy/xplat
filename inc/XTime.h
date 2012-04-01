#pragma once

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include "XPlat.h"

typedef struct XPlatDateTime
{
	uint32_t
		usecond	:10,	/* 10, microseconds after the millisecond [0-1000] */
		msecond	:10,	/* 20, milliseconds after the second [0-1000] */
		second	:6,		/* 26, seconds after the minute [0-59] */
		minute	:6;		/* 32, minute after the hour [0-59] */
	uint32_t
		hour	:5,		/* 37, hours since midnight [0-23] */
		mday	:5,		/* 42, day of the month [1-31] */
		month	:4,		/* 46, months since January [1-11] */
		year	:10,	/* 56, years since 1900, till 2923 [0-1023] */

		wday	:3,		/* 59, days since Sunday [0-6] */
		yday	:5;		/* 64, days since January 1 [0-365] */
} XPlatDateTime_t;

typedef struct XPlatTimeZone
{
	uint16_t
		minuteswest	:11,	/* Minutes west of GMT. */
		dsttime		:1,		/* Nonzero if DST is ever in effect. */
		dstnowset	:1,		/* Identifies dstnow is valid */
		dstnow		:1;		/* Nonzero if DST is in effect now */
} XPlatTimeZone_t;

#if defined (TARGET_OS_WINDOWS)
	#include "windows.h"
	#include <sys/timeb.h>
	#include <time.h>

	inline
	bool XPlatGetTime(XPlatDateTime_t *time, XPlatTimeZone_t *timezone) {
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

#elif defined (__GNUC__)
	#include <time.h>
	#include <sys/time.h>
	#include <sys/types.h>

	inline
	bool XPlatGetTime(XPlatDateTime_t *time, XPlatTimeZone_t *timezone) {
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

