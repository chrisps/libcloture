#pragma once

namespace cloture {
namespace util    {
namespace common  {
	/*
	integer primitive types
	*/
	using uint8		= unsigned char;
	using int8		= signed char;
	using uint16	= unsigned short;
	using int16		= signed short;
	using uint32	= unsigned int;
	using int32		= signed int;
	using uint64	= unsigned __int64;//unsigned long long;
	using int64		= __int64;//signed long long;

	using size16	= uint16;
	using size32	= uint32;
	using size64	= uint64;
	
	using boolean 	= size32;
	
	using ptrdiff32 = int32;


	/*
	real number types
	*/
	using real32 = float;
	using real64 = double;
	using real80 = long double;


	static constexpr ptrdiff_t 	none	=  -1;
	static constexpr size_t 	unone	= (none);


#if (defined(__clang__) || defined(__GNUC__) || defined(__INTEL_COMPILER)) && !defined(NO_INT128)
	#define	mCompilerSupportsInt128	1
#else
	#define	mCompilerSupportsInt128	0
#endif

#if mCompilerSupportsInt128 && defined(__x86_64__)
	using int128	= __int128;
	using uint128	= unsigned __int128;
#else
	/*
	 * include some header here containing a class to emulate __int128
	 */
	 #include "int128.hpp"
#endif
	/*
	sanity checks
	*/
	static_assert( sizeof(uint8) == 1	&& sizeof(int8) == 1, "sizeof int8/uint8 ought to be one byte...");
	static_assert( sizeof(uint16) == 2	&& sizeof(int16) == 2, "sizeof int16/uint16 should be two bytes...");
	static_assert( sizeof(uint32) == 4	&& sizeof(int32) == 4, "sizeof int32/uint32 should be four bytes...");
	static_assert( sizeof(uint64) == 8	&& sizeof(int64) == 8, "sizeof int64/uint64 should be eight bytes...");
#if !defined(NO_INT128_IMPL)
	static_assert( sizeof(uint128) == 16 && sizeof(int128) == 16, "sizeof int128/uint128 should be sixteen bytes...");
#endif
/*
 * todo: use some ctfe:: functions in here to clean this up
 * maybe make this ctfe::parser::parse<uint128>
 */
#if !defined(NO_INT128_IMPL)
constexpr uint128 toU128(const char* str)
{
	constexpr uint128 ZERO 		= static_cast<uint128>(0);
	constexpr uint128 TEN 		= static_cast<uint128>(10);
	constexpr uint128 SIXTEEN	= static_cast<uint128>(16);
	constexpr uint128 SIGN		= static_cast<uint128>(0x8000000000000000ULL) << 64;

	constexpr char digitSeperator = '\'';

	uint128 result = ZERO;
	size_t pos = 0;
	//hexadecimal literal
	if(str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
	{
		const char* s = &str[2];
		while(true)
		{
			const unsigned char c = static_cast<unsigned char>(s[pos++]);
			if(!c)
				return result;

			if(c == digitSeperator)
				continue;

			uint128 digit = ZERO;

			if(c >= '0' && c <= '9')
				digit = static_cast<uint128>(c - '0');
			else if(c >= 'a' && c <= 'z')
				digit = static_cast<uint128>(c - ('a' - 10));
			else
				digit = static_cast<uint128>(c - ('A' - 10));
			result *= SIXTEEN;
			result += digit;
		}
	}
	//binary literal
	else if(str[0] == '0' && (str[1] == 'b' || str[1] == 'B'))
	{
		const char* s = &str[2];

		while(true)
		{
			const char c = s[pos++];
			if(!c)
				return result;
			if(c == digitSeperator)
				continue;
			const uint128 bit = static_cast<uint128>(c - '0');
			result <<= 1;
			result |= bit;
		}
	}
	//decimal literal
	else
	{
		const bool negative = str[0] == '-';

		if(negative || str[0] == '+')
			++pos;

		bool first = true;
		for(char c = 0; 0 <= (c = str[pos] - '0' ) && c <= 9; ++pos)
		{
			if(!first)
				result *= TEN;
			if(c == digitSeperator)
				continue;
			uint128 c128 = static_cast<uint128>(c);
			result += c128;
			if(first)
				first = false;
		}

		return negative ? result | SIGN : result;
	}
}

constexpr uint128 operator"" _u128(const char *str)
{
	return toU128(str);
}
constexpr int128 operator"" _i128(const char *str)
{
	return static_cast<int128>(toU128(str));
}
#endif

template<typename T> static constexpr T maxValue 		= 0;
template<> static constexpr real64 	maxValue<real64> 	= 1.79769e+308;
template<> static constexpr real32 	maxValue<real32> 	= 3.40282e+38f;

template<typename T> static constexpr T minValue 		= 0;
template<> static constexpr real64 	minValue<real64> 	= 2.22507e-308;
template<> static constexpr real32 	minValue<real32> 	= 1.17549e-38f;

template<> static constexpr uint8 maxValue<uint8> 		= 0xFFu;
template<> static constexpr uint16 maxValue<uint16> 	= 0xFFFFu;
template<> static constexpr uint32 maxValue<uint32> 	= 0xFFFFFFFFu;
template<> static constexpr uint64 maxValue<uint64> 	= 0xFFFFFFFFFFFFFFFFULL;
#if !defined(NO_INT128_IMPL)
template<> static constexpr uint128 maxValue<uint128> 	= 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF_u128;
#endif
template<> static constexpr uint8 minValue<uint8> 		= 0u;
template<> static constexpr uint16 minValue<uint16> 	= 0u;
template<> static constexpr uint32 minValue<uint32> 	= 0u;
template<> static constexpr uint64 minValue<uint64> 	= 0ULL;
#if !defined(NO_INT128_IMPL)
template<> static constexpr uint128 minValue<uint128> 	= 0_u128;
#endif

template<typename T, typename metaInit>
struct Primitive
{
	T value;
	static constexpr T defaultValue = metaInit::value;

	__forceinline
	constexpr T operator =(T other)
	{
		return value = other;
	}

	__forceinline
	constexpr operator T()
	{
		return value;
	}
};



}//namespace common
}//namespace util
}//namespace cloture

#define		mClotureImportBasicTypes()			\
	using uint8 = cloture::util::common::uint8;		\
	using int8 = cloture::util::common::int8;		\
	using uint16 = cloture::util::common::uint16;	\
	using int16 = cloture::util::common::int16;		\
	using uint32 = cloture::util::common::uint32;	\
	using int32 = cloture::util::common::int32;		\
	using uint64 = cloture::util::common::uint64;	\
	using int64 = cloture::util::common::int64;		\
	using real32 = cloture::util::common::real32;	\
	using real64 = cloture::util::common::real64
