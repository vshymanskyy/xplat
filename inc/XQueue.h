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
	T	_data[SIZE];
	/// Index of writer
	int _inIdx;
	/// Index of reader
	int _outIdx;
};

template<class T, unsigned SIZE>
inline
XQueue<T, SIZE>::XQueue()
	: _inIdx(0)
	, _outIdx(SIZE - 1)
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
	return (((_outIdx+1)%SIZE) == _inIdx);
}

template<class T, unsigned SIZE>
inline
void
XQueue<T, SIZE>::Clear()
{
	_inIdx = 0;
	_outIdx = SIZE - 1;
}

template<class T, unsigned SIZE>
inline
void
XQueue<T, SIZE>::Push(const T& info)
{
	_data[_inIdx] = info;
	_inIdx = (_inIdx+1)%SIZE;
	XASSERT_NE(_inIdx, _outIdx, "%d");		//overflow
}

template<class T, unsigned SIZE>
inline
T
XQueue<T, SIZE>::Pop()
{
	_outIdx = (_outIdx+1)%SIZE;
	XASSERT_NE(_outIdx, _inIdx, "%d");		//underflow
	return _data[_outIdx];
}

template<class T, unsigned SIZE>
inline
T
XQueue<T, SIZE>::Peek()
{
	const int p = (_outIdx+1)%SIZE;
	XASSERT_NE(p, _inIdx, "%d");			//underflow
	return _data[p];
}

#endif // INC_QUEUE_HPP
