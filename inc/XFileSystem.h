#ifndef XFILESYSTEM_H_
#define XFILESYSTEM_H_

#include <XPlat.h>
#include <XDebug.h>

#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

inline
bool FileExists(const char* fn) {
	struct stat sts;
	if (stat(fn, &sts) == -1)
	{
		if (errno == ENOENT) {
			return false;
		}
		X_FATAL("stat failed: %d (%s)", errno, strerror(errno));
	}
	return true;
}

inline
bool FileDelete(const char* fn) {
	if (unlink(fn) == -1)
	{
		if (errno != ENOENT) {
			return false;
		}
	}
	return true;
}

inline
uint64_t FileSize(const char* fn)
{
	if (FILE* f = fopen(fn, "rb")) {
		fseek(f, 0, SEEK_END);
		uint64_t result = ftello64(f);
		fclose(f);
		return result;
	}
	return 0;
}

#endif /* XFILESYSTEM_H_ */
