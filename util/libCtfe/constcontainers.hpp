#pragma once

namespace cloture::util::ctfe
{
	/**
		basic pair class
	*/
	template<typename t1, typename t2>
	struct pair : __markAsCtfe()
	{
		t1 first;
		t2 second;

		constexpr __pure pair() : first(), second()
		{}

		constexpr __pure pair(t1 f, t2 s) : first(f), second(s)
		{}
	};

	/**
		ctfe::Array
			compile-time array copyable array
			has *all* of the bells and whistles
			all of them

		i think there's something wrong with this class. I can't recall what exactly the issue is, but...
		i guess we'll find out!
	*/
	template<typename T_, size_t sz = 0>
	class Array : __markAsCtfe()
	{
	public:
		using T = typename generic::removeConst<T_>::type;
		T data[sz + 1];

		constexpr size_t size() const
		{
			return sz;
		}
		constexpr bool empty() const
		{
			return size() == 0;
		}

		constexpr Array() : data(){}

		constexpr auto& operator[](const size_t index)
		{
			return data[index];
		}

		constexpr auto operator[](const size_t index) const
		{
			return data[index];
		}
		constexpr Array(const Array<T, sz>& other) : data()
		{
			for(size_t i = 0; i < sz; ++i)
				data[i]		=		other[i];
		}

		template<size_t otherSize>
		constexpr Array(const T (&other)[otherSize]) : data()
		{
			for(size_t i = 0; i < sz && i < otherSize; ++i)
					this->data[i]		=		other[i];
		}
		template<typename T>
		constexpr Array(T other) : data()
		{
			for(size_t i = 0; i < sz; ++i)
				data[i]		=		other[i];
		}



	/*	constexpr bool operator==(const Array<T, sz>& other) const
		{
			for(size_t i = 0; i < sz; ++i)
			{
				if(other[i] != (*this)[i])
					return false;
			}
			return true;
		}*/
/*
		constexpr bool operator!=(const Array<T, sz>& other) const
		{
			return !((*this) == other);
		}*/

		template<typename T2> constexpr bool operator ==(const T2 other) const
		{
			for(size_t i = 0; i < sz; ++i)
			{
				if(other[i] != data[i])
					return false;
			}
			return true;
		}
	
		template<typename T2>
		constexpr bool operator!=(const T2 other) const 
		{
			return !((*this) == other);
		}

		constexpr Array<T, sz>& operator=(const T* const RESTRICT rhs)
		{
			for(size_t i = 0; i < sz; ++i)
				data[i]		=	rhs[i];
			return (*this);
		}
		constexpr Array<T, sz>& operator=(const Array<T, sz>& rhs)
		{
			for(size_t i = 0; i < sz; ++i)
				data[i]		=	rhs[i];
			return (*this);
		}
		explicit constexpr operator T*()		{return data;}
		explicit constexpr operator const T*()	{return data;}
	};

	template<size_t sz>
	class CString : public Array<char, sz>, __markAsCtfe()
	{

	public:
		static constexpr size_t length_ = sz - 1;
		constexpr size_t length() const
		{
			/*size_t i = 0;
			for(; i < sz && ((*this)[i]) != 0; ++i)	;
			return i;*/
			return length_;
		}

		explicit constexpr operator const char*() const	{return &this->data[0];;}
		constexpr CString() : Array<char, sz>::Array()	{}


		template<size_t otherSize>
		constexpr CString(const char (&other)[otherSize])
		{
			for(size_t i = 0; i < sz && i < otherSize; ++i)
					this->data[i]		=		other[i];
		}

		constexpr const char operator[](const size_t idx) const
		{
			return this->data[idx];
		}

		constexpr char& operator[](const size_t idx)
		{
			return this->data[idx];
		}
	};

	template<size_t sz, bool reverse = false>
	class charStream : public CString<sz>,  __markAsCtfe()
	{

	};

	template<size_t sz>
	class charStream<sz, false> : public CString<sz>, __markAsCtfe()
	{
		size_t position;
	public:
		constexpr charStream() : CString<sz>::CString(), position(0)
		{}

		constexpr const bool endReached() const
		{
			return position >= sz;
		}
		constexpr char operator<<(const char c)
		{
			if( !endReached() )
			{
				(*this)[position]	=	c;
				++position;
			}
			return c;
		}

		constexpr char operator>>(char& c)
		{
			c = -1;
			if( !endReached() )
			{
				c	=	(*this)[position];
				++position;
			}
			return c;
		}

		constexpr const bool operator!() const
		{
			return endReached();
		}

		constexpr operator bool() const
		{
			return !endReached();
		}
		constexpr void reset()
		{
			position = 0;
		}
	};


	template<size_t sz>
	class charStream<sz, true> : public CString<sz>, __markAsCtfe()
	{
		size_t position;
	public:
		constexpr charStream() : CString<sz>::CString(), position(sz - 1)	{}

