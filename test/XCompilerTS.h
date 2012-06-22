#include <XHelpers.h>
#include <cxxtest/TestSuite.h>
#include <string.h>

class CompilerRvoTS: public CxxTest::TestSuite
{
	struct RvoTest {
		uint8_t buffer[1024];
		RvoTest() { memset(buffer, 0xFA, sizeof(buffer)); }
		RvoTest(const RvoTest&);
		RvoTest& operator = (const RvoTest&);
	};

	RvoTest RvoTestFun2();
	RvoTest RvoTestFun1();
	RvoTest RvoTestFun();

public:
	void testRvo(void)
	{
		RvoTest t1 = RvoTestFun();
		RvoTest t2(RvoTestFun());
		TS_ASSERT(t1.buffer[RandRange(0, 1023)] == 0xFA);
		TS_ASSERT(t2.buffer[RandRange(0, 1023)] == 0xFA);
	}

};
