#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdio.h>

struct ISerial {

	virtual ~ISerial() {}

	virtual ssize_t Write(const void* data, unsigned length) = 0;
	virtual ssize_t Read(void* data, unsigned length) = 0;

	virtual void FlushRX() {
		char buff[32];
		while(Read(buff, 32) != 0);
	}

	virtual void FlushTX() {
	}

	virtual ssize_t WriteExactly(const void* data, unsigned length) {
		for (unsigned sent = 0; sent < length; ) {
			ssize_t len = Write((char*)data + sent, length - sent);
			if (len == -1) return -1;
			sent += len;
		}
		return length;
	}

	virtual ssize_t ReadExactly(void* data, unsigned length) {
		for (unsigned rcvd = 0; rcvd < length; ) {
			ssize_t len = Read((char*)data + rcvd, length - rcvd);
			if (len == -1) return -1;
			rcvd += len;
		}
		return length;
	}

	virtual bool Opened() = 0;
	virtual void Close() = 0;
};

#endif /* SERIAL_H_ */
