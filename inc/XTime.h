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

bool XPlatGetTime(XPlatDateTime_t *time, XPlatTimeZone_t *timezone);
