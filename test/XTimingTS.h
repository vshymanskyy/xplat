#include "XTimer.h"
#include "XTimeCounter.h"
#include <cxxtest/TestSuite.h>

class TimingTS: public CxxTest::TestSuite
{

public:
	static const int TOLLERANCE = 10;

	struct Test1
	{
		static const int FIRES_MAX = 32;

		Test1()
			: mFiredQty	(0)
			, mDriftMs	(0)
		{}

		Test1(int drift)
			: mFiredQty	(0)
			, mDriftMs	(drift)
		{}

		void Callback() {
			TS_ASSERT(mFiredQty < FIRES_MAX);
			mFired[mFiredQty++] = mCounter.Reset();
			if (mDriftMs) {
				XThread::SleepMs(mDriftMs); // drift
			}
		}

		XTimeCounter mCounter;
		int mFired[FIRES_MAX];
		int mFiredQty;
		int mDriftMs;
	};

	struct TestHandlerCancel
	{
		TestHandlerCancel()
			: mFiredQty (0)
		{
			tc.SetTimer(XTimerContext::Handler(this, &TestHandlerCancel::Callback), 10, 10);
		}

		void Callback() {
			mFiredQty++;
			if (mFiredQty == 3) {
				tc.CancelTimer(XTimerContext::Handler(this, &TestHandlerCancel::Callback));
			}
		}

		XTimerContext tc;
		int mFiredQty;
	};

	void testSleep(void)
	{
		XTimeCounter tc;
		XThread::SleepMs(27);
		TS_ASSERT_DELTA(tc.Reset(), 27, TOLLERANCE);
		XThread::SleepMs(58);
		TS_ASSERT_DELTA(tc.Reset(), 58, TOLLERANCE);
		XThread::SleepMs(77);
		TS_ASSERT_DELTA(tc.Reset(), 77, TOLLERANCE);
		XThread::SleepMs(122);
		TS_ASSERT_DELTA(tc.Reset(), 122, TOLLERANCE);
	}

	void testTimer(void)
	{
		XTimerContext tc;
		XThread::SleepMs(50);

		Test1 t;
		tc.SetTimer(XTimerContext::Handler(&t, &Test1::Callback), 40);

		XThread::SleepMs(60);

		TS_ASSERT_EQUALS(t.mFiredQty, 1);
		TS_ASSERT_DELTA(t.mFired[0], 40, TOLLERANCE);
	}

	void testTimer0(void)
	{
		XTimerContext tc;
		XThread::SleepMs(50);

		Test1 t;
		tc.SetTimer(XTimerContext::Handler(&t, &Test1::Callback), 0);

		XThread::SleepMs(10);

		TS_ASSERT_EQUALS(t.mFiredQty, 1);
		TS_ASSERT_DELTA(t.mFired[0], 0, TOLLERANCE);
	}

	void testTimerCancel(void)
	{
		Test1 t1;
		XTimerContext tc;

		tc.SetTimer(XTimerContext::Handler(&t1, &Test1::Callback), 120);
		XThread::SleepMs(120-TOLLERANCE);
		tc.CancelTimer(XTimerContext::Handler(&t1, &Test1::Callback));
		XThread::SleepMs(TOLLERANCE*2);
		TS_ASSERT_EQUALS(t1.mFiredQty, 0);

		tc.SetTimer(XTimerContext::Handler(&t1, &Test1::Callback), 50, 50);
		XThread::SleepMs(100+TOLLERANCE);
		tc.CancelTimer(XTimerContext::Handler(&t1, &Test1::Callback));
		XThread::SleepMs(70);
		TS_ASSERT_EQUALS(t1.mFiredQty, 2);
	}

	void testTimerCancel2(void)
	{
		Test1 t1;
		Test1 t2;
		XTimerContext tc;
		tc.SetTimer(XTimerContext::Handler(&t1, &Test1::Callback), 80);
		tc.SetTimer(XTimerContext::Handler(&t2, &Test1::Callback), 50, 50);

		XThread::SleepMs(60);
		tc.CancelTimer(XTimerContext::Handler(&t2, &Test1::Callback));
		XThread::SleepMs(70);

		TS_ASSERT_EQUALS(t1.mFiredQty, 1);
		TS_ASSERT_EQUALS(t2.mFiredQty, 1);
	}

