#pragma once

#define		mUse128ForVec2f		1
#define		mNoVectorDefault	0


#define		mVecInline			__forceinline
#define		mVecCall			constexpr mVecInline
#define		vecIndexValue(ix)	vec[ix]

#define		vecX()				vecIndexValue(0)
#define		vecY()				vecIndexValue(1)
#define		vecZ()				vecIndexValue(2)
#define		vecW()				vecIndexValue(3)

#define		mVecPure			mVecInline __pure
#define		mVecPureCtfe		mVecCall __pure
#define		mVecPureOp			mVecInline __pure vecType operator

#define		mMarkAggregateVec()	static constexpr bool isAggregateVector = true
//using isAggregateVector = bool

namespace cloture::util::math::vector
{


	template<typename T>
	struct isAggregateVector_resolverHelper
	{
	private:
		template<typename TT, bool b>
		struct RESOLVE
		{

		};

		template<typename TT>
		struct RESOLVE<TT, false>
		{
			static constexpr bool value = false;
			using type = TT;
		};

		template<typename TT>
		struct RESOLVE<TT, true>
		{
		private:
			mCreateIdentifierChecker(isAggregateVector);
		public:
			static constexpr bool value = mCallIdentifierChecker(TT, isAggregateVector);
		};

	public:
		static constexpr bool value = RESOLVE<T, generic::isClass<T>()>::value;
	};

	template<typename T>
	static constexpr bool isAggregateVector = isAggregateVector_resolverHelper<T>::value;

	typedef common::int8 	int8x16 	__attribute__((__vector_size__(16)));
	typedef common::uint8 	uint8x16 	__attribute__((__vector_size__(16)));

	typedef common::int16 	int16x8 	__attribute__((__vector_size__(16)));
	typedef common::uint16 	uint16x8 	__attribute__((__vector_size__(16)));

	typedef common::int32 	int32x4 	__attribute__((__vector_size__(16)));
	typedef common::uint32 	uint32x4 	__attribute__((__vector_size__(16)));

	typedef common::real32 	real32x4 	__attribute__((__vector_size__(16)));
	typedef common::real64 	real64x2 	__attribute__((__vector_size__(16)));

	/*
	 * __m64 types
	 */
	typedef common::int8	int8x8 		__attribute__((__vector_size__(8)));
	typedef common::uint8	uint8x8 	__attribute__((__vector_size__(8)));

	typedef common::int16	int16x4 	__attribute__((__vector_size__(8)));
	typedef common::uint16	uint16x4 	__attribute__((__vector_size__(8)));

	typedef common::int32	int32x2 	__attribute__((__vector_size__(8)));
	typedef common::uint32	uint32x2 	__attribute__((__vector_size__(8)));

}

namespace cloture::util::generic
{
	template<typename T>
	static constexpr bool isVector()
	{
		return false;
	}

	template<typename T>
	static constexpr bool isVector(const T in)
	{
		return false;
	}

	#define		__declIsVectorSpecial(type)
}

namespace cloture::util::templates::casts
{
	template<typename T> struct vecResolveNativeType
	{
	private:
		template<typename TT, bool b> struct RESOLVE{};

		template<typename TT>
		struct RESOLVE<TT, false>
		{
			using type = TT;
		};

		template<typename TT>
		struct RESOLVE<TT, true>
		{
		private:
			mCreateTypeResolver(nativeType);
		public:
			using type = mGetTypeResolverTypename(TT, nativeType);
		};
	public:
		using type = typename RESOLVE<T, generic::isClass<T>()>::type;
	};

	/*
	 * requires that T1 and T2 be native types
	 */
	template<typename to_, typename from_>
	struct vectorNativeCaster
	{
	private:

		template<typename to, typename from, size_t elementsTo, size_t elementsFrom>
		struct downCAST
		{
			__forceinline __pure
			static constexpr to doCast(const from From)
			{
				return reinterpret_cast<to>(static_cast<from>(From));//__builtin_convertvector(
					//	static_cast<from>(From),
					//	to
					//);
			};
		};

		template<typename to, typename from>
		struct downCAST<to, from, 4, 2>
		{
			__forceinline __pure
			static constexpr to doCast(const from From)
			{
				struct TO_
				{
					to dummyTo;
				};
				struct FROM_
				{
					from dummyFrom;
				};
				return (to)choose_expr
				(
					sizeof(TO_) == sizeof(FROM_),
					static_cast<from>(From),
					((to){From[0], From[1], 0, 0})
				);
			};
		};

