#ifndef _X_GENERATOR_H_
#define _X_GENERATOR_H_

// Generator/continuation for C++
// Author: Andrew Fedoniouk @ terrainformatica.com
// Idea borrowed from: "coroutines in C" Simon Tatham,
// http://www.chiark.greenend.org.uk/~sgtatham/coroutines.html

class XGenerator {
protected:
	XGenerator() : _line(-1) {}
	int _line;
};

#define XGENERATOR(NAME)						\
struct NAME										\
	: public XGenerator							\

#define XEMIT(TYPE)								\
	bool operator() (TYPE& _rv) {				\
		if (_line < 0) {						\
			_line=0;							\
		}										\
		switch(_line) {							\
		case 0:;								\

#define XYIELD(VALUE)							\
		do {									\
			_line = __LINE__;					\
			_rv = (VALUE);						\
			return true;						\
			case __LINE__:;						\
		} while (0)								\

#define XSTOP									\
		}										\
		_line = 0;								\
		return false;							\
	}											\

/*****************************************
 * Example
 *****************************************

XGENERATOR(IntRange) {

public:
	IntRange(int a, int b)
		: _a(a)
		, _b(b)
	{
	}

	XEMIT(int)
		for (_i = _a; _i < _b; _i++) {
			XYIELD(_i);
		}
	XSTOP;

private:
	int _a;
	int _b;

private:
	int _i;
};

*/

/*****************************************
 * Usage
 *****************************************

	IntRange gen(3, 10);
	int n;
	while(gen(n)) {
		LOG("Generated", n);
	}

*/

#endif /* _X_GENERATOR_H_ */