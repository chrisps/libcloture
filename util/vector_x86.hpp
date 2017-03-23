#pragma once


/*
 * this file gets included in vector.hpp within the namespace
 * cloture::util::math::vector if the platform is x86
 */

#define vec__defnOpIndex()	\
VECTOR_INLINE __pseudopure __typeof__(arr[0]) operator [] (const size_t index) const \
{\
	return vecIndexValue(index);\
} \
VECTOR_INLINE __typeof__(arr[0])& operator [] (const size_t index)\
{\
	return arr[index];\
}

#if mUse128ForVec2f
	__align(16)
#else
	__align(8)
#endif
struct vector2f
{
	#if mUse128ForVec2f
		union
		{
			vec_t 	arr[4];
			__m128 	vec;
		};
	#else
		union
		{
			vec_t 	arr[2];
			__m64 	vec;
		};
	#endif

	using nativeType	=	__typeof__(vec);
	using vecType = vector2f;

	vec__defnOpIndex()

	#if !mNoVectorDefault
		VECTOR_INLINE vector2f() : vec{}	{}
	#endif

	VECTOR_INLINE vector2f(const vec_t __x, const vec_t __y)
	#if mUse128ForVec2f
		: arr{__x, __y, .0f, .0f}
	#else
		: arr{__x, __y}
	#endif
	{

	}

	VECTOR_INLINE vector2f(const nativeType other) : vec(other) {}


	mVecPureOp * (const vec_t scalar) const
	{
		return vecType(vecIndexValue(0) * scalar, vecIndexValue(1) * scalar);
	}

	mVecPureOp / (const vec_t scalar) const
	{
		return vecType(vecIndexValue(0) / scalar, vecIndexValue(1) / scalar);
	}

	mVecPureOp + (const vecType rhs) const
	{
		return vecType(vecIndexValue(0) + rhs[0], vecIndexValue(1) + rhs[1]);
	}

	mVecPureOp - (const vecType rhs) const
	{
		return vecType(vecIndexValue(0) - rhs[0], vecIndexValue(1) - rhs[1]);
	}

	VECTOR_INLINE __pure vec_t length() const
	{
		return static_cast<vec_t>(
			sqrtf(vecX() * vecX() + vecY() * vecY() )
		);
	}


	VECTOR_INLINE bool operator ==(const vecType rhs) const
	{
		return vecIndexValue(0) == rhs[0] && vecIndexValue(1) == rhs[1];
	}

	VECTOR_INLINE bool operator !=(const vecType rhs) const
	{
		return vecIndexValue(0) != rhs[0] || vecIndexValue(1) != rhs[1];
	}

	VECTOR_INLINE operator bool() const
	{
		return vecIndexValue(0) != .0f && vecIndexValue(1) != .0f;
	}

	VECTOR_INLINE explicit operator vec_t*()
	{
		return &arr[0];
	}

	VECTOR_INLINE  __pure vecType normalize() const
	{
		auto l = length();

		if ( unlikely(l == .0f) )
			return vecType( .0f, .0f );
		l = 1.0f / l;
		return vecType( vecX() *l, vecY() * l );

	}
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector2f";
	}
};

__align(16) struct vector3f
{
	union
	{
		__m128 vec;
		vec_t arr[4];
	};
	using vecType = vector3f;

	vec__defnOpIndex()

	#if !mNoVectorDefault
		VECTOR_INLINE vector3f() : vec{} {}
	#endif

	VECTOR_INLINE vector3f(const vec_t __x, const vec_t __y, const vec_t __z) : arr{__x, __y, __z, 1.0f}
	{
	}

	VECTOR_INLINE vector3f(const vec_t* RESTRICT const other) : arr{other[0], other[1], other[2], 1.0f}
	{
	}


	VECTOR_INLINE vector3f(const __m128 other) : vec(other) {}

	mVecPureOp * (const vec_t scalar) const
	{
		return vecType(arr[0] * scalar, vecIndexValue(1) * scalar, vecIndexValue(2) * scalar);
	}

	mVecPureOp / (const vec_t scalar) const
	{
		return vecType(arr[0] / scalar, vecIndexValue(1) / scalar, vecIndexValue(2) / scalar);
	}

	mVecPureOp *(const __m128 other) const
	{
		return _mm_mul_ps(vec, other);
	}

	mVecPureOp /(const __m128 other) const
	{
		return _mm_div_ps(vec, other);
	}

	mVecPureOp +(const __m128 other) const
	{
		return _mm_add_ps(vec, other);
	}
	mVecPureOp -(const __m128 other) const
	{
		return _mm_sub_ps(vec, other);
	}

	mVecPureOp + (const vecType rhs) const
	{
		return vecType(arr[0] + rhs[0], vecIndexValue(1) + rhs[1], vecIndexValue(2) + rhs[2]);
	}

	mVecPureOp - (const vecType rhs) const
	{
		return vecType(arr[0] - rhs[0], vecIndexValue(1) - rhs[1], vecIndexValue(2) - rhs[2]);
	}

	mVecPure vecType abs() const
	{
		const __m128i bits = _mm_castps_si128(vec);
		return _mm_castsi128_ps(
				_mm_and_si128(bits, _mm_set1_epi32(0x7FFFFFFF))
		);
	}

	VECTOR_INLINE operator vec_t*()
	{
		return &arr[0];
	}

	VECTOR_INLINE operator const vec_t*() const
	{
		return &arr[0];
	}

	#if defined(__i386__) || defined(__x86_64__)
		mVecPure operator __m128()
		{
			return vec;
		}
	#endif
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector3f";
	}
};

