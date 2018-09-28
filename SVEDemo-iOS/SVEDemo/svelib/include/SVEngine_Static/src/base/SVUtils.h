#ifndef SV_UTILS_H
#define SV_UTILS_H

#include "SVBase.h"

#ifdef min
	#undef min
#endif

#ifdef max
	#undef max
#endif

#define SORT_STACK	1024

namespace sv {
    
    namespace util {
        
        /******************************************************************************\
         *
         * min/max/clamp/lerp
         *
         \******************************************************************************/
        
        template <class Type> sv_inline Type min(Type v0,Type v1) {
            return (v0 < v1) ? v0 : v1;
        }
        
        template <class Type> sv_inline Type max(Type v0,Type v1) {
            return (v0 > v1) ? v0 : v1;
        }
        
        template <class Type> sv_inline Type clamp(Type v,Type v0,Type v1) {
            if(v < v0) return v0;
            if(v > v1) return v1;
            return v;
        }
        
        template <class Type> sv_inline Type lerp(Type v0,Type v1,Type k) {
            return v0 + (v1 - v0) * k;
        }
        
        /******************************************************************************\
         *
         * swap
         *
         \******************************************************************************/
        
        //
        template <class Type> sv_inline void swap(Type &v0,Type &v1) {
            Type temp = v0;
            v0 = v1;
            v1 = temp;
        }
        
        /******************************************************************************\
         *
         * quickSort
         *
         \******************************************************************************/
        
        //
        template <class Type> struct quick_sort_default_compare {
            sv_inline s32 operator()(const Type &t0,const Type &t1) const {
                return (t0 < t1);
            }
        };
        
        template <class Type> struct quick_sort_default_compare<Type*> {
            sv_inline s32 operator()(const Type *t0,const Type *t1) const {
                return (t0 < t1);
            }
        };
        
        //
        template <> struct quick_sort_default_compare<char> {
            sv_inline s32 operator()(char i0,char i1) const {
                return (i0 < i1);
            }
        };
        
        template <> struct quick_sort_default_compare<u8> {
            sv_inline s32 operator()(u8 i0,u8 i1) const {
                return (i0 < i1);
            }
        };
        
        //
        template <> struct quick_sort_default_compare<short> {
            sv_inline s32 operator()(short i0,short i1) const {
                return (i0 < i1);
            }
        };
        
        template <> struct quick_sort_default_compare<unsigned short> {
            sv_inline s32 operator()(unsigned short i0,unsigned short i1) const {
                return (i0 < i1);
            }
        };
        
        //
        template <> struct quick_sort_default_compare<s32> {
            sv_inline s32 operator()(s32 i0,s32 i1) const {
                return (i0 < i1);
            }
        };
        
        template <> struct quick_sort_default_compare<u32> {
            sv_inline s32 operator()(u32 i0,u32 i1) const {
                return (i0 < i1);
            }
        };
        
        //
        template <> struct quick_sort_default_compare<f32> {
            sv_inline s32 operator()(f32 f0,f32 f1) const {
                return (f0 < f1);
            }
        };
        
        //
        template <class Type,class Func> struct quick_sort_function_compare {
            quick_sort_function_compare(Func func) : func(func) { }
            sv_inline s32 operator()(const Type &t0,const Type &t1) const {
                return func(t0,t1);
            }
            Func func;
        };
        
        //
        template <class Type,class Compare> void quick_sort(Type *array,s32 size,Compare compare) {
            s32 depth = 1;
            s32 left_stack[SORT_STACK];
            s32 right_stack[SORT_STACK];
            left_stack[0] = 0;
            right_stack[0] = size - 1;
            while(depth > 0) {
                s32 changed = 0;
                s32 left = left_stack[--depth];
                s32 right = right_stack[depth];
                s32 l = left;
                s32 r = right;
                s32 m = (l + r) >> 1;
                do {
                    const Type &c = array[m];
                    while(l < right && compare(array[l],c)) l++;
                    while(r > left && compare(c,array[r])) r--;
                    if(l < r) {
                        changed = 1;
                        swap(array[l],array[r]);
                        if(m == l) m = r;
                        else if(m == r) m = l;
                        l++;
                        r--;
                    } else if(l == r) {
                        l++;
                        r--;
                    }
                } while(l <= r);
                if(changed == 0) {
                    for(s32 i = left + 1; i <= right; i++) {
                        for(s32 j = i, k = i - 1; j > left && compare(array[j],array[k]); j--, k--) {
                            swap(array[k],array[j]);
                        }
                    }
                } else {
                    if(left < r) {
                        assert(depth < SORT_STACK && "quick_sort(): stack overflow");
                        left_stack[depth] = left;
                        right_stack[depth++] = r;
                    }
                    if(l < right) {
                        assert(depth < SORT_STACK && "quick_sort(): stack overflow");
                        left_stack[depth] = l;
                        right_stack[depth++] = right;
                    }
                }
            }
        }
        
