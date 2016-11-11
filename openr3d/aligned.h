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
#include <exception>
#include <sstream>
#include <iostream>

namespace Alignment {
    enum Size : std::size_t
    {
        DEFAULT = sizeof(void*),
        SSE    = 16,
        AVX    = 32
    };
}

class BadAlignment: public std::exception {

public:

    std::size_t alignment_size;
    std::string log;

    BadAlignment(std::size_t alignment_size) : alignment_size(alignment_size) {
        std::stringstream ss;
        ss << "Memory not " << alignment_size << " bytes memory aligned!";
        log = ss.str();
    }

    virtual const char* what() const throw() {
        return log.c_str();
    }

};

template <std::size_t ALIGNMENT_SIZE>
class alignas(ALIGNMENT_SIZE) Aligned
{

protected:

    template<typename T> friend class AlignedAllocator;
    static const std::size_t alignment_size = ALIGNMENT_SIZE;

    Aligned() {
        if ((reinterpret_cast<std::size_t>(this) % alignment_size) != 0)
            throw BadAlignment(alignment_size);
    }

public:

    void* operator new (std::size_t size) {
        void* ptr = _mm_malloc (size, alignment_size);
        if (ptr == NULL)
            throw std::bad_alloc();
        return ptr;
    }
    void* operator new (std::size_t size, const std::nothrow_t& tag) {
        (void)tag;
        return _mm_malloc (size, alignment_size);
    }
    void* operator new (std::size_t size, void* ptr) {
        (void)size;
        if (reinterpret_cast<std::size_t>(ptr) % alignment_size != 0)
            throw BadAlignment(alignment_size);
        return ptr;
    }

    void* operator new[] (std::size_t size) {
        void* ptr = _mm_malloc (size, alignment_size);
        if (ptr == NULL)
            throw std::bad_alloc();
        return ptr;
    }
    void* operator new[] (std::size_t size, const std::nothrow_t& tag) {
        (void)tag;
        return _mm_malloc (size, alignment_size);
    }
    void* operator new[] (std::size_t size, void* ptr) {
        (void)size;
        if (reinterpret_cast<std::size_t>(ptr) % alignment_size != 0)
            throw BadAlignment(alignment_size);
        return ptr;
    }

    void operator delete (void *ptr) {
        _mm_free(ptr);
    }
    void operator delete (void *ptr, const std::nothrow_t& tag) {
        (void)tag;
        _mm_free(ptr);
    }

    void operator delete[] (void *ptr) {
        _mm_free(ptr);
    }
    void operator delete[] (void *ptr, const std::nothrow_t& tag) {
        (void)tag;
        _mm_free(ptr);
    }

};

template <std::size_t ALIGNMENT_SIZE>
class alignas(ALIGNMENT_SIZE) SafeAligned;
//Recursively virtually inherit alignment
//this allows to use the biggest alignment when a class has multiple alignment inheritence
template <std::size_t ALIGNMENT_SIZE>
class alignas(ALIGNMENT_SIZE) SafeAligned : public virtual SafeAligned<ALIGNMENT_SIZE/2>
{

protected:

    template<typename T> friend class AlignedAllocator;
    static const std::size_t alignment_size = ALIGNMENT_SIZE;

    SafeAligned() {
        if ((reinterpret_cast<std::size_t>(this) % alignment_size) != 0)
            throw BadAlignment(alignment_size);
    }

public:

    void* operator new (std::size_t size) {
        void* ptr = _mm_malloc (size, alignment_size);
        if (ptr == NULL)
            throw std::bad_alloc();
        return ptr;
    }
    void* operator new (std::size_t size, const std::nothrow_t& tag) {
        (void)tag;
        return _mm_malloc (size, alignment_size);
    }
    void* operator new (std::size_t size, void* ptr) {
        (void)size;
        if (reinterpret_cast<std::size_t>(ptr) % alignment_size != 0)
            throw BadAlignment(alignment_size);
        return ptr;
    }

