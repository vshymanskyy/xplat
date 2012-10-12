#include <net/XSockAddr.h>
#include <cxxtest/TestSuite.h>

class XSockAddrTS: public CxxTest::TestSuite
{
public:

	void testGoodAddrString(void)
	{
		XString goodaddr[] = {
				"192.168.0.1",										// private
				"192.168.0.1:675",									// private+port
				"::ffff:192.168.0.1",								// private transitional
				"[::ffff:192.168.0.1]:80",							// private transitional+port
#ifdef TARGET_OS_UNIX
				"fe80::f27b:cbff:fe7f:c20f%eth1",
#elif defined TARGET_OS_WINDOWS
				"fe80::f27b:cbff:fe7f:c20f%22",
#endif
				"127.0.0.1",										// loop back
				"127.0.0.1:80",										// loop back+port
				"::1",												// loop back
				"[::1]:80",											// loop back+port

				"::",												// unspecified
				"[::]:57",											// unspecified+port

				"2001:db8:85a3::8a2e:370:7334",						// doc
				"[2001:db8:85a3:8d3:1319:8a2e:370:7348]:443",		// doc +port

				"ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff",			//Good address
				"fff:ffff:ffff:ffff:ffff:ffff:ffff:ffff",			//Good address
				"fff:ffff:0:ffff:ffff:ffff:ffff:ffff"				//Good address
		};

		for (unsigned i=0; i<X_COUNTOF(goodaddr); i++) {
			XString addr = XSockAddr(goodaddr[i]).ToString();
			if (addr != goodaddr[i]) {
				TS_FAIL((char*)(goodaddr[i] + " - " + addr));
			}
		}
	}

	void testBadAddrString(void)
	{
		XString badaddr[] = {
				 "ffff:ffff:ffff:fffg:ffff:ffff:ffff:ffff",		//Bad address
				 "0000::0000::0000:0000",						//Bad address
				 "256.0.0.0",									//invalid
				 "g::1"											//invalid
				 "A:10"											//invalid
		};

		for (unsigned i=0; i<X_COUNTOF(badaddr); i++) {
			XString addr = XSockAddr(badaddr[i]).ToString();
			if (addr != XString()) {
				TS_FAIL((char*)(badaddr[i] + " - " + addr));
			}
		}
	}


	void testAltAddrString(void)
	{
		XString altaddr[] = {
				//"",											"127.0.0.1",
				//"[]",										"::1",
				//":3000",									"127.0.0.1:3000",
				//"[]:3000",									"[::1]:3000",
				"2001:db8:85a3:0:0:8a2e:370:7334",			"2001:db8:85a3::8a2e:370:7334",	// doc, IPv6 for 555-1234
				"0000:0000:0000:0000:0000:0000:0000:0000",	"::",
				"0000:0000:0000::0000:0000",				"::",
				"0000:0000:0000:0000:0000:0000:0000:1",		"::1",
				"0000:0000:0000::0000:001",					"::1",
				"0x10101010",								"16.16.16.16",
				"0x10101010:10",							"16.16.16.16:10",
				"10101010",									"0.154.33.18",
				"10101010:22",								"0.154.33.18:22"
		};


		for (unsigned i=0; i<X_COUNTOF(altaddr); i+=2) {
			XString addr1 = XSockAddr(altaddr[i]).ToString();
			XString addr2 = XSockAddr(altaddr[i+1]).ToString();
			if (addr1 != addr2) {
				TS_FAIL((char*)(addr1 + " - " + addr2));
			}
		}
	}

	void testLookup(void)
	{
		TS_ASSERT_DELTA(XSockAddr::Lookup("google.com").Count(), 10, 3);
	}

	void testResolve(void)
	{
		XString addr[] = {
				"173.194.39.98",	"bud01s11-in-f2.1e100.net",
				"98.139.183.24",	"ir2.fp.vip.bf1.yahoo.com"
		};


		for (unsigned i=0; i<X_COUNTOF(addr); i+=2) {
			XString addr1 = XSockAddr(addr[i]).ResolveName();
			if (addr1 != addr[i+1]) {
				TS_FAIL((char*)(addr1 + " - " + addr[i+1]));
			}
		}
	}

	void testLocal(void)
	{
		XList<XSockAddr> addr = XSockAddr::GetLocal();
		TS_ASSERT(addr.Count() > 0);
		TS_ASSERT(addr.Count() < 20);
		for (XList<XSockAddr>::It it = addr.First(); it != addr.End(); ++it) {
			TS_TRACE((char*)(addr[it].ToString()));
		}
	}
};
