#include "XCompilerTS.h"


CompilerRvoTS::RvoTest CompilerRvoTS::RvoTestFun2() {
	RvoTest t;
	return t;
}

CompilerRvoTS::RvoTest CompilerRvoTS::RvoTestFun1() {
	return RvoTestFun2();
}

CompilerRvoTS::RvoTest CompilerRvoTS::RvoTestFun() {
	return RvoTestFun1();
}

CompilerRvoTS::RvoTest::RvoTest(const RvoTest& t)
{ TS_TRACE("RVO not optimal: Copy detected"); memcpy(buffer,t.buffer, sizeof(t.buffer)); }

CompilerRvoTS::RvoTest& CompilerRvoTS::RvoTest::operator = (const RvoTest& t)
{ TS_TRACE("RVO not optimal: Assignment detected"); memcpy(buffer,t.buffer, sizeof(t.buffer)); return *this; }
