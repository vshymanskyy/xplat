#pragma once

class XStatus {

public:
	enum Code {
		OK = 0,

		INVALID_ARG,
		NOT_IMPLEMENTED,
		NOT_SUPPORTED,

		CANCELED,
		TIMEOUT,

		INSUFF_RAM,
		INSUFF_HDD,
		INSUFF_CPU,
		INSUFF_GPU,
		INSUFF_NETWORK,
		INSUFF_DISPLAY,

		INVALID_HEADER,
		INVALID_DATA,
		CORRUPT_DATA,

		ALREADY_EXISTS,
		ACCESS_DENIED,
		NOT_AVALIABLE,
		NOT_FOUND,

		EMPTY,
		FULL,
		TOO_MANY,
		TOO_FEW,
		TOO_LARGE,
		TOO_SMALL,

		UNKNOWN_ERROR,

		ERROR_CODE_QTY
	};

	XStatus(const Code& code = UNKNOWN_ERROR);

	operator bool() const;

	operator Code() const;

	const char* Message() const;

private:
	static const char* CodeStrings[ERROR_CODE_QTY];

private:
	Code _code;
};

inline
XStatus::XStatus(const Code& code)
	: _code (code)
{
}

inline
XStatus::operator bool() const
{
	return (_code == XStatus::OK);
}

inline
XStatus::operator Code() const
{
	return _code;
}

inline
const char* XStatus::Message() const
{
	if (_code >= 0 && _code < ERROR_CODE_QTY) {
		return CodeStrings[_code];
	} else {
		return CodeStrings[UNKNOWN_ERROR];
	}
}

#define CHECK_IGNORE(op) { Status r = (op); }

#define CHECK_ASSERT(op) { Status r = (op); assert(r == Status::OK); }

#define CHECK_BREAK(op) { if (!(op)) { DebugBreak(); } }

#define CHECK_LOG(op) { if (!(Status s = (op))) { Log(_T("Error %d at %s:%d: '%s'"), __FILE__, __LINE__, s, CodeStrings[s]); } }

#define CHECK_LOG_EXIT(op) { if (!(Status s = (op))) { Log(_T("Error %d at %s:%d: '%s' => exiting"), __FILE__, __LINE__, s, CodeStrings[s]); exit(r); } }
