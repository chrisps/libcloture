#pragma once

#define NO_INT128

#if defined(__i386__)
	#include <immintrin.h>

#elif defined(__x86_64__)
	#include <immintrin.h>
#elif defined(__ARM_ARCH)
	#include "arm_neon.h"
#endif

#if defined(__i386__) || defined(__x86_64__)
	#if defined(__SSE4A__)
		#include <ammintrin.h>
	#endif
#endif

__if_not_exists(ptrdiff_t)
{
	//needed for some files that don't include quakedef.h
	#include "stddef.h"

}

#include <cassert>

/**
 	 Cloture STL headers.
*/

/*
	common compiler extensions
	mostly wraps compiler attributes like __declspec and __attribute__
*/
#include	"../compiler_features.hpp"

#include	"codecontext.hpp"
/*
	basic types	(uint8, int8, int32, int64, real32, real64, etc)
*/
#include	"basic_types.hpp"
#include	"libMeta/metaApi.hpp"




//#include "../qtypes.h"
/*
	wrappers for compiler type-checking extensions
	provides macros for type checking. minimal use of templates.
	most compiler-specific extensions used can be easily replaced with templates
	if needed
*/
#include	"generic.hpp"
#include	"classid.hpp"

#include	"casts.hpp"

/**
 * helper templates for use with variadic templates
 */
#include	"va_template.hpp"

/**
 * helper templates for passing functions as template arguments
 */
#include	"functions.hpp"
/*
	common routines
*/
#include	"common.hpp"


/*
	templates/macros for implementing custom type traits that can be used
	for basic compile-time reflection.
	Verry basic reflection.//
*/

#include	"reflect.hpp"


/*
	CTFE stuff for compile-time tricks
*/
#include	"ctfe.hpp"
#include	"extended_types.hpp"

template<int line, const char* filename, const char* functionName>
class __sourcePositionStringBuilder
{
	static constexpr size_t filenameLength 	= cloture::util::ctfe::cstrlen(filename);
	static constexpr size_t functionLength 	= cloture::util::ctfe::cstrlen(functionName);
	static constexpr auto lineText 			= cloture::util::ctfe::parser::toString<line>;
	static constexpr size_t lineInfoLength	= lineText.size();

	static constexpr auto build()
	{
		char temp[4096] = {};

		//for(size_t i = 0; i < 4096; ++i)
		//	temp[i] = 0;
		size_t position = 0;
		{
			const char* msg = "at line ";
			for(size_t i = 0; msg[i]; ++i, ++position)
				temp[position] = msg[i];
		}
		for(size_t i = 0; lineText[i]; ++i, ++position)
			temp[position] = lineText[i];

		{
			const char* msg = " in file ";
			for(size_t i = 0; msg[i]; ++i, ++position)
				temp[position] = msg[i];
		}
		for(size_t i = 0; filename[i]; ++i, ++position)
			temp[position] = filename[i];

		{
			const char* msg = " in function ";
			for(size_t i = 0; msg[i]; ++i, ++position)
				temp[position] = msg[i];
		}
		for(size_t i = 0; functionName[i]; ++i, ++position)
			temp[position] = functionName[i];
		return temp;
	}

public:
	static constexpr auto sourceInfo = build();
};



#include	"typenames.hpp"


//#include	"simd_generic.hpp"
/*
	generic integer bitwise operations
	these are wrapped because some architectures provide their own
	bitop instructions.
	such as x86 its powerful bittest instructions and test-and-complement instructions
*/
//#include	"bitops.hpp"

/*	miscellaneous floating point tricks	*/
//#include	"fpOps.hpp"

/*	cmath replacement. mostly inline wrappers around __builtin versions	*/
#include	"math.hpp"

/*	wrapper for architecture's simd operations	*/
//#include	"vector.hpp"



/*
 * input-output streams
 */
#include	"io.hpp"

#include 	"endian.hpp"
/*
 * simple wrapper around a c-style string
 */
#include	"CString.hpp"

#include	"pointer.hpp"
//#include	"libSTL/routines.hpp"

#include	"libAsm/gasasm.hpp"

#include "Vector_t.h"
//so lazy
#include <pthread.h>
