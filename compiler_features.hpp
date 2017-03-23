#pragma once

#if !defined(__clang__)
	#define	HAS_CHOOSE_EXPR	0
#else
	#define HAS_CHOOSE_EXPR 1
#endif

#if defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER)
	#define		__forceinline							__attribute__((always_inline))

	#define		__noinline								__attribute__((noinline))
	#define 	__flatten								__attribute__((flatten))

	#define		__noreturn								__attribute__((noreturn))

	#define		__pure									__attribute__((__const__))
	#define		__pseudopure							__attribute__((pure))

	#define		__cold									__attribute__((cold))
	#define		__hot									__attribute__((hot))
	#define		__used									__attribute__((used))
	#define		__unused								__attribute__((unused))
	#define		__align(alignment)						__attribute__((aligned(alignment)))
	#define		__returns_aligned(alignment)			__attribute__((assume_aligned(alignment)))
	#define		__nonull_args(...)						__attribute__((nonnull(__VA_ARGS__)))
	#define		__returns_nonull 						__attribute__((returns_nonnull))
	#define		__returns_unaliased						__attribute__((malloc))

	#define		unreachable()							__builtin_unreachable()
	#define		__printf_type(n)						__attribute__ ((format (printf, n, n+1)))

	#ifdef		__GNUC__
		#define 	__assume(x)								if(!(x))	unreachable()
	#endif

	#define 	__assume_aligned(ptr,alignment)			__builtin_assume_aligned(ptr, alignment)
	#define		__object_size(obj, type)				__builtin_object_size(obj, type)
	#define		prefetch(addr, ...)						__builtin_prefetch(addr, ##__VA_ARGS__)
	#define 	clear_cache(begin, end)					__builtin___clear_cache(begin, end)

	#define		__allocSize(...)						__attribute__((alloc_size(__VA_ARGS__)))

	#define 	__offsetof(type, member)				__builtin_offsetof (type, member)
	#define 	likely(x)								__builtin_expect(bool(x), true)
	#define 	unlikely(x)								__builtin_expect(bool(x), false)
	#define		__expect(x, value)						__builtin_expect(x, value)
	#define 	RESTRICT 								__restrict
	#define 	restrict								__restrict
	#define		__packed								__attribute__((packed))
#else
	#ifndef _MSVC_VER
		#define	__forceinline	inline
		#define	__noinline
		#define	__noreturn	
		#define	__pure	
		#define	__assume(x)
		#define	__align(a)	
		#define	RESTRICT
	#else
		#define	__noreturn			__declspec(noreturn)
		#define	__noinline			__declspec(noinline)
		#define	__pure				__declspec(noalias)
		#define	__align(a)			__declspec( align( a))
		#define	__returns_unaliased	__declspec(restrict)
		#define RESTRICT 			__restrict
	#endif
	#define	__pseudopure	
	#define unlikely(x)		(x)
	#define likely(x)		(x)
	#define __flatten
	#define	__returns_nonull 
	#define	__nonull_args(...)
	#define	unreachable()
	#define __assume_aligned(ptr,alignment)		(ptr)
	#define	prefetch(addr, ...)	
	#define clear_cache(begin, end)	
	#define __offsetof(type, member)	offsetof(type, member)
#endif

#if defined(__clang__) || defined(_MSVC_VER) || defined(__INTEL_COMPILER)

	#define	__assertExists(identifier, ...)		\
	__if_not_exists(identifier) 				\
	{											\
		static_assert(false, __VA_ARGS__);		\
	}

	#define	__novtbl								__declspec(novtable)
	#define	__msvc_property_true(gt, st)			__declspec( property(get = gt, put = st))
	#define	__msvc_property_false(gt, ...)			__declspec( property(get = gt))

	#define	__expand_msvc_property__(call, gt, ...)	call(gt, __VA_ARGS__)
	#define	__property(rW, gt, ...)					__expand_msvc_property__(__msvc_property_##readWrite, gt, __VA_ARGS__)
	#define	__naked									__declspec(naked)
#else
	#define	__novtbl	
	#define	__msvc_property(readWrite, gt, ...)	
	#define	__returns_unaliased			
	#define	__assertExists(identifier, ...)
#endif

#if defined(__clang__)
	#define		vectorizeLoop	_Pragma("clang loop vectorize(enable)")
#elif defined(__INTEL_COMPILER)
	#define		vectorizeLoop	_Pragma("simd")
#elif defined(__GNUC__)
	#define		vectorizeLoop	_Pragma("GCC ivdep")
#else
	#define		vectorizeLoop
#endif

#if defined(__clang__)
	#define		__flag_enum				__attribute__((flag_enum))
	#define		__addressof(x)			__builtin_addressof(x)

	#define		__shuffleVector(...)	__builtin_shufflevector(__VA_ARGS__)
	#define		__unrollFull()			_Pragma("unroll")
	
	#define		__pointerAlignment(x)	__attribute__((align_value(x)))

	/*
	 * 	typestate attributes
	*/

	#define		callableWhen(...)		__attribute__((callable_when(__VA_ARGS__)))
	#define		consumableClass(x)		__attribute__((consumable(x)))
	/*
	 * applied to function parameters
	 */
	#define		paramTypestate(x)		__attribute__((param_typestate(x)))
	/*
	 * applied to functions or function parameters
	 * on functions, this sets the typestate of the return value
	 * on parameters, this sets the typestate of the parameter after the function
	 * returns
	 *
	 *
	 */
	#define		returnTypestate(x)		__attribute__((return_typestate(x)))

	//signals that a call to the method transitions the object into state x
	#define		methodSetsTypestate(x)	__attribute__((set_typestate(x)))
	#define		returnsTrueIfState(x)	__attribute__((test_typestate(x)))

	#define		__optnone				__attribute__((optnone))
	#define		__nodebug				__attribute__((__nodebug__))
	#define		__annotate(annotation)	__attribute__((annotate(annotation)))
#else

#endif

/**
	choose_expr

	GCC extension. sort of like a compile time ternary.
	different from __if_exists and __if_not_exists in that it takes uses a constant expression
	rather than an identifier

	on its own it isn't too powerful. combined with __if_exists, __if_not_exists, templates and constexpr functions
	it can be useful
*/
#if HAS_CHOOSE_EXPR
	#define 	choose_expr(const_expr, if_true, if_false)	__builtin_choose_expr(const_expr, if_true, if_false)
#else
	#define		choose_expr(const_expr, if_true, if_false)	((const_expr) ? (if_true) : (if_false))
#endif
/**
	constant_p

	GCC extension. returns true if value is known at compile time
	i'm not 100% sure, but I think this can be used in constexpr functions to
	determine if they are being evaluated at compile time

*/
#define		constant_p(value)				__builtin_constant_p(value)

#define		constexpr_ternary(condition, if_true, if_false)		\
			_Generic( *(choose_expr(condition, static_cast<int*>(nullptr), static_cast<float*>(nullptr))),\
					int:\
					(if_true),\
					float:\
					(if_false),\
					)

#define		backreference			__unused const

#if mCheckAssumes
	#define		checkedAssume(x)		assert(x)
#else
	#define		checkedAssume(x)		__assume(x)
#endif

#ifdef __assume 
#undef __assume 
#define __assume(x)		__builtin_assume(x)
#endif
