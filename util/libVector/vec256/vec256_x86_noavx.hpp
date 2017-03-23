#pragma once

#define		mVec256Align	32

template<typename T> struct pickVecType{using type = void;};

#define		pickVecTypeSpecial(typeName)	\
		template<>struct pickVecType<common::typeName>	\
		{\
			typedef common::typeName type __attribute__((__vector_size__(16)));\
		}

	pickVecTypeSpecial(int8);
	pickVecTypeSpecial(uint8);

	pickVecTypeSpecial(int16);
	pickVecTypeSpecial(uint16);

	pickVecTypeSpecial(int32);
	pickVecTypeSpecial(uint32);

	pickVecTypeSpecial(int64);
	pickVecTypeSpecial(uint64);

	pickVecTypeSpecial(real32);
	pickVecTypeSpecial(real64);

#undef pickVecTypeSpecial

template<typename elementType, typename inputType, typename... others>
class initVecHelper
{
	template<typename eleType, size_t paramcount, typename inputType_, typename... others_>
	struct init
	{

		//mVecInline __pure static void doInit
	};
	template<typename eleType, typename inputType_>
	struct init<
		eleType,
		2,
		inputType_,
		typename pickVecType<eleType>::type,
		typename pickVecType<eleType>::type
	>
	{
		using paramType = typename pickVecType<eleType>::type;
		mVecCall __pure static void doInit(inputType out1, inputType out2, paramType in1, paramType in2)
		{
			out1 = in1, out2 = in2;
		}
	};
public:
	using initializer = init<elementType, sizeof...(others), inputType, others...>;
};

template<typename T, typename... others>
mVecCall __pure void initVec(
		typename pickVecType<T>::type& out1,
		typename pickVecType<T>::type& out2,
		others... Others
)
{
	using initializer = typename initVecHelper<T, __typeof__(out1), others...>::initializer;
	initializer::doInit(out1, out2, Others...);
}

template<typename T>
__align(mVec256Align) struct vector256
{

	union
	{
		struct
		{
			__m128 vec1;
			__m128 vec2;
		};
		vec_t arr[8];
	};
	mMarkAggregateVec();
	using vecType = vector256<T>;
	using constele = const __typeof(vec1[0]);

	constexpr mVecPure vector256(
	constele v0, constele v1, constele v2, constele v3,
	constele v4, constele v5, constele v6, constele v7
	)
	{
		vec1 = (__typeof(vec1)){v0, v1, v2, v3};
		vec2 = (__typeof(vec2)){v4, v5, v6, v7};
	}

	constexpr mVecPure vector256(const __typeof(vec1) v0, const __typeof(vec2) v1)
	{
		vec1 = v0;
		vec2 = v1;
	}

	constexpr mVecPure vector256(constele v0)
	{
		vec2 = vec1 = (__typeof(vec1)){v0, v0, v0, v0};
	}

	constexpr mVecPure vector256(const __typeof(vec1[0])* RESTRICT const vIn)
	{
		vec1 = (__typeof(vec1)){vIn[0], vIn[1], vIn[2], vIn[3]};
		vec2 = (__typeof(vec1)){vIn[4], vIn[5], vIn[6], vIn[7]};
	}
};

/*
 * vector of 16 16 bit ints
 */
template<typename T>
 __align(mVec256Align)
