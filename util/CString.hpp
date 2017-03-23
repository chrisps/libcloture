#pragma once

namespace cloture
{
namespace util
{
namespace string
{
	class ceString
	{
		const char* s;
	public:
		using charType = __typeof__(*s);
		constexpr __pseudopure inline ceString(const char* ss) noexcept : s(ss)
		{}

		constexpr __pseudopure inline charType operator[](const size_t index) const noexcept
		{
			return s[index];
		}
		constexpr __pseudopure inline charType& operator[](const size_t index) noexcept
		{
			return s[index];
		}
		__pseudopure inline bool operator ==(ceString other) const noexcept
		{
			return __builtin_strcmp(s, other.s) == 0;
		}
		__pseudopure inline bool operator !=(ceString other) const noexcept
		{
			return !((*this) == other);
		}
		explicit constexpr __pseudopure inline operator bool() const noexcept
		{
			return s != nullptr;
		}
		constexpr __pseudopure inline bool operator !() const noexcept
		{
			return !(bool(*this));
		}

		constexpr __pseudopure inline operator const char*() noexcept
		{
			return s;
		}

		constexpr inline void swap(ceString& other) noexcept
		{
			const char* original = s;
			s = other.s;
			other.s = original;
		}

		constexpr __pseudopure size_t length() const noexcept
		{
			size_t i = 0;
			for(; s[i]; ++i)
				;
			return i;
		}
	};//class ceString



#if 1
    class String_t {

    public:
        char* data;
        unsigned length;
        __forceinline String_t() : data(nullptr), length(0u) {}

        explicit String_t(const char* s);
        String_t(char s);


        void setData(const char* s);


        __forceinline String_t(const String_t& other) : String_t(other.data) {

        }

        __forceinline  String_t(String_t&& other) : data(other.data), length(other.length) {}

		__forceinline String_t& operator=(String_t&& rhs) {
			if(data)
				delete data;
			data = rhs.data;
			length = rhs.length;
			return *this;
		}
		__forceinline String_t& operator=(const char* rhs) {
			*this = String_t(rhs);
			return *this;
		}

		__forceinline size_t getLength() const {
			return length;
		}
		__forceinline const char* getData() const {
			return data;
		}
        template<size_t n>
        __forceinline String_t(const char s[n]) {
            data = new char[n];
            length = n - 1;
            __builtin_memset(data, 0, n);
            __builtin_memcpy(data, &s[0], n);
        }

        __forceinline explicit operator const char* () const {
            return data;
        }
        ~String_t();

        String_t& append(const char* s);


        String_t operator +(String_t& rhs) const;
        String_t operator +(String_t&& rhs) const;
        String_t operator +(const char* s) const;
        String_t& operator +=(const char* rhs) {
            return append(rhs);
        }
		String_t operator +(long long i) const;

		String_t fileExtension() {
			for(unsigned i = length - 1; i >= 0; --i) {
				if(data[i] == '.') {
					return String_t(&data[i+1]);
				}
			}
			return String_t("");
		}
		__forceinline bool operator ==(const char* s) {
			return !__builtin_strcmp(data, s);
		}
		__forceinline bool operator !=(const char* s) {
			return !!__builtin_strcmp(data, s);
		}

    };
#endif

}//namespace string
}//namespace util
}//namespace cloture
