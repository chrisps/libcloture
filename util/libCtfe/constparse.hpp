#pragma once

namespace cloture::util::ctfe::parser
{
	template<typename T> constexpr auto		parse(		const char* text	){	return nullptr;	}
	template<typename T> constexpr bool		parseable(	const char* text	){	return false;	}
	template<typename T> constexpr size_t	skip(		const char* text	){	return unone;	}


	template<> constexpr auto
		parse<	common::int32	>(	const char* text	)
	{
		size_t i 			=	0;
		bool negative 		=	false;
		common::int32 value	=	0;

		if(	text[i] == '-'	)
		{
			negative = true;
			++i;
		}
		/*
			read each digit
		*/
		bool firstIteration = true;
		for(char c = 0; 0 <= (c = charToInt( text[i] ) ) && c <= 9; ++i)
		{
			if(!firstIteration)
				value *= 10;
			value += c;
			if(firstIteration)
				firstIteration = false;
		}

		return negative ? -value : value;
	}
	static_assert
	(
			parse<common::int32>("-1000")	==	-1000
		&&	parse<common::int32>("530395")	== 	530395
		&& 	parse<common::int32>("45")		==	45
		&& 	parse<common::int32>("20000")	==	20000,
		"parser::parse<int32> is not returning correct results"
	);

	template<> constexpr bool
		parseable<common::real32>(const char* text)
	{
		size_t i = 0;

		if( isWhitespace(text[i]) || !text[i])
			return false;

		char c = charToInt(text[i]);
		const bool negative		=	text[i] == '-';

		/*	not the sign, not a decimal point, not a number	*/
		if(!negative && text[i] != '.' && (0 > c || c > 9))
			return false;

		bool foundPoint		=	false;
		size_t numDigits	=	0;
		if(negative)	++i;
		while(true)
		{
			if( text[i] == '.' )
			{

				if(foundPoint)	/*	two decimal points? that aint right	*/
					return false;

				foundPoint = true;

				++i;
				continue;
			}

			c = charToInt(text[i]);

			if(isWhitespace(text[i]) || (0 > c || c > 9) && !isIdentifierChar(text[i]))
				return numDigits != 0;

			++i, ++numDigits;
		}
		return false;
	}

	static_assert
	(
			parseable<common::real32>("-200.056932")
		&&	parseable<common::real32>("3.141592654")
		&& 	parseable<common::real32>("2.718281828")
		&& 	parseable<common::real32>("666.666666"),
		"parser::parseable<real32> incorrectly returned false."
	);

	static_assert(
			!parseable<common::real32>("YO WUTS GOOD.")
		&&	!parseable<common::real32>("      ")
		&&	!parseable<common::real32>("~~~~~~")
		);

	template<> constexpr size_t
		skip<	common::real32	>(	const char* text	)
	{
		if(!parseable<common::real32>(text))
			return unone;
		size_t i = 0;
		for(
			;
			!isWhitespace(text[i]) &&
			(text[i] == '-' || text[i] == '.' || (text[i] >= '0' && text[i] <= '9'));
			++i
		)
			;
		return i;
	}



	template<> constexpr auto
		parse<	common::real32	>(const char* text)
	{
		size_t i 				=	0;
		bool negative 			=	false;
		common::real32 integral	=	.0f;

		if(text[i] == '-')
		{
			negative = true;
			++i;
		}
		/*
			read the integral part
		*/
		bool firstIteration = true;

		for(char c = 0; text[i] != '.' && (0 <= (c = charToInt( text[i] ) ) && c <= 9); ++i)
		{
			if(!firstIteration)
				integral *= 10.0f;
			integral += static_cast<common::real32>(c);
			if(firstIteration)
				firstIteration = false;
		}
		if(text[i] != '.')
			return negative ? -integral : integral;

		const size_t fractionalStart	=	i;
		common::real32 fractional = .0f;
		++i;
		/*
			seek to the end
		*/
		{
		char c = 0;
		while( 0 <= (c = charToInt(text[i])) && c <= 9)
			++i;
		}

		firstIteration = true;
		/*	read the fractional portion	*/
		for(char c = 0; i > fractionalStart; --i)
		{
			c	=	charToInt(text[i]);
			if( 0 > c || c > 9)
				continue;

			fractional	+=	static_cast<common::real32>(c);
			fractional	/=	10.0f;

		}

		/*	unify the fractional and integral parts	*/
		const common::real32 value = fractional + integral;
		return negative ? -value : value;
	}

	static_assert
	(
			parse<common::real32>("666.666") 		==	666.666f
		&&	parse<common::real32>("3.141592654")	== 	3.141592654f
		&& 	parse<common::real32>("2.718281828")	==	2.718281828f,
		"parser::parse<real32> is fuckt."
	);

	template<common::int32 value>
	static constexpr size_t calcRepresentationLen()
	{
		size_t totalLength = 0;

		if(value < 0)
			++totalLength;
		common::int32 shifter = value;
		do
		{
			++totalLength;
			shifter /= 10;
		}
		while(shifter);
		return totalLength;
	}

	template<common::int32 val>
	struct toStringHelper_int32 : __markAsCtfe()
	{
		static constexpr size_t sLen = calcRepresentationLen<val>() + 1;
		using sType = charStream< sLen, false>;
		//static constexpr
		sType b;// = sType();

		constexpr toStringHelper_int32()
		{
			common::int32 i = val;
			if (i >= 0)
			{
				do
				{
					const char c = static_cast<char>('0' + static_cast<char>(i % 10));
					//clang is complaining and thinks this is a bit shift...
					//worked fine in GCC

					//b << c;

					b.operator<<(c);
					i /= 10;
				}
				while (i != 0);
			}
			else
			{
				//b << '-';
				b.operator<<('-');
				do
				{
					const char c = static_cast<char>('0' - (i % 10));
				//	b << c;
					b.operator<<(c);
					i /= 10;
				}
				while (i != 0);
			}
		}

		explicit constexpr operator decltype(b) () const	{	return b;	}
	}__unused;
	static_assert(calcRepresentationLen<20>() == 2, "woops");

	template<common::int32 value> static constexpr
	CString<	calcRepresentationLen<value>() + 1	> toString =	toStringHelper_int32<value>().b;

	static_assert(toString<3>[0] == '3',
	"weird");
	static_assert((static_cast<const char*>(toString<40>))[1] == '4', "?");
	static_assert(toString<994>.size() == 4, "uhhh");
	static_assert(toString<1055>[4] == 0, "ruh roh");

}//namespace cloture::util::ctfe::parser
