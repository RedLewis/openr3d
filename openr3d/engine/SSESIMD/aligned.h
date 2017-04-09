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

#ifdef _WIN32
    #include <intrin.h>
#else
    #include <x86intrin.h>
#endif
#include <cstddef>
#include <stdexcept>
#include <string>

namespace Alignment
{
    enum Size : std::size_t
    {
        NATIVE = sizeof(void*),
        SSE    = 16,
        AVX    = 32
    };
}

class BadAlignment: public std::exception
{

public:

    std::size_t alignment_size;
    std::string log;

    BadAlignment(std::size_t alignment_size = 0) : alignment_size(alignment_size) {
        if (alignment_size == 0)
            log = "Memory does not meet the requiered alignment!";
        else
            log = std::string("Memory does not meet the requiered ") + std::to_string(alignment_size) + std::string(" bytes alignment!");
    }

    virtual const char* what() const throw() {
        return log.c_str();
    }

};

template <std::size_t ALIGNMENT_SIZE>
class alignas(ALIGNMENT_SIZE) Aligned
{

protected:

    template<typename, std::size_t> friend class AlignedAllocator;
    static const std::size_t alignment_size = ALIGNMENT_SIZE;

    Aligned() {
        //std::cout << "Constructing child of Aligned<" << ALIGNMENT_SIZE << ">" << std::endl;
        if ((reinterpret_cast<std::size_t>(this) % alignment_size) != 0)
            throw BadAlignment(alignment_size);
    }

public:

    void* operator new (std::size_t size) {
        //std::cout << "Allocating " << size << " bytes with alignment " << ALIGNMENT_SIZE << std::endl;
        void* ptr = _mm_malloc(size, alignment_size);
        if (ptr == NULL)
            throw std::bad_alloc();
        return ptr;
    }
    void* operator new (std::size_t size, const std::nothrow_t& tag) {
        (void)tag;
        //std::cout << "Allocating " << size << " bytes with alignment " << ALIGNMENT_SIZE << std::endl;
        return _mm_malloc(size, alignment_size);
    }
    void* operator new (std::size_t size, void* ptr) {
        (void)size;
        //std::cout << "Allocating " << size << " bytes with alignment " << ALIGNMENT_SIZE << std::endl;
        if (reinterpret_cast<std::size_t>(ptr) % alignment_size != 0)
            throw BadAlignment(alignment_size);
        return ptr;
    }

    void* operator new[] (std::size_t size) {
        //std::cout << "Allocating " << size << " bytes with alignment " << ALIGNMENT_SIZE << std::endl;
        void* ptr = _mm_malloc(size, alignment_size);
        if (ptr == NULL)
            throw std::bad_alloc();
        return ptr;
    }
    void* operator new[] (std::size_t size, const std::nothrow_t& tag) {
        (void)tag;
        //std::cout << "Allocating " << size << " bytes with alignment " << ALIGNMENT_SIZE << std::endl;
        return _mm_malloc(size, alignment_size);
    }
    void* operator new[] (std::size_t size, void* ptr) {
        (void)size;
        //std::cout << "Allocating " << size << " bytes with alignment " << ALIGNMENT_SIZE << std::endl;
        if (reinterpret_cast<std::size_t>(ptr) % alignment_size != 0)
            throw BadAlignment(alignment_size);
        return ptr;
    }

    void operator delete (void *ptr) {
        //std::cout << "Deleting with alignment " << ALIGNMENT_SIZE << std::endl;
        _mm_free(ptr);
    }
    void operator delete (void *ptr, const std::nothrow_t& tag) {
        //std::cout << "Deleting with alignment " << ALIGNMENT_SIZE << std::endl;
        (void)tag;
        _mm_free(ptr);
    }

    void operator delete[] (void *ptr) {
        //std::cout << "Deleting with alignment " << ALIGNMENT_SIZE << std::endl;
        _mm_free(ptr);
    }
    void operator delete[] (void *ptr, const std::nothrow_t& tag) {
        //std::cout << "Deleting with alignment " << ALIGNMENT_SIZE << std::endl;
        (void)tag;
        _mm_free(ptr);
    }

};


