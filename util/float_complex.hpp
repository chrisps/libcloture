#pragma once
/**
 * 	wrapper class for imaginary number types
 */
 #define		COMPLEX_METHOD	constexpr __pure

template<typename T>
class complex
{
	_Complex T value;

public:
	COMPLEX_METHOD complex(const _Complex T toSet) : value(toSet)
	{}

	COMPLEX_METHOD auto operator *(const auto f) const
	{
		return value * f;
	}
	COMPLEX_METHOD auto operator /(const auto f) const
	{
		return value / f;
	}
	COMPLEX_METHOD auto operator +(const auto f) const
	{
		return value + f;
	}
	COMPLEX_METHOD auto operator -(const auto f) const
	{
		return value - f;
	}
	COMPLEX_METHOD auto operator -() const
	{
		return -value;
	}
};



