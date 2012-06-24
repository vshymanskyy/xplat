#ifndef _X_ARRAY_EXT_H_
#define _X_ARRAY_EXT_H_

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <XDebug.h>
#include <stdlib.h>

/// Represents a generic Array
template <class T>
class XArrayExt {

public:
	/// Array iterator
	class It {
		friend class XArrayExt<T> ;

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
	XArrayExt()
		: mCount	(0)
		, mBuffSize	(2)
	{
		mData = malloc(mBuffSize*sizeof(T));
	}

	/// Destructor
	~XArrayExt() {
		free(mData);
	}

	/// Returns the size of the list
	int Count() const {
		return mCount;
	}

	/// Clears the Array
	void Clear() {
		mCount = 0;
		mBuffSize = 2;
		mData = realloc(mData, mBuffSize*sizeof(T));
	}

	/// Checks if Array is empty
	bool IsEmpty() const {
		return !mCount;
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
		Extend(1);
		mData[mCount++] = item;
	}

	/// Inserts an item on a specified position
	/// @param index Position to insert at
	/// @param item Item to insert
	void InsertBefore(const It it, const T& item) {
		Extend(1);
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
		Shrink();
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

	void Extend(int qty) {
		if (mCount + qty > mBuffSize) {
			mBuffSize *= 2;
			mData = realloc(mData, mBuffSize*sizeof(T));
		}
	}

	void Shrink() {
		if (mCount < mBuffSize/2) {
			mBuffSize /= 2;
			mData = realloc(mData, mBuffSize*sizeof(T));
		}
	}

private:
	/// Items count
	unsigned	mCount;
	/// Items count
	unsigned	mBuffSize;
	/// Data buffer
	T*			mData;
};

#endif /* _X_ARRAY_EXT_H_ */
