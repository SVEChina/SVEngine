#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "SVBase.h"
#include "SVPreDeclare.h"
//
namespace sv {
    
//    typedef bool (*sv_compare)(void* _e1,void* _e2);
//    typedef bool (*sv_compare_share)(SVObjectPtr _e1,SVObjectPtr _e2);
    
    namespace util {
        
        //
        template <class Type,class Counter = s32> class SVArray {
        public:
            class Iterator {
            private:
                friend class SVArray;
                Iterator(Type *ptr)
                : ptr(ptr) {
                }

            public:
                typedef Type DataType;
                
                Iterator()
                : ptr(0) {
                }
                
                Iterator(const Iterator &it)
                : ptr(it.ptr) {
                }
                
                ~Iterator() {
                }
                
                sv_inline Iterator &operator=(const Iterator &it) {
                    ptr = it.ptr;
                    return *this;
                }
                
                sv_inline s32 operator==(const Iterator &it) const {
                    return ptr == it.ptr;
                }
                
                sv_inline s32 operator!=(const Iterator &it) const {
                    return ptr != it.ptr;
                }
                
                sv_inline Type &operator*() {
                    return *ptr;
                }
                
                sv_inline const Type &operator*() const {
                    return *ptr;
                }
                
                sv_inline Type *operator->() {
                    return ptr;
                }
                
                sv_inline const Type *operator->() const {
                    return ptr;
                }
                
                sv_inline Type &get() {
                    return *ptr;
                }
                
                sv_inline const Type &get() const {
                    return *ptr;
                    
                }
                
                sv_inline Iterator &operator++() {
                    ptr++;
                    return *this;
                }
                
                sv_inline Iterator operator++(s32) {
                    Iterator temp = *this;
                    ptr++;
                    return temp;
                }
                
                Iterator operator+(s32 n) const {
                    return Iterator(ptr + n);
                }
                
                Iterator operator-(s32 n) const {
                    return Iterator(ptr - n);
                }
                
                Iterator &operator+=(s32 n) {
                    ptr += n;
                    return *this;
                }
                
                Iterator &operator-=(s32 n) {
                    ptr -= n;
                    return *this;
                }
                
                s32 operator-(const Iterator &it) const {
                    return (s32)(ptr - it.ptr);
                }
                
            private:
                Type *ptr;
            };
            
            // SVArray
            SVArray()
            : length(0)
            , capacity(0)
            , data(0) {
            }
            
            SVArray(const SVArray &v)
            : length(0)
            , capacity(0)
            , data(0) {
                append(v);
            }
            
            SVArray(const Type &t,s32 size)
            : length(0)
            , capacity(0)
            , data(0) {
                resize(size);
                for(s32 i = 0; i < size; i++) {
                    data[i] = t;
                }
            }
            
            SVArray(const Type *v,s32 size)
            : length(0)
            , capacity(0)
            , data(0) {
                append(v,size);
            }
            
            explicit SVArray(s32 size)
            : length(0)
            , capacity(0)
            , data(0) {
                resize(size);
            }
            
            ~SVArray() {
                destroy();
            }
            
            SVArray &operator=(const SVArray &v) {
                if(this == &v) {
                    return *this;
                }
                resize(v.length);
                for(s32 i = 0; i < length; i++) {
                    data[i] = v.data[i];
                }
                return *this;
            }
            
            void swap(SVArray &v) {
                if(this == &v) {
                }
                if((capacity & DYNAMIC_FLAG) && (v.capacity & DYNAMIC_FLAG)) {
                    Counter l = length;
                    length = v.length;
                    v.length = l;
                    Counter c = capacity;
                    capacity = v.capacity;
                    v.capacity = c;
                    Type *d = data;
                    data = v.data;
                    v.data = d;
                } else {
                    SVArray<Type> temp = *this;
                    *this = v;
                    v = temp;
                }
            }
            
            sv_inline Type &operator[](s32 index) {
                assert((u32)index < (u32)length && "SVArray::operator[](): bad index");
                return data[index];
            }
            sv_inline const Type &operator[](s32 index) const {
                assert((u32)index < (u32)length && "SVArray::operator[](): bad index");
                return data[index];
            }
            
            sv_inline void set(s32 index,const Type &t) {
                assert((u32)index < (u32)length && "SVArray::set(): bad index");
                data[index] = t;
            }
            
            sv_inline Type &get(s32 index) {
                assert((u32)index < (u32)length && "SVArray::get(): bad index");
                return data[index];
            }
            
            sv_inline const Type &get(s32 index) const {
                assert((u32)index < (u32)length && "SVArray::get(): bad index");
                return data[index];
            }
            
            sv_inline Type *get() {
                return data;
            }
            
            sv_inline const Type *get() const {
                return data;
            }
            
            sv_inline Iterator begin() const {
                return Iterator(data);
            }
            
            sv_inline Iterator back() const {
                return Iterator(data + length - 1);
            }
            
            sv_inline Iterator end() const {
                return Iterator(data + length);
            }
            
            sv_inline s32 size() const {
                return length;
            }
            
            sv_inline s32 empty() const {
                return (length == 0);
            }
            
            void resize(s32 size) {
                allocate(size);
                length = size;
            }
            
            void allocate(s32 size) {
                if(size <= (capacity & CAPACITY_MASK))
                    return;
                s32 dynamic = (capacity & DYNAMIC_FLAG);
                capacity = size | DYNAMIC_FLAG;
                Type *new_data = new Type[size];
                for(s32 i = 0; i < length; i++) {
                    new_data[i] = data[i];
                }
                if(dynamic) {
                    delete [] data;
                }
                data = new_data;
            }
            
            void reserve(s32 size) {
                if(size <= (capacity & CAPACITY_MASK))
                    return;
                s32 dynamic = (capacity & DYNAMIC_FLAG);
                capacity = (size * 2) | DYNAMIC_FLAG;
                Type *new_data = new Type[size * 2];
                for(s32 i = 0; i < length; i++) {
                    new_data[i] = data[i];
                }
                if(dynamic) {
                    delete [] data;
                }
                data = new_data;
            }
            
            sv_inline void clear() {
                length = 0;
            }

            void destroy() {
                s32 dynamic = (capacity & DYNAMIC_FLAG);
                length = 0;
                capacity = 0;
                if(dynamic) {
                    delete [] data;
                }
                data = 0;
            }
            
            template <class T> Iterator find(const T &t) const {
                for(s32 i = 0; i < length; i++) {
                    if(data[i] == t) {
                        return Iterator(data + i);
                    }
                }
                return Iterator(data + length);
            }
            
            template <class T> s32 findIndex(const T &t) const {
                for(s32 i = 0; i < length; i++) {
                    if(data[i] == t) {
                        return i;
                    }
                }
                return -1;
            }
            
            template <class T> s32 leftIndex(const T &t) const {
                if(length == 0 || t < data[0])
                    return -1;
                if(data[length - 1] < t)
                    return length - 1;
                s32 left = 0;
                s32 right = length - 1;
                while(left <= right) {
                    s32 middle = (left + right) >> 1;
                    if(data[middle] < t) {
                        left = middle + 1;
                    } else if(t < data[middle]) {
                        right = middle - 1;
                    } else {
                        return middle - 1;
                    }
                }
                return left - 1;
            }
            
            template <class T> s32 rightIndex(const T &t) const {
                if(length == 0 || data[length - 1] < t)
                    return -1;
                if(t < data[0])
                    return 0;
                s32 left = 0;
                s32 right = length - 1;
                while(left <= right) {
                    s32 middle = (left + right) >> 1;
                    if(data[middle] < t) {
                        left = middle + 1;
                    } else if(t < data[middle]) {
                        right = middle - 1;
                    } else {
                        return middle;
                    }
                }
                return right + 1;
            }
            
            sv_inline Type &append() {
                if(length + 1 > (capacity & CAPACITY_MASK)) {
                    reserve(length + 1);
                }
                static Type t;
                data[(s32)length] = t;
                return data[(s32)(length++)];
            }
            
            sv_inline void append(const Type &t) {
                if(length + 1 > (capacity & CAPACITY_MASK)) {
                    reserve(length + 1);
                }
                data[(s32)(length++)] = t;
            }
            
            sv_inline void appendFast(const Type &t) {
                data[(s32)(length++)] = t;
            }
            
            void append(s32 pos,const Type &t) {
                assert((u32)pos <= (u32)length && "SVArray::append(): bad position");
                if(length + 1 <= (capacity & CAPACITY_MASK)) {
                    for(s32 i = length - 1; i >= pos; i--) {
                        data[i + 1] = data[i];
                    }
                    data[pos] = t;
                    length++;
                    return;
                }
                s32 dynamic = (capacity & DYNAMIC_FLAG);
                capacity = (length * 2 + 1) | DYNAMIC_FLAG;
                Type *new_data = new Type[length * 2 + 1];
                for(s32 i = 0; i < pos; i++) {
                    new_data[i] = data[i];
                }
                new_data[pos] = t;
                for(s32 i = pos; i < length; i++) {
                    new_data[i + 1] = data[i];
                }
                if(dynamic) {
                    delete [] data;
                }
                data = new_data;
                length++;
            }
            
            sv_inline void append(const Iterator &it,const Type &t) {
                append(it - begin(),t);
            }
            
            void append(const SVArray &v) {
                if(length + v.length > (capacity & CAPACITY_MASK)) {
                    reserve(length + v.length);
                }
                for(s32 i = 0; i < v.length; i++) {
                    data[(s32)(length++)] = v.data[i];
                }
            }
            void append(const Type *v,s32 size) {
                if(length + size > (capacity & CAPACITY_MASK)) {
                    reserve(length + size);
                }
                for(s32 i = 0; i < size; i++) {
                    data[(s32)(length++)] = v[i];
                }
            }
            
            sv_inline void remove() {
                assert(length > 0 && "SVArray::remove(): bad length");
                length--;
            }
            
            void remove(s32 pos,s32 size = 1) {
                assert((u32)pos < (u32)length && "SVArray::remove(): bad position");
                assert(size >= 0 && pos + size <= length && "SVArray::remove(): bad size");
                for(s32 i = pos; i < length - size; i++) {
                    data[i] = data[i + size];
                }
                length -= size;
                
            }
            
            void removeForce(s32 pos,s32 size = 1) {
                assert((u32)pos < (u32)length && "SVArray::remove(): bad position");
                assert(size >= 0 && pos + size <= length && "SVArray::remove(): bad size");
                for(s32 i = pos; i < length - size; i++) {
                    data[i] = data[i + size];
                }
                length -= size;
                reserveForce(length);
            }
            
            void reserveForce(s32 size) {
                s32 dynamic = (capacity & DYNAMIC_FLAG);
                capacity = (size * 2) | DYNAMIC_FLAG;
                Type *new_data = new Type[size * 2];
                for(s32 i = 0; i < length; i++) {
                    new_data[i] = data[i];
                }
                if(dynamic) {
                    delete [] data;
                }
                data = new_data;
            }
            
            sv_inline void remove(const Iterator &it) {
                remove(it - begin());
            }
            
            sv_inline void removeFast(s32 pos) {
                assert((u32)pos < (u32)length && "SVArray::removeFast(): bad position");
                data[pos] = data[(s32)(length - 1)];
                length--;
            }
            
            sv_inline void removeFast(const Iterator &it) {
                removeFast(it - begin());
            }
            
        protected:
            enum {
                DYNAMIC_FLAG = (Counter)(1 << (sizeof(Counter) * 8 - 1)),
                CAPACITY_MASK = ~(Counter)DYNAMIC_FLAG,
            };
            Counter length;
            Counter capacity;
            Type *data;
        };
        
