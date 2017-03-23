#pragma once

/*
 * even though the compiler can determine the result of calling standard math functions with constant
 * arguments at compile time it is invalid to do that in constexpr functions,
 * as those functions are not declared constexpr
 */

namespace cloture::util::ctfe::math
{

	template<typename T>
	static constexpr bool isNaN(const T n)
	{
		return n != n;
	}

	template<typename T>
	static constexpr bool isFinite(const T n)
	{
		return common::minValue<T> <= n && n <= common::maxValue<T>;
	}

	template<typename T> struct fpRaw{};

	template<> struct fpRaw< common::real32 >
	{
		using type = common::int32;
	};

	template<> struct fpRaw< common::real64 >
	{
		using type = common::int64;
	};

	template<typename T>
	static constexpr typename fpRaw<T>::type rawBits(const T n)
	{
		static_assert(
		_Generic(n,
				common::real32:
					true,
				common::real64:
					true,
				const common::real32:
					true,
				const common::real64:
					true,
				default:
					false
				),
				"Invalid type provided to util::ctfe::math::rawBits.");
		return -1;
	}

	//from http://www.merlyn.demon.co.uk/js-exact.htm#IEEE
	template<>
	static constexpr int rawBits<common::real32>(const common::real32 n)
	{
		if (isNaN(n))
			return 0x7F800001;

		common::uint32 sign = 0;
		common::real32 n2 	= n;

		if (sign = ((n + 1.0f/n) < .0f) << 31)
			n2 = -n2;

		if (!n2)
			return sign;

		if (!isFinite(n2))
			return sign | 0x7F800000;

		n2 = static_cast<common::real64>(n2) * (1.0 + 2.9802322387695312e-8);
		common::uint32 exponent = 127;

		while (exponent < 254 && n2 >= 2.0f)
		{
			exponent++;
			n2 /= 2.0f;
		}
		if (n2 >= 2.0f)
			return sign | 0x7F800000;
		while (exponent > .0f && n2 <  1.0f)
		{
			exponent--;
			n2 *= 2.0f;
		}
		exponent ? n2-- : n2 /= 2.0f;
		const common::uint32 mantissa =
				static_cast<common::uint32>( n2 * static_cast<common::real32>(0x00800000) );
		return sign | exponent << 23 | mantissa;
	}

	static_assert(
			rawBits<common::real32>(1.0f) == 0x3f800000,
			"rawBits<common::real32> is incorrect."
			);

	/*
	 * based on rawBits<float>. All that was needed was to change the constants
	 * to work with double
	 */
	template<>
	static constexpr common::int64 rawBits<common::real64>(const common::real64 n)
	{
		if (isNaN(n))
			return 0x7FF0000000000001LL;
		common::uint64 sign = 0ULL;
		common::real64 n2 = n;

		if (sign = static_cast<common::uint64>((n + 1.0/n) < .0) << 63ULL)
			n2 = -n2;

		if (!n2)
			return sign;

		if (!isFinite(n2))
			return sign | 0x7FF0000000000000LL;

		common::int64 exponent = 1023ULL;

		while (exponent < 2046 && n2 >= 2.0)
		{
			exponent++;
			n2 /= 2.0;
		}
		if (n2 >= 2.0)
			return sign | 0x7FF0000000000000LL;
		while (exponent > 0LL && n2 < 1.0)
		{
			exponent--;
			n2 *= 2.0;
		}
		exponent ? n2-- : n2 /= 2.0;
		const common::uint64 mantissa =
				static_cast<common::uint64>(
				n2 * static_cast<common::real64>(1LL << 52LL)
				);
		return sign | exponent << 52LL | mantissa;
	}

	static_assert(
	rawBits<common::real64>(3.14159265358979323846) == 0x400921FB54442D18LL,
	"rawBits<common::real64> is fukt?"
	);

	/*
	 * ctfe::math::abs
	 * calculates the absolute value of x
	 * for integral and floating point types.
	 * there is no fabs
	 */
	template<typename T>
	static constexpr T abs(const T x)
	{
		constexpr T ZERO = static_cast<T>(0);
		if(x < ZERO)
			return -x;
		return x;
	}

	/**
	 * ctfe::math::sqrt
	 * calculates the square root of x (dumbass)
	 */
	template<typename T>
	static constexpr T sqrt(const T x)
	{
		constexpr T TWO 	= static_cast<T>(2);
		constexpr T ZERO 	= static_cast<T>(0);
		T result = x / TWO;
		if(result < ZERO)
			return ZERO;
		constexpr T babylonianConstant =
		_Generic(x,
			common::real64:
				0.000000000000001,
			const common::real64:
				0.000000000000001,
			common::real32:
				0.0001,
			const common::real32:
				0.0001
				);
		#define	__SQRT_LOOP_CONDITION()	static_cast<T>(\
		abs( result - (x / result)) / result	\
) > babylonianConstant

		#define		__SQRT_ONE_ITERATION()	\
			if(!(__SQRT_LOOP_CONDITION())) break; else result = (result + (x / result)) / TWO

