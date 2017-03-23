#pragma once
/*
 * this file gets included in vector.hpp within the namespace
 * cloture::util::math::vector if the platform is x86
*/



/*
 * vector of 2 32 bit ints
 */
template<typename T>
 __align(8)
struct vector2i32
{
private:
	template<typename T>
	struct getNativeType
	{

	};
	template<>
	struct getNativeType<signed int>
	{
		typedef signed int type __attribute__((__vector_size__(8)));
	};
	template<>
	struct getNativeType<unsigned int>
	{
		typedef unsigned int type __attribute__((__vector_size__(8)));
	};
public:
	using nativeType	= typename getNativeType<T>::type;
	using elementType 	= T;
	nativeType vec;

	using vecType = vector2i32<T>;
	mVecPure vector2i32() {}
	mVecCall vector2i32(const T dupVal)
	{
		vec[0] = vec[1] = dupVal;
	}

	mVecCall vector2i32(const T v0, const T v1)
	{
		vec[0] = v0;
		vec[1] = v1;
	}

	mVecCall vector2i32(const T* RESTRICT const vIn)
	{
		vec[0] = vIn[0];
		vec[1] = vIn[1];
	}

	mVecCall vector2i32(const nativeType vIn)
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
		return vecType(vec >> (nativeType){rhs, rhs});
	}
	mVecPureOp <<(const T rhs) const
	{
		return vecType(vec << (nativeType){rhs, rhs});
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
	mVecCall operator nativeType()
	{
		return vec;
	}
	mVecCall operator nativeType() const
	{
		return vec;
	}
	__pseudopure mVecInline elementType operator [](const common::size32 index) const
	{
		return vec[index];
	}

	mVecInline elementType operator ()(const common::size32 index, const elementType element)
	{
		return vec[index] = element;
	}
	mVecInline vecType operator =(const vecType other)
	{
		return vec = other.vec;
	}
};
#if 0
/*
 * vector of signed ints
 */
struct vector2si : public vector2i32<signed int>
{
	using vector2i32::vector2i32;
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector2si";
	}
};
/*
 * vector of unsigned ints
 */
struct vector2ui : public vector2i32<unsigned int>
{
	using vector2i32::vector2i32;
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector2ui";
	}
};
#endif
using vector2si = vector2i32<signed int>;
using vector2ui = vector2i32<unsigned int>;

using vecmask32x2 = vector2ui;

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
	using nativeType 	=	typename getNativeType<T>::type;
	using elementType 	=	T;
	nativeType vec;

	using vecType = vector4i32<T>;

	mVecPure vector4i32() {}
	mVecCall vector4i32(const T dupVal)
	{
		vec[0] = vec[1] = vec[2] = vec[3] = dupVal;
	}

	mVecCall vector4i32(const T v0, const T v1, const T v2, const T v3)
	{
		vec[0] = v0;
		vec[1] = v1;
		vec[2] = v2;
		vec[3] = v3;
	}

	mVecCall vector4i32(const T* RESTRICT const vIn)
	{
		vec[0] = vIn[0];
		vec[1] = vIn[1];
		vec[2] = vIn[2];
		vec[3] = vIn[3];
	}

	mVecCall vector4i32(const nativeType vIn)
	{
		vec = vIn;
	}

	mVecPureOp +(const vecType rhs) const
	{
		return vecType(vec + rhs.vec);
	}

	mVecInline vecType operator =(const vecType other)
	{
		return vec = other.vec;
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

	__pseudopure mVecInline elementType operator [](const common::size32 index) const
	{
		return vec[index];
	}

	mVecInline elementType operator ()(const common::size32 index, const elementType element)
	{
		return vec[index] = element;
	}

	mVecCall operator nativeType()
	{
		return vec;
	}
	mVecCall operator nativeType() const
	{
		return vec;
	}
};

