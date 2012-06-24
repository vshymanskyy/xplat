#include "XThread.h"
#include "XTimeCounter.h"
#include <cxxtest/TestSuite.h>

class ThreadTS: public CxxTest::TestSuite
{
	class ThreadForTesting1: public XThread
	{
		int Run()
		{
			while (val < 10) {
				XThread::SleepMs(1);
				val++;
			}
			return 0;
		}

		void Term() {}

	public:
		ThreadForTesting1()
				: XThread ("ThreadForTesting1")
				, val(0)
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
		TS_ASSERT(test.val == 10);
	}

	void ThreadSuspend(void)
	{
		ThreadForTesting1 test;
		test.Start();
		XThread::SleepMs(5);
		test.Suspend();
		XThread::SleepMs(5);
		TS_ASSERT(test.val == 5);
		test.Resume();
		test.Wait();
		TS_ASSERT(test.val == 10);
	}

	void testThreadId(void)
	{
		TS_ASSERT(XThread::GetCurrentId() != 0);

		ThreadForTesting1 test;
		test.Start();
		XThread::SleepMs(5);
		TS_ASSERT(test.GetId() != 0);
	}
};