    void* operator new[] (std::size_t size) {
        void* ptr = _mm_malloc (size, alignment_size);
        if (ptr == NULL)
            throw std::bad_alloc();
        return ptr;
    }
    void* operator new[] (std::size_t size, const std::nothrow_t& tag) {
        (void)tag;
        return _mm_malloc (size, alignment_size);
    }
    void* operator new[] (std::size_t size, void* ptr) {
        (void)size;
        if (reinterpret_cast<std::size_t>(ptr) % alignment_size != 0)
            throw BadAlignment(alignment_size);
        return ptr;
    }

    void operator delete (void *ptr) {
        _mm_free(ptr);
    }
    void operator delete (void *ptr, const std::nothrow_t& tag) {
        (void)tag;
        _mm_free(ptr);
    }

    void operator delete[] (void *ptr) {
        _mm_free(ptr);
    }
    void operator delete[] (void *ptr, const std::nothrow_t& tag) {
        (void)tag;
        _mm_free(ptr);
    }

};

template <>
class alignas(2) SafeAligned<2> {

protected:

    template <typename T> friend class AlignedAllocator;
    static const std::size_t alignment_size = 2;

    SafeAligned() {
        if ((reinterpret_cast<std::size_t>(this) % alignment_size) != 0)
            throw BadAlignment(alignment_size);
    }

public:

    void* operator new (std::size_t size) {
        void* ptr = _mm_malloc (size, alignment_size);
        if (ptr == NULL)
            throw std::bad_alloc();
        return ptr;
    }
    void* operator new (std::size_t size, const std::nothrow_t& tag) {
        (void)tag;
        return _mm_malloc (size, alignment_size);
    }
    void* operator new (std::size_t size, void* ptr) {
        (void)size;
        if (reinterpret_cast<std::size_t>(ptr) % alignment_size != 0)
            throw BadAlignment(alignment_size);
        return ptr;
    }

    void* operator new[] (std::size_t size) {
        void* ptr = _mm_malloc (size, alignment_size);
        if (ptr == NULL)
            throw std::bad_alloc();
        return ptr;
    }
    void* operator new[] (std::size_t size, const std::nothrow_t& tag) {
        (void)tag;
        return _mm_malloc (size, alignment_size);
    }
    void* operator new[] (std::size_t size, void* ptr) {
        (void)size;
        if (reinterpret_cast<std::size_t>(ptr) % alignment_size != 0)
            throw BadAlignment(alignment_size);
        return ptr;
    }

    void operator delete (void *ptr) {
        _mm_free(ptr);
    }
    void operator delete (void *ptr, const std::nothrow_t& tag) {
        (void)tag;
        _mm_free(ptr);
    }

    void operator delete[] (void *ptr) {
        _mm_free(ptr);
    }
    void operator delete[] (void *ptr, const std::nothrow_t& tag) {
        (void)tag;
        _mm_free(ptr);
    }

};

//
// AlignmentAllocator requires the type T to be a child of Aligned or SafeAligned
//
template <typename T>
class AlignedAllocator {
public:
  typedef T value_type;
  AlignedAllocator() {} // default constructor not required
  template <typename U> AlignedAllocator(const AlignedAllocator<U>&) { }
  template <typename U> bool operator==(const AlignedAllocator<U>&) const { return true; }
  template <typename U> bool operator!=(const AlignedAllocator<U>&) const { return false; }

  T* allocate(const std::size_t n) const {
      std::cout << "T* allocate(const std::size_t n) with alignment = " << T::alignment_size << std::endl;
      if (n == 0) { return NULL; }
      if (n > static_cast<std::size_t>(-1) / sizeof(T)) {
          throw std::bad_array_new_length();
      }
      void* const pv = _mm_malloc(n * sizeof(T), T::alignment_size);
      if (pv == NULL) { throw std::bad_alloc(); }
      return static_cast<T*>(pv);
  }

  void deallocate(T* const p, std::size_t) const {
      std::cout << "void deallocate(T* const p, std::size_t) with alignment = " << T::alignment_size << std::endl;
      _mm_free(p);
  }

};


#endif // !_ALIGNED_H
