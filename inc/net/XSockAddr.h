#ifndef _X_SOCK_ADDR_H_
#define _X_SOCK_ADDR_H_

#include <XPlat.h>

#if defined TARGET_OS_UNIX

#	include <unistd.h>
#	include <sys/socket.h>
#	include <sys/un.h>
#	include <netinet/in.h>
#	include <netdb.h>
#	include <ifaddrs.h>
#	include <sys/ioctl.h>

#	define x_socket_t int
#	define x_closesocket close

#elif defined TARGET_OS_WINDOWS

#include <Windows.h>
#include <Iphlpapi.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

#	define x_socket_t SOCKET
#	define x_closesocket closesocket


#endif

#include <XString.h>
#include <XLog.h>
#include <XList.h>

/*enum XSockFamily
{
	XSOCK_AF_UNSPEC,
	XSOCK_AF_INET,
	XSOCK_AF_INET6
};*/

union XSockAddr
{
private:
	sockaddr sa;
	sockaddr_in sa_in;
	sockaddr_in6 sa_in6;
	//struct sockaddr_storage sa_stor;

public:
	static XSockAddr Random();
	static XList<XSockAddr> Lookup(const XString& str);
	static XList<XSockAddr> GetLocal();

	XSockAddr() { memset (this, 0, sizeof(XSockAddr)); }
	XSockAddr(const XString& str);

	XString ToString() const;
	XString Resolve() const;

	sockaddr* SA() { return &sa; };
	sockaddr_in* SA4() { return &sa_in; };
	sockaddr_in6* SA6() { return &sa_in6; };
	const sockaddr* SA() const { return &sa; }
	const sockaddr_in* SA4() const { return &sa_in; };
	const sockaddr_in6* SA6() const { return &sa_in6; };

	bool IsLocal() const;
	bool IsLoopback() const;
	bool IsMulticast() const;
	bool IsAny() const;

	socklen_t SA_LEN() const {
		switch (sa.sa_family) {
		case AF_INET:	return sizeof(sa_in);
		case AF_INET6:	return sizeof(sa_in6);
		default:		return 0;
		}
	}

	void Port (in_port_t p) {
		switch (sa.sa_family) {
		case AF_INET:	sa_in.sin_port = htons(p);		break;
		case AF_INET6:	sa_in6.sin6_port = htons(p);	break;
		}
	}

	unsigned Port () {
		switch (sa.sa_family) {
		case AF_INET:	return ntohs(sa_in.sin_port);		break;
		case AF_INET6:	return ntohs(sa_in6.sin6_port);	break;
		default:		return 0;
		}
	}

	bool operator==(const XSockAddr& s) const {
		switch (sa.sa_family) {
		case AF_INET:	return sa_in.sin_port == sa_in.sin_port && !memcmp(&sa_in.sin_addr, &s.sa_in.sin_addr, sizeof(sa_in.sin_addr));	break;
		case AF_INET6:	return sa_in6.sin6_port == sa_in6.sin6_port && !memcmp(&sa_in6.sin6_addr, &s.sa_in6.sin6_addr, sizeof(sa_in6.sin6_addr));	break;
		default:		return false;
		}
	}

	bool operator!=(const XSockAddr& s) const {
		switch (sa.sa_family) {
		case AF_INET:	return sa_in.sin_port != sa_in.sin_port || memcmp(&sa_in.sin_addr, &s.sa_in.sin_addr, sizeof(sa_in.sin_addr));	break;
		case AF_INET6:	return sa_in6.sin6_port != sa_in6.sin6_port || memcmp(&sa_in6.sin6_addr, &s.sa_in6.sin6_addr, sizeof(sa_in6.sin6_addr));	break;
		default:		return false;
		}
	}


};

#endif /* _X_SOCK_ADDR_H_ */