		constexpr const bool endReached() const
		{
			return static_cast<ptrdiff_t>( position ) < 0;
		}

		constexpr char operator<<(const char c)
		{
			if( endReached() )
				return c;
			(*this)[position]		=	c;
			--position;
			return c;
		}
		constexpr char operator>>(char& c)
		{
			c = -1;
			if( !endReached() )
			{
				c	=	(*this)[position];
				--position;
			}
			return c;
		}
		constexpr const bool operator!() const
		{
			return endReached();
		}

		constexpr operator bool() const
		{
			return !endReached();
		}
		constexpr void reset()
		{
			position = sz - 1;
		}
	};

	/*
	 * packs a string into a 128 bit int
	 */
#if 0
	template<typename T>
	static constexpr T packString(const char* s, const size_t startIndex)
	{
		constexpr T baseShiftOffset = common::bitSizeof<T> - common::bitSizeof<char>;
		constexpr T ZERO = static_cast<T>(0);
		constexpr T SHIFTDECREMENT = common::bitSizeof<char>;
		T result = ZERO;

		T shiftfactor = baseShiftOffset;

		if(!s[startIndex])
			return result;

		for(size_t i = startIndex; s[i] != 0 && (shiftfactor >= 0 && shiftfactor < (baseShiftOffset + 1));
		++i, shiftfactor -= SHIFTDECREMENT)
		{
			T val = ZERO;
			val = static_cast<unsigned char>(s[i]);
			result |= (val << shiftfactor);
		}
		return result;
	}
#else
	template<char (* oper)(char), typename T = unsigned __int64>
	static constexpr T packString(const char* s, const size_t startIndex)
	{
		constexpr T baseShiftOffset = common::bitSizeof<T> - common::bitSizeof<char>;
		constexpr T ZERO = static_cast<T>(0);
		constexpr T SHIFTDECREMENT = common::bitSizeof<char>;
		T result = ZERO;

		T shiftfactor = baseShiftOffset;

		if(!s[startIndex])
			return result;

		for(size_t i = startIndex; s[i] != 0 && (shiftfactor >= 0 && shiftfactor < (baseShiftOffset + 1));
			++i, shiftfactor -= SHIFTDECREMENT)
		{
			T val = ZERO;
			val = static_cast<unsigned char>(oper(s[i]));
			result |= (val << shiftfactor);
		}
		return result;
	}
#endif
	template<typename T_, T_... packedVals_>
	struct packedString : __markAsCtfe()
	{
		template<typename T, T...packedVals>
		struct PACKED : __markAsCtfe()
		{
			using packType = templates::variadic::paramPack<packedVals...>;

			static constexpr packType pack = packType();

			static constexpr size_t indexScale 		= sizeof(T) / sizeof(char);
			static constexpr size_t baseShiftOffset = common::bitSizeof<T> - common::bitSizeof<char>;

			static constexpr T charMask = static_cast<T>(0xFF);

			static constexpr size_t charShiftIndex(const size_t originalIndex)
			{
				const size_t originalMod 	= originalIndex % indexScale;
				const size_t bitoffset 		= originalMod * common::bitSizeof<char>;
				return baseShiftOffset - bitoffset;
			}

			static constexpr size_t toPackedIndex(const size_t charIndex)
			{
				return charIndex / indexScale;
			}
			constexpr PACKED() {}
			static constexpr size_t packedStrlen()
			{
				size_t i = 0;

				while(true)
				{
					T indexed = pack[toPackedIndex(i)];//pack::arr[ toPackedIndex(i)];
					indexed >>= charShiftIndex(i);
					const char c = static_cast<char>(indexed);
					if(c == 0)
						break;
					++i;
				}
				return i;
			}
			static constexpr size_t stringSize = packedStrlen();



			//constexpr char operator []
			static constexpr char atIndex(const size_t index) //const
			{
				T val = pack[ toPackedIndex(index)];
				val >>= charShiftIndex(index);
				val &= charMask;
				return static_cast<char>(val);
			}

			static constexpr auto //CString<stringSize + 1>
			generateCString()
			{
				char temparray[stringSize + 1] = {};

				for(size_t i = 0; i < stringSize; ++i)
					temparray[i] = atIndex(i);

				auto result = CString<stringSize + 1>(temparray);
				return result;
			}

		}__unused;
		constexpr packedString() {}
		using PACKER = PACKED<T_, packedVals_...>;
		static constexpr auto str = PACKER::generateCString();
	};
#if !defined(NO_INT128_IMPL)
	//using __CONST_STRING_ELEMENT_TYPE	=	unsigned __int128;
#define __CONST_STRING_ELEMENT_TYPE unsigned __int128
#else
	//using __CONST_STRING_ELEMENT_TYPE	=	unsigned __int64;
#define __CONST_STRING_ELEMENT_TYPE unsigned __int64
#endif
#define mStrIndexScale		sizeof(__CONST_STRING_ELEMENT_TYPE)
#if 0
	#if 0

