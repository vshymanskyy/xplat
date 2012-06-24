#ifndef _X_ARRAY_H_
#define _X_ARRAY_H_

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <XDebug.h>

/// Represents a generic Array
template <class T, unsigned SIZE = 0xFF>
class XArray {

public:
	/// Array iterator
	class It {
		friend class XArray<T, SIZE> ;

	private:
		/// Index of element
		unsigned mIndex;

	public:
		/// Constructor
		It(const unsigned index)
			: mIndex(index)
		{
		}

		/// Destructor
		~It() {
		}

		/// Comparison operator
		bool operator ==(const It& it) const {
			return mIndex == it.mIndex;
		}
		/// Comparison operator
		bool operator !=(const It& it) const {
			return mIndex != it.mIndex;
		}

		/// Moves to next item
		It operator ++() {
			++mIndex;
			return (*this);
		}

		/// Moves to previous item
		It operator --() {
			--mIndex;
			return (*this);
		}

		/// Moves to next item
		It operator +(int i) { return It(mIndex + i); }
		/// Moves to previous item
		It operator -(int i) { return It(mIndex - i); }
	};

	/// Constructor
	XArray() :
		mCount(0) {
	}

	/// Destructor
	~XArray() {
	}

	/// Returns the size of the list
	int Count() const {
		return mCount;
	}

	/// Clears the Array
	void Clear() {
		mCount = 0;
	}

	/// Checks if Array is empty
	bool IsEmpty() const {
		return !mCount;
	}

	/// Checks if Array is full
	bool IsFull() const {
		return mCount >= SIZE;
	}

	/// Returns iterator that points to the first item in the Array
	It First() const {
		return It(0);
	}

	/// Returns iterator that points to the last item in the Array
	It Last() const {
		return It(mCount - 1);
	}

	/// Returns iterator that represents the end of the Array
	It End() const {
		return It(mCount);
	}

	/// Appends an item to the back of the Array
	/// @param item Item to append
	void Append(const T& item) {
		X_ASSERT(!IsFull());
		mData[mCount++] = item;
	}

	/// Inserts an item on a specified position
	/// @param index Position to insert at
	/// @param item Item to insert
	void InsertBefore(const It it, const T& item) {
		X_ASSERT(!IsFull());
		X_ASSERT(it.mIndex <= mCount);
		for (unsigned i = mCount; i > it.mIndex; --i) {
			mData[i] = mData[i - 1];
		}
		mCount++;
		mData[it.mIndex] = item;
	}

	/// Inserts an item on a specified position
	/// @param index Position to insert at
	/// @param item Item to insert
	void InsertAfter(It it, const T& item) {
		++it;
		InsertBefore(it, item);
	}

	/// Removes an item on a specified position
	/// @param index Position to remove at
	void Remove(const It it) {
		X_ASSERT(it.mIndex < mCount);
		for (unsigned i = it.mIndex + 1; i < mCount; ++i) {
			mData[i - 1] = mData[i];
		}
		mCount--;
	}

	/// Finds first occurrence of an item
	/// @param item Item to find
	/// @returns Iterator pointing to found item or End if not found
	It FindFirst(const T& item) const {
		for (unsigned i = 0; i < mCount; ++i) {
			if (mData[i] == item) {
				return It(i);
			}
		}
		return It(mCount);
	}

	/// @param it Iterator
	/// @returns Constant item at iterator position
	T& operator[](const It& it) {
		X_ASSERT(it.mIndex < mCount);
		return mData[it.mIndex];
	}

	/// @param it Iterator
	/// @returns An item at iterator position
	const T& operator[](const It& it) const {
		X_ASSERT(it.mIndex < mCount);
		return mData[it.mIndex];
	}

private:
	/// Items count
	unsigned mCount;
	/// Data buffer
	T mData[SIZE];
};

#endif /* _X_ARRAY_H_ */
