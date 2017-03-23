#pragma once

#define		MATH_FORCEINLINE	0
#define		TESTING_MATH_HAX	1
namespace cloture//::util::math
{
namespace util
{
namespace math
{
	static constexpr common::uint32 fabsMask 	= 0x7FFFFFFF;
	static constexpr common::uint32 fsignMask 	= 0x80000000;
	#if MATH_FORCEINLINE
		#define inline __forceinline
	#endif
	static inline __pure const double fabs(const double x)
	{
		return __builtin_fabs(x);
	}
	
	static inline __pure const float fabsf(const float x)
	{
		return __builtin_fabsf(x);
	}
	
	static inline __pure const double cos(const double x)
	{
		return __builtin_cos(x);
	}
	
	static inline __pure const float cosf(const float x)
	{
		return __builtin_cosf(x);
	}
	
	static inline __pure const double sin(const double x)
	{
		return __builtin_sin(x);
	} 
	
	static inline __pure const float sinf(const float x)
	{
		return __builtin_sinf(x);
	}
	
	static inline __pure const double atan2(const double y, const double x)
	{
		return __builtin_atan2(y, x);
	}
	
	static inline __pure const float atan2f(const float y, const float x)
	{
		return __builtin_atan2f(y, x);
	}
	
	static inline __pure const double round(const double x)
	{
		return __builtin_round(x);
	}
	
	static inline __pure const float roundf(const float x)
	{
		return __builtin_roundf(x);
	}
	
	static inline __pure const double floor(const double x)
	{
		return __builtin_floor(x);
	}
	static inline __pure const float floorf(const float x)
	{
		return __builtin_floorf(x);
	}
	
	static inline __pure const double sqrt(const double x)
	{
		return __builtin_sqrt(x);
	}
	static inline __pure const float sqrtf(const float x)
	{
		return __builtin_sqrtf(x);
	}
	#if MATH_FORCEINLINE
		#undef inline
	#endif
	#if TESTING_MATH_HAX
	#define		SIG_MASK				(0x007FFFFF)
		union unholy
		{
			float f;
			int i;
			unholy(float ff) : f(ff) {}
			unholy(int ii) : i(ii){}
			operator float()
			{ return f; }

			int getMantissa()
			{
				return i & SIG_MASK;
			}
			int setMantissa(int ii)
			{
				return i = (i & (~SIG_MASK)) | (ii & SIG_MASK);
			}
			int getExponent()
			{
				return ((i >> 22) & 0xFF) - 127;
			}
			int setExponent(int exp)
			{
				i &= ~(0xFF << 22);
				return i |= (((exp & 0xFF) + 127) << 22);
			}
			bool getSign()
			{
				return i < 0;
			}
			bool setSign(bool newsign)
			{
				i &= ~(0x80000000);
				i |= (newsign ? 0x80000000 : 0);
				return newsign;
			}


			__declspec(property(get = getMantissa, put = setMantissa)) int mantissa;
			__declspec(property(get = getExponent, put = setExponent)) int exponent;
			__declspec(property(get = getSign, put = setSign)) bool sign;
		};
		constexpr int CBRT_MAGIC_NUM = 0x2a517d86;
		static float cbrt(float number)
		{
			unholy result = number;
			const float third = result.i * .333333333f;
			const float magicNumber = float(CBRT_MAGIC_NUM
				//0x2a517d3c
				);
			result.i = int((magicNumber + third));
			/*
				result tends to vary in precision, so we do newton's method to refine it
			*/

			result.f = .333333333f * ((number / (result.f * result.f)) + 2.0f*result.f);

			/*
				you could do a second run of newton's method if you need the precision
			*/
			return result.f;
		}


	#endif
	#if 0
	static __pure __m128 cbrt(const __m128 numbers)
	{
		const __m128 pointThree		= _mm_set_ps1(.333333333f);
		/*
			  now we calculate the imprecise cube root for later refinement
		*/
		__m128 unrefined 			= _mm_castsi128_ps(_mm_add_epi32(_mm_mul_ps(_mm_castps_si128(numbers), pointThree), _mm_set1_epi32(0x2a517d86)));

		/*
		 * 	newton's method, in parallel
		*/
			const __m128 squared 		= _mm_mul_ps(unrefined, unrefined);
			const __m128 rfrac			= _mm_div_ps(numbers, squared);
			const __m128 twos			= _mm_mul_ps(_mm_set_ps1(2.0f), unrefined);
			const __m128 fracsum		= _mm_add_ps(twos, rfrac);

			const __m128 result			= _mm_mul_ps(pointThree, fracsum);

		return result;
	}
	#endif

	template<typename tMin, typename tNum, typename tMax>
	__pure __forceinline
	static constexpr auto bound(const tMin min, const tNum num, const tMax max)
	{
		return num >= min
		? (num < max ? num : max)
		: min;
	}

	template<typename aType, typename bType>
	static constexpr auto min(const aType A, const bType B)
	-> decltype(A < B ? A : B)
	{
		return A < B ? A : B;
	}

	template<typename aType, typename bType>
	static constexpr auto max(const aType A, const bType B)
	{
		return A > B ? A : B;
	}
}
}
};