struct vector16i16
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
	using elementType	=	T;
	nativeType vec1;
	nativeType vec2;

	using vecType = vector16i16<T>;

	mVecCall vector16i16(const T dupVal)
	{
		vec1[0] = vec1[1] = vec1[2] = vec1[3] =
		vec1[4] = vec1[5] = vec1[6] = vec1[7] =
		vec2[0] = vec2[1] = vec2[2] = vec2[3] =
		vec2[4] = vec2[5] = vec2[6] = vec2[7] = dupVal;
	}

	mVecCall vector16i16(
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
		vec1[0] = v0;
		vec1[1] = v1;
		vec1[2] = v2;
		vec1[3] = v3;
		vec1[4] = v0;
		vec1[5] = v1;
		vec1[6] = v2;
		vec1[7] = v3;

		vec2[0] = v8;
		vec2[1] = v9;
		vec2[2] = v10;
		vec2[3] = v11;
		vec2[4] = v12;
		vec2[5] = v13;
		vec2[6] = v14;
		vec2[7] = v15;
	}

	mVecCall vector16i16(const T* RESTRICT const vIn)
	{
		vec1[0] = vIn[0];
		vec1[1] = vIn[1];
		vec1[2] = vIn[2];
		vec1[3] = vIn[3];
		vec1[4] = vIn[4];
		vec1[5] = vIn[5];
		vec1[6] = vIn[6];
		vec1[7] = vIn[7];

		vec2[0] = vIn[8];
		vec2[1] = vIn[9];
		vec2[2] = vIn[10];
		vec2[3] = vIn[11];
		vec2[4] = vIn[12];
		vec2[5] = vIn[13];
		vec2[6] = vIn[14];
		vec2[7] = vIn[15];
	}

	mVecCall vector16i16(const nativeType vIn1, const nativeType vIn2)
	{
		vec1 = vIn1;
		vec2 = vIn2;
	}

	mVecPureOp +(const vecType rhs) const
	{
		return vecType(vec1 + rhs.vec1, vec2 + rhs.vec2);
	}
	mVecPureOp -(const vecType rhs) const
	{
		return vecType(vec1 - rhs.vec1, vec2 - rhs.vec2);
	}
	mVecPureOp *(const vecType rhs) const
	{
		return vecType(vec1 * rhs.vec1, vec2 * rhs.vec2);
	}
	mVecPureOp /(const vecType rhs) const
	{
		return vecType(vec1 / rhs.vec1, vec2 / rhs.vec2);
	}
	mVecPureOp &(const vecType rhs) const
	{
		return vecType(vec1 & rhs.vec1, vec2 & rhs.vec2);
	}
	mVecPureOp |(const vecType rhs) const
	{
		return vecType(vec1 | rhs.vec1, vec2 | rhs.vec2);
	}
	mVecPureOp ^(const vecType rhs) const
	{
		return vecType(vec1 ^ rhs.vec1, vec2 ^ rhs.vec2);
	}
	mVecPureOp >>(const T rhs) const
	{
		const nativeType shiftVec = (nativeType){rhs, rhs, rhs, rhs, rhs, rhs, rhs, rhs};
		return vecType(vec1 >> shiftVec, vec2 >> shiftVec);
	}

	mVecPureOp <<(const T rhs) const
	{
		const nativeType shiftVec = (nativeType){rhs, rhs, rhs, rhs, rhs, rhs, rhs, rhs};
		return vecType(vec1 << shiftVec, vec2 << shiftVec);
	}

	mVecPureOp >>(const vecType rhs) const
	{
		return vecType(vec1 >> rhs.vec1, vec2 >> rhs.vec2);
	}
	mVecPureOp <<(const vecType rhs) const
	{
		return vecType(vec1 << rhs.vec1, vec2 << rhs.vec2);
	}
	mVecPureOp ~()
	{
		return vecType(~vec1, ~vec2);
	}
	mVecPureOp -()
	{
		return vecType(-vec1, -vec2);
	}


	mVecInline __pseudopure

	T operator[](const common::size32 index) const
	__enableIf(index < (sizeof(nativeType) / sizeof(T)),
	"Enabled if index is into the first vector.")
	{
		return vec2[index];
	}


	mVecInline
	__pseudopure
	T operator[](const common::size32 index) const
	__enableIf(index >= (sizeof(nativeType) / sizeof(T)), "Enabled if index goes into the second vector.")

	{
		return vec2[index - static_cast<common::size32>(sizeof(nativeType) / sizeof(T))];
	}

	mVecInline __pseudopure T& operator[](const common::size32 index)
	{
		return reinterpret_cast<T*>(this)[index];
	}
	mVecCall operator nativeType()
	{
		return vec1;
	}
	mVecCall operator nativeType() const
	{
		return vec1;
	}
};

/*
 * vector of 16 signed shorts
 */
struct vector16ss : public vector16i16<signed short>
{
	using vector16i16::vector16i16;
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector16ss";
	}
};
/*
 * vector of 16 unsigned shorts
 */
struct vector16us : public vector16i16<unsigned short>
{
	using vector16i16::vector16i16;
	static constexpr auto clotureTypeName()
	{
		using namespace cloture::util::ctfe;
		return "vector16us";
	}
};

static_assert(isAggregateVector<
vector256<float>
 >);


