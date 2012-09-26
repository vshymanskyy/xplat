#ifndef _X_STRING_H_
#define _X_STRING_H_

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <XPlat.h>
#include <XDebug.h>

/// An ANSI COW string, length is stored internally
class XString
{
private:

	/// String data wrapper (to support reference counting)
	struct RefCountedData {
		/// Length of the string
		unsigned length;
		/// Count of references
		int refcount;
		/// Data buffer
		char buffer[1];
	};

	static RefCountedData EMPTY;

	/// Pointer to actual data
	RefCountedData* mRefData;

	/// Compares data to another String
	/// @param s String to compare
	/// @return Difference
	ssize_t Compare(const XString& s)const;

	/// @param s Length of the buffer
	XString(unsigned len);

public:
	/// Default constructor
	XString();
	/// @param s ANSI string to copy
	XString(const char* s);
	/// @param s String to copy
	XString(const XString& s);
	/// Destructor
	~XString();

	/// Constructs a string according to a specified format,
	/// just like sprintf function.
	/// @param format Format
	static XString Format(const char* format, ...);

	/// @param s String to assign to
	/// @returns Reference to self
	XString& operator  =(const XString& s);

	/// @param s Sting to append
	/// @returns Result ;)
	XString& operator +=(const XString& s);

	/// @returns The length of data
	unsigned Length() const;

	/// @param s The String to check against being prefix
	bool StartsWith(const XString& s) const;

	/// @param s The String to check against being postfix
	bool EndsWith(const XString& s) const;

	/// @param offset The offset of substring
	/// @returns substring
	XString Substring(int offset) const;

	/// @param offset The offset of substring
	/// @param length The length of substring
	/// @returns substring
	XString Substring(int offset, unsigned length) const;

	int	Find(const XString& s, int offset = 0) const;

	/// Sets data to a new value
	void Set(const XString& s);

	/// @returns pointer to the constant data
	operator const char*() const { return mRefData->buffer; }

	operator char*() { return mRefData->buffer; }

	//operator bool() const { return Length() != 0; };

	/// @param index the index of char
	/// @returns reference to a char with the specified index
	const char& operator[](int index) const;

	/// @param s String to compare with
	bool operator==(const XString& s)const;
	/// @param s String to compare with
	bool operator!=(const XString& s)const;
	/// @param s String to compare with
	bool operator <(const XString& s)const;
	/// @param s String to compare with
	bool operator >(const XString& s)const;
	/// @param s String to compare with
	bool operator<=(const XString& s)const;
	/// @param s String to compare with
	bool operator>=(const XString& s)const;
};

inline
unsigned XString::Length() const{
	return mRefData->length;
}

inline
XString operator+(const XString& s1, const XString& s2) {
	return (XString(s1)+=s2);
}

inline
const char&
XString::operator[](int index) const
{
	if (index < 0) index += mRefData->length;
	if (index < 0 || index >= (int)mRefData->length) return mRefData->buffer[mRefData->length];
	return mRefData->buffer[index];
}

inline
bool
XString::operator==(const XString& s) const
{
	return Compare(s) == 0;
}

inline
bool
XString::operator!=(const XString& s) const
{
	return Compare(s) != 0;
}

inline
bool
XString::operator <(const XString& s) const
{
	return Compare(s) < 0;
}

inline
bool
XString::operator >(const XString& s) const
{
	return Compare(s) > 0;
}

inline
bool
XString::operator<=(const XString& s) const
{
	return Compare(s) <= 0;
}

inline
bool
XString::operator>=(const XString& s) const
{
	return Compare(s) >= 0;
}

#endif /* _X_STRING_H_ */

