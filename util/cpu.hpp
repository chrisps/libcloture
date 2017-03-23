#pragma once

#if defined(__x86_64__) || defined(__i386__)
	#include <cpuid.h>
#endif

#if defined(__i386__) || defined(__x86_64__)
namespace cloture::util::cpu::x86
{
	struct cpufeatures_t
	{
		bool	hasPopcnt;
		
		bool	hasRdrand;
		
		bool	hasSSE3;
		bool	hasMMXExtensions;
		bool	has3DNow;
		bool	has3DNow2;
		bool	hasSSE4A;
	};

	static bool getFeatures(cpufeatures_t * const features)
	{
		unsigned int __eax, __ebx, __ecx, __edx;
		
		if(!__get_cpuid(1, &__eax, &__ebx, &__ecx, &__edx))
			return false;
		
		features->hasPopcnt 		=	(__ecx & bit_POPCNT)	!= 0;
		features->hasRdrand			=	(__ecx & bit_RDRND)		!= 0;
		features->hasSSE3			=	(__ecx & bit_SSE3)		!= 0;
		if(!__get_cpuid(0x80000001, &__eax, &__ebx, &__ecx, &__edx))
			return false;		
		features->hasMMXExtensions	=	(__edx & (1 << 22))		!= 0;
		features->has3DNow			=	(__edx & (1 << 30))		!= 0;
		features->has3DNow2			=	(__edx & (1 << 31))		!= 0;
		features->hasSSE4A			=	(__ecx & (1 << 6))		!= 0;
		return true;
	}

}
#elif defined(__ARM_ARCH)

namespace cloture::util::cpu::arm
{
	
}

#else

#error Unrecognized cpu architecture.

#endif
