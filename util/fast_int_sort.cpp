#include <stdint.h>

#define 	likely(x)								__builtin_expect(bool(x), true)
#define 	unlikely(x)								__builtin_expect(bool(x), false)
#define assume(x)       if(!(x)) __builtin_unreachable()
#define MED3_ASM    1

#define 	MAYBE_USE_LONG	0
#define	SWAP(p1, p2)	({typeof(*(p1)) temp = *(p1); *(p1) = *(p2); *(p2) = temp;})

#define SIGNBITMASK(x)  ((typeof(x))((int)(x) >> 31))
#define BIT_TERNARY(cond, v1, v2)   ((((cond)&1)*v1) | ((((cond)&1)^1)*v2))
#define MASK_TERNARY(cond, v1, v2)  (((cond)&v1) | (~(cond) & v2))


template<typename T>
static __inline void
swapfunc(T *a, T *b, int n)
{
	//__builtin_prefetch(a);
	//__builtin_prefetch(b);
	
	
	int i = n / sizeof (T);
	T *pi = a;
	T *pj = b;
	do { 
		T t = *pi;
		*pi++ = *pj; 
		*pj++ = t; 
	} while (--i > 0); 
}
#if !MED3_ASM
template<typename T, int (*cmp)(const T* a, const T*b)>
static __inline T *med3(const T *a, const T *b, const T *c)
{
	int cmpab = cmp(a, b);

    int cmpac = cmp(a, c);
    int cmpbc = cmp(b, c);

    uintptr_t res1 = MASK_TERNARY(SIGNBITMASK(cmpbc), (uintptr_t)b, MASK_TERNARY(SIGNBITMASK(cmpac), (uintptr_t)c, (uintptr_t)a));
    uintptr_t res2 = BIT_TERNARY(cmpbc > T(0), (uintptr_t)b, MASK_TERNARY(SIGNBITMASK(cmpac), (uintptr_t)a, (uintptr_t)c));
    uintptr_t result = MASK_TERNARY( SIGNBITMASK(cmpab), res1, res2);

    return (T*)result;
        /*cmp(a, b) < T(0) ?
           (cmp(b, c) < T(0) ? b : (cmp(a, c) < T(0) ? c : a ))
                            :(cmp(b, c) > T(0) ? b : (cmp(a, c) < T(0) ? a : c ));*/

}
#else
/*
template<typename T, int (*cmp)(const T* a, const T*b)>
static __attribute__((always_inline)) __attribute__((pure)) T *med3(const T *a, const T *b, const T *c) {
	return cmp(a, b) < T(0) ?
           (cmp(b, c) < T(0) ? b : (cmp(a, c) < T(0) ? c : a ))
                            :(cmp(b, c) > T(0) ? b : (cmp(a, c) < T(0) ? a : c ));
}*/
template<typename T, int (*cmp)(const T* a, const T*b)>
static __attribute__((always_inline)) __attribute__((pure)) T *med3(const T *a, const T *b, const T *c) {
	int cmpab = cmp(a, b);
	int cmpac = cmp(a, c);
	int cmpbc = cmp(b, c);
	int r4, r5, r7, r8;
	#if 0
	ptrdiff_t abd = a - b;
	ptrdiff_t acd = a - c;
	ptrdiff_t bcd = b - c;
	#define makepos(x)	if(x < 0) x = -x
	makepos(abd);
	makepos(acd);
	makepos(bcd);
	//ALWAYS a hit!
	
	if((uintptr_t)abd < 65536 && acd < 65536 && bcd < 65536)
		hits++;
	else
		misses++;
	#endif
	asm (

	"cmp	%[cmpbc], #0;"
	"mov	%[R8], %[cmpac], asr #31;"
	"mov	%[R4], %[cmpbc], asr #31;"
	"and	%[R7], %[R8], %[a];"
	"mov	%[cmpac], %[R8];"
	"movgt	%[cmpbc], #0;"
	"movle	%[cmpbc], #1;"
	"mvn	%[R5], %[cmpac];"
	"and	%[R8], %[R5], %[c];"
	"orr	%[R7], %[R8], %[R7];"
	"mov	%[cmpab], %[cmpab], asr #31;"
	"and	%[a], %[R5], %[a];"
	"and	%[c], %[cmpac], %[c];"
	"mul	%[cmpbc], %[cmpbc], %[R7];"
	"movle	%[R7], #0;"
	"movgt	%[R7], #1;"
	"mul	%[R7], %[b], %[R7];"
	"cmp	%[cmpab], #0;"
	"movne	%[cmpbc], #0;"
	"orreq	%[cmpbc], %[cmpbc], %[R7];"
	"cmp	%[R4], #0;"
	"orreq	%[cmpac], %[a], %[c];"
	"and	%[R5], %[R4], %[b];"
	"movne	%[cmpac], #0;"
	"orr	%[b], %[cmpac], %[R5];"
	"and	%[a], %[b], %[cmpab];"
	"orr	%[a], %[cmpbc], %[a];"
	: /* outputs*/
//		[a] "+r" (a)
		[cmpab] "+r" (cmpab),
		[cmpac] "+r" (cmpac),
		[cmpbc] "+r" (cmpbc),
		[a] "+r" (a),
		[b] "+r" (b),
		[c] "+r" (c),
		[R4] "+r" (r4),
		[R5] "+r" (r5),
		//[R6] "+r" (r6),
		[R7] "+r" (r7),
		[R8] "+r" (r8)
	: /* inputs */
	: /* clobbers */
	);
	return const_cast<T*>(a);
};
#endif
template<typename T, int (*cmp)(const T*, const T*)>
static __attribute__ ((noinline)) void qsortLessThanSeven(T* a, unsigned n) {
    for (T* pm = &a[1]; pm < &a[n]; pm++) {
        for (T* pl = pm; pl > a && cmp(&pl[-1], pl) > 0; pl--) {
            SWAP(pl, &pl[-1]);
        }
    }
    return;
};
#define NO_RECURSION    1

