#pragma once


	using cloture::util::common::powerOfTwo;
template<size_t integralBits, size_t fractionalBits>
class fixed
{

	template<size_t bitsum>
	struct type_selector
	{
		using type = void;
	};

	template<>
	struct type_selector<8>
	{
		using type = int8;
	};

	template<>
	struct type_selector<16>
	{
		using type = int16;
	};

	template<>
	struct type_selector<32>
	{
		using type = int32;
	};

	template<>
	struct type_selector<64>
	{
		using type = int64;
	};

	using T = typename type_selector<integralBits+fractionalBits>::type;
	static_assert(
		!type_is_void(T),
		"invalid integral/fractional bit combination."
	);

	T i;
public:
	template<typename TT>
	static constexpr TT one = static_cast<TT>(1LL << fractionalBits);

	#define		quickCtor(Type)	\
	__pure constexpr fixed(const Type f) : i(static_cast<T>(f * one<Type>))	\
	{}

	quickCtor(float)
	quickCtor(double)

	__pure constexpr fixed(const T f) : i(f)
	{}

	__pure constexpr fixed(const fixed& other) : i(other.i)
	{}

	__pure constexpr fixed operator ++()
	{
		i += one<T>;
		return *this;
	}

	__pure constexpr fixed operator --()
	{
		i -= one<T>;
		return *this;
	}

	__pure constexpr fixed operator +(const fixed(other)) const
	{
		const T i2 = i + other.i;
		return fixed(i2);
	}

	__pure constexpr fixed operator -(const fixed(other)) const
	{
		const T i2 = i - other.i;
		return fixed(i2);
	}

	__pure constexpr fixed operator *(const fixed(other)) const
	{
		const auto i2 = i * other.i;
		const T i3 = i2 >> fractionalBits;
		return fixed(i3);
	}

};

namespace fixed88
{
	constexpr uint16 operator"" _fix16(const long double val)
	__enableIf(val >= .0L && val <= 255.0L, "Enabled if the constant can be represented in 8:8 format.") 
	{
		return static_cast<uint16>((val < .0L ? -val : val) * 256.0L);
	}
	//fixed point 8:8 format is decently accurate for small numbers
}
