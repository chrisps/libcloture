#pragma once

template<typename T>
static constexpr auto phatDiv(
	const T& numerator, 
	const T& denominator, 
	T& quotient,
	T& remainder
)
{
	constexpr size_t bitSizeof = sizeof(T)*8;
	constexpr T ONE = static_cast<T>(1);
	constexpr T ZERO = static_cast<T>(0);
	
	constexpr T DENOMSHIFT = static_cast<T>(bitSizeof) - ONE;
	if(!denominator)
		return false;
	
	T n = numerator;
	T d = denominator;
	T x = ONE;
	T result = ZERO;


	while(n >= d && !((d >> DENOMSHIFT) & ONE)) 
	{
		x = x << ONE;
		d = d << ONE;
	}

	while(x != ZERO) 
	{
		if(n >= d) 
		{
			n 		= n - d;
			result 	= result | x;
		}

		x 	= x >> ONE;
		d 	= d >> ONE;
	}

	quotient 	= result;
	remainder 	= n;
	return true;
}

template<typename T, typename THigh, bool isSigned = false>
struct phatInt
{
	using baseType 	= T;
	using phType 	= phatInt<T, THigh, isSigned>;
	baseType low;
	//baseType high;
	THigh high;
	static constexpr size_t bitSizeof = (sizeof(low) + sizeof(high)) * 8;
	static constexpr T ZERO = static_cast<T>(0);

	constexpr phatInt() : low(ZERO), high(ZERO)
	{}

	constexpr phatInt(baseType low_, baseType high_) : low(low_), high(high_)
	{}

	constexpr phatInt(unsigned int s) : low(s), high(ZERO)
	{}

	constexpr phatInt(signed int s) : low(s), high(-1)
	{}
	
	static constexpr phType PHATZERO = phType(ZERO, ZERO);

	constexpr phType operator =(const phType other)
	{
		low 	= other.low;
		high 	= other.high;
		return *this;
	}

	constexpr bool operator ==(const phType other) const
	{
		return low == other.low && high == other.high;
	}

	constexpr bool operator !=(const phType other) const
	{
		return low != other.low || high != other.high;
	}

	constexpr operator bool() const
	{
		return low != ZERO || high != ZERO;
	}

	constexpr bool operator !() const
	{
		return low == ZERO && high == ZERO;
	}
	
	constexpr bool operator <(const phType other) const
	{
		return (high == other.high) ? low < other.low : high < other.high;
	}
	
	constexpr bool operator <=(const phType other) const
	{
		return 
		((high == other.high) ? low < other.low : high < other.high)
		|| (high == other.high && low == other.low);
	}
	
	constexpr bool operator >(const phType other) const
	{
		return !((*this) <= other);
	}
	
	constexpr bool operator >=(const phType other) const
	{
		return !((*this) < other);
	}

	constexpr phType operator ++()
	{
		if(++low == ZERO)
			++high;
		return *this;
	}

	constexpr phType operator --()
	{
		if(--low == ZERO)
			--high;
		return *this;
	}

	constexpr phType operator ~() const
	{
		return phType(~low, ~high);
	}

	constexpr phType operator +(const phType other) const
	{

		const auto addLow 	= low 	+ other.low;
		const auto addHigh	= high 	+ other.high;
		if(addLow < low)
			++addHigh;

		return phType(addLow, addHigh);
	}

	constexpr phType operator -() const
	{
		return ~phType(*this) + static_cast<phType>(1);
	}

	constexpr phType operator -(const phType other) const
	{
		return (*this) + (-other);
	}

	constexpr phType operator |(const phType other) const
	{
		return phType(low | other.low, high | other.high);
	}
	
	constexpr phType operator &(const phType other) const
	{
		return phType(low & other.low, high & other.high);
	}
	
	constexpr phType operator ^(const phType other) const
	{
		return phType(low ^ other.low, high ^ other.high);
	}
	
	constexpr phType operator <<(const phType other) const
	{
		size_t shiftFactor = static_cast<size_t>(other.low) & 0xFFFFFFFF;
		if(shiftFactor >= bitSizeof)
			return PHATZERO;
			
		phType result = *this;
		
		const size_t halfBitSize = bitSizeof >> 1;
		
		if(shiftFactor >= halfBitSize)
		{
			shiftFactor -= halfBitSize;
			result.high = result.low;
			result.low = ZERO;
		}
		if(!shiftFactor)
			return result;
		
		result.high = result.high << shiftFactor;
		const baseType mask = ~(static_cast<baseType>(-1) >> shiftFactor);
		result.high = result.high | ((result.low & mask)  >> (halfBitSize - shiftFactor));
		result.low = result.low << shiftFactor;
		return result;
	}
	
	constexpr phType operator >>(const phType other) const
	{
		size_t shiftFactor = static_cast<size_t>(other.low) & 0xFFFFFFFF;
		if(shiftFactor >= bitSizeof)
			return PHATZERO;
			
		phType result = *this;
		
		const size_t halfBitSize = bitSizeof >> 1;
		
		if(shiftFactor >= halfBitSize)
		{
			shiftFactor -= halfBitSize;
			result.low = result.high;
			result.high = ZERO;
		}
		if(!shiftFactor)
			return result;
		
		result.low = result.low >> shiftFactor;
		const baseType mask = ~(static_cast<baseType>(-1) << shiftFactor);
		result.low = result.low | ((result.high & mask)  << (halfBitSize - shiftFactor));
		result.high = result.high >> shiftFactor;
		return result;
	}
	
	constexpr phType operator *(const phType other) const
	{
		if(!other)
			return phType(ZERO, ZERO);
		if(other.high == ZERO && other.low == static_cast<baseType>(1))
			return *this;

		phType temp = other;
		
		phType result = PHATZERO;
		
		
		constexpr phType lowBit = 1u;
		
		for(size_t i = 0; i < bitSizeof; ++i)
		{
			if( (temp & lowBit) != PHATZERO)
				result = result + (*this << i);
			
			temp = temp >> 1;
		}
		
		return result;

	}
	
	constexpr phType operator /(const phType other) const
	{
		phType rem = ZERO;
		phType temp = *this;
		phatDiv(temp, other, temp, rem);
		return temp;
	}
	constexpr phType operator %(const phType other) const
	{
		phType quo = ZERO;
		phType temp = *this;
		phatDiv(temp, other, quo, temp);
		return temp;
	}
};

using int256	= phatInt<unsigned __int128, __int128, true>;
using uint256	= phatInt<unsigned __int128, unsigned __int128>;

using int512	= phatInt<uint256, int256, true>;
using uint512	= phatInt<uint256, uint256, true>;
