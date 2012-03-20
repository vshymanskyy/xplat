#include "XThread.h"
#include "XTimeCounter.h"
#include <cxxtest/TestSuite.h>

class ThreadTS: public CxxTest::TestSuite
{
	class ThreadForTesting1: public XThread
	{
		int Run()
		{
			while (val < 100) {
				XThread::SleepMs(1);
				val++;
			}
			return 0;
		}

		void Term() {}

	public:
		ThreadForTesting1()
				: val(0)
		{
		}

		virtual ~ThreadForTesting1() {}

		int val;
	};
public:
	void testThread(void)
	{
		ThreadForTesting1 test;
		test.Start();
		test.Wait();
		TS_ASSERT(test.val == 100);
	}
};
