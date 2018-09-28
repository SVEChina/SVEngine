//
// SVCoreAllocator.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
// copy from irr
//

#ifndef SV_ALLOCATOR_H
#define SV_ALLOCATOR_H

#include "SVTypes.h"
#include <memory.h>
#include <new>

namespace sv{

#ifdef DEBUG_CLIENTBLOCK
#undef DEBUG_CLIENTBLOCK
#define DEBUG_CLIENTBLOCK new
#endif

//! Very simple allocator implementation, containers using it can be used across dll boundaries
    template<typename T>
    class SVCoreAllocator {
    public:

        //! Destructor
        virtual ~SVCoreAllocator() {}

        //! Allocate memory for an array of objects
        T *allocate(size_t cnt) {
            return (T *) internal_new(cnt * sizeof(T));
        }

        //! Deallocate memory for an array of objects
        void deallocate(T *ptr) {
            internal_delete(ptr);
        }

        //! Construct an element
        void construct(T *ptr, const T &e) {
            new((void *) ptr) T(e);
        }

        //! Destruct an element
        void destruct(T *ptr) {
            ptr->~T();
        }

    protected:

        virtual void *internal_new(size_t cnt) {
            return operator new(cnt);
        }

        virtual void internal_delete(void *ptr) {
            operator delete(ptr);
        }

    };


//! Fast allocator, only to be used in containers inside the same memory heap.
/** Containers using it are NOT able to be used it across dll boundaries. Use this
when using in an internal class or function or when compiled into a static lib */
    template<typename T>
    class irrAllocatorFast {
    public:

        //! Allocate memory for an array of objects
        T *allocate(size_t cnt) {
            return (T *) operator new(cnt * sizeof(T));
        }

        //! Deallocate memory for an array of objects
        void deallocate(T *ptr) {
            operator delete(ptr);
        }

        //! Construct an element
        void construct(T *ptr, const T &e) {
            new((void *) ptr) T(e);
        }

        //! Destruct an element
        void destruct(T *ptr) {
            ptr->~T();
        }
    };

#ifdef DEBUG_CLIENTBLOCK
#undef DEBUG_CLIENTBLOCK
#define DEBUG_CLIENTBLOCK new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

//! defines an allocation strategy
    enum eAllocStrategy {
        ALLOC_STRATEGY_SAFE = 0,
        ALLOC_STRATEGY_DOUBLE = 1,
        ALLOC_STRATEGY_SQRT = 2
    };

}//!namespace sv

#endif  //__SV_ALLOCATOR_H_INCLUDED__

