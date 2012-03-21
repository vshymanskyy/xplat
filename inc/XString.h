#ifndef INC_STRING_HPP
#define INC_STRING_HPP

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <stddef.h>

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
	};

	/// Pointer to actual data
	RefCountedData* mRefData;

	char* GetBuffer() const {
		return mRefData ? (char*)(mRefData+1) : NULL;
	}

	/// Compares data to another String
	/// @param s String to compare
	/// @return Difference
	ptrdiff_t Compare(const XString& s)const;

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
	bool StartsWith(const XString& s)const;

	/// @param s The String to check against being postfix
	bool EndsWith(const XString& s)const;

	/// @param offset The offset of substring
	/// @returns substring
	XString Substring(int offset);

	/// @param offset The offset of substring
	/// @param length The length of substring
	/// @returns substring
	XString Substring(int offset, int length);

	/// Sets data to a new value
	void Set(const XString& s);

	/// @returns pointer to the constant data
	operator const char*() const;

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
	return mRefData ? mRefData->length : 0;
}

inline
XString operator+(const XString& s1, const XString& s2) {
	return (XString(s1)+=s2);
}

inline
XString::operator const char*() const
{
	return GetBuffer();
}

inline
const char&
XString::operator[](int index) const
{
	return (GetBuffer())[index];
}


inline
bool
XString::operator!=(const XString& s) const
{
	return Compare(s)!=0;
}

inline
bool
XString::operator <(const XString& s) const
{
	return Compare(s) <0;
}

inline
bool
XString::operator >(const XString& s) const
{
	return Compare(s) >0;
}

inline
bool
XString::operator<=(const XString& s) const
{
	return Compare(s)<=0;
}

inline
bool
XString::operator>=(const XString& s) const
{
	return Compare(s)>=0;
}

#endif // INC_STRING_HPP