__align(16) struct vector4f
{
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector4f";
	}
	union
	{
		__m128 vec;
		vec_t arr[4];
	};
	using vecType = vector4f;
	vec__defnOpIndex()

	#if !mNoVectorDefault
		VECTOR_INLINE vector4f() : vec{} {}
	#endif

	VECTOR_INLINE vector4f(const vec_t __x, const vec_t __y, const vec_t __z, const vec_t __w) : arr{__x, __y, __z, __w}
	{
	}

	VECTOR_INLINE vector4f(const vec_t* RESTRICT const other) : arr{other[0], other[1], other[2], other[3]}
	{
	}
	VECTOR_INLINE vector4f(const __m128 other) : vec(other) {}

	mVecPureOp * (const vec_t scalar) const
	{
		return vecType(arr[0] * scalar, vecIndexValue(1) * scalar, vecIndexValue(2) * scalar, vecIndexValue(3) * scalar);
	}

	mVecPureOp / (const vec_t scalar) const
	{
		return vecType(arr[0] / scalar, vecIndexValue(1) / scalar, vecIndexValue(2) / scalar, vecIndexValue(3) / scalar);
	}

	mVecPureOp *(const __m128 other) const
	{
		return _mm_mul_ps(vec, other);//vec * other;
	}

	mVecPureOp /(const __m128 other) const
	{
		return _mm_div_ps(vec, other);//vec / other;
	}

	mVecPureOp +(const __m128 other) const
	{
		return _mm_add_ps(vec, other);//vec + other;
	}
	mVecPureOp -(const __m128 other) const
	{
		return _mm_sub_ps(vec, other);//vec - other;
	}

	VECTOR_INLINE operator vec_t*()
	{
		return &arr[0];
	}

	VECTOR_INLINE operator const vec_t*() const
	{
		return &arr[0];
	}

	#if defined(__i386__) || defined(__x86_64__)
		mVecPure operator __m128()
		{
			return vec;
		}
	#endif

};

/*
 * vector of 32 bit ints
 */
template<typename T>
 __align(16)
struct vector4i32
{
private:
	template<typename T>
	struct getNativeType
	{

	};
	template<>
	struct getNativeType<signed int>
	{
		typedef signed int type __attribute__((__vector_size__(16)));
	};
	template<>
	struct getNativeType<unsigned int>
	{
		typedef unsigned int type __attribute__((__vector_size__(16)));
	};
public:
	//typedef T nativeType __attribute__((__vector_size__(16)));
	using nativeType = typename getNativeType<T>::type;
	nativeType vec;

	using vecType = vector4i32<T>;

	mVecPure vector4i32(const T dupVal)
	{
		vec[0] = vec[1] = vec[2] = vec[3] = dupVal;
	}

	mVecPure vector4i32(const T v0, const T v1, const T v2, const T v3)
	{
		vec[0] = v0;
		vec[1] = v1;
		vec[2] = v2;
		vec[3] = v3;
	}

	mVecPure vector4i32(const T* RESTRICT const vIn)
	{
		vec[0] = vIn[0];
		vec[1] = vIn[1];
		vec[2] = vIn[2];
		vec[3] = vIn[3];
	}

	mVecPure vector4i32(const nativeType vIn)
	{
		vec = vIn;
	}