        //
        template <class Type> void quickSort(Type *array,s32 size) {
            if(size < 2) return;
            quick_sort_default_compare<Type> compare;
            quick_sort(array,size,compare);
        }
        
        template <class Type,class Compare> void quickSort(Type *array,s32 size,Compare compare) {
            if(size < 2)
                return;
            quick_sort(array,size,compare);
        }
        
        template <class Type,class A0,class A1> void quickSort(Type *array,s32 size,s32 (*func)(A0,A1)) {
            if(size < 2)
                return;
            quick_sort_function_compare<Type,s32 (*)(A0,A1)> compare(func);
            quick_sort(array,size,compare);
        }
        
        //
        template <class Iterator> void quickSort(Iterator begin,Iterator end) {
            s32 size = end - begin;
            if(size < 2)
                return;
            quick_sort_default_compare<typename Iterator::DataType> compare;
            quick_sort(&(*begin),size,compare);
        }
        
        template <class Iterator,class Compare> void quickSort(Iterator begin,Iterator end,Compare compare) {
            s32 size = end - begin;
            if(size < 2)
                return;
            quick_sort(&(*begin),size,compare);
        }
        
        template <class Iterator,class A0,class A1> void quickSort(Iterator begin,Iterator end,s32 (*func)(A0,A1)) {
            s32 size = end - begin;
            if(size < 2)
                return;
            quick_sort_function_compare<typename Iterator::DataType,s32 (*)(A0,A1)> compare(func);
            quick_sort(&(*begin),size,compare);
        }
        
        /******************************************************************************\
         *
         * quickDoubleSort
         *
         \******************************************************************************/
        
        //
        template <class Type,class Data,class Compare> void quick_double_sort(Type *array,Data *data,s32 size,Compare compare) {
            s32 depth = 1;
            s32 left_stack[SORT_STACK];
            s32 right_stack[SORT_STACK];
            left_stack[0] = 0;
            right_stack[0] = size - 1;
            while(depth > 0) {
                s32 changed = 0;
                s32 left = left_stack[--depth];
                s32 right = right_stack[depth];
                s32 l = left;
                s32 r = right;
                s32 m = (l + r) >> 1;
                do {
                    const Type &c = array[m];
                    while(l < right && compare(array[l],c)) l++;
                    while(r > left && compare(c,array[r])) r--;
                    if(l < r) {
                        changed = 1;
                        swap(array[l],array[r]);
                        swap(data[l],data[r]);
                        if(m == l) m = r;
                        else if(m == r) m = l;
                        l++;
                        r--;
                    } else if(l == r) {
                        l++;
                        r--;
                    }
                } while(l <= r);
                if(changed == 0) {
                    for(s32 i = left + 1; i <= right; i++) {
                        for(s32 j = i, k = i - 1; j > left && compare(array[j],array[k]); j--, k--) {
                            swap(array[k],array[j]);
                            swap(data[k],data[j]);
                        }
                    }
                } else {
                    if(left < r) {
                        assert(depth < SORT_STACK && "quick_double_sort(): stack overflow");
                        left_stack[depth] = left;
                        right_stack[depth++] = r;
                    }
                    if(l < right) {
                        assert(depth < SORT_STACK && "quick_double_sort(): stack overflow");
                        left_stack[depth] = l;
                        right_stack[depth++] = right;
                    }
                }
            }
        }
        
        //
        template <class Type,class Data> void quickDoubleSort(Type *array,Data *data,s32 size) {
            if(size < 2)
                return;
            quick_sort_default_compare<Type> compare;
            quick_double_sort(array,data,size,compare);
        }
        
