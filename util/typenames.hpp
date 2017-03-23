#pragma once

#if 0
namespace cloture	{
namespace util		{
namespace typenames	{

static constexpr auto unknownTypename = constantCString("unknown");

template<typename T>
static constexpr auto defaultGetTypename()
	-> decltype(T::clotureTypeName())
{
	return T::clotureTypeName();
}

template<typename T>
static constexpr auto defaultGetTypename()
{

	return unknownTypename;
}

template<typename T>
static constexpr auto typeName = defaultGetTypename<T>();

#define		__decltypeName(T)		\
template<> static constexpr auto typeName<T> = constantCString(#T)

using namespace cloture::util::common;

__decltypeName(int8);
__decltypeName(uint8);
__decltypeName(int16);
__decltypeName(uint16);
__decltypeName(int32);
__decltypeName(uint32);
__decltypeName(int64);
__decltypeName(uint64);
#if !defined(NO_INT128_IMPL)
__decltypeName(int128);
__decltypeName(uint128);
#endif
//__decltypeName(real16);
__decltypeName(real32);
__decltypeName(real64);

}//namespace typenames
}//namespace util
}//namespace cloture

#endif