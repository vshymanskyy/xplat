#include <XHelpers.h>
#include <cxxtest/TestSuite.h>
#include <string.h>

class CompilerRvoTS: public CxxTest::TestSuite
{
	struct RvoTest {
		uint8_t buffer[1024];
		RvoTest() { memset(buffer, 0xFA, sizeof(buffer)); }
		RvoTest(const RvoTest& t)
		{ TS_TRACE("RVO not optimal: Copy detected"); memcpy(buffer,t.buffer, sizeof(t.buffer)); }

		RvoTest& operator = (const RvoTest& t)
		{ TS_TRACE("RVO not optimal: Assignment detected"); memcpy(buffer,t.buffer, sizeof(t.buffer)); return *this; }
	};

	RvoTest RvoTestFun2() {
		RvoTest t;
		return t;
	}

	RvoTest RvoTestFun1() {
		return RvoTestFun2();
	}

	RvoTest RvoTestFun() {
		return RvoTestFun1();
	}




public:
	void testRvo(void)
	{
		RvoTest t1 = RvoTestFun();
		RvoTest t2(RvoTestFun());
		TS_ASSERT(t1.buffer[RandRange(0, 1023)] == 0xFA);
		TS_ASSERT(t2.buffer[RandRange(0, 1023)] == 0xFA);
	}

};
