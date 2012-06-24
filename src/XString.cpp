#include <XString.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

XString::RefCountedData XString::EMPTY = { 0, 1, {'\0'} };

XString
XString::Format(const char* format, ...)
{
	int qty;
	{
		char c;
		va_list args;
		va_start(args, format);
#if defined (TARGET_OS_WINDOWS)
		qty = _vscprintf(format, args);
#elif defined (TARGET_OS_UNIX)
		qty = vsnprintf(&c, 1, format, args);
#endif
		va_end(args);
	}

	XString buff(qty);
	va_list args;
	va_start(args, format);
	vsnprintf(buff.mRefData->buffer, qty+1, format, args);
	va_end(args);
	return buff;
}

XString::XString() {
	mRefData = &EMPTY;
	mRefData->refcount++;
}

XString::XString(const char* s)
{
	const unsigned len = s?strlen(s):0;
	if (len) {
		mRefData = (RefCountedData*) malloc (len + sizeof(RefCountedData));
		memcpy (mRefData->buffer, s, len+1);
		mRefData->refcount = 1;
		mRefData->length = len;
	} else {
		mRefData = &EMPTY;
		mRefData->refcount++;
	}
}

XString::XString(const XString& s)
	: mRefData(s.mRefData)
{
	X_ASSERT(mRefData);
	mRefData->refcount++;
}

XString::XString(size_t len)
{
	if (len) {
		mRefData = (RefCountedData*) malloc (len + sizeof(RefCountedData));
		mRefData->refcount = 1;
		mRefData->length = len;
	} else {
		mRefData = &EMPTY;
		mRefData->refcount++;
	}
}

XString::~XString() {
	if (!(--(mRefData->refcount))) {
		free(mRefData);
	}
}

XString&
XString::operator=(const XString& s)
{
	X_DBG(X_ASSERT (mRefData));
	X_DBG(X_ASSERT (s.mRefData));
	if (this != &s) {
		if (!(--(mRefData->refcount))) {
			free(mRefData);
		}
		mRefData = s.mRefData;
		if (mRefData) {
			++mRefData->refcount;
		}
	}
	return *this;
}

XString&
XString::operator+=(const XString& s)
{
	X_DBG(X_ASSERT (mRefData));
	X_DBG(X_ASSERT (s.mRefData));

	if (!s.Length()) return *this;

	const unsigned len = mRefData->length + s.mRefData->length;
	if (--(mRefData->refcount)) {
		RefCountedData* data = (RefCountedData*)malloc(len + sizeof(RefCountedData));
		memcpy(data->buffer, mRefData->buffer, mRefData->length);
		memcpy(data->buffer + mRefData->length, s.mRefData->buffer, s.mRefData->length + 1);
		mRefData = data;
	} else {
		mRefData = (RefCountedData*)realloc(mRefData, len + sizeof(RefCountedData));
		memcpy(mRefData->buffer + mRefData->length, s.mRefData->buffer, s.mRefData->length + 1);
	}
	mRefData->length = len;
	mRefData->refcount = 1;
	return *this;
}

ssize_t
XString::Compare(const XString& s) const
{
	X_DBG(X_ASSERT (mRefData));
	X_DBG(X_ASSERT (s.mRefData));

	if (Length() == s.Length()) {
		if (Length() == 0) return 0;
		return strcmp(mRefData->buffer, s.mRefData->buffer);
	} else {
		return (Length() < s.Length())?-1:1;
	}
}

bool
XString::StartsWith(const XString& s) const
{
	X_DBG(X_ASSERT (mRefData));
	X_DBG(X_ASSERT (s.mRefData));

	if (s.Length() == 0) return true;
	if (s.Length() > Length()) {
		return false;
	}
	return memcmp(mRefData->buffer, s.mRefData->buffer, s.Length()) == 0;
}

bool
XString::EndsWith(const XString& s) const
{
	X_DBG(X_ASSERT (mRefData));
	X_DBG(X_ASSERT (s.mRefData));

	if (s.Length() == 0) return true;
	if (s.Length() > Length())
		return false;
	return memcmp(mRefData->buffer+Length()-s.Length(), s.mRefData->buffer, s.Length()) == 0;
}

XString
XString::Substring(int offset, unsigned length) const
{
	X_DBG(X_ASSERT (mRefData));

	if (length == 0) return XString();
	if (Length() == 0) return XString();

	if (offset < 0) offset += Length();
	if (offset < 0 || offset >= Length()) return XString();

	if (offset + length > Length()) length = Length()-offset;
	XString buff(length);
	memcpy(buff.mRefData->buffer, mRefData->buffer+offset, length);
	buff.mRefData->buffer[length] = '\0';
	return buff;
}

XString
XString::Substring(int offset) const
{
	X_DBG(X_ASSERT(mRefData));

	return Substring(offset, Length() - offset);
}

int
XString::Find(const XString& s, int offset) const
{
	X_DBG(X_ASSERT(mRefData));
	X_DBG(X_ASSERT (s.mRefData));

	if (s.Length() == 0) return 0;
	if (Length() == 0) return -1;

	if (offset < 0) offset += Length();
	if (offset < 0 || offset >= Length()) return -1;


	const char* p = strstr(mRefData->buffer+offset, s);
	if (p) {
		return p-mRefData->buffer;
	} else {
		return -1;
	}
}