#if NO_RECURSION
template<typename T>
struct recursionState_t {
    T *pc, *pd, *pn;
    T *a;
    size_t n;
    struct allocationInfo_t {
        bool allocatedArray = false;
        recursionState_t<T>* currentArray = nullptr;
        unsigned currentAllocationIndex = 0;
        unsigned maxAllocations = 1024;
    };
    static allocationInfo_t static_alloc_info;

    recursionState_t<T>* previous = nullptr;
    allocationInfo_t* allocInfo = &static_alloc_info;
    ~recursionState_t() {
        //if(previous)
        //    delete previous;
        previous = nullptr;
    }
#define allocInfo       (&static_alloc_info)
    static void ensureArrayAllocated() {
        if(unlikely(!allocInfo->allocatedArray)) {
            allocInfo->currentArray = (recursionState_t<T>*)__builtin_malloc(sizeof(recursionState_t) * allocInfo->maxAllocations);
            allocInfo->allocatedArray = true;
        }
    }
    static __attribute__((noinline)) void reallocArray() {
        allocInfo->maxAllocations *= 2;
        allocInfo->currentArray = (recursionState_t<T>*)__builtin_realloc(allocInfo->currentArray, allocInfo->maxAllocations);
    }
    static
    inline void* operator new(size_t sz) {
        if(unlikely(allocInfo->currentAllocationIndex >= allocInfo->maxAllocations))
            reallocArray();
        return (void*) &allocInfo->currentArray[allocInfo->currentAllocationIndex++];
    }
    static inline
    void operator delete(void* d) {

    }
#undef allocInfo
};

template<typename T>
typename recursionState_t<T>::allocationInfo_t recursionState_t< T>::static_alloc_info;
#endif

template<typename T, int (*cmp)(const T* a, const T* b)>
static __attribute__((noinline)) void
swapcountZero(T* a, unsigned n) {
	for (T* pm = a + 1; pm < a + n; pm++) {
                for (T* pl = pm; pl > a && ((cmp(pl - 1, pl))) > 0; pl--) {
					SWAP(pl, &pl[-1]);
                }
            }
}

