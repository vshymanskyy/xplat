#ifndef _X_SERIAL_TTY_H_
#define _X_SERIAL_TTY_H_

#include <XLog.h>
#include <XSerial.h>

class XSerialTty
	: public ISerial
{

public:
	XSerialTty();
	virtual ~XSerialTty();

	virtual bool Open(const char* fn, unsigned baud);
	virtual void Close();
	virtual bool Opened();

	virtual ssize_t Write(const void* data, unsigned length);
	virtual ssize_t Read(void* data, unsigned length);

	void DumpStats() const;

private:
	int mDescr;
	XLog mLog;

	struct Statistics {
		unsigned long txBytes;
		unsigned long rxBytes;
		unsigned txErrors;
		unsigned rxErrors;
	} mStats;
};

#endif /* _X_SERIAL_TTY_H_ */
