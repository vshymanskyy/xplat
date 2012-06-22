#ifndef INC_POOL_HPP
#define INC_POOL_HPP

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <XDebug.h>
#include <XStack.h>

template <class T, int QTY, unsigned ALIGN = 0>
class XPool {

public:
    XPool() {
        for (int i=0; i<QTY; i++) {
            mFree.Push(mData+i*sizeof(T));
        };
    };

    bool IsEmpty() {
    	return mFree.IsEmpty();
    }

    T* Allocate() {
        X_ASSERT(!mFree.IsEmpty());
        return (T*)mFree.Pop();
    }

    void Release(void* ptr) {
    	X_ASSERT(!mFree.IsFull());
        mFree.Push(ptr);
    }

private:
    char              mData[sizeof(T)*QTY];
    XStack<void*, QTY> mFree;
};

#endif // INC_POOL_HPP