/*
 * vector of signed ints
 */
 /*
struct vector4si : public vector4i32<signed int>
{
	using vector4i32::vector4i32;
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector4si";
	}
};*/
using vector4si = vector4i32<signed int>;
/*
 * vector of unsigned ints
 */
 /*
struct vector4ui : public vector4i32<unsigned int>
{
	using vector4i32::vector4i32;
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector4ui";
	}
};*/
using vector4ui = vector4i32<unsigned int>;

using vecmask32x4 = vector4ui;

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
	using nativeType	=	typename getNativeType<T>::type;
	using elementType 	=	T;
	nativeType vec;

	using vecType = vector8i16<T>;
	mVecPure vector8i16() {}
	mVecCall vector8i16(const T dupVal)
	{
		vec[0] = vec[1] = vec[2] = vec[3] =
		vec[4] = vec[5] = vec[6] = vec[7] = dupVal;
	}
	mVecInline vecType operator =(const vecType other)
	{
		return vec = other.vec;
	}
	mVecCall vector8i16(
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

	mVecCall vector8i16(const T* RESTRICT const vIn)
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

	mVecCall vector8i16(const nativeType vIn)
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
	mVecCall operator nativeType()
	{
		return vec;
	}
	mVecCall operator nativeType() const
	{
		return vec;
	}
	__pseudopure mVecInline elementType operator [](const common::size32 index) const
	{
		return vec[index];
	}

	mVecInline elementType operator ()(const common::size32 index, const elementType element)
	{
		return vec[index] = element;
	}
};
#if 0
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
#endif
using vector8ss = vector8i16<signed short>;
using vector8us = vector8i16<unsigned short>;
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
	using nativeType	=	typename getNativeType<T>::type;
	using elementType 	=	T;
	nativeType vec;

	using vecType = vector8i16<T>;
	mVecPure vector16i8() {}
	mVecCall vector16i8(const T dupVal)
	{
		vec[0] = vec[1] = vec[2] = vec[3] =
		vec[4] = vec[5] = vec[6] = vec[7] =
		vec[8] = vec[9] = vec[10] = vec[11] =
		vec[12] = vec[13] = vec[14] = vec[15] = dupVal;
	}
	mVecInline vecType operator =(const vecType other)
	{
		return vec = other.vec;
	}
	mVecCall vector16i8(
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

	mVecCall vector16i8(const T* RESTRICT const vIn)
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

	mVecCall vector16i8(const nativeType vIn)
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
	mVecCall operator nativeType()
	{
		return vec;
	}
	mVecCall operator nativeType() const
	{
		return vec;
	}
	__pseudopure mVecInline elementType operator [](const common::size32 index) const
	{
		return vec[index];
	}

	mVecInline elementType operator ()(const common::size32 index, const elementType element)
	{
		return vec[index] = element;
	}
};
#if 0
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
#endif

using vector16sb = vector16i8<signed char>;
using vector16ub = vector16i8<unsigned char>;