	mVecPureOp +(const vecType rhs) const
	{
		return vecType(vec + rhs.vec);
	}
	mVecPureOp -(const vecType rhs) const
	{
		return vecType(vec - rhs.vec);
	}
	mVecPureOp *(const vecType rhs) const
	{
		return vecType(vec * rhs.vec);
	}
	mVecPureOp /(const vecType rhs) const
	{
		return vecType(vec / rhs.vec);
	}
	mVecPureOp &(const vecType rhs) const
	{
		return vecType(vec & rhs.vec);
	}
	mVecPureOp |(const vecType rhs) const
	{
		return vecType(vec | rhs.vec);
	}
	mVecPureOp ^(const vecType rhs) const
	{
		return vecType(vec ^ rhs.vec);
	}
	mVecPureOp >>(const T rhs) const
	{
		return vecType(vec >> (nativeType){rhs, rhs, rhs, rhs});
	}
	mVecPureOp <<(const T rhs) const
	{
		return vecType(vec << (nativeType){rhs, rhs, rhs, rhs});
	}
	mVecPureOp >>(const vecType rhs) const
	{
		return vecType(vec >> rhs.vec);
	}
	mVecPureOp <<(const vecType rhs) const
	{
		return vecType(vec << rhs.vec);
	}
	mVecPureOp ~()
	{
		return vecType(~vec);
	}
	mVecPureOp -()
	{
		return vecType(-vec);
	}
};

/*
 * vector of signed ints
 */
struct vector4si : public vector4i32<signed int>
{
	using vector4i32::vector4i32;
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector4si";
	}
};
/*
 * vector of unsigned ints
 */
struct vector4ui : public vector4i32<unsigned int>
{
	using vector4i32::vector4i32;
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector4ui";
	}
};

/*
 * vector of 8 16 bit ints
 */
template<typename T>
 __align(16)
struct vector8i16
{
private:
	template<typename T>
	struct getNativeType
	{

	};
	template<>
	struct getNativeType<signed short>
	{
		typedef signed short type __attribute__((__vector_size__(16)));
	};
	template<>
	struct getNativeType<unsigned short>
	{
		typedef unsigned short type __attribute__((__vector_size__(16)));
	};
public:
	using nativeType = typename getNativeType<T>::type;
	nativeType vec;

	using vecType = vector8i16<T>;

	mVecPure vector8i16(const T dupVal)
	{
		vec[0] = vec[1] = vec[2] = vec[3] =
		vec[4] = vec[5] = vec[6] = vec[7] = dupVal;
	}

	mVecPure vector8i16(
	const T v0,
	const T v1,
	const T v2,
	const T v3,
	const T v4,
	const T v5,
	const T v6,
	const T v7
	)
	{
		vec[0] = v0;
		vec[1] = v1;
		vec[2] = v2;
		vec[3] = v3;
		vec[4] = v0;
		vec[5] = v1;
		vec[6] = v2;
		vec[7] = v3;
	}

	mVecPure vector8i16(const T* RESTRICT const vIn)
	{
		vec[0] = vIn[0];
		vec[1] = vIn[1];
		vec[2] = vIn[2];
		vec[3] = vIn[3];
		vec[4] = vIn[4];
		vec[5] = vIn[5];
		vec[6] = vIn[6];
		vec[7] = vIn[7];
	}

	mVecPure vector8i16(const nativeType vIn)
	{
		vec = vIn;
	}

	mVecPureOp +(const vecType rhs) const
	{
		return vecType(vec + rhs.vec);
	}
	mVecPureOp -(const vecType rhs) const
	{
		return vecType(vec - rhs.vec);
	}
	mVecPureOp *(const vecType rhs) const
	{
		return vecType(vec * rhs.vec);
	}
	mVecPureOp /(const vecType rhs) const
	{
		return vecType(vec / rhs.vec);
	}
	mVecPureOp &(const vecType rhs) const
	{
		return vecType(vec & rhs.vec);
	}
	mVecPureOp |(const vecType rhs) const
	{
		return vecType(vec | rhs.vec);
	}
	mVecPureOp ^(const vecType rhs) const
	{
		return vecType(vec ^ rhs.vec);
	}
	mVecPureOp >>(const T rhs) const
	{
		return vecType(vec >> (nativeType){rhs, rhs, rhs, rhs, rhs, rhs, rhs, rhs});
	}
	mVecPureOp <<(const T rhs) const
	{
		return vecType(vec << (nativeType){rhs, rhs, rhs, rhs, rhs, rhs, rhs, rhs});
	}
	mVecPureOp >>(const vecType rhs) const
	{
		return vecType(vec >> rhs.vec);
	}
	mVecPureOp <<(const vecType rhs) const
	{
		return vecType(vec << rhs.vec);
	}
	mVecPureOp ~()
	{
		return vecType(~vec);
	}
	mVecPureOp -()
	{
		return vecType(-vec);
	}

};
/*
 * vector of signed shorts
 */
struct vector8ss : public vector8i16<signed short>
{
	using vector8i16::vector8i16;
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector8ss";
	}
};
/*
 * vector of unsigned shorts
 */
