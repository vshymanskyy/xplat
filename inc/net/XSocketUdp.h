#ifndef _X_SOCKET_H_
#define _X_SOCKET_H_

#include <XPlat.h>
#include <net/XSockAddr.h>

class XSocketUdp
{

public:
	XSocketUdp();
	XSocketUdp(const XSockAddr& addr) : mSocket(-1) { Bind(addr); }
	XSocketUdp(unsigned port) : mSocket(-1) { Bind(port); }

	virtual ~XSocketUdp();

	bool Bind(const XSockAddr& addr);

	bool Bind(unsigned port);

	ssize_t SendTo(const void* data, size_t len, const XSockAddr& addr);

	ssize_t RecvFrom(void* data, size_t len, XSockAddr* addr);

	//int GetAvaliableBytes() const;

	XSockAddr GetBindAddr() const;


private:
	int mSocket;
};

#endif /* _X_SOCKET_H_ */
