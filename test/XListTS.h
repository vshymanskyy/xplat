#include "XList.h"
#include <cxxtest/TestSuite.h>

class ListTS: public CxxTest::TestSuite
{
public:

	void testFirstAndLast(void)
	{
		XList<int> a;

		a.Append(1);
		a.Append(2);
		a.Append(3);
		a.Append(8);
		TS_ASSERT(a.Count() == 4);

		XList<int>::It it = ++a.First();
		TS_ASSERT(a[++it] == 3);
		TS_ASSERT(a[a.Last()] == 8);
		TS_ASSERT(a[--a.Last()] == 3);
		TS_ASSERT(a[a.First()] == 1);
	}

	void testIteratorOffsets(void)
	{
		XList<int> a;

		a.Append(1);
		a.Append(2);
		a.Append(3);
		a.Append(8);

		TS_ASSERT(a[a.First()+0] == 1);
		TS_ASSERT(a[a.First()+2] == 3);
		TS_ASSERT(a[a.Last()-2] == 2);
		TS_ASSERT(a[a.Last()-1] == 3);
		TS_ASSERT(a[a.Last()-0] == 8);
		TS_ASSERT(a[a.Last()-5] == 8);
	}

	void testIteratorOutOfBounts()
	{
		XList<int> a;

		a.Append(1);
		a.Append(2);
		a.Append(3);

		XList<int>::It it = a.End();
		a.InsertAfter(it, 0);
		TS_ASSERT(a[a.First()] == 0);

		it = a.End();
		a.InsertBefore(it, 4);
		TS_ASSERT(a[a.Last()] == 4);

		TS_ASSERT(a.Count() == 5);
	}

	void testAppendPrepend()
	{
		XList<int> a;

		a.Append(1);
		a.Append(2);
		a.Append(3);

		a.Prepend(0);
		TS_ASSERT(a[a.First()] == 0);

		a.Append(4);
		TS_ASSERT(a[a.Last()] == 4);

		TS_ASSERT(a.Count() == 5);
	}

	void testInsertBeforeAfter()
	{
		XList<int> a;

		a.Append(1);
		a.Append(2);
		a.Append(3);

		XList<int>::It it = a.First();
		++it;

		a.InsertAfter(it, 0);
		TS_ASSERT(a[it] == 0);

		a.InsertBefore(it, 4);
		TS_ASSERT(a[it] == 4);
	}

	void testRemoveInCycle()
	{
		XList<int> a;

		a.Append(1);
		a.Append(2);
		a.Append(3);
		a.Append(5);
		a.Append(4);
		a.Append(1);
		a.Append(10);

		for (XList<int>::It it = a.First(); it != a.End(); ++it) {
			if (a[it] % 2 != 0) {
				a.Remove(it);
			}
		}

		TS_ASSERT(a.Count() == 3);
		TS_ASSERT(a[a.First()] == 2);
		TS_ASSERT(a[++a.First()] == 4);
		TS_ASSERT(a[++(++a.First())] == 10);
	}

	void testReplaceInCycle()
	{
		XList<int> a;

		a.Append(1);
		a.Append(2);
		a.Append(3);
		a.Append(5);
		a.Append(4);
		a.Append(1);
		a.Append(10);

		for (XList<int>::It it = a.First(); it != a.End(); ++it) {
			int i = a[it];
			if (i % 2 != 0) {
				a.Remove(it);
				a.InsertAfter(it, i * 10);
				//++it;
			}
		}

		TS_ASSERT(a.Count() == 7);

		int result[] =
			{ 10, 2, 30, 50, 4, 10, 10 };

		{
			int i = 0;
			for (XList<int>::It it = a.First(); it != a.End(); ++it) {
				TS_ASSERT(a[it] == result[i++]);
			}
			TS_ASSERT(i == 7);
		}
		{
			int i = 6;
			for (XList<int>::It it = a.Last(); it != a.End(); --it) {
				TS_ASSERT(a[it] == result[i--]);
			}
			TS_ASSERT(i == -1);
		}
	}

	void testFindFirstLast()
	{
		XList<int> a;

		a.Append(1);
		a.Append(2);
		a.Append(3);
		a.Append(5);
		a.Append(4);
		a.Append(1);
		a.Append(10);

		TS_ASSERT(--a.FindAfter(a.First(), 1) == a.End());
		TS_ASSERT(a[++a.FindAfter(a.First(), 1)] == 2);
		TS_ASSERT(a[++a.FindBefore(a.Last(), 1)] == 10);

		TS_ASSERT(a.FindBefore(a.Last(), 11) == a.End());
		TS_ASSERT(a.FindAfter(a.First(), 15) == a.End());

		TS_ASSERT(a.FindBefore(--a.Last(), 11) == a.End());
		TS_ASSERT(a.FindAfter(++a.First(), 15) == a.End());
	}

	void testReverse()
	{
		XList<int> a;

		a.Append(1);
		a.Append(2);
		a.Append(3);
		a.Append(5);
		a.Append(4);
		a.Append(1);
		a.Append(10);

		a.Reverse();

		TS_ASSERT(a.Count() == 7);

		int result[] =
			{ 10, 1, 4, 5, 3, 2, 1 };

		{
			int i = 0;
			for (XList<int>::It it = a.First(); it != a.End(); ++it) {
				TS_ASSERT(a[it] == result[i++]);
			}
			TS_ASSERT(i == 7);
		}
		{
			int i = 6;
			for (XList<int>::It it = a.Last(); it != a.End(); --it) {
				TS_ASSERT(a[it] == result[i--]);
			}
			TS_ASSERT(i == -1);
		}
	}

	void testSwapLinks()
	{
		XList<int> a;

		a.Append(1);
		a.Append(2);
		a.Append(3);
		a.Append(5);
		a.Append(4);
		a.Append(1);
		a.Append(10);

		a.SwapLinks(++a.First(), --a.Last());

		int result[] =
			{ 1, 1, 3, 5, 4, 2, 10 };

		{
			int i = 0;
			for (XList<int>::It it = a.First(); it != a.End(); ++it) {
				TS_ASSERT(a[it] == result[i++]);
			}
			TS_ASSERT(i == 7);
		}
		{
			int i = 6;
			for (XList<int>::It it = a.Last(); it != a.End(); --it) {
				TS_ASSERT(a[it] == result[i--]);
			}
			TS_ASSERT(i == -1);
		}
	}

	class __TestListCRTP: public XList<__TestListCRTP> {
private:
	int _data;
};

void testCRTP() {
	__TestListCRTP a, b, c, d;
	a.Append(b);
	b.Append(a);
	b.Append(c);
	b.Append(d);
	c.Append(a);
}

};
