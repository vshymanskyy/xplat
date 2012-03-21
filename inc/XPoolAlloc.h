#ifndef INC_POOL_ALLOC_HPP
#define INC_POOL_ALLOC_HPP

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
		XASSERT(entries);
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

#endif // INC_POOL_ALLOC_HPP