#define vec__defnOpIndex()	\
mVecInline __pseudopure __typeof__(vec[0]) operator [] (const size_t index) const \
{\
	return vecIndexValue(index);\
} \
mVecInline __typeof__(vec[0])& operator [] (const size_t index)\
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
			real32x4 	vec;
		};
	#else
		union
		{
			vec_t 	arr[2];
			__m64 	vec;
		};
	#endif

	using nativeType	=	real32x4;
	using vecType 		=	vector2f;
	using elementType 	=	common::real32;
	vec__defnOpIndex()

	#if !mNoVectorDefault
		mVecCall vector2f() : vec{}	{}
	#endif

	mVecCall vector2f(const vec_t __x, const vec_t __y)
	#if mUse128ForVec2f
		: vec{__x, __y, .0f, .0f}
	#else
		: vec{__x, __y}
	#endif
	{

	}
	mVecInline vecType operator =(const vecType other)
	{
		return vec = other.vec;
	}
	mVecCall vector2f(const nativeType other) : vec(other) {}


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

	mVecInline __pure vec_t length() const
	{
		return static_cast<vec_t>(
			sqrtf(vecX() * vecX() + vecY() * vecY() )
		);
	}


	mVecInline bool operator ==(const vecType rhs) const
	{
		auto result = vec == rhs.vec;//vecIndexValue(0) == rhs[0] && vecIndexValue(1) == rhs[1];

		return result[0] & result[1] == 1;
	}

	mVecInline bool operator !=(const vecType rhs) const
	{
		auto result = vec == rhs.vec;//vecIndexValue(0) == rhs[0] && vecIndexValue(1) == rhs[1];

		return result[0] & result[1] == 0;
	}

	mVecInline operator bool() const
	{
		return vecIndexValue(0) != .0f && vecIndexValue(1) != .0f;
	}

	mVecCall explicit operator vec_t*()
	{
		return &arr[0];
	}

	mVecInline  __pure vecType normalize() const
	{
		auto l = length();

		if ( unlikely(l == .0f) )
			return vecType( .0f, .0f );
		l = 1.0f / l;
		return vecType( vecX() *l, vecY() * l );

	}

	mVecCall operator nativeType()
	{
		return vec;
	}
	mVecCall operator nativeType() const
	{
		return vec;
	}
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector2f";
	}


	mVecInline elementType operator ()(const common::size32 index, const elementType element)
	{
		return vec[index] = element;
	}
};

static constexpr auto test = real32x4(vector2f(2.0f, 2.0f));

