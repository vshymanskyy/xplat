#include <net/XSockAddr.h>
#include <XHelpers.h>

XSockAddr XSockAddr::Random() {
	XSockAddr addr;
	MemRand(&addr, sizeof(XSockAddr));
	addr.sa.sa_family = AF_INET;
	return addr;
}

XSockAddr::XSockAddr(const XString& str)
{
    addrinfo hint, *info = NULL;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_flags = AI_NUMERICHOST;	// disable DNS lookup

    // Detect IPv6 with port
    if (str[0] == '[') {
    	int e = str.Find("]:", 1);
    	if (e > 0) {
			if (0 == getaddrinfo(str.Substring(1, e-1), str.Substring(e+2), &hint, &info)) {
				memcpy(&sa, info->ai_addr, info->ai_addrlen);
				freeaddrinfo(info);
				return;
			}
    	}
    }

    // Detect addr without port
	if (0 == getaddrinfo(str, NULL, &hint, &info)) {
		memcpy(&sa, info->ai_addr, info->ai_addrlen);
		freeaddrinfo(info);
		return;
	}

	// Detect IPv4 addr with port
	int e = str.Find(":");
	if (0 == getaddrinfo(str.Substring(0, e), str.Substring(e+1), &hint, &info)) {
		memcpy(&sa, info->ai_addr, info->ai_addrlen);
		freeaddrinfo(info);
		return;
	}

    memset(&sa, 0, sizeof(XSockAddr));
}

XString XSockAddr::ToString() const
{
	char addr[NI_MAXHOST];
	char serv[NI_MAXSERV];
	if (0 == getnameinfo(&sa, sizeof(XSockAddr), addr, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV)) {
		if (0 == strcmp(serv, "0")) {
			return addr;
		} else {
			switch (sa.sa_family) {
			case AF_INET:	return XString::Format("%s:%s", addr, serv);
			case AF_INET6:	return XString::Format("[%s]:%s", addr, serv);
			}
		}
	}
	return XString();
}

XString XSockAddr::ResolveName() const
{
	char addr[NI_MAXHOST];
	if (0 == getnameinfo(&sa, sizeof(XSockAddr), addr, NI_MAXHOST, NULL, 0, 0)) {
		return addr;
	}
	return XString();
}

XList<XSockAddr> XSockAddr::Lookup(const XString& str)
{
	XList<XSockAddr> res;
    addrinfo *info = NULL;

    // Detect IPv6 with port
    if (str[0] == '[') {
    	int e = str.Find("]:", 1);
    	if (e > 0) {
    		XString name = str.Substring(0, e-1);
    		XString serv = str.Substring(e+2);
			if (0 == getaddrinfo(name, NULL, NULL, &info)) {
				for(addrinfo* i = info; i != NULL; i = i->ai_next) {
					XSockAddr addr;
					memcpy(&addr.sa, i->ai_addr, i->ai_addrlen);
					addr.Port(atoi(serv));
					res.Append(addr);
				}
				freeaddrinfo(info);
				return res;
			}
    	}
    }

    // Detect addr without port
	if (0 == getaddrinfo(str, NULL, NULL, &info)) {
		for(addrinfo* i = info; i != NULL; i = i->ai_next) {
			XSockAddr addr;
			memcpy(&addr.sa, i->ai_addr, i->ai_addrlen);
			res.Append(addr);
		}
		freeaddrinfo(info);
		return res;
	}

	{ // Detect IPv4 addr with port
		int e = str.Find(":");
		XString name = str.Substring(0, e);
		XString serv = str.Substring(e+1);
		if (!name.Length()) {
			return res;
		}
		fprintf(stderr, "Resolving: %s\n", (char*)name);
		if (int error = getaddrinfo((char*)name, NULL, NULL, &info)) {
			X_FATAL("%d: %s", error, gai_strerror(error));
		} else {
			for(addrinfo* i = info; i != NULL; i = i->ai_next) {
				XSockAddr addr;
				memcpy(&addr.sa, i->ai_addr, i->ai_addrlen);
				addr.Port(atoi(serv));
				res.Append(addr);
			}
			freeaddrinfo(info);
			return res;
		}
	}
    return res;
}

XList<XSockAddr> XSockAddr::GetLocal()
{
	XList<XSockAddr> res;
#if defined TARGET_OS_UNIX
	ifaddrs* iflist = NULL;
	if (0 == getifaddrs(&iflist)) {
		for (ifaddrs* ifa = iflist; ifa != NULL; ifa = ifa->ifa_next) {
			if (ifa->ifa_addr == NULL)
				continue;

			XSockAddr addr;
			switch (ifa->ifa_addr->sa_family) {
			case AF_INET:	memcpy(&addr.sa, ifa->ifa_addr, sizeof(sockaddr_in)); break;
			case AF_INET6:	memcpy(&addr.sa, ifa->ifa_addr, sizeof(sockaddr_in6)); break;
			//case AF_PACKET:	memcpy(&addr.sa, ifa->ifa_addr, sizeof(sockaddr_ll)); break;
			default:
				continue;
			}
			res.Append(addr);
		}

		freeifaddrs(iflist);
	} else {
		perror("getifaddrs:");
	}
#elif defined TARGET_OS_WINDOWS
	IP_ADAPTER_INFO  *pAdapterInfo;
	ULONG            ulOutBufLen;
	DWORD            dwRetVal;
	pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
	ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS) {
		free (pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc ( ulOutBufLen );
	}
	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) != ERROR_SUCCESS) {
		printf("GetAdaptersInfo call failed with %d\n", dwRetVal);
	}
	res.Append(XSockAddr("127.0.0.1"));
	res.Append(XSockAddr("::1"));
	PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
	while (pAdapter) {
		PIP_ADDR_STRING pAddr = &(pAdapter->IpAddressList);
		while (pAddr) {
			//printf("Addr: %s\n", pAddr->IpAddress.String);
			XSockAddr addr(pAddr->IpAddress.String);
			if (addr != XSockAddr("::") && addr != XSockAddr("0")) {
				res.Append(addr);
			}
			pAddr = pAddr->Next;
		}

		pAdapter = pAdapter->Next;
	}

	if (pAdapterInfo)
		free(pAdapterInfo);

#endif
	return res;
}

#if defined TARGET_OS_WINDOWS
static struct WSA_Initializer {
	WSA_Initializer() {
		WSADATA wsaData;
		int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (err != 0) {
			printf("WSAStartup failed with error: %d\n", err);
		} else if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
			printf("Could not find a usable version of Winsock.dll\n");
			WSACleanup();
		}
	}

	~WSA_Initializer() {
		WSACleanup();
		X_DBG(getchar());
	}

} GL_WSA_INIT;
#endif
