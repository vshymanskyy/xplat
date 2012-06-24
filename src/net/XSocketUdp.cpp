#include <net/XSocketUdp.h>
#include <string.h>
#include <stdlib.h>

XSocketUdp::XSocketUdp()
	: mSocket(-1)
{

}

XSocketUdp::~XSocketUdp()
{
	if (mSocket >= 0) {
		x_closesocket(mSocket);
	}
}

bool XSocketUdp::Bind(const XSockAddr& addr)
{
	if (mSocket >= 0) {
		x_closesocket(mSocket);
	}
	if ((mSocket = socket(addr.SA()->sa_family, SOCK_DGRAM, 0)) >= 0) {
#ifdef TARGET_OS_WINDOWS
		// Set the exclusive address option
		int iOptval = 1;
		if (-1 == setsockopt(mSocket, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&iOptval, sizeof(iOptval))) {
			printf("setsockopt for SO_EXCLUSIVEADDRUSE failed with error: %ld\n", WSAGetLastError());
		}
#endif
		if (-1 != bind(mSocket, addr.SA(), addr.SA_LEN())) {
			return true;
		} else {
			x_closesocket(mSocket);
			mSocket = -1;
		}
	}
	return false;
}
/*
bool XSocketUdp::Bind(unsigned port)
{
	char strPort[8];

	struct addrinfo hints;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;		// Use INADDR_ANY or in6addr_any

	snprintf(strPort, sizeof(strPort), "%d", port);

	if (mSocket >= 0) {
		x_closesocket(mSocket);
	}

	addrinfo* servinfo = NULL;
	if (0 == getaddrinfo(NULL, strPort, &hints, &servinfo)) {
		// loop through all the results and bind to the first we can
		for (addrinfo* p = servinfo; p != NULL; p = p->ai_next) {

			// Open socket
			if ((mSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) >= 0) {

#ifdef TARGET_OS_WINDOWS
				// Set the exclusive address option
				int iOptval = 1;
				if (-1 == setsockopt(mSocket, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&iOptval, sizeof(iOptval))) {
					printf("setsockopt for SO_EXCLUSIVEADDRUSE failed with error: %ld\n", WSAGetLastError());
				}
#endif
				if (-1 != bind(mSocket, p->ai_addr, p->ai_addrlen)) {
					freeaddrinfo(servinfo);
					return true;
				} else {
					x_closesocket(mSocket);
					mSocket = -1;
				}
			}
		}
		freeaddrinfo(servinfo);
	}
	return false;
}
*/

XSockAddr XSocketUdp::GetBindAddr() const
{
	XSockAddr addr;
	socklen_t l = sizeof(addr);
	if (-1 != mSocket && 0 != getsockname(mSocket, addr.SA(), &l)) {
		X_FATAL("cannot GetBindAddr");
	}
	return addr;
}

/*int KadSocket::GetAvaliableBytes() const
{
	int result = 0;
	ioctl(mSocket, FIONREAD, &result);
	return result;
}*/

ssize_t XSocketUdp::SendTo(const void* data, size_t len, const XSockAddr& addr) const
{
	return sendto(mSocket, (char*)data, len, 0, addr.SA(), addr.SA_LEN());
}

ssize_t XSocketUdp::RecvFrom(void* data, size_t len, XSockAddr* addr) const
{
	socklen_t fromLen = sizeof(XSockAddr);
	return recvfrom(mSocket, (char*)data, len, 0, addr->SA(), &fromLen);
}
