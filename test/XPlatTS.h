#include <limits.h>
#include <stdint.h>
#include <XPlat.h>
#include <cxxtest/TestSuite.h>

#if CHAR_BIT != 8
#	error "unsupported char size"
#endif

enum {
	O32_LITTLE_ENDIAN = 0x03020100ul,
	O32_BIG_ENDIAN = 0x00010203ul,
	O32_PDP_ENDIAN = 0x01000302ul
};

static const union { unsigned char bytes[4]; uint32_t value; } o32_host_order = { { 0, 1, 2, 3 } };

class XPlatTS: public CxxTest::TestSuite
{
public:
	void testEndianness(void)
	{
#if defined (X_ENDIAN_LITTLE) && !defined (X_ENDIAN_BIG) && !defined (X_ENDIAN_PDP)
		TS_ASSERT(o32_host_order.value == O32_LITTLE_ENDIAN);
#elif defined (X_ENDIAN_BIG) && !defined (X_ENDIAN_LITTLE) && !defined (X_ENDIAN_PDP)
		TS_ASSERT(o32_host_order.value == O32_BIG_ENDIAN);
#elif defined (X_ENDIAN_PDP) && !defined (X_ENDIAN_LITTLE) && !defined (X_ENDIAN_BIG)
		TS_ASSERT(o32_host_order.value == O32_PDP_ENDIAN);
#else
#	error "Only one X_ENDIAN_* macro should be defined"
#endif
	}


};
