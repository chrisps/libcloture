#pragma once

#if defined(__MMX__) && __MMX__

#if !defined(_MM3DNOW_H_INCLUDED) && defined(__3dNOW__) && __3dNOW__
	#include <mm3dnow.h>
	#define _m_from_float(f)		_mm_cvtsi32_si64(*reinterpret_cast<const unsigned int*>(&f))
#endif


#define		MMX_INLINE	__forceinline
#define		MMX_PURE	__pure

#define		__AMDONLY	__attribute__((enable_if(codepath == USE_3DNOW, "only for 3DNow codepaths.")))	

#define		MMX_OP		MMX_INLINE MMX_PURE
#define		AMD_OP		__AMDONLY MMX_OP

static constexpr bool USE_3DNOW		=	true;
static constexpr bool NO_3DNOW		=	false;

namespace cloture//::util::math::mmx
{
namespace util
{
namespace math
{
namespace mmx
{

	/**
		mmx variables should only be used in code sequences where xmm (vector)
		variables are not used. moving data between xmm and mmx registers is probably slow
		
		they should also not be passed as parameters to non-inline functions
		on x86-64 mmx variables get passed in the general purpose register (slow)
		
		this is problematic because passing mmx registers to general-purpose registers
		is extremely slow
		
		the calling convention for x86-64 also does not preserve mmx registers between calls 
		some compilers may attempt to preserve them but the implementation should be assumed to be half-assed
		
		so mmx registers are intended specifically for functions that perform a lot of arithmetic 
		and call no other functions
		
		since mmx registers have no direct ARM equivalent all functions using them should be ifdef'd 
		and alternative versions of those functions should exist
		
		this is not a problem with SSE since ARM has an SSE equivalent - the NEON instruction set
		
		if used as outlined above MMX registers can bring pretty neat performance gains.
		their use drastically reduces register pressure
		
	*/
	#if 0
	template<bool codepath> struct mmx
	{
		__m64 m;
		
		using mmType = mmx<codepath>;
		
		MMX_INLINE mmx() : m{}
		{}
		
		MMX_INLINE mmx(const __m64 other) : m(other)
		{}
		
		MMX_INLINE mmx(const unsigned int v1, const unsigned int v2) : m{}
		{
			set(v1, v2);
		}
		
		MMX_INLINE mmx(const int v1, const int v2) : m{}
		{
			set(v1, v2);
		}
		
		MMX_INLINE mmx(const short v1, const short v2, const short v3, const short v4) : m{}
		{
			set(v1, v2, v3, v4);
		}		
		
		MMX_INLINE mmx(const unsigned short v1, const unsigned short v2, const unsigned short v3, const unsigned short v4) : m{}
		{
			set(v1, v2, v3, v4);
		}			
		
		MMX_INLINE mmx(const char v1, const char v2, const char v3, const char v4, const char v5, const char v6, const char v7, const char v8) : m{}
		{
			set(v1, v2, v3, v4, v5, v6, v7, v8);
		}		
		
		MMX_INLINE mmx(const unsigned char v1, const unsigned char v2, const unsigned char v3, const unsigned char v4, const unsigned char v5, const unsigned char v6, const unsigned char v7, const unsigned char v8) : m{}
		{
			set(v1, v2, v3, v4, v5, v6, v7, v8);
		}				
	


		/**
			bitwise and - full 64 bits
		*/
		MMX_OP mmType operator &(mmType other) const
		{
			return _mm_and_si64(m, other.m);
		} 
		/**
			bitwise and with assignment
		*/
		MMX_INLINE mmType operator &=(mmType other) const
		{
			return m = _mm_and_si64(m, other.m);
		}
		
		/**
			bitwise or - full 64 bits
		*/
		MMX_OP mmType operator |(mmType other) const
		{
			return _mm_or_si64(m, other.m);
		} 
		/**
			bitwise or with assignment
		*/
		MMX_INLINE mmType operator |=(mmType other) const
		{
			return m = _mm_or_si64(m, other.m);
		}		
		
		/**
			bitwise xor - full 64 bits
		*/
		MMX_OP mmType operator ^(mmType other) const
		{
			return _mm_xor_si64(m, other.m);
		} 
		/**
			bitwise xor with assignment
		*/
		MMX_INLINE mmType operator ^=(mmType other) const
		{
			return m = _mm_xor_si64(m, other.m);
		}				
		
		MMX_OP operator __m64() const
		{
			return m;
		}
		/**
			returns the absolute value of the two floating point
			values in the register
		*/
		MMX_OP mmType fabsf() const
		{
			return _mm_and_si64(m, _mm_set1_pi32(0x7FFFFFFF));
		}
		
		/**
			returns the lower 32 bits of the register
		*/
		MMX_OP int lower() const
		{
			return _mm_cvtsi64_si32(m);
		}
		/**
			sets the full 64 bit value to zero
		*/
		MMX_INLINE void clear()
		{
			m = _mm_setzero_si64();
		}
		
		template<typename... v>
		MMX_INLINE mmType set(v... vals)
		{
			return m = mmType();
		}
		
		template<>
		MMX_INLINE mmType set<int, int>(const int v1, const int v2)
		{
			return m = _mm_set_pi32(v2, v1);//v1, v2);
		}

		template<>
		MMX_INLINE mmType set<unsigned int, unsigned int>(const unsigned int v1, const unsigned int v2)
		{
			return m = _mm_set_pi32(v2, v1);//v1, v2);
		}

		template<>
		MMX_INLINE mmType set<short, short, short, short>(const short v1, const short v2, const short v3, const short v4)
		{
			return m = _mm_set_pi16 (v4, v3, v2, v1);//v1, v2, v3, v4);
		}
		
		template<>
		MMX_INLINE mmType set<unsigned short, unsigned short, unsigned short, unsigned short>(
		const unsigned short v1,
		const unsigned short v2,
		const unsigned short v3,
		const unsigned short v4
		)
		{
			return m = _mm_set_pi16 (v4, v3, v2, v1);//v1, v2, v3, v4);
		}
		
		template<>
		MMX_INLINE mmType set<unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char>(
		const unsigned char v1,
		const unsigned char v2,
		const unsigned char v3,
		const unsigned char v4,
		const unsigned char v5,
		const unsigned char v6,
		const unsigned char v7,
		const unsigned char v8
		)
		{
			return m = _mm_set_pi8 (v8, v7, v6, v5, v4, v3, v2, v1);//v1, v2, v3, v4);
		}

		template<>
		MMX_INLINE mmType set<char, char, char, char, char, char, char, char>(
		const char v1,
		const char v2,
		const char v3,
		const char v4,
		const char v5,
		const char v6,
		const char v7,
		const char v8
		)
		{
			return m = _mm_set_pi8 (v8, v7, v6, v5, v4, v3, v2, v1);//v1, v2, v3, v4);
		}


		/**
			sets all elements to value
		*/
		template<typename T>
		MMX_INLINE mmType setAll(const T value)
		{

		}
		template<>
		MMX_INLINE mmType setAll<int>(const int value)
		{
			return m = _mm_set1_pi32(value);
		}

		template<>
		MMX_INLINE mmType setAll<unsigned int>(const unsigned int value)
		{
			return m = _mm_set1_pi32(value);
		}

		template<>
		MMX_INLINE mmType setAll<short>(const short value)
		{
			return m = _mm_set1_pi16(value);
		}

		template<>
		MMX_INLINE mmType setAll<unsigned short>(const unsigned short value)
		{
			return m = _mm_set1_pi16(value);
		}

		template<>
		MMX_INLINE mmType setAll<char>(const char value)
		{
			return m = _mm_set1_pi8(value);
		}

		template<>
		MMX_INLINE mmType setAll<unsigned char>(const unsigned char value)
		{
			return m = _mm_set1_pi8(value);
		}

		/*
				-------------		ALL CODE USING 3DNOW BEGINS HERE		-------------------
				-------------		ALL CODE USING 3DNOW BEGINS HERE		-------------------
				-------------		ALL CODE USING 3DNOW BEGINS HERE		-------------------
		*/
		
		static MMX_INLINE void EMMS()
		{
		choose_expr
		(
			codepath == USE_3DNOW,
			_m_femms(),
			_mm_empty()
		);
		}
		/**
			swaps the upper and lower halves of the register
		*/
		AMD_OP mmType swap32() const
		{
			return _m_pswapdsf(m);
		}
		
		template<>
		MMX_INLINE __AMDONLY mmType set<float, float>(const float v1, const float v2)
		{
			return m =_mm_or_si64( _m_pswapdsf(_m_from_float(v2)), _m_from_float(v2));
		}
		
	};
	
