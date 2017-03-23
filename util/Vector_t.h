#pragma once


#if USE_VECTOR_T == 1
namespace cloture::util::vector {
    struct voidvector_t {
        void* array;
        size_t n, alloc;
    };

    void vector_t_reserve(voidvector_t* vvec, size_t r, size_t typesz);

    template<typename T> class Vector_t {
        T* array;
        size_t n, alloc;

    public:
        __forceinline Vector_t() : array(nullptr), n(0), alloc(0) {}
        __forceinline Vector_t(Vector_t<T>& other) {
            if(other.n > 0) {
                array = new T[other.alloc];
                alloc = other.alloc;
                while ( n < other.n )
                {
                    new (array+n) T(other.array[n]);
                    ++n;
                }
            }
        }

        __forceinline __pseudopure T* begin() { return array;}
        __forceinline __pseudopure T* end() { return &array[n];}
        __forceinline __pseudopure const T* begin() const { return array;}
        __forceinline __pseudopure const T* end() const { return &array[n];}
        void reserve(size_t r) {
            vector_t_reserve(reinterpret_cast<voidvector_t*>(this), r, sizeof(T));
        }
        __forceinline void push_back(const T &x)
        {
            reserve(n+1);
            new (array+n) T(x); // create a new element in the qvector
            ++n;
        }
        __forceinline __pseudopure size_t size() const { return n; }

        __forceinline __pseudopure bool empty() const { return !n; }
        __forceinline const T &operator[](size_t _idx) const { return array[_idx]; }
        __forceinline T &operator[](size_t _idx)       { return array[_idx]; }
        __forceinline const T &at(size_t _idx) const { return array[_idx]; }
        __forceinline T &at(size_t _idx)       { return array[_idx]; }
        __forceinline const T &front() const { return array[0]; }
        __forceinline T &front()       { return array[0]; }
        __forceinline const T &back() const { return array[n-1]; }
        __forceinline T &back()       { return array[n-1]; }
        /*static_assert(sizeof(Vector_t<T>) == sizeof(voidvector_t));
        static_assert(__offsetof(Vector_t<T>, array) == __offsetof(voidvector_t, array));
        static_assert(__offsetof(Vector_t<T>, n) == __offsetof(voidvector_t, n));
        static_assert(__offsetof(Vector_t<T>, alloc) == __offsetof(voidvector_t, alloc));*/
    };
}
#else
#include <vector>
namespace cloture::util::vector {
    template<typename T> using Vector_t = std::vector<T>;
}
#endif
