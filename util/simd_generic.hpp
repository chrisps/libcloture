#pragma once


#if defined(__i386__) || defined(__x86_64__)

	#if		defined(__AVX__) && !defined(__AVX512F__)
		#define		mSimdAlign			32
		#define		mNativeVectorSize	mSimdAlign
	#elif	!defined(__AVX__)
		#define		mSimdAlign			16
		#define		mNativeVectorSize	mSimdAlign
	#elif	defined(__AVX512F__)
		#define		mSimdAlign			64
		#define		mNativeVectorSize	mSimdAlign
	#endif

#elif defined(__ARM_ARCH) && defined(__ARM_NEON__)

	#define 	mSimdAlign			16
	#define		mNativeVectorSize	mSimdAlign

#else

	#define 	mSimdAlign			16
	#define		mNativeVectorSize	mSimdAlign

#endif

#define		mDebugVectorOps		1


constexpr size_t padSizeSimd(size_t insize)
{
	//#if mNativeVectorSize == 16
	while(insize%mSimdAlign != 0) ++insize;
	return insize;
	//#else
	//static_assert(false, "Do something here to make sure that the padding is to the smallest vector size.");
	//#endif
}