		template<typename to, typename from>
		struct downCAST<to, from, 2, 4>
		{
			__forceinline __pure
			static constexpr to doCast(const from From)
			{
				struct TO_
				{
					to dummyTo;
				};
				struct FROM_
				{
					from dummyFrom;
				};
				return (to)choose_expr
				(
					sizeof(TO_) == sizeof(FROM_),
					static_cast<from>(From),
					((to){From[0], From[1]})
				);
			};
		};
	public:
		using castop = downCAST<
			to_,
			from_,
			sizeof(to_)		/	sizeof( (*static_cast<to_*>(nullptr))[0]),
			sizeof(from_)	/	sizeof( (*static_cast<to_*>(nullptr))[0])
		>;
	};

	template<bool toTypeIsClass, bool fromTypeIsClass> struct vector_cast_picker{};
	/*
	 * converting between native vector types - easy
	 */
	template<>
	struct vector_cast_picker<false, false>
	{
		template<typename toType, typename FromType>
		__forceinline __pure static constexpr toType doCast(const FromType from)
		{
			using caster = typename vectorNativeCaster<toType, FromType>::castop;
			return caster::doCast(from);
			//__builtin_convertvector(from, toType);
		}
	};
	/*
	 * 	converting from native vector type to class vector type
	 */
	template<>
	struct vector_cast_picker<true, false>
	{
		template<typename toType, typename FromType>
		__forceinline __pure static constexpr toType doCast(const FromType from)
		{
			using nativeTo = typename vecResolveNativeType<toType>::type;
			using caster = typename vectorNativeCaster<nativeTo, FromType>::castop;
			return toType(caster::doCast(from));
		}
	};
	/*
	 * converting from class vector type to native vector type
	 */
	template<>
	struct vector_cast_picker<false, true>
	{
		template<typename toType, typename FromType>
		__forceinline __pure static constexpr toType doCast(const FromType from)
		{
			using nativeFrom = typename vecResolveNativeType<FromType>::type;
			using caster = typename vectorNativeCaster<toType, nativeFrom>::castop;
			return //__builtin_convertvector(
				caster::doCast(static_cast<nativeFrom>(from));
				//toType
			//);
		}
	};
	/*
	 * converting from class vector type to class vector type
	 */
	template<>
	struct vector_cast_picker<true, true>
	{
		template<typename toType, typename FromType>
		__forceinline __pure static constexpr toType doCast(const FromType from)
		{
			using nativeFrom 	= typename vecResolveNativeType<FromType>::type;
			using nativeTo 		= typename vecResolveNativeType<toType>::type;
			using caster 		= typename vectorNativeCaster<nativeTo, nativeFrom>::castop;
			return toType(
					caster::doCast(static_cast<nativeFrom>(from))
		//	__builtin_convertvector(
		//		static_cast<nativeFrom>(from),
			//	nativeTo
		//	)
			);
		}
	};

	template<typename toType, typename FromType>
	static constexpr toType vector_cast(const FromType from)
	{
		constexpr bool toTypeIsClass	= generic::isClass<toType>();
		constexpr bool fromTypeIsClass	= generic::isClass<FromType>();
		using caster = vector_cast_picker<toTypeIsClass, fromTypeIsClass>;
		return caster::doCast<toType, FromType>(from);
	}

}

namespace cloture
{
namespace util
{
namespace math
{
namespace vector
{
	using templates::casts::vector_cast;
	#if defined(__i386__) || defined(__x86_64__)

		#if defined(__SSE__)
			#include "libVector/vector_x86.hpp"
		#else
			#warning "Building for x86 with __SSE__ not defined... Choosing generic vector implementation."
			#include "libVector/vector_generic.hpp"
		#endif

	#elif defined(__ARM_ARCH)

		#if defined(__ARM_NEON__)
			#include "libVector/vector_arm.hpp"
		#else
			#warning "Architecture is arm, but __ARM_NEON__ is not defined. Using generic vector classes."
			#include "libVector/vector_generic.hpp"
		#endif

	#else
		#include "libVector/vector_generic.hpp"
	#endif
} //namespace vector
} //namespace math
} //namespace util
}

/**
 * 	these macros are for use within structures/functions in header files
 * 	never use these at global scope in header files
 * 	you can use these in source files, but why bother when you can just
 * 	import the namespace?
 */
#define		__import_vector2f()		\
		using vector2f = cloture::util::math::vector::vector2f

#define		__import_vector3f()		\
		using vector3f = cloture::util::math::vector::vector3f

#define		__import_vector4f()		\
		using vector4f = cloture::util::math::vector::vector4f

#define		__import_float_vector_types()	\
		__import_vector2f();				\
		__import_vector3f();				\
		__import_vector4f()
#if 0
void bored()
{
	__asm
	{
		extrq xmm0, 16, 32
		movdq2q mm0, xmm0
		pfmax mm0, mm1
	}
}
#endif
