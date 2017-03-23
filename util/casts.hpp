#pragma once


#define		castChecking	1

namespace cloture	{
namespace util		{
namespace templates	{
namespace casts		{


template<typename To, typename From>
static constexpr To safe_cast(const From from)
{
	To to = static_cast<To>(from);
	#if castChecking
	assert(static_cast<From>(to) == from);
	#else

	#endif
	return to;
}

}//namespace casts
}//namespace templates
}//namespace util
}//namespace cloture