        //
        template <class Type,s32 Capacity = 128,class Counter = s32> class SVArrayStack : public SVArray<Type,Counter> {
        public:
            SVArrayStack() {
                SVArray<Type,Counter>::capacity = Capacity;
                SVArray<Type,Counter>::data = stack_data;
            }
            
            explicit SVArrayStack(s32 size) {
                SVArray<Type,Counter>::capacity = Capacity;
                SVArray<Type,Counter>::data = stack_data;
                SVArray<Type,Counter>::resize(size);
            }
            
            SVArrayStack(const SVArray<Type,Counter> &v) {
                SVArray<Type,Counter>::capacity = Capacity;
                SVArray<Type,Counter>::data = stack_data;
                SVArray<Type,Counter>::append(v);
            }
            
            SVArrayStack(const Type &t,s32 size) {
                SVArray<Type,Counter>::capacity = Capacity;
                SVArray<Type,Counter>::data = stack_data;
                SVArray<Type,Counter>::resize(size);
                for(s32 i = 0; i < size; i++) {
                    SVArray<Type,Counter>::data[i] = t;
                }
            }
            
            SVArrayStack(const Type *v,s32 size) {
                SVArray<Type,Counter>::capacity = Capacity;
                SVArray<Type,Counter>::data = stack_data;
                SVArray<Type,Counter>::append(v,size);
            }
            
            ~SVArrayStack() {
            }
            
            SVArrayStack &operator=(const SVArrayStack &v) {
                if(this == &v)
                    return *this;
                SVArray<Type,Counter>::resize(v.length);
                for(s32 i = 0; i < SVArray<Type,Counter>::length; i++) {
                    SVArray<Type,Counter>::data[i] = v.data[i];
                }
                return *this;
            }
            
            SVArrayStack &operator=(const SVArray<Type,Counter> &v) {
                if(this == &v)
                    return *this;
                SVArray<Type,Counter>::resize(v.size());
                for(s32 i = 0; i < SVArray<Type,Counter>::length; i++) {
                    SVArray<Type,Counter>::data[i] = v[i];
                }
                return *this;
            }
            
            void destroy() {
                SVArray<Type,Counter>::destroy();
                SVArray<Type,Counter>::capacity = Capacity;
                SVArray<Type,Counter>::data = stack_data;
            }
        private:
            Type stack_data[Capacity];
        };
        
    }//!namespace util
    
}//!namespace sv



#endif /* __VECTOR_H__ */
