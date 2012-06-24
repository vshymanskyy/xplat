#ifndef _X_MEMORY_BUFFER_H_
#define _X_MEMORY_BUFFER_H_

class XMemoryBuffer {
public:
	XMemoryBuffer(size_t len)
		: mLength(len)
	{
		mBuffer = new char[mLength];
	}

	~XMemoryBuffer() {
		if(mBuffer) {
			delete[] mBuffer;
		}
	}

	bool Extend(size_t size) {
		if(mLength < size) {
			if(mBuffer) {
				delete[] mBuffer;
			}
			mLength = size;
			mBuffer = new char[mLength];
		}
		return mBuffer != NULL;
	}

	void* Buffer() { return mBuffer; }
	const void* Buffer() const { return mBuffer; }

private:
	size_t	mLength;
	size_t	mPos;
	char*	mBuffer;
};

#endif /* _X_MEMORY_BUFFER_H_ */
