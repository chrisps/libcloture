#include "stl.hpp"

#include "stdio.h"

#include <cstdlib>
using namespace cloture;
using namespace util;
using namespace common;
using namespace stream;
using namespace ctfe::machine;

static ostream<typeof(printf), printf> o;

static constexpr auto testMem = Memory<65535>();

struct alignas(16) memsetTest
{
	int ints[8];
};

memsetTest meme[8] = {};

using namespace util::routines::general;

using namespace templates::variadic;


template<typename T, typename T2>
__forceinline __hot
static unsigned int testAndComplement(T& val, const T2 bit) noexcept
{
	register unsigned int result = 0;
	
	static_assert(generic::isInteger<T>() && sizeof(T) == sizeof(T2) && sizeof(T) == sizeof(int));
	

	backreference T2 originalBit = bit;
	backreference T originalVal = val;

	asm (
	"btcl %[_bit], %[input]\n\t"
	"adcl %[result], %[result]\n\t"
	: [input] "+rm" (val), [result] "+r" (result)
	: [_bit] "ri" (bit)
	: "cc"
	);
	
	__assume(val != originalVal);

	__assume(val == (originalVal ^ (1u << (unsigned int)bit)));
	
	__assume(bit == originalBit);
	__assume(result == static_cast<unsigned int>(originalVal >> bit) & 1u);
	__assume(result == 0u || result == 1u);
	return result;
}

enum class fieldType
{
	Float,
	Int, 
	Reference,
	Double,
	Boolean
};

struct fieldDescriptor
{
	union
	{
		struct
		{
			union
			{
				struct
				{
					const uint16 	namelength;
					const uint16 	namecrc;
				};
				const uint32 namecmp;
			};
			const uint16	offset;
			const uint16	signature;
		}__packed;
		uint64 fieldInfo;
	} __packed;

	static constexpr uint64 nameLengthMask 		= 0xFFFFULL;
	static constexpr uint64 nameCRCMask 		= nameLengthMask << 16ULL;
	static constexpr uint64 offsetMask 			= nameCRCMask << 16ULL;
	static constexpr uint64 signatureMask 		= offsetMask << 16ULL;

	__forceinline fieldDescriptor(const uint16 _namelength, const uint16 _namecrc, const uint16 offs, const uint16 sig) :
	 namelength(_namelength), namecrc(_namecrc), offset(offs), signature(sig)
	{
	}
	
	constexpr fieldDescriptor(const char* name, size_t typeSz, fieldType type) :
	namelength(ctfe::cstrlen(name)), 
	namecrc(ctfe::CRC::calculate(name, ctfe::cstrlen(name))),
	offset(0xFFFF),
	signature(static_cast<uint16>(typeSz << 3) | static_cast<uint16>(type))
	{}
	
};

class alignas(mSimdAlign) dynamicFieldProto
{
	fieldDescriptor descr;
	union fieldUnion
	{
		void*		referenceField;
		boolean 	boolField;
		int			intField;
		float		floatField;
		double 		doubleField;
	};
	fieldUnion field;
public:
	__forceinline 
	dynamicFieldProto(const uint16 _namelength, const uint16 _namecrc, const uint16 offs, const uint16 sig) 
	: descr(_namelength, _namecrc, offs, sig)
	{}
};

class dynamicField final : public dynamicFieldProto
{
	char padding[padSizeSimd(sizeof(dynamicFieldProto))];
public:
	using dynamicFieldProto::dynamicFieldProto;
	
	
};

static_assert(sizeof(dynamicField) % mSimdAlign == 0);

class dynamicClassProto
{
	dynamicField* __restrict const fields;
	size_t 			fieldCount;
public:
};

static int tttt = 4979842;
volatile int bbb = 0b1011;

int main(int argc, const char** args)
{
	/*
	unsigned int val = bbb;
	unsigned int i;
	
	for(i = 0; i < 32; ++i)
	{
		const unsigned int testresult = bitwise::test(val, i);
		if(testresult & bitwise::test(i, 2))
			break;
	}
	
	const int jj = 3;

	const int result = bitwise::test(3, 0);//testAndComplement(jj, 0);
	*/
	uint64 ffff = 0;
	fieldDescriptor *f = reinterpret_cast<fieldDescriptor*>(&ffff);
	f->fieldInfo = 0xFFFFEEEE33335555UL;
	
	printf("igggigigi");
	return 0;
}
