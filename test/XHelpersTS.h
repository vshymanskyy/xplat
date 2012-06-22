#include "XHelpers.h"
#include <cxxtest/TestSuite.h>

class HelpersTS: public CxxTest::TestSuite
{
public:
	void testMin(void)
	{
		TS_ASSERT_EQUALS(Min(0,0), 0);
		TS_ASSERT_EQUALS(Min(1,2), 1);
		TS_ASSERT_EQUALS(Min(0,-3), -3);

		TS_ASSERT_EQUALS(Min(0,0,0), 0);
		TS_ASSERT_EQUALS(Min(4,2,3), 2);
		TS_ASSERT_EQUALS(Min(0,-3,5), -3);

		TS_ASSERT_EQUALS(Min(0,0,0,0), 0);
		TS_ASSERT_EQUALS(Min(3,2,5,4), 2);
		TS_ASSERT_EQUALS(Min(0,-3,5,-3), -3);
	}

	void testMax(void)
	{
		TS_ASSERT_EQUALS(Max(0,0), 0);
		TS_ASSERT_EQUALS(Max(1,2), 2);
		TS_ASSERT_EQUALS(Max(0,-3), 0);

		TS_ASSERT_EQUALS(Max(0,0,0), 0);
		TS_ASSERT_EQUALS(Max(4,2,3), 4);
		TS_ASSERT_EQUALS(Max(0,-3,5), 5);

		TS_ASSERT_EQUALS(Max(0,0,0,0), 0);
		TS_ASSERT_EQUALS(Max(3,2,5,4), 5);
		TS_ASSERT_EQUALS(Max(0,-3,1,-3), 1);
	}

	void testSwap()
	{
		int q = 6;
		int w = 2;
		TS_ASSERT((Swap(q, w), q==2 && w==6));
	}

	void testInRange()
	{
		TS_ASSERT(InRange(5, 5, 5));
		TS_ASSERT(InRange(763, 760, 770));
		TS_ASSERT(InRange(763, 770, 760));

		TS_ASSERT(!InRange(21, 22, 23));
		TS_ASSERT(!InRange(24, 22, 23));
	}

	void testOutOfRange()
	{
		TS_ASSERT(!OutOfRange(5, 5, 5));
		TS_ASSERT(!OutOfRange(763, 760, 770));
		TS_ASSERT(!OutOfRange(763, 770, 760));

		TS_ASSERT(OutOfRange(21, 22, 23));
		TS_ASSERT(OutOfRange(24, 22, 23));
	}

	void testAbsDiff()
	{
		TS_ASSERT_EQUALS(AbsDiff(2, 3), 1);
		TS_ASSERT_EQUALS(AbsDiff(-5, 3), 8);
		TS_ASSERT_EQUALS(AbsDiff(-4, 50), 54);
		TS_ASSERT_EQUALS(AbsDiff(50, -4), 54);
	}
};
