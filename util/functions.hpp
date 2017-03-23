#pragma once
/*
 * a bunch of templates for finding info on function pointers
 */

namespace cloture	{
namespace util		{
namespace templates	{
namespace functions	{
using namespace generic;



template<bool isVariadic_, typename T, typename rType = void, typename... argTypes>
struct funcptrInfo
{

	using type = T;
	using returnType = rType;
	static constexpr size_t numArgs = sizeof...(argTypes);
	static constexpr bool didDeduceType = true;
	static constexpr bool isVariadic = isVariadic_;
	static constexpr bool isDelegate = false;
	T func;
	using strippedType = typeof(*func);
	static_assert(isFunction<strippedType>() || isMethod<strippedType>());

	template<typename TT>
	static constexpr bool callableWith = false;

	template<>
	static constexpr bool callableWith<T> = true;

	constexpr funcptrInfo(T f) : func(f){}

	template<bool vaCheck, typename rT, typename... args>
	struct vaSwitch
	{
		using vtype = rT (*)(args..., ...);
	};
	template<typename rT, typename... args>
	struct vaSwitch<false, rT, args...>
	{
		using vtype = rT (*)(args...);
	};

	template<typename rT, typename... args>
	constexpr bool signaturesMatch(rT tryRtype, args... tryArgType)

	{
		return callableWith<
				typename vaSwitch<isVariadic, rT, args...>::vtype
				>;
	}
	template<typename rT, typename... args>
	constexpr bool signaturesMatch()

	{
		return callableWith<
				typename vaSwitch<isVariadic, rT, args...>::vtype
				>;
	}

};
template<bool isVariadic_, typename T, typename _classType, typename rType = void, typename... argTypes>
struct delegateInfo
{

	using type = T;
	using returnType = rType;
	using classType = _classType;

	static constexpr size_t numArgs = sizeof...(argTypes);
	static constexpr bool didDeduceType = true;
	static constexpr bool isVariadic = isVariadic_;
	static constexpr bool isDelegate = true;

	T func;
	using strippedType = T;
	constexpr delegateInfo(T f) : func(f)
	{

	}
};

template<typename T>
struct failedDeductionInfo
{
	using type = T;
	static constexpr size_t numArgs = common::unone;
	static constexpr bool didDeduceType = false;
	T func;
	using strippedType = typeof(*func);
	static_assert(isFunction<strippedType>() || isMethod<strippedType>());
	constexpr failedDeductionInfo(T f) : func(f)
	{

	}
};


template<typename T, typename... Args>
static constexpr typeHolder<T> extractReturnType(T (*f)(Args...))
{
	return typeHolder<T>();
}

/*
 * chosen if f was not a function/method pointer
 */
template<typename T>
static constexpr auto functionTypeInfo(T f)
{
	return failedDeductionInfo<T>(f);
}

/*
 * chosen if f is a non-variadic function pointer
 */
template<typename returnType, typename... Args>
static constexpr auto functionTypeInfo(returnType (*f)(Args...))
{
	return funcptrInfo<false, typeof(f), returnType, Args...>(f);
}

/*
 * chosen if f is a variadic function pointer
 */

template<typename returnType, typename... Args>
static constexpr auto functionTypeInfo(returnType (*f)(Args..., ...))
{
	return funcptrInfo<true, typeof(f), returnType, Args...>(f);
}

/*
 * chosen if f is a non-variadic delegate (pointer to a member function of a class)
 */
template<typename classType, typename returnType, typename... Args>
static constexpr auto functionTypeInfo(returnType (classType::*f)(Args...))
{
	return delegateInfo<false, typeof(f), classType, returnType, Args...>(f);
}

/*
 * chosen if f is a variadic delegate
 */
template<typename classType, typename returnType, typename... Args>
static constexpr auto functionTypeInfo(returnType (classType::*f)(Args..., ...))
{
	return delegateInfo<true, typeof(f), classType, returnType, Args...>(f);
}
#if 0
	static constexpr auto testType = functionTypeInfo(strlen);

	static_assert(decltype(functionTypeInfo(sprintf))::numArgs != common::unone);

	class __TEMPLATE_FUNCTIONS_TEST___
	{
	public:
		int a;
		void doStuff(int c, int d)
		{
		}
		void doStuffs(int c, int d, ...)
		{
		}
	};

	static_assert(decltype(functionTypeInfo(&__TEMPLATE_FUNCTIONS_TEST___::doStuff))::numArgs == 2);
	static_assert(decltype(functionTypeInfo(&__TEMPLATE_FUNCTIONS_TEST___::doStuffs))::isVariadic);
#endif

}//namespace functions
}//namespace templates
}//namespace util
}//namespace cloture