/*
** AlignedAllocator is a modification of Mallocator by:
** Stephan T. Lavavej, Visual C++ Libraries Developer
** https://blogs.msdn.microsoft.com/vcblog/2008/08/28/the-mallocator/
**
*/

template <typename T, std::size_t ALIGNMENT_SIZE = 0> class AlignedAllocator
{
public:

    // The following will be the same for virtually all allocators.
    typedef T * pointer;
    typedef const T * const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    T* address(T& r) const { return &r; }
    const T* address(const T& s) const { return &s; }

    size_t max_size() const {
        // The following has been carefully written to be independent of
        // the definition of size_t and to avoid signed/unsigned warnings.
        return (static_cast<size_t>(0) - static_cast<size_t>(1)) / sizeof(T);
    }


    // The following must be the same for all allocators.
    template <typename U> struct rebind {
        typedef AlignedAllocator<U, ALIGNMENT_SIZE> other;
    };

    bool operator!=(const AlignedAllocator& other) const { return !(*this == other); }

    void construct(T * const p, const T& t) const {
        void * const pv = static_cast<void *>(p);
        new (pv) T(t);
    }

    void destroy(T * const p) const { p->~T(); }


    // Returns true if and only if storage allocated from *this
    // can be deallocated from other, and vice versa.
    // Always returns true for stateless allocators.
    bool operator==(const AlignedAllocator& other) const { return true; }

    // Default constructor, copy constructor, rebinding constructor, and destructor.
    // Empty for stateless allocators.
    AlignedAllocator() { }
    AlignedAllocator(const AlignedAllocator&) { }
    template <typename U> AlignedAllocator(const AlignedAllocator<U, ALIGNMENT_SIZE>&) { }
    ~AlignedAllocator() { }

    // The following will be different for each allocator.
    T * allocate(const size_t n) const {
        // AlignedAllocator prints a diagnostic message to demonstrate
        // what it’s doing. Real allocators won’t do this.
        //std::cout << "Allocating " << n << (n == 1 ? " object" : " objects") << " of size " << sizeof(T) << " with a memory alignment of " << ALIGNMENT_SIZE << "." << std::endl;
        // The return value of allocate(0) is unspecified.
        // AlignedAllocator returns NULL in order to avoid depending
        // on malloc(0)’s implementation-defined behavior
        // (the implementation can define malloc(0) to return NULL,
        // in which case the bad_alloc check below would fire).
        // All allocators can return NULL in this case.
        if (n == 0) { return NULL; }
        // All allocators should contain an integer overflow check.
        // The Standardization Committee recommends that std::length_error
        // be thrown in the case of integer overflow.
        if (n > max_size()) {
            throw std::length_error("AlignedAllocator<T>::allocate() – Integer overflow.");
        }
        // AlignedAllocator wraps malloc().
        void * const pv = _mm_malloc(n * sizeof(T), ALIGNMENT_SIZE);
        // Allocators should throw std::bad_alloc in the case of memory allocation failure.
        if (pv == NULL) { throw std::bad_alloc(); }
        return static_cast<T *>(pv);
    }

    void deallocate(T * const p, const size_t n) const {
        (void)n;
        // AlignedAllocator prints a diagnostic message to demonstrate
        // what it’s doing. Real allocators won’t do this.
        //std::cout << "Deallocating " << n << (n == 1 ? " object" : " objects") << " of size " << sizeof(T) << " with a memory alignment of " << ALIGNMENT_SIZE << "." << std::endl;
        // AlignedAllocator wraps free().
        _mm_free(p);
    }

    // The following will be the same for all allocators that ignore hints.
    template <typename U> T * allocate(const size_t n, const U *) const {
        return allocate(n);
    }

private:
    AlignedAllocator& operator=(const AlignedAllocator&);

};