template<typename T, int (*cmp)(const T* a, const T* b)>
static __attribute__((noinline)) void
ngreaterThan40(T*& pl, T*& pm, T*& pn, int n) {
	int d = n / 8;
	pl = med3<T, cmp>(pl, pl + d, &pl[2 * d]);
	pm = med3<T, cmp>(pm - d, pm, &pm[d]);
	pn = med3<T, cmp>(pn - 2 * d, &pn[-d], pn);
}

template< typename T, int (*cmp)(const T* a, const T* b)>
static __attribute__((noinline)) void
__qsort(T *aa, size_t n)
{
	

#if NO_RECURSION
    //static recursionState_t<T>::allocationInfo_t recursionState_t<T>::static_alloc_info =  {false, nullptr, 0, 1024};
    recursionState_t<T>::ensureArrayAllocated();

    recursionState_t<T>::static_alloc_info.currentAllocationIndex = 0;

    recursionState_t<T>* recursion_state = nullptr;
#endif
begin:
    T *pa, *pb, *pc, *pd, *pl, *pm, *pn;
    int r, swap_cnt;
    T *a = aa;

loop:
/*
	unlikely :(
	if((((uintptr_t)a) % 16) == 0) {
		hits++;
	}
	else
		misses++;*/
    do {
		__builtin_prefetch(a);
        swap_cnt = 0;
        if (unlikely(n < 7)) {
            qsortLessThanSeven<T, cmp>(a, n);
            goto loop_end;
        }
        //__builtin_prefetch(&a[n-1]);
        pm = &a[n / 2];
        //__builtin_prefetch(pm);
        if (likely(n > 7)) {
            pl = a;
            pn = &a[n - 1];

            if (unlikely(n > 40)) {
                ngreaterThan40<T, cmp>(pl, pm, pn, n);
            }
		 
            pm = med3<T, cmp>(pl, pm, pn);
        }
        SWAP(a, pm);
        pa = pb = &a[1];

        pc = pd = &a[n - 1];
        for (; ;) {
            while (likely(pb <= pc) && likely((r = cmp(pb, a)) <= 0)) {
                if (unlikely(!r)) {
                    swap_cnt = 1;
                    SWAP(pa, pb);
                    pa++;
                }
                pb++;
            }
            while (likely(pb <= pc) && likely((r = cmp(pc, a)) >= 0)) {
                if (unlikely(!r)) {
                    swap_cnt = 1;
                    SWAP(pc, pd);
                    pd--;
                }
                pc--;
            }
            if (unlikely(pb > pc)) 
                break;
			
			SWAP(pb, pc);
            swap_cnt = 1;
            pb++;
            pc--;
        }
        if (unlikely(!swap_cnt)) {
            swapcountZero<T, cmp>(a, n);
            goto loop_end;
        }

        pn = &a[n];

        r = (pa - a) < (pb - pa) ? pa - a : pb - pa;
        
        if (likely(r > 0)) {
            swapfunc(a, pb - r, r);
		}
        r = (pd - pc) < (pn - pd - 1) ? pd - pc : pn - pd - 1;
        if (likely(r > 0)) {
            swapfunc(pb, pn - r, r);
		}
        if (likely((r = pb - pa) > 1)) {
#if !NO_RECURSION
            __qsort<T, cmp>(a, r);
#else
            recursionState_t<T> *newState = new recursionState_t<T>;
#define     RECURSIONSTATE(x)      newState->x = x;
#include "recursionstate.h"
            newState->previous = recursion_state;
            recursion_state = newState;
            n = r;
            goto loop;
#endif
        } 
       
recursion_return:
        r = pd - pc;
        if(likely(r > 1)) {
            a = pn - r;
            n = r;
        } 

    }while(r > 1);
loop_end:
#if NO_RECURSION
    if(likely(recursion_state)) {
#define RECURSIONSTATE(x)       x = recursion_state->x;
#include "recursionstate.h"

        recursionState_t<T>* next = recursion_state->previous;
        delete recursion_state;
        recursion_state = next;
        goto recursion_return;
    }
#else
;
#endif
}