struct vector8us : public vector8i16<unsigned short>
{
	using vector8i16::vector8i16;
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector8us";
	}
};

/*
 * vector of 16 8 bit ints
 */
template<typename T>
 __align(16)
struct vector16i8
{
private:
	template<typename T>
	struct getNativeType
	{

	};
	template<>
	struct getNativeType<signed char>
	{
		typedef signed char type __attribute__((__vector_size__(16)));
	};
	template<>
	struct getNativeType<unsigned char>
	{
		typedef unsigned char type __attribute__((__vector_size__(16)));
	};
public:
	using nativeType = typename getNativeType<T>::type;
	nativeType vec;

	using vecType = vector8i16<T>;

	mVecPure vector16i8(const T dupVal)
	{
		vec[0] = vec[1] = vec[2] = vec[3] =
		vec[4] = vec[5] = vec[6] = vec[7] =
		vec[8] = vec[9] = vec[10] = vec[11] =
		vec[12] = vec[13] = vec[14] = vec[15] = dupVal;
	}

	mVecPure vector16i8(
	const T v0,
	const T v1,
	const T v2,
	const T v3,
	const T v4,
	const T v5,
	const T v6,
	const T v7,

	const T v8,
	const T v9,
	const T v10,
	const T v11,
	const T v12,
	const T v13,
	const T v14,
	const T v15
	)
	{
		vec[0] = v0;
		vec[1] = v1;
		vec[2] = v2;
		vec[3] = v3;
		vec[4] = v0;
		vec[5] = v1;
		vec[6] = v2;
		vec[7] = v3;

		vec[8] = v8;
		vec[9] = v9;
		vec[10] = v10;
		vec[11] = v11;
		vec[12] = v12;
		vec[13] = v13;
		vec[14] = v14;
		vec[15] = v15;
	}

	mVecPure vector16i8(const T* RESTRICT const vIn)
	{
		vec[0] = vIn[0];
		vec[1] = vIn[1];
		vec[2] = vIn[2];
		vec[3] = vIn[3];
		vec[4] = vIn[4];
		vec[5] = vIn[5];
		vec[6] = vIn[6];
		vec[7] = vIn[7];

		vec[8] = vIn[8];
		vec[9] = vIn[9];
		vec[10] = vIn[10];
		vec[11] = vIn[11];
		vec[12] = vIn[12];
		vec[13] = vIn[13];
		vec[14] = vIn[14];
		vec[15] = vIn[15];
	}

	mVecPure vector16i8(const nativeType vIn)
	{
		vec = vIn;
	}

	mVecPureOp +(const vecType rhs) const
	{
		return vecType(vec + rhs.vec);
	}
	mVecPureOp -(const vecType rhs) const
	{
		return vecType(vec - rhs.vec);
	}
	mVecPureOp *(const vecType rhs) const
	{
		return vecType(vec * rhs.vec);
	}
	mVecPureOp /(const vecType rhs) const
	{
		return vecType(vec / rhs.vec);
	}
	mVecPureOp &(const vecType rhs) const
	{
		return vecType(vec & rhs.vec);
	}
	mVecPureOp |(const vecType rhs) const
	{
		return vecType(vec | rhs.vec);
	}
	mVecPureOp ^(const vecType rhs) const
	{
		return vecType(vec ^ rhs.vec);
	}
	mVecPureOp >>(const T rhs) const
	{
		return vecType(vec >> (nativeType){rhs, rhs, rhs, rhs, rhs, rhs, rhs, rhs, rhs, rhs, rhs,
		rhs, rhs, rhs, rhs, rhs});
	}
	mVecPureOp <<(const T rhs) const
	{
		return vecType(vec << (nativeType){rhs, rhs, rhs, rhs, rhs, rhs, rhs, rhs,
		rhs, rhs, rhs, rhs, rhs, rhs, rhs, rhs});
	}
	mVecPureOp >>(const vecType rhs) const
	{
		return vecType(vec >> rhs.vec);
	}
	mVecPureOp <<(const vecType rhs) const
	{
		return vecType(vec << rhs.vec);
	}
	mVecPureOp ~()
	{
		return vecType(~vec);
	}
	mVecPureOp -()
	{
		return vecType(-vec);
	}

};
/*
 * vector of signed chars
 */
struct vector16sb : public vector16i8<signed char>
{
	using vector16i8::vector16i8;
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector16sb";
	}
};
/*
 * vector of unsigned chars
 */
struct vector16ub : public vector16i8<unsigned char>
{
	using vector16i8::vector16i8;
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector16ub";
	}
};