	void testTimerCancelInHandler(void)
	{
		TestHandlerCancel t;

		XThread::SleepMs(100);

		TS_ASSERT_EQUALS(t.mFiredQty, 3);
	}

	void testTimerMultiple(void)
	{
		Test1 t1, t2, t3, t4;
		{
			XTimerContext tc;
			tc.SetTimer(XTimerContext::Handler(&t1, &Test1::Callback), 180, 0);
			tc.SetTimer(XTimerContext::Handler(&t2, &Test1::Callback), 50);
			tc.SetTimer(XTimerContext::Handler(&t3, &Test1::Callback), 120, 0);
			tc.SetTimer(XTimerContext::Handler(&t4, &Test1::Callback), 210);

			XThread::SleepMs(220);
		}

		TS_ASSERT_EQUALS(t1.mFiredQty, 1);
		TS_ASSERT_DELTA(t1.mFired[0], 180, TOLLERANCE);

		TS_ASSERT_EQUALS(t2.mFiredQty, 1);
		TS_ASSERT_DELTA(t2.mFired[0], 50, TOLLERANCE);

		TS_ASSERT_EQUALS(t3.mFiredQty, 1);
		TS_ASSERT_DELTA(t3.mFired[0], 120, TOLLERANCE);

		TS_ASSERT_EQUALS(t4.mFiredQty, 1);
		TS_ASSERT_DELTA(t4.mFired[0], 210, TOLLERANCE);
	}

	void testTimerRepeats(void)
	{
		Test1 t1, t2, t3, t4;
		{
			XTimerContext tc;
			tc.SetTimer(XTimerContext::Handler(&t1, &Test1::Callback), 60, 50);
			tc.SetTimer(XTimerContext::Handler(&t2, &Test1::Callback), 100, 100);
			tc.SetTimer(XTimerContext::Handler(&t3, &Test1::Callback), 25, 20);
			tc.SetTimer(XTimerContext::Handler(&t4, &Test1::Callback), 25, 20);

			XThread::SleepMs(280);
		}

		TS_ASSERT_EQUALS(t1.mFiredQty, 5);
		TS_ASSERT_DELTA(t1.mFired[0], 60, TOLLERANCE);
		for (int i=1; i < t1.mFiredQty; i++) {
			TS_ASSERT_DELTA(t1.mFired[i], 50, TOLLERANCE);
		}

		TS_ASSERT_EQUALS(t2.mFiredQty, 2);
		TS_ASSERT_DELTA(t2.mFired[0], 100, TOLLERANCE);
		TS_ASSERT_DELTA(t2.mFired[1], 100, TOLLERANCE);

		TS_ASSERT_EQUALS(t3.mFiredQty, 13);
		TS_ASSERT_DELTA(t3.mFired[0], 25, TOLLERANCE);
		for (int i=1; i < t3.mFiredQty; i++) {
			TS_ASSERT_DELTA(t3.mFired[i], 20, TOLLERANCE);
		}

		TS_ASSERT_EQUALS(t4.mFiredQty, 13);
		TS_ASSERT_DELTA(t4.mFired[0], 25, TOLLERANCE);
		for (int i=1; i < t4.mFiredQty; i++) {
			TS_ASSERT_DELTA(t4.mFired[i], 20, TOLLERANCE);
		}
	}

	void testTimerDrift(void)
	{
		Test1 t1(25);
		{
			XTimerContext tc;
			tc.SetTimer(XTimerContext::Handler(&t1, &Test1::Callback), 30, 30);

			XThread::SleepMs(100);
		}

		TS_ASSERT_EQUALS(t1.mFiredQty, 3);
		for (int i=0; i < t1.mFiredQty; i++) {
			TS_ASSERT_DELTA(t1.mFired[i], 30, TOLLERANCE);
		}
	}


};
