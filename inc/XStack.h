#ifndef _X_STACK_H_
#define _X_STACK_H_

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

/// Represents a generic Stack (LIFO) with fixed size
template <class T, unsigned SIZE>
class XStack
{

public:
	/// Constructor
	XStack();

	/// Destructor
	~XStack();

    /// Checks if stack is empty
    bool IsEmpty() const;

    /// Checks if stack is full
    bool IsFull() const;

	/// Inserts an item into the queue
	/// @param item an item to push
	/// @returns false if buffer overflow occurs
	void Push(const T& item);

	/// Reads an item from the queue
	/// @returns false if buffer is empty
	T Pop();

	/// Reads the top item from the queue, not removing it
	/// @param item an item to read to
	/// @returns false if buffer is empty
	bool Peek(T& item);

private:
	/// Data buffer
    T  mData[SIZE];
	/// The 'top' element of the stack
    T* mTop;
};

template <class T, unsigned SIZE>
inline
XStack<T, SIZE>::XStack()
    : mTop	(&mData[0])
{
}

template <class T, unsigned SIZE>
inline
XStack<T, SIZE>::~XStack()
{
}

template<class T, unsigned SIZE>
inline
bool
XStack<T, SIZE>::IsEmpty() const
{
    return (mTop <= &mData[0]);
}

template<class T, unsigned SIZE>
inline
bool
XStack<T, SIZE>::IsFull() const
{
    return (mTop >= &mData[SIZE]);
}

template <class T, unsigned SIZE>
inline
void
XStack<T, SIZE>::Push(const T& item)
{
    *(mTop++) = item;
}

template <class T, unsigned SIZE>
inline
T
XStack<T, SIZE>::Pop()
{
    return *(--mTop);
}

template <class T, unsigned SIZE>
inline
bool
XStack<T, SIZE>::Peek(T& item)
{
    if (mTop <= &mData[0])		//underflow
		return false;
    item = *(mTop-1);
	return true;
}

#endif /* _X_STACK_H_ */