//
// Requires the type T to be a child of Aligned
//
template <typename T> class AlignedAllocator<T, 0>
{
public:

    // The following will be the same for virtually all allocators.
    typedef T * pointer;
    typedef const T * const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    T* address(T& r) const { return &r; }
    const T* address(const T& s) const { return &s; }

    size_t max_size() const {
        // The following has been carefully written to be independent of
        // the definition of size_t and to avoid signed/unsigned warnings.
        return (static_cast<size_t>(0) - static_cast<size_t>(1)) / sizeof(T);
    }


    // The following must be the same for all allocators.
    template <typename U> struct rebind {
        typedef AlignedAllocator<U> other;
    };

    bool operator!=(const AlignedAllocator& other) const { return !(*this == other); }

    void construct(T * const p, const T& t) const {
        void * const pv = static_cast<void *>(p);
        new (pv) T(t);
    }

    void destroy(T * const p) const { p->~T(); }


    // Returns true if and only if storage allocated from *this
    // can be deallocated from other, and vice versa.
    // Always returns true for stateless allocators.
    bool operator==(const AlignedAllocator& other) const { return true; }

    // Default constructor, copy constructor, rebinding constructor, and destructor.
    // Empty for stateless allocators.
    AlignedAllocator() { }
    AlignedAllocator(const AlignedAllocator&) { }
    template <typename U> AlignedAllocator(const AlignedAllocator<U>&) { }
    ~AlignedAllocator() { }

    // The following will be different for each allocator.
    T * allocate(const size_t n) const {
        // AlignedAllocator prints a diagnostic message to demonstrate
        // what it’s doing. Real allocators won’t do this.
        //std::cout << "Allocating " << n << (n == 1 ? " object" : " objects") << " of size " << sizeof(T) << " with a memory alignment of " << T::Aligned::alignment_size << "." << std::endl;
        // The return value of allocate(0) is unspecified.
        // AlignedAllocator returns NULL in order to avoid depending
        // on malloc(0)’s implementation-defined behavior
        // (the implementation can define malloc(0) to return NULL,
        // in which case the bad_alloc check below would fire).
        // All allocators can return NULL in this case.
        if (n == 0) { return NULL; }
        // All allocators should contain an integer overflow check.
        // The Standardization Committee recommends that std::length_error
        // be thrown in the case of integer overflow.
        if (n > max_size()) {
            throw std::length_error("AlignedAllocator<T>::allocate() – Integer overflow.");
        }
        // AlignedAllocator wraps malloc().
        void * const pv = _mm_malloc(n * sizeof(T), T::Aligned::alignment_size);
        // Allocators should throw std::bad_alloc in the case of memory allocation failure.
        if (pv == NULL) { throw std::bad_alloc(); }
        return static_cast<T *>(pv);
    }

    void deallocate(T * const p, const size_t n) const {
        (void)n;
        // AlignedAllocator prints a diagnostic message to demonstrate
        // what it’s doing. Real allocators won’t do this.
        //std::cout << "Deallocating " << n << (n == 1 ? " object" : " objects") << " of size " << sizeof(T) << " with a memory alignment of " << T::Aligned::alignment_size << "." << std::endl;
        // AlignedAllocator wraps free().
        _mm_free(p);
    }

    // The following will be the same for all allocators that ignore hints.
    template <typename U> T * allocate(const size_t n, const U *) const {
        return allocate(n);
    }

private:
    AlignedAllocator& operator=(const AlignedAllocator&);

};


/*
 * Simple version the AlignedAllocator that requires the type T to be a child of Aligned
 * But does not support specifying the alignment as a second template parameter
 *
template <typename T>
class AlignedAllocator {
public:
  typedef T value_type;
  AlignedAllocator() {} // default constructor not required
  template <typename U> AlignedAllocator(const AlignedAllocator<U>&) { }
  template <typename U> bool operator==(const AlignedAllocator<U>&) const { return true; }
  template <typename U> bool operator!=(const AlignedAllocator<U>&) const { return false; }
  T* allocate(const std::size_t n) const {
      //std::cout << "T* allocate(const std::size_t n) with alignment = " << T::alignment_size << std::endl;
      if (n == 0) { return NULL; }
      if (n > static_cast<std::size_t>(-1) / sizeof(T)) {
          throw std::bad_array_new_length();
      }
      void* const pv = _mm_malloc(n * sizeof(T), T::alignment_size);
      if (pv == NULL) { throw std::bad_alloc(); }
      return static_cast<T*>(pv);
  }
  void deallocate(T* const p, std::size_t) const {
      //std::cout << "void deallocate(T* const p, std::size_t) with alignment = " << T::alignment_size << std::endl;
      _mm_free(p);
  }
};
*/

#endif // !_ALIGNED_H
