#ifndef INC_QUEUE_HPP
#define INC_QUEUE_HPP

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <XDebug.h>

/// Represents a generic circular (FIFO) index-based Queue with fixed size
/// Recommendation: Use this Queue with SIZE = 2^n to get fast results
template<class T, unsigned SIZE>
class XQueue
{

public:
	/// Constructor
	XQueue();
	/// Destructor
	~XQueue();

	/// Checks if Queue is empty
	bool IsEmpty() const;

	/// Clears the Queue
	void Clear();

	/// Inserts an item into the Queue
	void Push(const T& info);

	/// Reads an item from the Queue
	T Pop();

	/// Reads the top item from the Queue, not removing it
	T Peek();

private:
	/// Data buffer
	T	mData[SIZE];
	/// Index of writer
	int mInIdx;
	/// Index of reader
	int mOutIdx;
};

template<class T, unsigned SIZE>
inline
XQueue<T, SIZE>::XQueue()
	: mInIdx(0)
	, mOutIdx(SIZE - 1)
{
}

template<class T, unsigned SIZE>
inline
XQueue<T, SIZE>::~XQueue()
{
}

template<class T, unsigned SIZE>
inline
bool
XQueue<T, SIZE>::IsEmpty() const
{
	return (((mOutIdx+1)%SIZE) == mInIdx);
}

template<class T, unsigned SIZE>
inline
void
XQueue<T, SIZE>::Clear()
{
	mInIdx = 0;
	mOutIdx = SIZE - 1;
}

template<class T, unsigned SIZE>
inline
void
XQueue<T, SIZE>::Push(const T& info)
{
	mData[mInIdx] = info;
	mInIdx = (mInIdx+1)%SIZE;
	X_ASSERT_NE(mInIdx, mOutIdx, "%d");		//overflow
}

template<class T, unsigned SIZE>
inline
T
XQueue<T, SIZE>::Pop()
{
	mOutIdx = (mOutIdx+1)%SIZE;
	X_ASSERT_NE(mOutIdx, mInIdx, "%d");		//underflow
	return mData[mOutIdx];
}

template<class T, unsigned SIZE>
inline
T
XQueue<T, SIZE>::Peek()
{
	const int p = (mOutIdx+1)%SIZE;
	X_ASSERT_NE(p, mInIdx, "%d");			//underflow
	return mData[p];
}

#endif // INC_QUEUE_HPP