        template <class Type,class Data,class Compare> void quickDoubleSort(Type *array,Data *data,s32 size,Compare compare) {
            if(size < 2)
                return;
            quick_double_sort(array,data,size,compare);
        }
        
        template <class Type,class Data,class A0,class A1> void quickDoubleSort(Type *array,Data *data,s32 size,s32 (*func)(A0,A1)) {
            if(size < 2)
                return;
            quick_sort_function_compare<Type,s32 (*)(A0,A1)> compare(func);
            quick_double_sort(array,data,size,compare);
        }
        
        /******************************************************************************\
         *
         * quickSort2
         *
         \******************************************************************************/
        
        //
        template <class Type> struct quick_sort_2_default_compare {
            sv_inline s32 operator()(const Type &t0,const Type &t1) const {
                if(t0 < t1)
                    return 1;
                if(t0 > t1)
                    return -1;
                return 0;
            }
        };
        
        template <class Type> struct quick_sort_2_default_compare<Type*> {
            sv_inline s32 operator()(const Type *t0,const Type *t1) const {
                if(t0 < t1)
                    return 1;
                if(t0 > t1)
                    return -1;
                return 0;
            }
        };
        
        //
        template <> struct quick_sort_2_default_compare<char> {
            sv_inline s32 operator()(char i0,char i1) const {
                if(i0 < i1)
                    return 1;
                if(i0 > i1)
                    return -1;
                return 0;
            }
        };
        
        template <> struct quick_sort_2_default_compare<u8> {
            sv_inline s32 operator()(u8 i0,u8 i1) const {
                if(i0 < i1)
                    return 1;
                if(i0 > i1)
                    return -1;
                return 0;
            }
        };
        
        //
        template <> struct quick_sort_2_default_compare<short> {
            sv_inline s32 operator()(short i0,short i1) const {
                if(i0 < i1)
                    return 1;
                if(i0 > i1)
                    return -1;
                return 0;
            }
        };
        
        template <> struct quick_sort_2_default_compare<unsigned short> {
            sv_inline s32 operator()(unsigned short i0,unsigned short i1) const {
                if(i0 < i1)
                    return 1;
                if(i0 > i1)
                    return -1;
                return 0;
            }
        };
        
        //
        template <> struct quick_sort_2_default_compare<s32> {
            sv_inline s32 operator()(s32 i0,s32 i1) const {
                if(i0 < i1)
                    return 1;
                if(i0 > i1)
                    return -1;
                return 0;
            }
        };
        
        template <> struct quick_sort_2_default_compare<u32> {
            sv_inline s32 operator()(u32 i0,u32 i1) const {
                if(i0 < i1)
                    return 1;
                if(i0 > i1)
                    return -1;
                return 0;
            }
        };
        
        //
        template <> struct quick_sort_2_default_compare<f32> {
            sv_inline s32 operator()(f32 f0,f32 f1) const {
                if(f0 < f1)
                    return 1;
                if(f0 > f1)
                    return -1;
                return 0;
            }
        };
        
        //
        template <class Type,class Func> struct quick_sort_2_function_compare {
            quick_sort_2_function_compare(Func func) : func(func) { }
            sv_inline s32 operator()(const Type &t0,const Type &t1) const {
                return func(t0,t1);
            }
            Func func;
        };
        
        //
        template <class Type,class Compare> void quick_sort_2(Type *array,s32 size,Compare compare) {
            s32 depth = 1;
            s32 left_stack[SORT_STACK];
            s32 right_stack[SORT_STACK];
            left_stack[0] = 0;
            right_stack[0] = size - 1;
            while(depth > 0) {
                s32 changed = 0;
                s32 left = left_stack[--depth];
                s32 right = right_stack[depth];
                s32 l = left;
                s32 r = right;
                s32 m = (l + r) >> 1;
                do {
                    const Type &c = array[m];
                    while(l < right && compare(array[l],c) > 0) l++;
                    while(r > left && compare(array[r],c) < 0) r--;
                    if(l < r) {
                        changed = 1;
                        swap(array[l],array[r]);
                        if(m == l)
                            m = r;
                        else if(m == r)
                            m = l;
                        l++;
                        r--;
                    } else if(l == r) {
                        l++;
                        r--;
                    }
                } while(l <= r);
                if(changed == 0) {
                    for(s32 i = left + 1; i <= right; i++) {
                        for(s32 j = i, k = i - 1; j > left && compare(array[j],array[k]) > 0; j--, k--) {
                            swap(array[k],array[j]);
                        }
                    }
                } else {
                    if(left < r) {
                        assert(depth < SORT_STACK && "quick_sort_2(): stack overflow");
                        left_stack[depth] = left;
                        right_stack[depth++] = r;
                    }
                    if(l < right) {
                        assert(depth < SORT_STACK && "quick_sort_2(): stack overflow");
                        left_stack[depth] = l;
                        right_stack[depth++] = right;
                    }
                }
            }
        }
        
