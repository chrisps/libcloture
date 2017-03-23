#pragma once

namespace cloture::util::common
{
	/**
	 * like sizeof, but for bits instead of bytes
	 * intended for use with basic types
	 */
	template<typename T> constexpr size_t bitSizeof = 	sizeof(T) * 8;

	static_assert
	(
			bitSizeof<char>			==	8
		&&	bitSizeof<short>		==	16
		&&	bitSizeof<int>			==	32
		&&	bitSizeof<long long>	==	64,
		"Apparently bitSizeof is fuckt. Pretty cray."
	);
	/**
	 * popcnt
	 *
	 * counts the number of bits set
	 */
	template<typename T> constexpr
	size_t popcnt(const T inputVal)
	{
		__import_make_unsigned();
		static_assert(is_integral(T), "popcnt requires an integral type.");

		/*
			signed shift would produce inaccurate results
		*/
		make_unsigned(T) val = static_cast<make_unsigned(T)>(inputVal);
		size_t cnt = 0;

		for(size_t i = 0; i < bitSizeof<T>; ++i )
		{
			if(val & 1 != 0)
				++cnt;
			val >>= 1;
		}
		return cnt;
	}

	static_assert
	(
		popcnt<int32>(3) == 2,
		"meta::popcnt is returning inaccurate results..."
	);

	template<typename T>
	constexpr bool powerOfTwo(const T x)
	{
		static_assert(
		is_integral(T),
		"common::powerOfTwo requires an integral type"
		);

		return x != static_cast<T>(0) && (x & (x - static_cast<T>(1))) == static_cast<T>(0);
	}

	template<typename T>
	constexpr size_t findBitSet(const T x)
	{
		__import_make_unsigned();
		__import_make_signed();

		constexpr make_signed(T) negOne 		= static_cast<T>(-1);
		constexpr make_unsigned(T) highestBit 	= ~(static_cast<make_unsigned(T)>(negOne) >> 1);

		make_unsigned(T) bitmask = highestBit;
		size_t i = 0;
		while((bitmask & x) == static_cast<T>(0) && bitmask != static_cast<T>(0))
		{
			bitmask >>= 1;
			++i;
		}
		return bitSizeof<T> - i - 1;
	}
	

	static_assert(
	findBitSet(1) == 0
	);
	static_assert(!powerOfTwo(0));

	template<typename T>
	constexpr T makeMask(size_t bits) {
		return ~(static_cast<T>(~0) << bits);
	}
	template<typename T>
	constexpr T makeMask(unsigned start, unsigned end) {
		T result = 0;
		for(unsigned i = start; i != end; ++i) {
			result |= (1 << i);
		}
		return result;
	}

	template<typename T1, typename T2> bool testBit(T1 value, T2 bit) {
		return (value & bit);
	}

}