__align(16) struct vector3f
{
	union
	{
		real32x4 vec;
		vec_t arr[4];
	};
	using vecType		=	vector3f;
	using nativeType	=	real32x4;
	using elementType 	=	common::real32;
	vec__defnOpIndex()

	#if !mNoVectorDefault
		mVecCall vector3f() : vec{} {}
	#endif

	mVecCall vector3f(const vec_t __x, const vec_t __y, const vec_t __z) : vec{__x, __y, __z, 1.0f}
	{
	}

	mVecCall vector3f(const vec_t* RESTRICT const other) : vec{other[0], other[1], other[2], 1.0f}
	{
	}

	mVecInline vecType operator =(const vecType other)
	{
		return vec = other.vec;
	}
	mVecCall vector3f(const real32x4 other) : vec(other) {}

	mVecPureOp * (const vec_t scalar) const
	{
		return vecType(vec[0] * scalar, vecIndexValue(1) * scalar, vecIndexValue(2) * scalar);
	}

	mVecPureOp / (const vec_t scalar) const
	{
		return vecType(vec[0] / scalar, vecIndexValue(1) / scalar, vecIndexValue(2) / scalar);
	}

	mVecPureOp *(const real32x4 other) const
	{
		return _mm_mul_ps(vec, other);
	}

	mVecPureOp /(const real32x4 other) const
	{
		return _mm_div_ps(vec, other);
	}

	mVecPureOp +(const real32x4 other) const
	{
		return _mm_add_ps(vec, other);
	}
	mVecPureOp -(const real32x4 other) const
	{
		return _mm_sub_ps(vec, other);
	}

	mVecPureOp + (const vecType rhs) const
	{
		return vecType(vec[0] + rhs[0], vecIndexValue(1) + rhs[1], vecIndexValue(2) + rhs[2]);
	}

	mVecPureOp - (const vecType rhs) const
	{
		return vecType(vec[0] - rhs[0], vecIndexValue(1) - rhs[1], vecIndexValue(2) - rhs[2]);
	}

	mVecPure vecType abs() const
	{
		#if !defined(__clang__)
			const __m128i bits = _mm_castps_si128(vec);
			return _mm_castsi128_ps(
					_mm_and_si128(bits, _mm_set1_epi32(math::fabsMask))
			);
		#else
			static const uint32x4 vec3Fabs = (uint32x4)
			{
				math::fabsMask,
				math::fabsMask,
				math::fabsMask,
				math::fabsMask
			};
			return vector_cast<real32x4>(vector_cast<uint32x4>(vec) & vec3Fabs);
		#endif
	}



	mVecCall operator vec_t*()
	{
		return &arr[0];
	}

	mVecCall operator const vec_t*() const
	{
		return &arr[0];
	}

	mVecCall operator nativeType()
	{
		return vec;
	}
	mVecCall operator nativeType() const
	{
		return vec;
	}
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector3f";
	}

	mVecInline elementType operator ()(const common::size32 index, const elementType element)
	{
		return vec[index] = element;
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
		real32x4 vec;
		vec_t arr[4];
	};
	using vecType		=	vector4f;
	using nativeType	=	real32x4;
	using elementType 	=	common::real32;
	vec__defnOpIndex()

	#if !mNoVectorDefault
		mVecCall vector4f() : vec{} {}
	#endif

	mVecCall vector4f(const vec_t __x, const vec_t __y, const vec_t __z, const vec_t __w) : vec{__x, __y, __z, __w}
	{
	}

	mVecCall vector4f(const vec_t* RESTRICT const other) : vec{other[0], other[1], other[2], other[3]}
	{
	}
	mVecCall vector4f(const real32x4 other) : vec(other) {}

	mVecPureOp * (const vec_t scalar) const
	{
		return vecType(vec[0] * scalar, vecIndexValue(1) * scalar, vecIndexValue(2) * scalar, vecIndexValue(3) * scalar);
	}

	mVecPureOp / (const vec_t scalar) const
	{
		return vecType(vec[0] / scalar, vecIndexValue(1) / scalar, vecIndexValue(2) / scalar, vecIndexValue(3) / scalar);
	}

	mVecPureOp *(const real32x4 other) const
	{
		return _mm_mul_ps(vec, other);//vec * other;
	}

	mVecPureOp /(const real32x4 other) const
	{
		return _mm_div_ps(vec, other);//vec / other;
	}

	mVecPureOp +(const real32x4 other) const
	{
		return _mm_add_ps(vec, other);//vec + other;
	}
	mVecPureOp -(const real32x4 other) const
	{
		return _mm_sub_ps(vec, other);//vec - other;
	}
	mVecPure vecType abs() const
	{
	#if !defined(__clang__)
		const __m128i bits = _mm_castps_si128(vec);
		return _mm_castsi128_ps(
				_mm_and_si128(bits, _mm_set1_epi32(math::fabsMask))
		);
	#else
		static const uint32x4 vec3Fabs = (uint32x4)
		{
			math::fabsMask,
			math::fabsMask,
			math::fabsMask,
			math::fabsMask
		};
		return vector_cast<real32x4>(vector_cast<uint32x4>(vec) & vec3Fabs);
	#endif
	}

	__pure mVecInline vecmask32x4 getSignMasks() const
	{
		static const vecmask32x4 vec4Masks = (vecmask32x4)
		{
			~math::fabsMask,
			~math::fabsMask,
			~math::fabsMask,
			~math::fabsMask
		};
		return vector_cast<vecmask32x4>(vec) & vec4Masks;
	}

	mVecPure vecmask32x4 operator >(const vecType rhs) const
	{
		return vec > rhs.vec;
	}

	mVecPure vecmask32x4 operator <(const vecType rhs) const
	{
		return vec < rhs.vec;
	}

	mVecPure vecmask32x4 operator <=(const vecType rhs) const
	{
		return vec <= rhs.vec;
	}

	mVecPure vecmask32x4 operator >=(const vecType rhs) const
	{
		return vec >= rhs.vec;
	}

	mVecCall operator vec_t*()
	{
		return &arr[0];
	}

	mVecCall operator const vec_t*() const
	{
		return &arr[0];
	}

	mVecCall operator nativeType()
	{
		return vec;
	}
	mVecCall operator nativeType() const
	{
		return vec;
	}
	mVecInline vecType operator =(const vecType other)
	{
		return vec = other.vec;
	}
	mVecInline elementType operator ()(const common::size32 index, const elementType element)
	{
		return vec[index] = element;
	}
};