        //
        template <class Type> void quickSort2(Type *array,s32 size) {
            if(size < 2) return;
            quick_sort_2_default_compare<Type> compare;
            quick_sort_2(array,size,compare);
        }
        
        template <class Type,class Compare> void quickSort2(Type *array,s32 size,Compare compare) {
            if(size < 2) return;
            quick_sort_2(array,size,compare);
        }
        
        template <class Type,class A0,class A1> void quickSort2(Type *array,s32 size,s32 (*func)(A0,A1)) {
            if(size < 2) return;
            quick_sort_2_function_compare<Type,s32 (*)(A0,A1)> compare(func);
            quick_sort_2(array,size,compare);
        }
        
        //
        template <class Iterator> void quickSort2(Iterator begin,Iterator end) {
            s32 size = end - begin;
            if(size < 2) return;
            quick_sort_2_default_compare<typename Iterator::DataType> compare;
            quick_sort_2(&(*begin),size,compare);
        }
        
        template <class Iterator,class Compare> void quickSort2(Iterator begin,Iterator end,Compare compare) {
            s32 size = end - begin;
            if(size < 2) return;
            quick_sort_2(&(*begin),size,compare);
        }
        
        template <class Iterator,class A0,class A1> void quickSort2(Iterator begin,Iterator end,s32 (*func)(A0,A1)) {
            s32 size = end - begin;
            if(size < 2) return;
            quick_sort_2_function_compare<typename Iterator::DataType,s32 (*)(A0,A1)> compare(func);
            quick_sort_2(&(*begin),size,compare);
        }
        
        /******************************************************************************\
         *
         * quickDoubleSort2
         *
         \******************************************************************************/
        
        //
        template <class Type,class Data,class Compare> void quick_double_sort_2(Type *array,Data *data,s32 size,Compare compare) {
            s32 depth = 1;
            s32 left_stack[SORT_STACK];
            s32 right_stack[SORT_STACK];
            left_stack[0] = 0;
            right_stack[0] = size - 1;
            while(depth > 0) {
                s32 changed = 0;
                s32 left = left_stack[--depth];
                s32 right = right_stack[depth];
                s32 l = left;
                s32 r = right;
                s32 m = (l + r) >> 1;
                do {
                    const Type &c = array[m];
                    while(l < right && compare(array[l],c) > 0) l++;
                    while(r > left && compare(array[r],c) < 0) r--;
                    if(l < r) {
                        changed = 1;
                        swap(array[l],array[r]);
                        swap(data[l],data[r]);
                        if(m == l) m = r;
                        else if(m == r) m = l;
                        l++;
                        r--;
                    } else if(l == r) {
                        l++;
                        r--;
                    }
                } while(l <= r);
                if(changed == 0) {
                    for(s32 i = left + 1; i <= right; i++) {
                        for(s32 j = i, k = i - 1; j > left && compare(array[j],array[k]) > 0; j--, k--) {
                            swap(array[k],array[j]);
                            swap(data[k],data[j]);
                        }
                    }
                } else {
                    if(left < r) {
                        assert(depth < SORT_STACK && "quick_double_sort_2(): stack overflow");
                        left_stack[depth] = left;
                        right_stack[depth++] = r;
                    }
                    if(l < right) {
                        assert(depth < SORT_STACK && "quick_double_sort_2(): stack overflow");
                        left_stack[depth] = l;
                        right_stack[depth++] = right;
                    }
                }
            }
        }
        
        //
        template <class Type,class Data> void quickDoubleSort2(Type *array,Data *data,s32 size) {
            if(size < 2) return;
            quick_sort_2_default_compare<Type> compare;
            quick_double_sort_2(array,data,size,compare);
        }
        
