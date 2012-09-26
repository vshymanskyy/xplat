#include <XSerialTty.h>

#ifdef TARGET_OS_UNIX
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

static unsigned getBaudrate(unsigned speed)
{
	switch (speed) {
		case 0       : return B0;
		case 50      : return B50;
		case 75      : return B75;
		case 110     : return B110;
		case 134     : return B134;
		case 150     : return B150;
		case 200     : return B200;
		case 300     : return B300;
		case 600     : return B600;
		case 1200    : return B1200;
		case 1800    : return B1800;
		case 2400    : return B2400;
		case 4800    : return B4800;
		case 9600    : return B9600;
		case 19200   : return B19200;
		case 38400   : return B38400;
		case 57600   : return B57600;
		case 115200  : return B115200;
		case 230400  : return B230400;
		case 460800  : return B460800;
		case 500000  : return B500000;
		case 576000  : return B576000;
		case 921600  : return B921600;
		case 1000000 : return B1000000;
		case 1152000 : return B1152000;
		case 1500000 : return B1500000;
		case 2000000 : return B2000000;
		case 2500000 : return B2500000;
		case 3000000 : return B3000000;
		case 3500000 : return B3500000;
		case 4000000 : return B4000000;
		default      : return B0;
	}
}

XSerialTty::XSerialTty()
	: mLog("RS232")
{
	memset(&mStats, 0x00, sizeof(Statistics));
}


bool XSerialTty::Open(const char* fn, unsigned baud)
{
	mDescr = ::open(fn, O_RDWR | O_EXCL);
	if (Opened()) {
		LOG(mLog, "Opened");
		struct termios tio;
		memset(&tio,0,sizeof(tio));
		tio.c_iflag = IGNBRK;
		tio.c_oflag = 0;
		tio.c_cflag = CS8 | CREAD | CLOCAL;			// 8n1, see termios.h for more information
		tio.c_cflag &= ~HUPCL;					// disable hang-up-on-close to avoid reset
		tio.c_lflag = 0;
		tio.c_cc[VMIN] = 0;						// return from read(), when at least 1 char is read
		tio.c_cc[VTIME] = 1;					// read() timeout in tenth of second

		if(unsigned baudRate = getBaudrate(baud)) {
			cfsetospeed(&tio, baudRate);
			cfsetispeed(&tio, baudRate);
		} else {
			LOG_WARN(mLog, "Cannot set baud rate to " << baud);
		}
		tcflush(mDescr, TCIOFLUSH);
		tcsetattr(mDescr, TCSANOW, &tio);
		return true;
	} else {
		LOG_CRIT(mLog, "Device not opened");
		return false;
	}

}

XSerialTty::~XSerialTty()
{
	Close();
}

void XSerialTty::Close()
{
	if(Opened()) {
		::close(mDescr);
		mDescr = -1;
		LOG(mLog, "Closed");
	}
}

bool XSerialTty::Opened()
{
	return mDescr != -1;
}

ssize_t XSerialTty::Write(const void* data, unsigned length)
{
	if(!Opened() || !data || !length) {
		mStats.txErrors++;
		return -1;
	}
	const ssize_t len = ::write(mDescr, data, length);
	if (len > 0) {
		mStats.txBytes += len;
	} else if (len < 0) {
		mStats.txErrors++;
	}
	return len;
}

ssize_t XSerialTty::Read(void* data, unsigned length)
{
	if(!Opened() || !data || !length) {
		mStats.rxErrors++;
		return -1;
	}
	const ssize_t len = ::read(mDescr, data, length);
	if (len > 0) {
		mStats.rxBytes += len;
	} else if (len < 0) {
		mStats.rxErrors++;
	}
	return len;
}

void XSerialTty::DumpStats() const
{
	printf(" tx bytes : %ld\n", mStats.txBytes);
	printf(" rx bytes : %ld\n", mStats.rxBytes);

	printf(" tx errors: %d\n", mStats.txErrors);
	printf(" rx errors: %d\n", mStats.rxErrors);
}

#endif