	/**
		typed mmx register - holds two signed integers
	*/
	template<bool codepath> struct mmx2si : public mmx<codepath>
	{
		using mmType	=	mmx2si<codepath>;
		using baseType	=	mmx<codepath>;
		
		
		/**
			loads a 32 bit value into the register, and ZERO EXTENDS 
			the value to 64 bits
		*/
		MMX_INLINE mmType operator =(const int toExtend) 
		{
			return this->m = _mm_cvtsi32_si64(toExtend);
		}
		
		MMX_OP mmType operator +(const int v) const
		{
			return _mm_add_pi32(this->m, _mm_set1_pi32(v));
		}
		MMX_OP mmType operator -(const int v) const
		{
			return _mm_sub_pi32(this->m, _mm_set1_pi32(v));
		}
		
		MMX_INLINE mmType operator +=(const int v)
		{
			return this->m = _mm_add_pi32(this->m, _mm_set1_pi32(v));
		}
		
		MMX_INLINE mmType operator -=(const int v)
		{
			return this->m = _mm_sub_pi32(this->m, _mm_set1_pi32(v));
		}		

	};
	/**
		typed mmx register - holds two unsigned integers
	*/
	template<bool codepath> struct mmx2ui : public mmx<codepath>
	{
		using mmType	=	mmx2ui<codepath>;
		using baseType	=	mmx<codepath>;
		
		
		/**
			loads a 32 bit value into the register, and ZERO EXTENDS 
			the value to 64 bits
		*/
		MMX_INLINE mmType operator =(const unsigned int toExtend) 
		{
			return this->m = _mm_cvtsi32_si64(toExtend);
		}
		
		MMX_OP mmType operator +(const unsigned int v) const
		{
			return _mm_add_pi32(this->m, _mm_set1_pi32(v));
		}
		MMX_OP mmType operator -(const unsigned int v) const
		{
			return _mm_sub_pi32(this->m, _mm_set1_pi32(v));
		}
		
		MMX_INLINE mmType operator +=(const unsigned int v)
		{
			return this->m = _mm_add_pi32(this->m, _mm_set1_pi32(v));
		}
		
		MMX_INLINE mmType operator -=(const unsigned int v)
		{
			return this->m = _mm_sub_pi32(this->m, _mm_set1_pi32(v));
		}	
		
		MMX_OP mmType operator >>(const unsigned int factor) const
		{
			return _mm_srli_pi32(this->m, factor);
		}	
		MMX_INLINE mmType operator >>=(const unsigned int factor) 
		{
			return this->m = _mm_srli_pi32(this->m, factor);
		}
		
		MMX_OP mmType operator <<(const unsigned int factor) const
		{
			return _mm_slli_pi32(this->m, factor);
		}	
		
		MMX_INLINE mmType operator <<=(const unsigned int factor) 
		{
			return this->m = _mm_slli_pi32(this->m, factor);
		}
		
	};
	#endif //#if 0
}
}
}
}

namespace cloture//::util::math
{
namespace util
{
namespace math
{
}
}
}

#endif	//#if defined(__MMX__) && __MMX__
