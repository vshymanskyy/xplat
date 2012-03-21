#ifndef INC_BITARRAY_HPP
#define INC_BITARRAY_HPP

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <XDebug.h>
#include <limits.h>

#ifndef SIZE_MAX
#define SIZE_MAX ((size_t)-1)
#define SIZE_MIN ((size_t)0)
#endif

class BitArray
{
public:
	BitArray(size_t n);
	~BitArray();

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

BitArray::BitArray(size_t n)
	: mSize(n)
{
	mData = new size_t[INDEX_FROM_BIT(mSize)];
	ClearAll();
}

BitArray::~BitArray()
{
	delete[] mData;
}

void
BitArray::ClearAll()
{
	for (size_t i = 0; i < INDEX_FROM_BIT(mSize); i++)
		mData[i] = SIZE_MIN;
}

void
BitArray::Set(const size_t i)
{
	XASSERT(i < mSize);
	mData[INDEX_FROM_BIT(i)] |= (0x1 << OFFSET_FROM_BIT(i));
}

void
BitArray::Clear(const size_t i)
{
	XASSERT(i < mSize);
	mData[INDEX_FROM_BIT(i)] &= ~(0x1 << OFFSET_FROM_BIT(i));
}

bool
BitArray::Test(const size_t i) const
{
	XASSERT(i < mSize);
	return (mData[INDEX_FROM_BIT(i)] & (0x1 << OFFSET_FROM_BIT(i)));
}

size_t
BitArray::FindFirstClear() const
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
BitArray::FindFirstSet() const
{
	for (size_t i = 0; i < INDEX_FROM_BIT(mSize); i++) {
		if (mData[i] != SIZE_MIN) {						// nothing set, exit early.
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

#endif // INC_BITARRAY_HPP
