#pragma once


/**
 * The majority of this code I wrote a few months ago when I was attempting to create
 * a metacompiler using constexpr.
 *
 * it failed miserably, but here some of it LIVES ON
 *
*/
namespace cloture::util::ctfe
{
	using common::unone;
	using common::none;
	using strhash_t = common::int64;
	template<typename T>
	constexpr strhash_t cstrhash(T str)
	{
		int h = 0;
		int x = 0;
		while (*str) {
			h = h << 1 ^ *str++;
			++x;
		}
		return (static_cast<strhash_t>(x) << 32 ) | h;//h;
	}
	static constexpr int charToInt(const char c)
	{
		return c - '0';
	}
	static constexpr bool isNumericChar(const char c) {
		return c >= '0' && c <= '9';
	}
	
	static constexpr bool isWhitespace(const char c)
	{
		return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
	}
	static constexpr bool isIdentifierChar(const char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
	}
	template<typename T>
	static constexpr size_t cstrlen(T s)
	{
		size_t i = 0;
		for(; s[i]; ++i)
			;
		return i;
	}
	template<typename T>
	static constexpr size_t skipLeadingWhitespace(T buffer, const size_t position = 0)
	{
		size_t i = 0;

		if( !isWhitespace( buffer[position] ) )
			return 0;

		for(; isWhitespace(buffer[ i + position ]); ++i )
			if(!buffer[ i + position ])
				return unone;

		return i + position;
	}

	template<typename T>
	static constexpr size_t whitespaceEnd(T buffer)
	{
		size_t i = 0;
		if(isWhitespace(buffer[i]))
		{
			while( isWhitespace( buffer[i] ) )
				++i;
		}
		return i;
	}
	template<typename T>
	static constexpr size_t nextWhitespaceEnd(T buffer, const size_t start = 0)
	{
		size_t i = start;
		while( !isWhitespace(buffer[i]) && buffer[i])
			++i;
		while(isWhitespace(buffer[i]))
			++i;
		return i;
	}
	template<typename T>
	constexpr unsigned indexOf(T s, const char* substr) {
		auto sublen = cstrlen(substr);
		auto slen = cstrlen(substr);

		unsigned comparisonIndex = 0;
		for(unsigned i = 0; i < slen; ++i) {
			if(s[i] == substr[comparisonIndex]) {
				if(++comparisonIndex == sublen)
					return i - sublen + 1;
			}
			else
				comparisonIndex = 0;
		}
		return -1;
	}
	template<typename T>
	constexpr unsigned indexOfBounded(T s, unsigned start, unsigned end, const char* substr) {
		auto sublen = cstrlen(substr);
		auto slen = cstrlen(substr);

		unsigned comparisonIndex = 0;
		for(unsigned i = start; i < end; ++i) {
			if(s[i] == substr[comparisonIndex]) {
				if(++comparisonIndex == sublen)
					return i - sublen + 1;
			}
			else
				comparisonIndex = 0;
		}
		return -1;
	}

	static_assert(indexOf("i like donkeys a lot", "i like") == 0);

} //namespace cloture::util::ctfe

#define constantCString(s)		\
cloture::util::ctfe::CString<cloture::util::ctfe::cstrlen(s)>::CString<cloture::util::ctfe::cstrlen(s)>(s)


#include 	"libCtfe/constcontainers.hpp"
#include 	"libCtfe/constparse.hpp"
#include 	"libCtfe/constmath.hpp"
#include 	"libCtfe/constcrc.hpp"

#include	"libCtfe/constmemory.hpp"
