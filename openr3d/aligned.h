#ifndef _ALIGNED_H
#define _ALIGNED_H

/*
** #include <x86intrin.h> includes all the SSE/AVX instructions enabled by the compiler.
** For the compiler flag -march=native and a Sandy Bridge processor, the following headers are included:
** <mmintrin.h>  MMX
** <xmmintrin.h> SSE
** <emmintrin.h> SSE2
** <pmmintrin.h> SSE3
** <tmmintrin.h> SSSE3
** <smmintrin.h> SSE4.1
** <nmmintrin.h> SSE4.2
** <ammintrin.h> SSE4A
** <wmmintrin.h> AES
** <immintrin.h> AVX
** <zmmintrin.h> AVX512
*/

#include <cstddef>
#ifdef _WIN32
    #include <intrin.h>
#else
    #include <x86intrin.h>
#endif

template <std::size_t ALIGNMENT>
class alignas(ALIGNMENT) Aligned {

public:

    void* operator new (std::size_t size) {
        return _mm_malloc (size, ALIGNMENT);
    }

    void* operator new[] (std::size_t size) {
        return _mm_malloc (size, ALIGNMENT);
    }

    void operator delete (void *ptr) {
        _mm_free(ptr);
    }

    void operator delete[] (void *ptr) {
        _mm_free(ptr);
    }

};

#endif // !_ALIGNED_H
