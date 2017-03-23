#pragma once

//#include "generic.hpp"

/**
 	 IS_FLOAT_ZERO is intended for use with non-local float variables.
 	 The float/double should be somewhere in memory, not a local variable.
 	 Otherwise significant performance drops will occur, since the optimizer
 	 will have to keep the float values on the stack instead of keeping
 	 them in registers
*/
#define		IS_FLOAT_ZERO(f)						\
({													\
	static_assert(cloture::util::generic::isPointer<typeof(f)>(),\
	"IS_FLOAT_ZERO takes a POINTER to a "			\
	"float as an operand. Input was not a pointer."	\
	);												\
	static_assert(cloture::util::generic::isRealNumber<typeof(*f)>(),\
	"IS_FLOAT_ZERO was provided a pointer to an "	\
	"object that is neither float nor double.");	\
	!(choose_expr									\
	(												\
			sizeof(*f) == 4,						\
			*reinterpret_cast<unsigned int*>(f),	\
			*reinterpret_cast<unsigned __int64*>(f)	\
	));												\
})
