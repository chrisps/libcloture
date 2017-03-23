#pragma once


namespace cloture	{
namespace util		{
namespace pointers	{

#define		WRAPPED_PTR_INLINE	inline

/*
 * 	basic pointer-wrapping template
 *
 */
template<typename T, __printf_type(1) __noreturn void (*onError)(const char*, ...) = nullptr>
class wrapped_ptr
{
	T* ptr;
public:
	using wrapperType = wrapped_ptr<T, onError>;

	/*
	 * called by isNull
	 */
	__pure __forceinline
	static constexpr bool clotureIsNull(const wrapperType wrap)
	{
		return wrap.ptr == nullptr;
	}

	__forceinline
	static constexpr bool clotureMakeNull(wrapperType& wrap)
	{
		return wrap.ptr = nullptr;
	}

	template<__printf_type(1) __noreturn void (*onError_)(const char*, ...) = nullptr>
	__noinline __cold
	static void PtrNullError()
	{
		onError_("INTERNAL ERROR: Attempted to dereference NULL pointer in wrapped_ptr.");
	}

	template<> __noinline __cold static void PtrNullError<nullptr>(){}

	constexpr WRAPPED_PTR_INLINE wrapped_ptr(T* p) : ptr(p)
	{}

	constexpr WRAPPED_PTR_INLINE T* operator ->()
	{
		#if mCheckPointerNull
			if(unlikely(ptr == nullptr))
				PtrNullError<onError>();
		#endif
		return ptr;
	}
	constexpr WRAPPED_PTR_INLINE const T* operator ->() const
	{
		#if mCheckPointerNull
			if(unlikely(ptr == nullptr))
				PtrNullError<onError>();
		#endif
		return ptr;
	}

	__pseudopure constexpr WRAPPED_PTR_INLINE bool operator ==(const T* const other) const
	{
		return ptr == other;
	}

	__pseudopure constexpr WRAPPED_PTR_INLINE bool operator !=(const T* const other) const
	{
		return ptr != other;
	}


	__pseudopure constexpr WRAPPED_PTR_INLINE explicit operator bool() const
	{
		return ptr != nullptr;
	}

	__pseudopure constexpr WRAPPED_PTR_INLINE bool operator !() const
	{
		return ptr == nullptr;
	}

	__pseudopure constexpr WRAPPED_PTR_INLINE explicit operator T*()
	{
		return ptr;
	}

	__pseudopure constexpr WRAPPED_PTR_INLINE T* getPtr()
	{
		return ptr;
	}

};//class wrapped_ptr

template<typename T>
__pure __forceinline static bool isNull(T toCheck)
{
	constexpr bool typeIsPointer = generic::isPointer<T>();
	mIfMetaTrue(typeIsPointer)
	{
		return toCheck == nullptr;
	}
	mIfMetaFalse(typeIsPointer)
	{
		constexpr bool typeIsClass = generic::isClass<T>();

		mIfMetaTrue(typeIsClass)
		{
			__if_exists(T::clotureIsNull)
			{
				return T::clotureIsNull(toCheck);
			}
			__if_not_exists(T::clotureIsNull)
			{
				static_assert(
				!typeIsClass,
				"Class passed to isNull does not provide the static method clotureIsNull."
				);
				return false;
			}
		}
		mIfMetaFalse(typeIsClass)
		{
			static_assert(typeIsClass, "Unwrapped primitive class passed to isNull!");
			return true;
		}
	}
}

template<typename T>
__forceinline static void setNull(T& toNull)
{
	constexpr bool typeIsPointer = generic::isPointer<T>();
	mIfMetaTrue(typeIsPointer)
	{
		toNull = nullptr;
		return;
	}
	mIfMetaFalse(typeIsPointer)
	{
		constexpr bool typeIsClass = generic::isClass<T>();

		mIfMetaTrue(typeIsClass)
		{
			__if_exists(T::clotureMakeNull)
			{
				T::clotureMakeNull(toNull);
				return;
			}
			__if_not_exists(T::clotureMakeNull)
			{
				static_assert(
				!typeIsClass,
				"Class passed to setNull does not provide the static method clotureMakeNull."
				);
				return;
			}
		}
		mIfMetaFalse(typeIsClass)
		{
			static_assert(typeIsClass, "Unwrapped primitive class passed to setNull!");
			return;
		}
	}
}

} //namespace pointers
} //namespace util
} //namespace cloture