		while(	__SQRT_LOOP_CONDITION()	)
		{
			result = (result + (x / result)) / TWO;
			__SQRT_ONE_ITERATION();
			__SQRT_ONE_ITERATION();
		}
		return result;
	}

	/**
	 * regression tests for sqrt
	 */
	static_assert(
	sqrt<double>(536.0) == 23.15167380558045
	&&
	sqrt<double>(8000.0 * 1336.0) == 3269.250678672408
	&&
	sqrt<double>(1.79769e+308) == 1.3407796239501852e+154);

	/**
	 * 	ctfe::math::hypot
	 * 	calculates hypotenuse of a right-angled triangle with legs x and y
	 */
	template<typename T>
	static constexpr T hypot(const T x, const T y)
	{
		return sqrt<T>(x*x+y*y);
	}

	static_assert(
	hypot<double>(50.0, 119.0) == 129.07749610214788);




	/*
	 * cbrt
	 */
	template<typename T>
	static constexpr T cbrt(const T x)
	{
		constexpr T FOUR = static_cast<T>(4);
		constexpr T THREE = static_cast<T>(3);
		T result = x / FOUR;

		#define __CBRT_LOOP_CONDITION()		\
			abs(result - (x / result / result)) / result > 0.00000000000001

		while(__CBRT_LOOP_CONDITION())
		{
			result = (result + (x / result / result) + result) / THREE;
		}
		return result;
	}

	/*
	 * pow
	 */
	template<typename T>
	static constexpr T pow(const T base, const T exponent)
	{
		using iType = common::int32;

		constexpr T ZERO 	= static_cast<T>(0);
		constexpr T ONE 	= static_cast<T>(1);
		//need the absolute value of the exponent
		const T absExponent = abs(exponent);


		//right now, only integer exponent is supported
		iType i = static_cast<iType>(absExponent) - 1;

		//base^0 = 1
		if(!i)
			return ONE;

		T result = base;
		while(i)
		{
			result = result * base;
			i--;
		}
		//return the inverse of base^absExponent if exponent was negative
		//this accurately handles negative exponents
		return (	exponent < ZERO		)
		? ONE / result
		: result;
	}

	static_assert(pow<double>(8.5, 6.0) == 377149.515625);

	template<typename T>
	static constexpr T fromRaw(typename fpRaw<T>::type raw)
	{
	}

	template<>
	static constexpr common::real32 fromRaw<common::real32>(common::int32 raw)
	{
		using common::int32;
		using common::uint32;
		using common::real32;
		uint32 sign		= (raw >> 31) & 1;
		int32 exponent	= (raw >> 23) & 0xFF;

		int32 mantissa 	= raw & 0x007FFFFF;
		//they check for an invalid exponent, but we cant return NaN
		if(exponent != 0)
		{
			exponent -= 127;
			mantissa |= 0x00800000;
		}
		else
			exponent = -126;

		const real32 f = static_cast<real32>(mantissa) *
		pow<real32>(2.0f, static_cast<real32>(exponent - 23));
		return sign != 0 ? -f : f;
	}
	static_assert(fromRaw<float>(1062668861) == 0.84f);


	/*
	 * floor
	 */
	template<typename T>
	static constexpr T floor(const T x)
	{
		constexpr T POINTNINE = static_cast<T>(0.9999999999999999);
		constexpr T ZERO = static_cast<T>(0);
		if(x > ZERO)
			return static_cast<common::int32>(x);
		return static_cast<common::int32>(x - POINTNINE);
	}

	/*
	 * ceil
	 */
	template<typename T>
	static constexpr T ceil(const T x)
	{
		using iType = common::int64;

		constexpr T ZERO	= static_cast<T>(0);
		constexpr T ONE		= static_cast<T>(1);

		const iType xInt = static_cast<iType>(x);
		if(x < ZERO)
			return static_cast<T>(xInt);
		return static_cast<T>(xInt + ONE);
	}

	template<typename T>
	static constexpr T fmod(const T a, const T b)
	{
		constexpr T POINTFIVE = static_cast<T>(0.5);
		using iType = common::int32;

		const T aDivB 			= a / b;
		const iType aDivBInt 	= static_cast<iType>(aDivB);

		const T diffOfDivs 	= aDivB - aDivBInt;
		const iType iResult	= static_cast<iType>( (diffOfDivs * b) + POINTFIVE);
		return static_cast<T>(iResult);
	}

	template<typename T>
	static constexpr T fmax(const T a, const T b)
	{
		return a > b ? a : b;
	}

	template<typename T>
	static constexpr T fmin(const T a, const T b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	static constexpr T round(const T a)
	{
		using iType = common::int32;
		constexpr T POINTFIVE 	= static_cast<T>(0.5);
		const T pluspointfive 	= a + POINTFIVE;
		const iType asInt 		= static_cast<iType>(pluspointfive);

		return static_cast<T>(asInt);
	}

	template<typename T>
	static constexpr int trunc(const T a)
	{
		using iType = common::int32;
		const iType truncated = static_cast<iType>(a);
		return truncated;
	}

	template<typename T>
	static constexpr T nearbyint(const T arg)
	{
		return round(arg);
	}

	template<typename T>
	static constexpr T exp2(const T exp)
	{
		using iType = common::int32;
		constexpr T ONE = static_cast<T>(1);
		constexpr T TWO = static_cast<T>(2);
		T value = ONE;

		const iType iExp = static_cast<iType>(exp);

		for(iType i = 1; i <= iExp; i++)
			value *= TWO;
		return value;
	}

	template<typename T>
	static constexpr T fma(const T a, const T b, const T c)
	{
		using iType = common::int32;
		constexpr T POINTFIVE 	= static_cast<T>(0.5);
		const T aMulB 			= a * b;
		const T added 			= aMulB + c + POINTFIVE;
		const iType iResult 	= static_cast<iType>(added);
		return static_cast<T>(iResult);
	}

	template<typename T>
	static constexpr T ldexp(const T num, const common::int32 exp)
	{
		return num * exp2(exp);
	}
	/*
		generates a bit pattern that, when added to or subtracted from a raw float
		yields that float times/divided by multiplier
		
		it's very inaccurate for large numbers. 
		in some scenarios it might be accurate enough. best used with NEON/SSE.
	*/
	static constexpr common::int32 generateFMulMask(const float multiplier)
	{
		return rawBits(multiplier*multiplier) - rawBits(multiplier);
	}

}//namespace cloture::util::ctfe::math
