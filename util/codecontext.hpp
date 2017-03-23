#pragma once

namespace cloture
{
	/*
	 * classes/structs that are intended to be used at runtime inherit from
	 * runtimeContext
	 */
	struct runtimeContext{};
	static_assert(sizeof(runtimeContext) == 1);
	/*
	 * classes/structs that are intended for use at compile time inherit from
	 * ctfeContext
	 */
	struct ctfeContext {};
	static_assert(sizeof(runtimeContext) == 1);

	/*
	 * structures/classes that consist entirely of static methods, static fields
	 * and type declarations inherit from apiObject
	 */
	struct apiObject{};

	template<typename T>
	struct markImplicitlyConvertible
	{
		using implicitConversion = T;
	};

}

#define		mAssertZeroOffset(classname, membername)	\
			static_assert(__offsetof(classname, membername) == 0)

#define		__markAsCtfe()					public cloture::ctfeContext
#define		__markAsRuntime()				public cloture::runtimeContext

#define		__markApiObject()				public cloture::apiObject

#define		__allowConversionTo(Typename)	public cloture::markImplicitlyConvertible<Typename>