        template <class Type,class Data,class Compare> void quickDoubleSort2(Type *array,Data *data,s32 size,Compare compare) {
            if(size < 2) return;
            quick_double_sort_2(array,data,size,compare);
        }
        
        template <class Type,class Data,class A0,class A1> void quickDoubleSort2(Type *array,Data *data,s32 size,s32 (*func)(A0,A1)) {
            if(size < 2) return;
            quick_sort_2_function_compare<Type,s32 (*)(A0,A1)> compare(func);
            quick_double_sort_2(array,data,size,compare);
        }
        
        /******************************************************************************\
         *
         * radixSort
         *
         \******************************************************************************/
        
        //
        template <class Type> struct radix_sort_default_compare;
        
        //
        template <class Type> struct radix_sort_default_compare<Type*> {
            typedef size_t Hash;
            sv_inline Hash operator()(Type *p) const {
                return (size_t)p;
            }
        };
        
        //
        template <> struct radix_sort_default_compare<char> {
            typedef u8 Hash;
            sv_inline Hash operator()(char i) const {
                return i ^ 0x80;
            }
        };
        
        template <> struct radix_sort_default_compare<u8> {
            typedef u8 Hash;
            sv_inline Hash operator()(u8 i) const {
                return i;
            }
        };
        
        //
        template <> struct radix_sort_default_compare<short> {
            typedef unsigned short Hash;
            sv_inline Hash operator()(short i) const {
                return i ^ 0x8000;
            }
        };
        
        template <> struct radix_sort_default_compare<unsigned short> {
            typedef unsigned short Hash;
            sv_inline Hash operator()(unsigned short i) const {
                return i;
            }
        };
        
        //
        template <> struct radix_sort_default_compare<s32> {
            typedef u32 Hash;
            sv_inline Hash operator()(s32 i) const {
                return i ^ 0x80000000;
            }
        };
        
        template <> struct radix_sort_default_compare<u32> {
            typedef u32 Hash;
            sv_inline Hash operator()(u32 i) const {
                return i;
            }
        };
        
        //
        template <> struct radix_sort_default_compare<s64> {
            typedef u64 Hash;
            sv_inline Hash operator()(s64 i) const {
                return i ^ 0x8000000000000000LL;
            }
        };
        
        template <> struct radix_sort_default_compare<u64> {
            typedef u64 Hash;
            sv_inline Hash operator()(u64 i) const {
                return i;
            }
        };
        
        //
        template <> struct radix_sort_default_compare<f32> {
            typedef u32 Hash;
            sv_inline Hash operator()(f32 f) const {
                union { f32 f; u32 i; } hash = { f };
                if(hash.i & 0x80000000) return hash.i ^ 0xffffffff;
                return hash.i ^ 0x80000000;
            }
        };
        
        //
        template <class Type,class Ret,class Func> struct radix_sort_function_compare {
            radix_sort_function_compare(Func func) : func(func) { }
            typedef Ret Hash;
            sv_inline Hash operator()(const Type &t) const {
                return func(t);
            }
            Func func;
        };
        
