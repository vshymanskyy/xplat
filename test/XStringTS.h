#include "XString.h"
#include <cxxtest/TestSuite.h>

class StringTS: public CxxTest::TestSuite
{
public:
	void testFormat(void)
	{
		TS_ASSERT(XString::Format("hello %s %d", "world", 1024) == XString("hello world 1024"));
		TS_ASSERT(XString::Format("0x%08x", 0x1a3b5c7) == XString("0x01a3b5c7"));
		TS_ASSERT(XString::Format("0x%08X", 0x1a3b5c7) == XString("0x01A3B5C7"));
		TS_ASSERT(XString::Format("%-10s", "a") == XString("a         "));
		TS_ASSERT(XString::Format("%10s", "a") == XString("         a"));
	}

	void testCompare(void)
	{
		TS_ASSERT(XString("hello") != XString("bye"));
		TS_ASSERT(XString("a") < XString("b"));
		TS_ASSERT(XString("c") > XString("b"));
		TS_ASSERT(XString("hello") == XString("hello"));
		TS_ASSERT(XString("hello") >= XString("hello"));
		TS_ASSERT(XString("hello") <= XString("hello"));

		TS_ASSERT(XString() == XString());
		TS_ASSERT(XString("a") != XString());
		TS_ASSERT(XString() >= XString());
		TS_ASSERT(XString() <= XString());
		TS_ASSERT(XString("a") > XString());
		TS_ASSERT(XString() < XString("a"));
	}

	void testArray(void)
	{
		TS_ASSERT(XString("hello")[0] == 'h');
		TS_ASSERT(XString("hello")[1] == 'e');
		TS_ASSERT(XString("hello")[-1] == 'o');
		TS_ASSERT(XString("hello")[-2] == 'l');

		TS_ASSERT(XString("hello")[-6] == '\0');
		TS_ASSERT(XString("hello")[5] == '\0');
		TS_ASSERT(XString("hello")[6] == '\0');
	}

	void testAppend(void)
	{
		TS_ASSERT(XString("hello ") + XString("world") == XString("hello world"));
		TS_ASSERT(XString() + XString("world") == XString("world"));
		TS_ASSERT(XString() + XString() == XString());
		TS_ASSERT(XString("hello") + XString() == XString("hello"));
	}

	void testStartsEndsWith(void)
	{
		TS_ASSERT(XString("hello").StartsWith("he"));
		TS_ASSERT(XString("hello").StartsWith("hello"));
		TS_ASSERT(!XString("hello").StartsWith("ho"));

		TS_ASSERT(XString("hello").EndsWith("lo"));
		TS_ASSERT(XString("hello").EndsWith("hello"));
		TS_ASSERT(!XString("hello").EndsWith("ko"));

		TS_ASSERT(!XString().EndsWith("lo"));
		TS_ASSERT(!XString().StartsWith("lo"));

		TS_ASSERT(XString("hello").EndsWith(XString()));
		TS_ASSERT(XString("hello").StartsWith(XString()));

	}

	void testLength(void)
	{
		TS_ASSERT(XString("hello").Length() == 5);
		TS_ASSERT(XString("author").Length() == 6);

		TS_ASSERT(XString().Length() == 0);

		if (!XString()) {
			TS_FAIL("Empty string is FALSE");
		}

		if (!XString("")) {
			TS_FAIL("Empty string is FALSE");
		}

		if (!XString(" ")) {
			TS_FAIL("Non-empty string is FALSE");
		}
	}

	void testSubstring(void)
	{
		TS_ASSERT(XString("hello").Substring(1,2) == XString("el"));
		TS_ASSERT(XString("hello").Substring(-3,2) == XString("ll"));

		TS_ASSERT(XString("hello").Substring(1,0) == XString());
		TS_ASSERT(XString().Substring(0,2) == XString());

		TS_ASSERT(XString("hello").Substring(4,5) == XString("o"));
		TS_ASSERT(XString("hello").Substring(-1,-5) == XString("o"));
		TS_ASSERT(XString("hello").Substring(6,1) == XString());
		TS_ASSERT(XString("hello").Substring(-6,1) == XString());
	}

	void testFind(void)
	{
		TS_ASSERT(XString("hello").Find("ab") == -1);
		TS_ASSERT(XString("hello").Find("h") == 0);
		TS_ASSERT(XString("hello").Find("el") == 1);
		TS_ASSERT(XString("hello").Find("l") == 2);
		TS_ASSERT(XString("hello").Find("lo") == 3);

		TS_ASSERT(XString("hello").Find("l", 3) == 3);

		TS_ASSERT(XString().Find(XString()) == 0);
		TS_ASSERT(XString().Find("a") == -1);
		TS_ASSERT(XString("a").Find(XString()) == 0);

	}
};
