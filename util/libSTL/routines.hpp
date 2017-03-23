#pragma once

namespace cloture::util::routines
{
	namespace general
	{
		static constexpr common::size32 unrollThreshold = 16;
		
		template<typename T, size_t elements>
		__forceinline void zeroArray(T (&mem)[elements])
		__enableIf(
			alignof(T) % 16  == 0 && sizeof(T) % 16 == 0 && (sizeof(T) * elements) / 16 <= unrollThreshold, 
			"Enabled if T is SSE-memsetable and unrollable"
		)
		{
			__m128i val = {0, 0};
			__unrollFull()
			for(unsigned i = 0; i < (sizeof(T) * elements) / 16; ++i)
				reinterpret_cast<__m128i*>(mem)[i] = val;
		}
		
		template<typename T, size_t elements>
		__forceinline void zeroArray(T (&mem)[elements])
		__enableIf(
			alignof(T) % 16 == 0 && sizeof(T) % 16 == 0 && (sizeof(T) * elements) / 16 > unrollThreshold, 
			"Enabled if T is SSE-memsetable and not unrollable"
		)
		{
			__m128i val = {0, 0};
			for(unsigned i = 0; i < (sizeof(T) * elements) / 16; ++i)
				reinterpret_cast<__m128i*>(mem)[i] = val;
		}
			
		template<typename T, size_t elements>
		__forceinline void zeroArray(T (&mem)[elements])
		__enableIf(
		alignof(T) % 16 != 0 || sizeof(T) % 16 != 0,
		"Selected as default."
		)
		{
			__builtin_memset(mem, 0, elements * sizeof(T));
		}	
		template<typename T, size_t elements>
		__forceinline void zeroArray(T (&mem)[elements])
		__enableIf(
		(alignof(T) % 16 != 0 || sizeof(T) % 16 != 0) && sizeof(void*) == 8
		&& alignof(T) % 8 == 0 && sizeof(T) % 8 == 0 && (sizeof(T) * elements) / 8 < unrollThreshold,
		"Selected if platform is 64 bit and we can unroll a 64 bit memset"
		)
		{
			const common::uint64 val = 0ULL;
			__unrollFull()
			for(unsigned i = 0; i < (sizeof(T) * elements) / 8; ++i)
				reinterpret_cast<common::uint64*>(mem)[i] = val;
		}		
		
	}
}
