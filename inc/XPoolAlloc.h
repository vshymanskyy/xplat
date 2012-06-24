#ifndef _X_POOL_ALLOC_H_
#define _X_POOL_ALLOC_H_

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <XDebug.h>
#include <XPool.h>

template <class T, int QTY>
class XPoolAlloc {
#ifndef DMALLOC
public:

	void* operator new(size_t) {
		X_ASSERT(entries);
		return entries->Allocate();
	}

	void operator delete(void* ptr) {
		entries->Release(ptr);
	}

private:
	static XPool<T, QTY>* entries;
#endif
};

#ifndef DMALLOC
template <class T, int QTY> XPool<T, QTY>* XPoolAlloc<T, QTY>::entries = new XPool<T, QTY>();
#endif

#endif // _X_POOL_ALLOC_H_