#if mNativeVectorSize < 32
	#include "vec256/vec256_x86_noavx.hpp"
#else
	#include "vec256/vec256_x86_avx.hpp"
#endif

#if defined(__SSE4A__)
	template<typename T>
	__pure __forceinline __nodebug
	static auto extrqi(const T bitfield, const common::int8 bitindex, const common::int8 length)
	{
		return __builtin_ia32_extrqi(
				_Generic(bitfield,
						int8x16:
						((__v2di)(__m128i)bitfield),
						uint8x16:
						((__v2di)(__m128i)bitfield),
						int16x8:
						((__v2di)(__m128i)bitfield),
						uint16x8:
						((__v2di)(__m128i)bitfield),
						int32x4:
						((__v2di)(__m128i)bitfield),
						uint32x4:
						((__v2di)(__m128i)bitfield),
						real32x4:
						((__v2di)(__m128i)bitfield),
						int8x8:
						((__v2di)(__m128i)bitfield),
						uint8x8:
						((__v2di)(__m128i)bitfield),
						int16x4:
						((__v2di)(__m128i)bitfield),
						uint16x4:
						((__v2di)(__m128i)bitfield),
						int32x2:
						((__v2di)(__m128i)bitfield),
						uint32x2:
							((__v2di)(__m128i)bitfield),
						default:
							((__v2di)(__m128i)bitfield.vec)
				),
				length,
				bitindex
		);
	}
	template<typename T1, typename T2>
	__pure __forceinline __nodebug
	static auto insertqi(T1 toInsertInto, const T2 inserted, const common::int8 bitindex, const common::int8 length)
	{
		return __builtin_ia32_insertqi(
				_Generic(toInsertInto,
						int8x16:
						((__v2di)(__m128i)toInsertInto),
						uint8x16:
						((__v2di)(__m128i)toInsertInto),
						int16x8:
						((__v2di)(__m128i)toInsertInto),
						uint16x8:
						((__v2di)(__m128i)toInsertInto),
						int32x4:
						((__v2di)(__m128i)toInsertInto),
						uint32x4:
						((__v2di)(__m128i)toInsertInto),
						real32x4:
						((__v2di)(__m128i)toInsertInto),
						int8x8:
						((__v2di)(__m128i)toInsertInto),
						uint8x8:
						((__v2di)(__m128i)toInsertInto),
						int16x4:
						((__v2di)(__m128i)toInsertInto),
						uint16x4:
						((__v2di)(__m128i)toInsertInto),
						int32x2:
						((__v2di)(__m128i)toInsertInto),
						uint32x2:
							((__v2di)(__m128i)toInsertInto),
						default:
							((__v2di)(__m128i)toInsertInto.vec)
				),
				_Generic(inserted,
						int8x16:
						((__v2di)(__m128i)inserted),
						uint8x16:
						((__v2di)(__m128i)inserted),
						int16x8:
						((__v2di)(__m128i)inserted),
						uint16x8:
						((__v2di)(__m128i)inserted),
						int32x4:
						((__v2di)(__m128i)inserted),
						uint32x4:
						((__v2di)(__m128i)inserted),
						real32x4:
						((__v2di)(__m128i)inserted),
						int8x8:
						((__v2di)(__m128i)inserted),
						uint8x8:
						((__v2di)(__m128i)inserted),
						int16x4:
						((__v2di)(__m128i)inserted),
						uint16x4:
						((__v2di)(__m128i)inserted),
						int32x2:
						((__v2di)(__m128i)inserted),
						uint32x2:
							((__v2di)(__m128i)inserted),
						default:
							((__v2di)(__m128i)inserted.vec)
				),
				length,
				bitindex
		);
	}
#endif
