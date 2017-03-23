#include "stl.hpp"
#if 1
using namespace cloture::util::string;
using namespace cloture::util::common;
String_t::String_t(const char* s) {
    unsigned l = __builtin_strlen(s);
    char* d = new char[l+1];
    __builtin_memset(d, 0, l+1);
    __builtin_memcpy(d, s, l+1);
    data = d;
    length = l;
}

String_t::String_t(char s) : length(1){
    data = new char[2];
    data[0] = s;
    data[1] = 0;
}

void String_t::setData(const char* s) {
    if(data)
        delete[] data;
    length = __builtin_strlen(s);
    data = new char[length+1];
    __builtin_memset(data, 0, length+1);
    __builtin_memcpy(data, s, length+1);
}

String_t& String_t::append(const char* s) {
    size_t newlen = __builtin_strlen(s) + length;
    char* old = data;
    char* new_ = new char[newlen + 1];
    __builtin_memset(new_, 0, newlen+1);
    __builtin_memcpy(new_, old, length);
    __builtin_strcat(new_, s);
    data = new_;
    length = newlen;
    delete[] old;
    return *this;
}
String_t::~String_t() {
    if(data)
        delete [] data;
}
String_t String_t::operator +(String_t& rhs) const {
    unsigned newlength = length + rhs.length;
    char* d = new char[newlength + 1];
    __builtin_memset(d, 0, newlength+1);
    __builtin_memcpy(d, data, length);
    __builtin_strcat(d, rhs.data);
    return String_t(d);
}

String_t String_t::operator +(String_t&& rhs) const {
    unsigned newlength = length + rhs.length;
    char* d = new char[newlength + 1];
    __builtin_memset(d, 0, newlength+1);
    __builtin_memcpy(d, data, length);
    __builtin_strcat(d, rhs.data);
    return String_t(d);
}

String_t String_t::operator +(const char* s) const {
    unsigned newlen = __builtin_strlen(s) + length;
    char* d = new char[newlen + 1];
    __builtin_memset(d, 0, newlen+1);
    __builtin_memcpy(d, data, length);
    __builtin_strcat(d, s);
    return String_t(d);
}
#include <cstdio>
String_t String_t::operator+(long long i) const {
    char buf[128];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%lld", i);
    return (*this) + (const char*)&buf[0];
}
#endif