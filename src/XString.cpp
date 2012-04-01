#include <XString.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

XString
XString::Format(const char* format, ...)
{
	char buff[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(buff, 1024, format, args);
	va_end(args);
	return XString(buff);
}

XString::XString() {
	mRefData = NULL;
}

XString::XString(const char* s)
	: mRefData(NULL)
{
	if (const unsigned len = strlen(s)) {
		mRefData = (RefCountedData*) malloc (len + 2 + sizeof(RefCountedData));
		memcpy (GetBuffer(), s, len+1);
		mRefData->refcount = 1;
		mRefData->length = len;
	}
}

XString::XString(const XString& s)
	: mRefData(s.mRefData)
{
	mRefData->refcount++;
}

XString::~XString() {
	if (mRefData && !(--(mRefData->refcount))) {
		free(mRefData);
	}
}

XString&
XString::operator=(const XString& s)
{
	if (this != &s) {
		if (mRefData && !(--(mRefData->refcount))) {
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
	if (!s.mRefData) {
		return *this;
	}

	if (!mRefData) {
		mRefData = s.mRefData;
		mRefData->refcount++;
		return *this;
	}

	const unsigned len = mRefData->length + s.mRefData->length;
	if (--(mRefData->refcount)) {
		RefCountedData* data = (RefCountedData*)malloc(len + 2 + sizeof(RefCountedData));
		memcpy((char*)(data+1), GetBuffer(), mRefData->length);
		memcpy((char*)(data+1) + mRefData->length, s.GetBuffer(), s.mRefData->length + 1);
		mRefData = data;
	} else {
		mRefData = (RefCountedData*)realloc(mRefData, len + 2 + sizeof(RefCountedData));
		memcpy((char*)(mRefData+1) + mRefData->length, s.GetBuffer(), s.mRefData->length + 1);
	}
	mRefData->length = len;
	mRefData->refcount = 1;
	return *this;
}

bool
XString::operator==(const XString& s) const
{
	if (Length() != s.Length())
		return false;
	return strcmp(GetBuffer(), s.GetBuffer()) == 0;
}

ptrdiff_t
XString::Compare(const XString& s) const
{
	if (Length() == s.Length()) {
		return strcmp(GetBuffer(), s.GetBuffer());
	} else {
		return (Length() < s.Length())?-1:1;
	}
}

bool
XString::StartsWith(const XString& s) const
{
	if (s.Length() > Length()) {
		return false;
	}
	char* s1 = GetBuffer();
	char* s2 = s.GetBuffer();
	while(*s2) {
		if (*s1++ != *s2++)
			return false;
	}
	return true;
}

bool
XString::EndsWith(const XString& s) const
{
	assert(s.Length() > 0);
	if (s.Length() > Length())
		return false;
	char* s1 = GetBuffer() + Length();
	char* s2 = s.GetBuffer() + s.Length();
	for(int i = s.Length()-1; i; i--) {
		if (*s1-- != *s2--)
			return false;
	}
	return true;
}

XString
XString::Substring(int offset, int length)
{
	//TODO implement it
	return "";
}

XString
XString::Substring(int offset)
{
	return Substring(offset, Length() - offset);
}

