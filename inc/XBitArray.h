#ifndef _X_BITARRAY_H_
#define _X_BITARRAY_H_

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <XDebug.h>
#include <limits.h>

#ifndef SIZE_MAX
#define SIZE_MAX ((size_t)-1)
#define SIZE_MIN ((size_t)0)
#endif

class XBitArray
{
public:
	XBitArray(size_t n);
	~XBitArray();

	void ClearAll();
	void Set(size_t i);
	void Clear(size_t i);
	bool Test(size_t i) const;

	size_t FindFirstClear() const;
	size_t FindFirstSet() const;

private:
	size_t* mData;
	size_t mSize;
};

#define BITS_PER_ITEM		(8 * sizeof(size_t))
#define INDEX_FROM_BIT(a)	(a / BITS_PER_ITEM)
#define BIT_FROM_INDEX(a)	(a * BITS_PER_ITEM)
#define OFFSET_FROM_BIT(a)	(a % BITS_PER_ITEM)

XBitArray::XBitArray(size_t n)
	: mSize(n)
{
	mData = new size_t[INDEX_FROM_BIT(mSize)];
	ClearAll();
}

XBitArray::~XBitArray()
{
	delete[] mData;
}

void
XBitArray::ClearAll()
{
	for (size_t i = 0; i < INDEX_FROM_BIT(mSize); i++)
		mData[i] = 0;
}

void
XBitArray::Set(const size_t i)
{
	X_ASSERT(i < mSize);
	mData[INDEX_FROM_BIT(i)] |= (0x1 << OFFSET_FROM_BIT(i));
}

void
XBitArray::Clear(const size_t i)
{
	X_ASSERT(i < mSize);
	mData[INDEX_FROM_BIT(i)] &= ~(0x1 << OFFSET_FROM_BIT(i));
}

bool
XBitArray::Test(const size_t i) const
{
	X_ASSERT(i < mSize);
	return (mData[INDEX_FROM_BIT(i)] & (0x1 << OFFSET_FROM_BIT(i)));
}

size_t
XBitArray::FindFirstClear() const
{
	for (size_t i = 0; i < INDEX_FROM_BIT(mSize); i++) {
		if (mData[i] != SIZE_MAX) {
			for (size_t j = 0; j < BITS_PER_ITEM; j++) {
				if (!(mData[i] & (1 << j))) {
					return BIT_FROM_INDEX(i) + j;
				}
			}
		}
	}
	return SIZE_MAX;
}

size_t
XBitArray::FindFirstSet() const
{
	for (size_t i = 0; i < INDEX_FROM_BIT(mSize); i++) {
		if (mData[i] != 0) {						// nothing set, exit early.
			for (size_t j = 0; j < BITS_PER_ITEM; j++) {	// at least one bit is set here.
				if (mData[i] & (1 << j)) {
					return BIT_FROM_INDEX(i) + j;
				}
			}
		}
	}
	return SIZE_MAX;
}

#undef BITS_PER_ITEM
#undef INDEX_FROM_BIT
#undef BIT_FROM_INDEX
#undef OFFSET_FROM_BIT

#endif /* _X_BITARRAY_H_ */
