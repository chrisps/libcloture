#pragma once

namespace cloture	{
namespace util		{
namespace common	{

#if !defined(NO_INT128_IMPL)
	static_assert(
	0x7ffeffffffffffffffffffffffffffff_u128 ==
	((static_cast<uint128>(0x7ffeffffffffffffULL) << 64)
	|
	static_cast<uint128>(0xffffffffffffffffULL)),
	"hexadecimal int128/uint128 literals are fuckt"
	);
#endif
	//fixed point types
	//#include	"fixed.hpp"

	//half-precision float type
	//#include	"half.hpp"

	//quad-precision float type
	//#include	"quad.hpp"

}//namespace common
}//namespace util
}//namespace cloture
