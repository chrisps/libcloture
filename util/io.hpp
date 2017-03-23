#pragma once


namespace cloture		{
namespace util			{
namespace stream		{

using templates::variadic::isValidVArg;

enum class Radix : common::uint8
{
	decimal,
	hex
};

#define		ostreamFlexible		1

#if ostreamFlexible
	template<typename funcType, funcType outputFunc,//void (*outputFunc)(const char*, ...),
	typename... userDataTypes>
#else
	template<void (*output_func)(const char*, ...)>
#endif
//
class ostream
{
	Radix rad;

	#if ostreamFlexible
	using udata1 = getNthTypename(1, userDataTypes);
	udata1 ud1;
	#endif
public:
	#if ostreamFlexible
		template<typename... args>
		inline void output_func(const char* s, args... Args)
		{
		choose_expr
		(
			isValidVArg<udata1>,
			outputFunc(ud1, s, Args...),
			outputFunc(s, Args...)
		);
		}
	#endif
	ostream& operator <<(const bool b)
	{
		if(b)
			output_func("true");
		else
			output_func("false");
		return *this;
	}

	ostream& operator <<(Radix radix)
	{
		rad = radix;
		return *this;
	}
	ostream& operator <<(const common::uint8 b)
	{
		switch(rad)
		{
		case Radix::decimal:
			output_func("%hhu", b);
			break;
		case Radix::hex:
			output_func("0x%hhX", b);
			break;
		};
		return *this;
	}
	ostream& operator <<(const common::uint16 b)
	{
		switch(rad)
		{
		case Radix::decimal:
			output_func("%hu", b);
			break;
		case Radix::hex:
			output_func("0x%hX", b);
			break;
		};
		return *this;
	}
	ostream& operator <<(const common::uint32 b)
	{
		switch(rad)
		{
		case Radix::decimal:
			output_func("%iu", b);
			break;
		case Radix::hex:
			output_func("0x%iX", b);
			break;
		};
		return *this;
	}
	ostream& operator <<(const common::uint64 b)
	{
		switch(rad)
		{
		case Radix::decimal:
			output_func("%llu", b);
			break;
		case Radix::hex:
			output_func("0x%llX", b);
			break;
		};
		return *this;
	}
	/**
	 * signed int types
	 */
	ostream& operator <<(const common::int8 b)
	{
		switch(rad)
		{
		case Radix::decimal:
			output_func("%hhi", b);
			break;
		case Radix::hex:
			output_func("0x%hhX", b);
			break;
		};
		return *this;
	}
	ostream& operator <<(const common::int16 b)
	{
		switch(rad)
		{
		case Radix::decimal:
			output_func("%hi", b);
			break;
		case Radix::hex:
			output_func("0x%hX", b);
			break;
		};
		return *this;
	}
	ostream& operator <<(const common::int32 b)
	{
		switch(rad)
		{
		case Radix::decimal:
			output_func("%d", b);
			break;
		case Radix::hex:
			output_func("0x%dX", b);
			break;
		};
		return *this;
	}
	ostream& operator <<(const common::int64 b)
	{
		switch(rad)
		{
		case Radix::decimal:
			output_func("%lli", b);
			break;
		case Radix::hex:
			output_func("0x%llX", b);
			break;
		};
		return *this;
	}
	ostream& operator <<(const char* b)
	{
		output_func("%s", b);
		return *this;
	}
	ostream& operator <<(void* b)
	{
		constexpr char* fmt = 
		choose_expr
		(
			sizeof(void*) == 8,
			"0x%llX",
			"0x%iX"
		);
		
		output_func(fmt, b);
		return *this;
	}
	
};//class ostream

__if_exists(sprintf)
{
	template<size_t arraysize = common::unone>
	class sstream : public ostream<decltype(sprintf), sprintf, char*>
	{

		template<size_t sz>
		struct dynhelper
		{
			using T = char[sz];
		};

		template<>
		struct dynhelper<common::unone>
		{
			using T = char*;
		};

		using stringType = typename dynhelper<arraysize>::T;
		stringType s;

		static constexpr bool isAllocedString = arraysize == common::unone;

		template<bool isAllocedString_>
		void __CONSTRUCTOR()
		{
		}

		template<> void __CONSTRUCTOR<false>()
		{
			__builtin_memset(s, 0, arraysize);
		}

		template<> void __CONSTRUCTOR<true>()
		{
			s = nullptr;
		}

		template<bool isAllocedString_>
		constexpr void __COPYER(const char* other)
		{
		}

		template<>
		constexpr void __COPYER<false>(const char* other)
		{
			__builtin_strcpy(s, other);
		}
		template<>
		constexpr void __COPYER<true>(const char* other)
		{
			s = other;
		}
	public:

		sstream()
		{
			__CONSTRUCTOR<isAllocedString>();
		}

		constexpr void operator =(const char* other)
		{
			__COPYER<isAllocedString>(other);
		}

	};
}
}//namespace stream
}//namespace util
}//namespace cloture
