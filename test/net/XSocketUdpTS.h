#include <net/XSocketUdp.h>
#include <cxxtest/TestSuite.h>

class XSocketUdpTS: public CxxTest::TestSuite
{
public:

	void testBindLocal(void)
	{
		XSocketUdp sock1024;
		// Bind
		TS_ASSERT(sock1024.Bind(XSockAddr("0:1024")));
		// Rebind same
		TS_ASSERT(sock1024.Bind(XSockAddr("0:1024")));
		// Try bind another
		TS_ASSERT(!XSocketUdp().Bind(XSockAddr("0:1024")));

		int i = 0;
		XSocketUdp socks[30];
		XList<XSockAddr> addr = XSockAddr::GetLocal();
		for (XList<XSockAddr>::It it = addr.First(); it != addr.End(); ++it) {
			addr[it].Port(3000);
			if (!socks[i++].Bind(addr[it])) {
				TS_FAIL((char*)(addr[it].ToString() + " was not bound"));
			}
		}
	}

	void testBindLocal2(void)
	{
		testBindLocal();
	}

	void testLoopback4(void)
	{
		XSockAddr a1("127.0.0.1:2048");
		XSockAddr a2("127.0.0.1:2049");
		XSocketUdp sock1(a1);
		XSocketUdp sock2(a2);
		{
			sock1.SendTo("hello", 6, a2);

			char buff[10];
			XSockAddr from;
			ssize_t len = sock2.RecvFrom(buff, 10, &from);

			TS_ASSERT_EQUALS(len, 6);
			TS_ASSERT_SAME_DATA("hello", buff, 6);
			TS_ASSERT_EQUALS(a1, from);
		}
		{
			sock2.SendTo("bye", 4, a1);

			char buff[10];
			XSockAddr from;
			ssize_t len = sock1.RecvFrom(buff, 10, &from);

			TS_ASSERT_EQUALS(len, 4);
			TS_ASSERT_SAME_DATA("bye", buff, 4);
			TS_ASSERT_EQUALS(a2, from);
		}
	}

	void testLoopback6(void)
	{
		XSockAddr a1("[::1]:2048");
		XSockAddr a2("[::1]:2049");
		XSocketUdp sock1(a1);
		XSocketUdp sock2(a2);

		{
			sock1.SendTo("hello", 6, a2);

			char buff[10];
			XSockAddr from;
			ssize_t len = sock2.RecvFrom(buff, 10, &from);

			TS_ASSERT_EQUALS(len, 6);
			TS_ASSERT_SAME_DATA("hello", buff, 6);
			TS_ASSERT_EQUALS(a1, from);
		}
		{
			sock2.SendTo("bye", 4, a1);

			char buff[10];
			XSockAddr from;
			ssize_t len = sock1.RecvFrom(buff, 10, &from);

			TS_ASSERT_EQUALS(len, 4);
			TS_ASSERT_SAME_DATA("bye", buff, 4);
			TS_ASSERT_EQUALS(a2, from);
		}
	}
};