	#define mConstString16(str, i) \
		(sizeof(str) > ((i) * mStrIndexScale) ? packString<unsigned __int128>(str, ((i) * mStrIndexScale)) : 0)

	#else
		#define mConstString16(str, i) 								\
			(choose_expr(											\
			sizeof(str) > ((i) * mStrIndexScale),					\
			cloture::util::ctfe::packString<__CONST_STRING_ELEMENT_TYPE>(		\
			str, ((i) * mStrIndexScale)),							\
			0))
	#endif


	#define mConstString64(str, i) \
		mConstString16(str, i),  \
		mConstString16(str, i+1),  \
		mConstString16(str, i+2),  \
		mConstString16(str, i+3)

	#define	mConstString128(str, i)	\
		mConstString64(str, i),		\
		mConstString64(str, i+4)

	#define mConstString256(str, i) 	\
		mConstString64(str, i),   		\
		mConstString64(str, i+4),   	\
		mConstString64(str, i+8),   	\
		mConstString64(str, i+12)

	#define mConstString1024(str, i) 	\
		mConstString256(str, i),  		\
		mConstString256(str, i+16), 	\
		mConstString256(str, i+32), 	\
		mConstString256(str, i+48)

	#define mConstString4096(str, i) 	\
		mConstString1024(str, i), 		\
		mConstString1024(str, i+64), 	\
		mConstString1024(str, i+128), 	\
		mConstString1024(str, i+192)

	#define mConstString16384(str, i) 	\
		mConstString4096(str, i), 		\
		mConstString4096(str, i+256), 	\
		mConstString4096(str, i+512), 	\
		mConstString4096(str, i+768)

	/*
	 * this is terrifying
	 * i'm tempted to test this one
	 */
	#define mConstString65536(str, i) 	\
		mConstString4096(str, i), 		\
		mConstString4096(str, i+1024), 	\
		mConstString4096(str, i+2048), 	\
		mConstString4096(str, i+3072)

	#define	mIndirectConstString(prefix, sz, str)	\
		prefix##sz(str, 0)

	#define	mMetaString(string, expectedSize)	\
	cloture::util::ctfe::packedString<__CONST_STRING_ELEMENT_TYPE, \
	mIndirectConstString(mConstString, expectedSize, string) >

#else

#define mConstString16(oper, str, i) 								\
			(choose_expr(											\
			sizeof(str) > ((i) * mStrIndexScale),					\
			cloture::util::ctfe::packString<oper>(		\
			str, ((i) * mStrIndexScale)),							\
			0))


#define mConstString64(oper, str, i) \
		mConstString16(oper, str, i),  \
		mConstString16(oper, str, i+1),  \
		mConstString16(oper, str, i+2),  \
		mConstString16(oper, str, i+3)

#define	mConstString128(oper, str, i)	\
		mConstString64(oper, str, i),		\
		mConstString64(oper, str, i+4)

#define mConstString256(oper, str, i) 	\
		mConstString64(oper, str, i),   		\
		mConstString64(oper, str, i+4),   	\
		mConstString64(oper, str, i+8),   	\
		mConstString64(oper, str, i+12)

#define mConstString1024(oper, str, i) 	\
		mConstString256(oper, str, i),  		\
		mConstString256(oper, str, i+16), 	\
		mConstString256(oper, str, i+32), 	\
		mConstString256(oper, str, i+48)

#define mConstString4096(oper, str, i) 	\
		mConstString1024(oper, str, i), 		\
		mConstString1024(oper, str, i+64), 	\
		mConstString1024(oper, str, i+128), 	\
		mConstString1024(oper, str, i+192)

#define mConstString16384(oper, str, i) 	\
		mConstString4096(oper, str, i), 		\
		mConstString4096(oper, str, i+256), 	\
		mConstString4096(oper, str, i+512), 	\
		mConstString4096(oper, str, i+768)

	/*
	 * this is terrifying
	 * i'm tempted to test this one
	 */
#define mConstString65536(oper, str, i) 	\
		mConstString4096(oper, str, i), 		\
		mConstString4096(oper, str, i+1024), 	\
		mConstString4096(oper, str, i+2048), 	\
		mConstString4096(oper, str, i+3072)

	constexpr char defaultOper(char i) {return i;}

#define	mIndirectConstString(oper, prefix, sz, str)	\
		prefix##sz(oper, str, 0)

#define mMetaStringOp(oper, string, expectedSize)\
	cloture::util::ctfe::packedString<__CONST_STRING_ELEMENT_TYPE, \
	mIndirectConstString(oper, mConstString, expectedSize, string) >

#define	mMetaString(string, expectedSize)	mMetaStringOp(cloture::util::ctfe::defaultOper, string, expectedSize)



#endif
}


