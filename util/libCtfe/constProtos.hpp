#pragma once

namespace cloture::util::ctfe
{
	static constexpr int charToInt(const char c);
	static constexpr bool isWhitespace(const char c);
	
	static constexpr bool isIdentifierChar(const char c);
	static constexpr size_t cstrlen(const char* s);
	static constexpr size_t skipLeadingWhitespace(const char* buffer, const size_t position = 0);
	static constexpr size_t whitespaceEnd(const char* buffer);
	static constexpr size_t nextWhitespaceEnd(const char* buffer, const size_t start = 0);
	
}