        //
        template <class Type,class Compare> class RadixSort {
            
        public:
            
            RadixSort(Type *array,s32 size,Compare compare) {
                reserve(size);
                for(s32 i = 0; i < size; i++) {
                    src[i].hash = compare(array[i]);
                    src[i].index = i;
                    data[i] = array[i];
                }
                s32 index[256];
                s32 distribution[256];
#ifdef USE_BIG_ENDIAN
                for(s32 j = (s32)sizeof(Hash) - 1; j >= 0; j--) {
#else
                    for(s32 j = 0; j < (s32)sizeof(Hash); j++) {
#endif
                        for(s32 i = 0; i < 256; i += 4) {
                            distribution[i + 0] = 0;
                            distribution[i + 1] = 0;
                            distribution[i + 2] = 0;
                            distribution[i + 3] = 0;
                        }
                        for(s32 i = 0; i < size; i++) {
                            distribution[get_byte(src[i].hash,j)]++;
                        }
                        index[0] = 0;
                        index[1] = distribution[0];
                        index[2] = index[1] + distribution[1];
                        index[3] = index[2] + distribution[2];
                        for(s32 i = 4; i < 256; i += 4) {
                            index[i + 0] = index[i - 1] + distribution[i - 1];
                            index[i + 1] = index[i + 0] + distribution[i + 0];
                            index[i + 2] = index[i + 1] + distribution[i + 1];
                            index[i + 3] = index[i + 2] + distribution[i + 2];
                        }
                        for(s32 i = 0; i < size; i++) {
                            dest[index[get_byte(src[i].hash,j)]++] = src[i];
                        }
                        swap(src,dest);
                    }
                    for(s32 i = 0; i < size; i++) {
                        array[i] = data[src[i].index];
                    }
                }
                
                static void clear() {
                    capacity = 0;
                    delete [] src;
                    delete [] dest;
                    delete [] data;
                    src = 0;
                    dest = 0;
                    data = 0;
                }
                
                static void allocate(s32 size) {
                    if(size <= capacity) return;
                    capacity = size;
                    delete [] src;
                    delete [] dest;
                    delete [] data;
                    src = new Data[capacity];
                    dest = new Data[capacity];
                    data = new Type[capacity];
                }
                
                static void reserve(s32 size) {
                    if(size <= capacity) return;
                    capacity = size * 2;
                    delete [] src;
                    delete [] dest;
                    delete [] data;
                    src = new Data[capacity];
                    dest = new Data[capacity];
                    data = new Type[capacity];
                }
                
            private:
                
                typedef typename Compare::Hash Hash;
                
                sv_inline s32 get_byte(const Hash &hash,s32 i) const {
                    return *((const u8*)&hash + i);
                }
                
                struct Data {
                    Hash hash;
                    s32 index;
                };
                
                static s32 capacity;
                static Data *src;
                static Data *dest;
                static Type *data;
            };
            
            /*
             */
            template <class Type,class Compare> s32 RadixSort<Type,Compare>::capacity = 0;
            template <class Type,class Compare> typename RadixSort<Type,Compare>::Data *RadixSort<Type,Compare>::src = 0;
            template <class Type,class Compare> typename RadixSort<Type,Compare>::Data *RadixSort<Type,Compare>::dest = 0;
            template <class Type,class Compare> Type *RadixSort<Type,Compare>::data = 0;
            
            /*
             */
            template <class Type> void radixSort(Type *array,s32 size) {
                if(size < 2) return;
                radix_sort_default_compare<Type> compare;
                RadixSort<Type,radix_sort_default_compare<Type> > radix_sort(array,size,compare);
            }
            
            template <class Type,class Compare> void radixSort(Type *array,s32 size,Compare compare) {
                if(size < 2) return;
                RadixSort<Type,Compare> radix_sort(array,size,compare);
            }
            
            template <class Type,class Ret,class A0,class A1> void radixSort(Type *array,s32 size,Ret (*func)(A0,A1)) {
                if(size < 2) return;
                radix_sort_function_compare<Type,Ret,Ret (*)(A0,A1)> compare(func);
                RadixSort<Type,radix_sort_function_compare<Type,Ret,Ret (*)(A0,A1)> > radix_sort(array,size,compare);
            }
            
            /*
             */
            template <class Iterator> void radixSort(Iterator begin,Iterator end) {
                s32 size = end - begin;
                if(size < 2) return;
                radix_sort_default_compare<typename Iterator::DataType> compare;
                RadixSort<typename Iterator::DataType,radix_sort_default_compare<typename Iterator::DataType> > radix_sort(&(*begin),size,compare);
            }
            
            template <class Iterator,class Compare> void radixSort(Iterator begin,Iterator end,Compare compare) {
                s32 size = end - begin;
                if(size < 2) return;
                RadixSort<typename Iterator::DataType,Compare> radix_sort(&(*begin),size,compare);
            }
            
            template <class Iterator,class Ret,class A0,class A1> void radixSort(Iterator begin,Iterator end,Ret (*func)(A0,A1)) {
                s32 size = end - begin;
                if(size < 2) return;
                radix_sort_function_compare<typename Iterator::DataType,Ret,Ret (*)(A0,A1)> compare(func);
                RadixSort<typename Iterator::DataType,radix_sort_function_compare<typename Iterator::DataType,Ret,Ret (*)(A0,A1)> > radix_sort(&(*begin),size,compare);
            }
        
    }
}


#endif /* SV_UTILS_H */
