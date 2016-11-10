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

class BadAlignment: public std::exception {

public:

    std::size_t alignment;
    std::string log;

    BadAlignment(std::size_t alignment) : alignment(alignment) {
        std::stringstream ss;
        ss << "Memory not " << alignment << " bytes memory aligned!";
        log = ss.str();
    }

    virtual const char* what() const throw() {
        return log.c_str();
    }

};

template <std::size_t ALIGNMENT>
class alignas(ALIGNMENT) Aligned
{

protected:

    Aligned() {
        if ((reinterpret_cast<std::size_t>(this) % ALIGNMENT) != 0)
            throw BadAlignment(ALIGNMENT);
    }

public:

    void* operator new (std::size_t size) {
        void* ptr = _mm_malloc (size, ALIGNMENT);
        if (ptr == NULL)
            throw std::bad_alloc();
        return ptr;
    }
    void* operator new (std::size_t size, const std::nothrow_t& tag) {
        return _mm_malloc (size, ALIGNMENT);
    }
    void* operator new (std::size_t size, void* ptr) {
        (void)size;
        if (reinterpret_cast<std::size_t>(ptr) % ALIGNMENT != 0)
            throw BadAlignment(ALIGNMENT);
        return ptr;
    }

    void* operator new[] (std::size_t size) {
        void* ptr = _mm_malloc (size, ALIGNMENT);
        if (ptr == NULL)
            throw std::bad_alloc();
        return ptr;
    }
    void* operator new[] (std::size_t size, const std::nothrow_t& tag) {
        return _mm_malloc (size, ALIGNMENT);
    }
    void* operator new[] (std::size_t size, void* ptr) {
        (void)size;
        if (reinterpret_cast<std::size_t>(ptr) % ALIGNMENT != 0)
            throw BadAlignment(ALIGNMENT);
        return ptr;
    }

    void operator delete (void *ptr) {
        _mm_free(ptr);
    }
    void operator delete (void *ptr, const std::nothrow_t& tag) {
        _mm_free(ptr);
    }

    void operator delete[] (void *ptr) {
        _mm_free(ptr);
    }
    void operator delete[] (void *ptr, const std::nothrow_t& tag) {
        _mm_free(ptr);
    }

};

/**
 * Allocator for aligned data.
 *
 * Modified from the Mallocator from Stephan T. Lavavej.
 * <http://blogs.msdn.com/b/vcblog/archive/2008/08/28/the-mallocator.aspx>
 */
/*
template <typename T, std::size_t ALIGNMENT>
class AlignedAllocator
{

    public:

        // The following will be the same for virtually all allocators.
        typedef T * pointer;
        typedef const T * const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T value_type;
        typedef std::size_t size_type;
        typedef ptrdiff_t difference_type;

        T * address(T& r) const
        {
            return &r;
        }

        const T * address(const T& s) const
        {
            return &s;
        }

        std::size_t max_size() const
        {
            // The following has been carefully written to be independent of
            // the definition of size_t and to avoid signed/unsigned warnings.
            return (static_cast<std::size_t>(0) - static_cast<std::size_t>(1)) / sizeof(T);
        }


        // The following must be the same for all allocators.
        template <typename U>
        struct rebind
        {
            typedef AlignedAllocator<U, ALIGNMENT> other;
        } ;

        bool operator!=(const AlignedAllocator& other) const
        {
            return !(*this == other);
        }

        void construct(T * const p, const T& t) const
        {
            new (static_cast<void *>(p)) T(t);
        }

        void destroy(T * const p) const
        {
            p->~T();
        }

        // Returns true if and only if storage allocated from *this
        // can be deallocated from other, and vice versa.
        // Always returns true for stateless allocators.
        bool operator==(const AlignedAllocator& other) const
        {
            return true;
        }


        // Default constructor, copy constructor, rebinding constructor, and destructor.
        // Empty for stateless allocators.
        AlignedAllocator() { }

        AlignedAllocator(const AlignedAllocator&) { }

        template <typename U> AlignedAllocator(const AlignedAllocator<U, ALIGNMENT>&) { }

        ~AlignedAllocator() { }


        // The following will be different for each allocator.
        T * allocate(const std::size_t n) const
        {
            // The return value of allocate(0) is unspecified.
            // Mallocator returns NULL in order to avoid depending
            // on malloc(0)'s implementation-defined behavior
            // (the implementation can define malloc(0) to return NULL,
            // in which case the bad_alloc check below would fire).
            // All allocators can return NULL in this case.
            if (n == 0) {
                return NULL;
            }

            // All allocators should contain an integer overflow check.
            // The Standardization Committee recommends that std::length_error
            // be thrown in the case of integer overflow.
            if (n > max_size())
            {
                throw std::length_error("AlignedAllocator<T>::allocate() - Integer overflow.");
            }

            // Mallocator wraps malloc().
            void * const pv = _mm_malloc(n * sizeof(T), ALIGNMENT);

            // Allocators should throw std::bad_alloc in the case of memory allocation failure.
            if (pv == NULL)
            {
                throw std::bad_alloc();
            }

            return static_cast<T *>(pv);
        }

        void deallocate(T * const p, const std::size_t n) const
        {
            (void)n;
            _mm_free(p);
        }


        // The following will be the same for all allocators that ignore hints.
        template <typename U>
        T * allocate(const std::size_t n, const U *) const //const U * => const Hint
        {
            return allocate(n);
        }


        // Allocators are not required to be assignable, so
        // all allocators should have a private unimplemented
        // assignment operator. Note that this will trigger the
        // off-by-default (enabled under /Wall) warning C4626
        // "assignment operator could not be generated because a
        // base class assignment operator is inaccessible" within
        // the STL headers, but that warning is useless.
    private:
        AlignedAllocator& operator=(const AlignedAllocator&);
};
*/

#endif // !_ALIGNED_H
