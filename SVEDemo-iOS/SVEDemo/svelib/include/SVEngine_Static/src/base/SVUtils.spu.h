#ifndef SV_UTILS_SPU_H
#define SV_UTILS_SPU_H

#include "SVBase.spu.h"

//
#define SORT_TAG_0			27
#define SORT_TAG_1			28
#define SORT_TAG_2			29
#define SORT_TAG_3			30
#define SORT_TAG_MASK_0		(1 << SORT_TAG_0)
#define SORT_TAG_MASK_1		(1 << SORT_TAG_1)
#define SORT_TAG_MASK_2		(1 << SORT_TAG_2)
#define SORT_TAG_MASK_3		(1 << SORT_TAG_3)

namespace sv {
    
    namespace util {
        
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
         * double buffering
         *
         \******************************************************************************/
        
        //
        template <u16 Batch,class Callback>
        void doubleBuffer(u32 size,Callback callback) {
            
            const u16 Batch_2 = Batch * 2;
            
            u32 offset_0 = 0;
            
            // double buffering
            if(LIKELY(size >= Batch_2)) {
                
                size -= Batch_2;
                
                u32 offset_1 = Batch;
                
                // first buffer
                callback.getFirst(offset_0,Batch);
                
                // second buffer
                callback.getSecond(offset_1,Batch);
                
                while(LIKELY(size >= Batch_2)) {
                    
                    size -= Batch_2;
                    
                    // first buffer
                    callback.syncFirst();
                    callback.putFirst(offset_0,Batch);
                    offset_0 += Batch_2;
                    
                    callback.getFirst(offset_0,Batch);
                    
                    // second buffer
                    callback.syncSecond();
                    callback.putSecond(offset_1,Batch);
                    offset_1 += Batch_2;
                    
                    callback.getSecond(offset_1,Batch);
                }
                
                // first buffer
                callback.syncFirst();
                callback.putFirst(offset_0,Batch);
                
                // second buffer
                callback.syncSecond();
                callback.putSecond(offset_1,Batch);
                
                offset_0 += Batch_2;
            }
            
            // single buffering
            if(LIKELY(size >= Batch)) {
                
                size -= Batch;
                
                // first buffer
                callback.getFirst(offset_0,Batch);
                
                // first buffer
                callback.syncFirst();
                callback.putFirst(offset_0,Batch);
                
                offset_0 += Batch;
            }
            
            if(LIKELY(size >= 1)) {
                
                // second buffer
                callback.getSecond(offset_0,size);
                
                // second buffer
                callback.syncSecond();
                callback.putSecond(offset_0,size);
            }
            
            callback.syncFirst();
            callback.syncSecond();
        }
        
        /******************************************************************************\
         *
         * triple buffering
         *
         \******************************************************************************/
        
        //
        template <u16 Batch,class Callback>
        void tripleBuffer(u32 size,Callback callback) {
            
            const u16 Batch_2 = Batch * 2;
            const u16 Batch_3 = Batch * 3;
            
            u32 offset_0 = 0;
            u32 offset_1 = Batch;
            
            // triple buffering
            if(LIKELY(size >= Batch_2)) {
                
                size -= Batch_2;
                
                u32 offset_2 = Batch * 2;
                
                // first buffer
                callback.getFirst(offset_0,Batch);
                
                // second buffer
                callback.getSecond(offset_1,Batch);
                
                while(LIKELY(size >= Batch_3)) {
                    
                    size -= Batch_3;
                    
                    // first buffer
                    callback.syncFirst();
                    callback.putFirst(offset_0,Batch);
                    offset_0 += Batch_3;
                    
                    // third buffer
                    callback.syncThird();
                    callback.getThird(offset_2,Batch);
                    
                    // second buffre
                    callback.syncSecond();
                    callback.putSecond(offset_1,Batch);
                    offset_1 += Batch_3;
                    
                    // first buffer
                    callback.syncFirst();
                    callback.getFirst(offset_0,Batch);
                    
                    // third buffer
                    callback.syncThird();
                    callback.putThird(offset_2,Batch);
                    offset_2 += Batch_3;
                    
                    // second buffer
                    callback.syncSecond();
                    callback.getSecond(offset_1,Batch);
                }
                
                // first buffer
                callback.syncFirst();
                callback.putFirst(offset_0,Batch);
                
                // second buffer
                callback.syncSecond();
                callback.putSecond(offset_1,Batch);
                
                offset_0 += Batch_2;
                offset_1 += Batch_2;
            }
            
            // double buffering
            if(LIKELY(size >= Batch_2)) {
                
                size -= Batch_2;
                
                // first buffer
                callback.syncFirst();
                callback.getFirst(offset_0,Batch);
                
                // second buffer
                callback.syncSecond();
                callback.getSecond(offset_1,Batch);
                
                // first buffer
                callback.syncFirst();
                callback.putFirst(offset_0,Batch);
                
                // second buffer
                callback.syncSecond();
                callback.putSecond(offset_1,Batch);
                
                offset_0 += Batch_2;
            }
            
            // single buffering
            if(LIKELY(size >= Batch)) {
                
                size -= Batch;
                
                // first buffer
                callback.syncFirst();
                callback.getFirst(offset_0,Batch);
                
                // first buffer
                callback.syncFirst();
                callback.putFirst(offset_0,Batch);
                
                offset_0 += Batch;
            }
            
            if(LIKELY(size >= 1)) {
                
                // second buffer
                callback.syncSecond();
                callback.getSecond(offset_0,size);
                
                // second buffer
                callback.syncSecond();
                callback.putSecond(offset_0,size);
            }
            
            callback.syncFirst();
            callback.syncSecond();
            callback.syncThird();
        }
        
        /******************************************************************************\
         *
         * sort
         *
         \******************************************************************************/
        
        //
        template <class Type> sv_inline void sort_get_buffer(Type *buffer,size_t pointer,u32 buffer_size) {
            
            assert(IS_ALIGNED16(pointer) && "sort_get_buffer(): bad pointer align");
            
            buffer_size *= sizeof(Type);
            
            for(u32 offset = 0; LIKELY(offset < buffer_size); offset += MFC_MAX_DMA_SIZE) {
                u32 size = buffer_size - offset;
                if(LIKELY(size > MFC_MAX_DMA_SIZE)) size = MFC_MAX_DMA_SIZE;
                spu_mfcdma32((unsigned char*)buffer + offset,pointer + offset,size,SORT_TAG_0,MFC_GET_CMD);
            }
            
            spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
            spu_mfcstat(MFC_TAG_UPDATE_ALL);
        }
        
        template <class Type> sv_inline void sort_put_buffer(Type *buffer,size_t pointer,u32 buffer_size) {
            
            assert(IS_ALIGNED16(pointer) && "sort_put_buffer(): bad pointer align");
            
            buffer_size *= sizeof(Type);
            
            for(u32 offset = 0; LIKELY(offset < buffer_size); offset += MFC_MAX_DMA_SIZE) {
                u32 size = buffer_size - offset;
                if(LIKELY(size > MFC_MAX_DMA_SIZE)) size = MFC_MAX_DMA_SIZE;
                spu_mfcdma32((unsigned char*)buffer + offset,pointer + offset,size,SORT_TAG_0,MFC_PUT_CMD);
            }
            
            spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
            spu_mfcstat(MFC_TAG_UPDATE_ALL);
        }
        
        /******************************************************************************\
         *
         * quickSortLocal
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
        template <class Type,class Compare> sv_inline void quick_sort_local(Type *array,s32 size,Compare compare) {
            
            short depth = 1;
            s32 left_stack[128];
            s32 right_stack[128];
            
            left_stack[0] = 0;
            right_stack[0] = size - 1;
            
            while(LIKELY(depth > 0)) {
                
                s32 left = left_stack[--depth];
                s32 right = right_stack[depth];
                s32 l = left;
                s32 r = right;
                s32 m = (l + r) >> 1;
                
                do {
                    Type c = array[m];
                    while(LIKELY(l < right) && compare(array[l],c)) l++;
                    while(LIKELY(r > left) && compare(c,array[r])) r--;
                    if(LIKELY(l < r)) {
                        swap(array[l],array[r]);
                        if(UNLIKELY(m == l)) m = r;
                        else if(UNLIKELY(m == r)) m = l;
                        l++;
                        r--;
                    } else if(LIKELY(l == r)) {
                        l++;
                        r--;
                    }
                } while(LIKELY(l <= r));
                
                if(LIKELY(left < r)) {
                    assert(depth < 128 && "quick_sort_local(): stack overflow");
                    left_stack[depth] = left;
                    right_stack[depth++] = r;
                }
                if(LIKELY(l < right)) {
                    assert(depth < 128 && "quick_sort_local(): stack overflow");
                    left_stack[depth] = l;
                    right_stack[depth++] = right;
                }
            }
        }
        
        //
        template <class Type> void quickSortLocal(Type *array,s32 size) {
            if(UNLIKELY(size < 2)) return;
            quick_sort_default_compare<Type> compare;
            quick_sort_local(array,size,compare);
        }
        
        template <class Type,class Compare> void quickSortLocal(Type *array,s32 size,Compare compare) {
            if(UNLIKELY(size < 2)) return;
            quick_sort_local(array,size,compare);
        }
        
        /******************************************************************************\
         *
         * quickSortGlobal
         *
         \******************************************************************************/
        
        //
        template <class Type,s32 Cache> class quick_sort_global_cache {
            
        public:
            
            quick_sort_global_cache(size_t pointer) : pointer(pointer) {
                left = 0x7fffffff;
                right = 0x7fffffff;
            }
            
            sv_inline void clear() {
                left = 0x7fffffff;
                right = 0x7fffffff;
            }
            
            sv_inline void set(u32 index,const Type &v) {
                if(LIKELY((index - (u32)left) < Cache)) {
                    cache[index - left] = v;
                }
            }
            
            sv_inline Type getLeft(u32 index) {
                
                if(LIKELY((index - (u32)left) < Cache)) {
                    return cache[index - left];
                }
                
                left = index;
                right = index + Cache;
                spu_mfcdma32(cache,pointer + sizeof(Type) * left,sizeof(Type) * Cache,SORT_TAG_0,MFC_GET_CMD);
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                return cache[0];
            }
            
            sv_inline Type getRight(u32 index) {
                
                if(LIKELY((index - (u32)left) < Cache)) {
                    return cache[index - left];
                }
                
                left = index - Cache + 1;
                right = index + 1;
                spu_mfcdma32(cache,pointer + sizeof(Type) * left,sizeof(Type) * Cache,SORT_TAG_0,MFC_GET_CMD);
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                return cache[Cache - 1];
            }
            
        private:
            
            size_t pointer;
            
            s32 left;
            s32 right;
            
            Type cache[Cache];
        };
        
        //
        template <class Type,s32 Buffer,s32 Cache,class Compare> void quick_sort_global(size_t pointer,s32 size,Compare compare) {
            
            assert(IS_ALIGNED16(pointer) && "quick_sort_global(): bad pointer align");
            assert(sizeof(Type) >= 16 && "quick_sort_global(): bad type size");
            
            Type buffer[Buffer];
            
            if(LIKELY(size <= Buffer)) {
                sort_get_buffer(buffer,pointer,size);
                quick_sort_local(buffer,size,compare);
                sort_put_buffer(buffer,pointer,size);
                return;
            }
            
            short depth = 1;
            s32 left_stack[128];
            s32 right_stack[128];
            
            left_stack[0] = 0;
            right_stack[0] = size - 1;
            
            Type left_value;
            Type right_value;
            Type compare_value;
            
            quick_sort_global_cache<Type,Cache> left_cache(pointer);
            quick_sort_global_cache<Type,Cache> right_cache(pointer);
            
            while(LIKELY(depth > 0)) {
                
                s32 left = left_stack[--depth];
                s32 right = right_stack[depth];
                s32 l = left;
                s32 r = right;
                s32 m = (l + r) >> 1;
                
                spu_mfcdma32(&compare_value,pointer + sizeof(Type) * m,sizeof(Type),SORT_TAG_0,MFC_GET_CMD);
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                do {
                    
                    while(LIKELY(l < right)) {
                        left_value = left_cache.getLeft(l);
                        if(compare(left_value,compare_value) == 0) break;
                        l++;
                    }
                    while(LIKELY(r > left)) {
                        right_value = right_cache.getRight(r);
                        if(compare(compare_value,right_value) == 0) break;
                        r--;
                    }
                    
                    if(LIKELY(l < r)) {
                        
                        spu_mfcdma32(&left_value,pointer + sizeof(Type) * r,sizeof(Type),SORT_TAG_0,MFC_PUT_CMD);
                        spu_mfcdma32(&right_value,pointer + sizeof(Type) * l,sizeof(Type),SORT_TAG_0,MFC_PUT_CMD);
                        
                        left_cache.set(l,right_value);
                        left_cache.set(r,left_value);
                        right_cache.set(l,right_value);
                        right_cache.set(r,left_value);
                        
                        if(UNLIKELY(m == l)) {
                            compare_value = right_cache.getRight(r);
                            m = r;
                        } else if(UNLIKELY(m == r)) {
                            compare_value = left_cache.getLeft(l);
                            m = l;
                        }
                        
                        l++;
                        r--;
                        
                        spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
                        spu_mfcstat(MFC_TAG_UPDATE_ALL);
                    }
                    else if(LIKELY(l == r)) {
                        
                        l++;
                        r--;
                    }
                    
                } while(LIKELY(l <= r));
                
                
                if(LIKELY(left < r)) {
                    s32 left_size = r - left + 1;
                    if(LIKELY(left_size <= Buffer)) {
                        size_t offset = sizeof(Type) * left;
                        sort_get_buffer(buffer,pointer + offset,left_size);
                        quick_sort_local(buffer,left_size,compare);
                        sort_put_buffer(buffer,pointer + offset,left_size);
                        left_cache.clear();
                        right_cache.clear();
                    } else {
                        assert(depth < 128 && "quick_sort_global(): stack overflow");
                        left_stack[depth] = left;
                        right_stack[depth++] = r;
                    }
                }
                if(LIKELY(l < right)) {
                    s32 right_size = right - l + 1;
                    if(LIKELY(right_size <= Buffer)) {
                        size_t offset = sizeof(Type) * l;
                        sort_get_buffer(buffer,pointer + offset,right_size);
                        quick_sort_local(buffer,right_size,compare);
                        sort_put_buffer(buffer,pointer + offset,right_size);
                        left_cache.clear();
                        right_cache.clear();
                    } else {
                        assert(depth < 128 && "quick_sort_global(): stack overflow");
                        left_stack[depth] = l;
                        right_stack[depth++] = right;
                    }
                }
            }
        }
        
        //
        template <class Type,s32 Buffer,s32 Cache> void quickSortGlobal(size_t pointer,s32 size) {
            if(UNLIKELY(size < 2)) return;
            quick_sort_default_compare<Type> compare;
            quick_sort_global<Type,Buffer,Cache>(pointer,size,compare);
        }
        
        template <class Type,s32 Buffer,s32 Cache,class Compare> void quickSortGlobal(size_t pointer,s32 size,Compare compare) {
            if(UNLIKELY(size < 2)) return;
            quick_sort_global<Type,Buffer,Cache>(pointer,size,compare);
        }
        
        /******************************************************************************\
         *
         * radixSortLocal
         *
         \******************************************************************************/
        
        //
        template <class Type> struct radix_sort_default_compare;
        
        //
        template <class Type,s32 Byte> sv_inline void radix_sort_local_index(vec_uint4 *index,Type *dest,const Type *src,s32 size) {
            
            while(LIKELY(size >= 8)) {
                
                vec_uchar16 hash_0 = (vec_uchar16)spu_promote(src[0].hash,0);
                vec_uchar16 hash_1 = (vec_uchar16)spu_promote(src[1].hash,0);
                vec_uchar16 hash_2 = (vec_uchar16)spu_promote(src[2].hash,0);
                vec_uchar16 hash_3 = (vec_uchar16)spu_promote(src[3].hash,0);
                vec_uchar16 hash_4 = (vec_uchar16)spu_promote(src[4].hash,0);
                vec_uchar16 hash_5 = (vec_uchar16)spu_promote(src[5].hash,0);
                vec_uchar16 hash_6 = (vec_uchar16)spu_promote(src[6].hash,0);
                vec_uchar16 hash_7 = (vec_uchar16)spu_promote(src[7].hash,0);
                
                u32 index_0 = index[spu_extract(hash_0,Byte)][Byte]++;
                u32 index_1 = index[spu_extract(hash_1,Byte)][Byte]++;
                u32 index_2 = index[spu_extract(hash_2,Byte)][Byte]++;
                u32 index_3 = index[spu_extract(hash_3,Byte)][Byte]++;
                u32 index_4 = index[spu_extract(hash_4,Byte)][Byte]++;
                u32 index_5 = index[spu_extract(hash_5,Byte)][Byte]++;
                u32 index_6 = index[spu_extract(hash_6,Byte)][Byte]++;
                u32 index_7 = index[spu_extract(hash_7,Byte)][Byte]++;
                
                dest[index_0] = src[0];
                dest[index_1] = src[1];
                dest[index_2] = src[2];
                dest[index_3] = src[3];
                dest[index_4] = src[4];
                dest[index_5] = src[5];
                dest[index_6] = src[6];
                dest[index_7] = src[7];
                
                src += 8;
                size -= 8;
            }
            
            while(LIKELY(size >= 1)) {
                
                vec_uchar16 hash = (vec_uchar16)spu_promote(src[0].hash,0);
                dest[index[spu_extract(hash,Byte)][Byte]++] = src[0];
                
                src += 1;
                size -= 1;
            }
        }
        
        //
        template <class Type,class Compare> sv_inline void radix_sort_local(Type *array,s32 size,Compare compare) {
            
            for(s32 i = 0; LIKELY(i < size); i++) {
                array[i].hash = compare(array[i]);
            }
            
            vec_uint4 index[256];
            vec_uint4 distribution[256];
            for(u16 i = 0; LIKELY(i < 256); i += 8) {
                distribution[i + 0] = (vec_uint4)spu_splats(0);
                distribution[i + 1] = (vec_uint4)spu_splats(0);
                distribution[i + 2] = (vec_uint4)spu_splats(0);
                distribution[i + 3] = (vec_uint4)spu_splats(0);
                distribution[i + 4] = (vec_uint4)spu_splats(0);
                distribution[i + 5] = (vec_uint4)spu_splats(0);
                distribution[i + 6] = (vec_uint4)spu_splats(0);
                distribution[i + 7] = (vec_uint4)spu_splats(0);
            }
            
            for(s32 i = 0; LIKELY(i < size); i++) {
                vec_uchar16 hash = (vec_uchar16)spu_promote(array[i].hash,0);
                distribution[spu_extract(hash,0)][0]++;
                distribution[spu_extract(hash,1)][1]++;
                distribution[spu_extract(hash,2)][2]++;
                distribution[spu_extract(hash,3)][3]++;
            }
            
            index[0] = (vec_uint4)spu_splats(0);
            index[1] = distribution[0];
            index[2] = spu_add(index[1],distribution[1]);
            index[3] = spu_add(index[2],distribution[2]);
            index[4] = spu_add(index[3],distribution[3]);
            index[5] = spu_add(index[4],distribution[4]);
            index[6] = spu_add(index[5],distribution[5]);
            index[7] = spu_add(index[6],distribution[6]);
            for(u16 i = 8; LIKELY(i < 256); i += 8) {
                index[i + 0] = spu_add(index[i - 1],distribution[i - 1]);
                index[i + 1] = spu_add(index[i + 0],distribution[i + 0]);
                index[i + 2] = spu_add(index[i + 1],distribution[i + 1]);
                index[i + 3] = spu_add(index[i + 2],distribution[i + 2]);
                index[i + 4] = spu_add(index[i + 3],distribution[i + 3]);
                index[i + 5] = spu_add(index[i + 4],distribution[i + 4]);
                index[i + 6] = spu_add(index[i + 5],distribution[i + 5]);
                index[i + 7] = spu_add(index[i + 6],distribution[i + 6]);
            }
            
            Type *src = array;
            Type *dest = array + size;
            radix_sort_local_index<Type,3>(index,dest,src,size);
            radix_sort_local_index<Type,2>(index,src,dest,size);
            radix_sort_local_index<Type,1>(index,dest,src,size);
            radix_sort_local_index<Type,0>(index,src,dest,size);
        }
        
        //
        template <class Type> void radixSortLocal(Type *array,s32 size) {
            if(UNLIKELY(size < 2)) return;
            radix_sort_default_compare<Type> compare;
            radix_sort_local(array,size,compare);
        }
        
        template <class Type,class Compare> void radixSortLocal(Type *array,s32 size,Compare compare) {
            if(UNLIKELY(size < 2)) return;
            radix_sort_local(array,size,compare);
        }
        
        /******************************************************************************\
         *
         * radixSortGlobal
         *
         \******************************************************************************/
        
        //
        template <class Type,class Compare> sv_inline void radix_sort_global_distribution(vec_uint4 *distribution,Type *dest,const Type *src,u16 size,Compare compare) {
            
            while(LIKELY(size >= 8)) {
                
                dest[0] = src[0];
                dest[1] = src[1];
                dest[2] = src[2];
                dest[3] = src[3];
                dest[4] = src[4];
                dest[5] = src[5];
                dest[6] = src[6];
                dest[7] = src[7];
                
                dest[0].hash = compare(dest[0]);
                dest[1].hash = compare(dest[1]);
                dest[2].hash = compare(dest[2]);
                dest[3].hash = compare(dest[3]);
                dest[4].hash = compare(dest[4]);
                dest[5].hash = compare(dest[5]);
                dest[6].hash = compare(dest[6]);
                dest[7].hash = compare(dest[7]);
                
                vec_uchar16 hash_0 = (vec_uchar16)spu_promote(dest[0].hash,0);
                vec_uchar16 hash_1 = (vec_uchar16)spu_promote(dest[1].hash,0);
                vec_uchar16 hash_2 = (vec_uchar16)spu_promote(dest[2].hash,0);
                vec_uchar16 hash_3 = (vec_uchar16)spu_promote(dest[3].hash,0);
                vec_uchar16 hash_4 = (vec_uchar16)spu_promote(dest[4].hash,0);
                vec_uchar16 hash_5 = (vec_uchar16)spu_promote(dest[5].hash,0);
                vec_uchar16 hash_6 = (vec_uchar16)spu_promote(dest[6].hash,0);
                vec_uchar16 hash_7 = (vec_uchar16)spu_promote(dest[7].hash,0);
                
                distribution[spu_extract(hash_0,0)][0]++;
                distribution[spu_extract(hash_0,1)][1]++;
                distribution[spu_extract(hash_0,2)][2]++;
                distribution[spu_extract(hash_0,3)][3]++;
                
                distribution[spu_extract(hash_1,0)][0]++;
                distribution[spu_extract(hash_1,1)][1]++;
                distribution[spu_extract(hash_1,2)][2]++;
                distribution[spu_extract(hash_1,3)][3]++;
                
                distribution[spu_extract(hash_2,0)][0]++;
                distribution[spu_extract(hash_2,1)][1]++;
                distribution[spu_extract(hash_2,2)][2]++;
                distribution[spu_extract(hash_2,3)][3]++;
                
                distribution[spu_extract(hash_3,0)][0]++;
                distribution[spu_extract(hash_3,1)][1]++;
                distribution[spu_extract(hash_3,2)][2]++;
                distribution[spu_extract(hash_3,3)][3]++;
                
                distribution[spu_extract(hash_4,0)][0]++;
                distribution[spu_extract(hash_4,1)][1]++;
                distribution[spu_extract(hash_4,2)][2]++;
                distribution[spu_extract(hash_4,3)][3]++;
                
                distribution[spu_extract(hash_5,0)][0]++;
                distribution[spu_extract(hash_5,1)][1]++;
                distribution[spu_extract(hash_5,2)][2]++;
                distribution[spu_extract(hash_5,3)][3]++;
                
                distribution[spu_extract(hash_6,0)][0]++;
                distribution[spu_extract(hash_6,1)][1]++;
                distribution[spu_extract(hash_6,2)][2]++;
                distribution[spu_extract(hash_6,3)][3]++;
                
                distribution[spu_extract(hash_7,0)][0]++;
                distribution[spu_extract(hash_7,1)][1]++;
                distribution[spu_extract(hash_7,2)][2]++;
                distribution[spu_extract(hash_7,3)][3]++;
                
                src += 8;
                dest += 8;
                size -= 8;
            }
            
            while(UNLIKELY(size >= 1)) {
                
                dest[0] = src[0];
                dest[0].hash = compare(dest[0]);
                
                vec_uchar16 hash = (vec_uchar16)spu_promote(dest[0].hash,0);
                
                distribution[spu_extract(hash,0)][0]++;
                distribution[spu_extract(hash,1)][1]++;
                distribution[spu_extract(hash,2)][2]++;
                distribution[spu_extract(hash,3)][3]++;
                
                src += 1;
                dest += 1;
                size -= 1;
            }
        }
        
        //
        template <class Type,s32 Batch,class Compare> void radix_sort_global_distribution(vec_uint4 *distribution,size_t pointer,s32 size,Compare compare) {
            
            Type src_buffer_0[Batch];
            Type src_buffer_1[Batch];
            Type src_buffer_2[Batch];
            Type src_buffer_3[Batch];
            Type dest_buffer_0[Batch];
            Type dest_buffer_1[Batch];
            Type dest_buffer_2[Batch];
            Type dest_buffer_3[Batch];
            
            s32 Batch_2 = Batch * 2;
            s32 Batch_4 = Batch * 4;
            
            size_t src_pointer = pointer;
            size_t dest_pointer = pointer;
            
            // quad buffering
            if(LIKELY(size >= Batch_4)) {
                
                // first buffer
                spu_mfcdma32(src_buffer_0,src_pointer,sizeof(Type) * Batch,SORT_TAG_0,MFC_GET_CMD);
                src_pointer += sizeof(Type) * Batch;
                size -= Batch;
                
                // second buffer
                spu_mfcdma32(src_buffer_1,src_pointer,sizeof(Type) * Batch,SORT_TAG_1,MFC_GET_CMD);
                src_pointer += sizeof(Type) * Batch;
                size -= Batch;
                
                // third buffer
                spu_mfcdma32(src_buffer_2,src_pointer,sizeof(Type) * Batch,SORT_TAG_2,MFC_GET_CMD);
                src_pointer += sizeof(Type) * Batch;
                size -= Batch;
                
                // fourth buffer
                spu_mfcdma32(src_buffer_3,src_pointer,sizeof(Type) * Batch,SORT_TAG_3,MFC_GET_CMD);
                src_pointer += sizeof(Type) * Batch;
                size -= Batch;
                
                while(LIKELY(size >= Batch_4)) {
                    
                    // first buffer
                    spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
                    spu_mfcstat(MFC_TAG_UPDATE_ALL);
                    
                    radix_sort_global_distribution<Type>(distribution,dest_buffer_0,src_buffer_0,Batch,compare);
                    spu_mfcdma32(dest_buffer_0,dest_pointer,sizeof(Type) * Batch,SORT_TAG_0,MFC_PUT_CMD);
                    dest_pointer += sizeof(Type) * Batch;
                    
                    spu_mfcdma32(src_buffer_0,src_pointer,sizeof(Type) * Batch,SORT_TAG_0,MFC_GET_CMD);
                    src_pointer += sizeof(Type) * Batch;
                    size -= Batch;
                    
                    // second buffer
                    spu_writech(MFC_WrTagMask,SORT_TAG_MASK_1);
                    spu_mfcstat(MFC_TAG_UPDATE_ALL);
                    
                    radix_sort_global_distribution<Type>(distribution,dest_buffer_1,src_buffer_1,Batch,compare);
                    spu_mfcdma32(dest_buffer_1,dest_pointer,sizeof(Type) * Batch,SORT_TAG_1,MFC_PUT_CMD);
                    dest_pointer += sizeof(Type) * Batch;
                    
                    spu_mfcdma32(src_buffer_1,src_pointer,sizeof(Type) * Batch,SORT_TAG_1,MFC_GET_CMD);
                    src_pointer += sizeof(Type) * Batch;
                    size -= Batch;
                    
                    // third buffer
                    spu_writech(MFC_WrTagMask,SORT_TAG_MASK_2);
                    spu_mfcstat(MFC_TAG_UPDATE_ALL);
                    
                    radix_sort_global_distribution<Type>(distribution,dest_buffer_2,src_buffer_2,Batch,compare);
                    spu_mfcdma32(dest_buffer_2,dest_pointer,sizeof(Type) * Batch,SORT_TAG_2,MFC_PUT_CMD);
                    dest_pointer += sizeof(Type) * Batch;
                    
                    spu_mfcdma32(src_buffer_2,src_pointer,sizeof(Type) * Batch,SORT_TAG_2,MFC_GET_CMD);
                    src_pointer += sizeof(Type) * Batch;
                    size -= Batch;
                    
                    // fourth buffer
                    spu_writech(MFC_WrTagMask,SORT_TAG_MASK_3);
                    spu_mfcstat(MFC_TAG_UPDATE_ALL);
                    
                    radix_sort_global_distribution<Type>(distribution,dest_buffer_3,src_buffer_3,Batch,compare);
                    spu_mfcdma32(dest_buffer_3,dest_pointer,sizeof(Type) * Batch,SORT_TAG_3,MFC_PUT_CMD);
                    dest_pointer += sizeof(Type) * Batch;
                    
                    spu_mfcdma32(src_buffer_3,src_pointer,sizeof(Type) * Batch,SORT_TAG_3,MFC_GET_CMD);
                    src_pointer += sizeof(Type) * Batch;
                    size -= Batch;
                }
                
                // first buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                radix_sort_global_distribution<Type>(distribution,dest_buffer_0,src_buffer_0,Batch,compare);
                spu_mfcdma32(dest_buffer_0,dest_pointer,sizeof(Type) * Batch,SORT_TAG_0,MFC_PUT_CMD);
                dest_pointer += sizeof(Type) * Batch;
                
                // second buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_1);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                radix_sort_global_distribution<Type>(distribution,dest_buffer_1,src_buffer_1,Batch,compare);
                spu_mfcdma32(dest_buffer_1,dest_pointer,sizeof(Type) * Batch,SORT_TAG_1,MFC_PUT_CMD);
                dest_pointer += sizeof(Type) * Batch;
                
                // third buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_2);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                radix_sort_global_distribution<Type>(distribution,dest_buffer_2,src_buffer_2,Batch,compare);
                spu_mfcdma32(dest_buffer_2,dest_pointer,sizeof(Type) * Batch,SORT_TAG_2,MFC_PUT_CMD);
                dest_pointer += sizeof(Type) * Batch;
                
                // fourth buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_3);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                radix_sort_global_distribution<Type>(distribution,dest_buffer_3,src_buffer_3,Batch,compare);
                spu_mfcdma32(dest_buffer_3,dest_pointer,sizeof(Type) * Batch,SORT_TAG_3,MFC_PUT_CMD);
                dest_pointer += sizeof(Type) * Batch;
            }
            
            // double buffering
            if(LIKELY(size >= Batch_2)) {
                
                // first buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                spu_mfcdma32(src_buffer_0,src_pointer,sizeof(Type) * Batch,SORT_TAG_0,MFC_GET_CMD);
                src_pointer += sizeof(Type) * Batch;
                size -= Batch;
                
                // second buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_1);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                spu_mfcdma32(src_buffer_1,src_pointer,sizeof(Type) * Batch,SORT_TAG_1,MFC_GET_CMD);
                src_pointer += sizeof(Type) * Batch;
                size -= Batch;
                
                // first buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                radix_sort_global_distribution<Type>(distribution,dest_buffer_0,src_buffer_0,Batch,compare);
                spu_mfcdma32(dest_buffer_0,dest_pointer,sizeof(Type) * Batch,SORT_TAG_0,MFC_PUT_CMD);
                dest_pointer += sizeof(Type) * Batch;
                
                // second buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_1);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                radix_sort_global_distribution<Type>(distribution,dest_buffer_1,src_buffer_1,Batch,compare);
                spu_mfcdma32(dest_buffer_1,dest_pointer,sizeof(Type) * Batch,SORT_TAG_1,MFC_PUT_CMD);
                dest_pointer += sizeof(Type) * Batch;
            }
            
            // single buffering
            if(LIKELY(size >= Batch)) {
                
                // first buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                spu_mfcdma32(src_buffer_0,src_pointer,sizeof(Type) * Batch,SORT_TAG_0,MFC_GET_CMD);
                src_pointer += sizeof(Type) * Batch;
                size -= Batch;
                
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                radix_sort_global_distribution<Type>(distribution,dest_buffer_0,src_buffer_0,Batch,compare);
                spu_mfcdma32(dest_buffer_0,dest_pointer,sizeof(Type) * Batch,SORT_TAG_0,MFC_PUT_CMD);
                dest_pointer += sizeof(Type) * Batch;
            }
            
            if(LIKELY(size >= 1)) {
                
                // second buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_1);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                spu_mfcdma32(src_buffer_1,src_pointer,sizeof(Type) * size,SORT_TAG_1,MFC_GET_CMD);
                
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_1);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                radix_sort_global_distribution<Type>(distribution,dest_buffer_1,src_buffer_1,size,compare);
                spu_mfcdma32(dest_buffer_1,dest_pointer,sizeof(Type) * size,SORT_TAG_1,MFC_PUT_CMD);
            }
            
            spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0 | SORT_TAG_MASK_1 | SORT_TAG_MASK_2 | SORT_TAG_MASK_3);
            spu_mfcstat(MFC_TAG_UPDATE_ALL);
        }
        
        //
        template <class Type,s32 Byte> sv_inline void radix_sort_global_index(vec_uint4 *index,vec_uint4 *pointers,size_t dest_pointer,Type *dest,const Type *src,u16 size) {
            
            while(LIKELY(size >= 8)) {
                
                vec_uchar16 hash_0 = (vec_uchar16)spu_promote(src[0].hash,0);
                vec_uchar16 hash_1 = (vec_uchar16)spu_promote(src[1].hash,0);
                vec_uchar16 hash_2 = (vec_uchar16)spu_promote(src[2].hash,0);
                vec_uchar16 hash_3 = (vec_uchar16)spu_promote(src[3].hash,0);
                vec_uchar16 hash_4 = (vec_uchar16)spu_promote(src[4].hash,0);
                vec_uchar16 hash_5 = (vec_uchar16)spu_promote(src[5].hash,0);
                vec_uchar16 hash_6 = (vec_uchar16)spu_promote(src[6].hash,0);
                vec_uchar16 hash_7 = (vec_uchar16)spu_promote(src[7].hash,0);
                
                vec_uint4 pointer_0 = spu_splats((u32)sizeof(Type));
                vec_uint4 pointer_1 = spu_splats((u32)sizeof(Type));
                vec_uint4 pointer_2 = spu_splats((u32)sizeof(Type));
                vec_uint4 pointer_3 = spu_splats((u32)sizeof(Type));
                
                u32 index_0 = index[spu_extract(hash_0,Byte)][Byte]++;
                u32 index_1 = index[spu_extract(hash_1,Byte)][Byte]++;
                u32 index_2 = index[spu_extract(hash_2,Byte)][Byte]++;
                u32 index_3 = index[spu_extract(hash_3,Byte)][Byte]++;
                u32 index_4 = index[spu_extract(hash_4,Byte)][Byte]++;
                u32 index_5 = index[spu_extract(hash_5,Byte)][Byte]++;
                u32 index_6 = index[spu_extract(hash_6,Byte)][Byte]++;
                u32 index_7 = index[spu_extract(hash_7,Byte)][Byte]++;
                
                pointer_0 = spu_insert(dest_pointer + sizeof(Type) * index_0,pointer_0,1);
                pointer_1 = spu_insert(dest_pointer + sizeof(Type) * index_2,pointer_1,1);
                pointer_2 = spu_insert(dest_pointer + sizeof(Type) * index_4,pointer_2,1);
                pointer_3 = spu_insert(dest_pointer + sizeof(Type) * index_6,pointer_3,1);
                
                pointer_0 = spu_insert(dest_pointer + sizeof(Type) * index_1,pointer_0,3);
                pointer_1 = spu_insert(dest_pointer + sizeof(Type) * index_3,pointer_1,3);
                pointer_2 = spu_insert(dest_pointer + sizeof(Type) * index_5,pointer_2,3);
                pointer_3 = spu_insert(dest_pointer + sizeof(Type) * index_7,pointer_3,3);
                
                pointers[0] = pointer_0;
                pointers[1] = pointer_1;
                pointers[2] = pointer_2;
                pointers[3] = pointer_3;
                
                dest[0] = src[0];
                dest[1] = src[1];
                dest[2] = src[2];
                dest[3] = src[3];
                dest[4] = src[4];
                dest[5] = src[5];
                dest[6] = src[6];
                dest[7] = src[7];
                
                pointers += 4;
                dest += 8;
                src += 8;
                size -= 8;
            }
            
            while(UNLIKELY(size >= 2)) {
                
                vec_uchar16 hash_0 = (vec_uchar16)spu_promote(src[0].hash,0);
                vec_uchar16 hash_1 = (vec_uchar16)spu_promote(src[1].hash,0);
                
                vec_uint4 pointer = spu_splats((u32)sizeof(Type));
                
                u32 index_0 = index[spu_extract(hash_0,Byte)][Byte]++;
                u32 index_1 = index[spu_extract(hash_1,Byte)][Byte]++;
                
                pointer = spu_insert(dest_pointer + sizeof(Type) * index_0,pointer,1);
                pointer = spu_insert(dest_pointer + sizeof(Type) * index_1,pointer,3);
                
                pointers[0] = pointer;
                
                dest[0] = src[0];
                dest[1] = src[1];
                
                pointers += 1;
                dest += 2;
                src += 2;
                size -= 2;
            }
            
            if(UNLIKELY(size >= 1)) {
                
                vec_uchar16 hash_0 = (vec_uchar16)spu_promote(src[0].hash,0);
                vec_uchar16 hash_1 = (vec_uchar16)spu_promote(src[1].hash,0);
                
                vec_uint4 pointer = spu_splats((u32)sizeof(Type));
                
                u32 index_0 = index[spu_extract(hash_0,Byte)][Byte]++;
                u32 index_1 = index[spu_extract(hash_1,Byte)][Byte]++;
                
                pointer = spu_insert(dest_pointer + sizeof(Type) * index_0,pointer,1);
                pointer = spu_insert(dest_pointer + sizeof(Type) * index_1,pointer,3);
                
                pointers[0] = pointer;
                
                dest[0] = src[0];
                dest[1] = src[1];
            }
        }
        
        //
        template <class Type,s32 Batch,s32 Byte> void radix_sort_global_index(vec_uint4 *index,size_t dest_pointer,size_t src_pointer,s32 size) {
            
            Type src_buffer_0[Batch];
            Type src_buffer_1[Batch];
            Type src_buffer_2[Batch];
            Type src_buffer_3[Batch];
            Type dest_buffer_0[Batch];
            Type dest_buffer_1[Batch];
            Type dest_buffer_2[Batch];
            Type dest_buffer_3[Batch];
            vec_uint4 pointers_buffer_0[Batch];
            vec_uint4 pointers_buffer_1[Batch];
            vec_uint4 pointers_buffer_2[Batch];
            vec_uint4 pointers_buffer_3[Batch];
            
            s32 Batch_2 = Batch * 2;
            s32 Batch_4 = Batch * 4;
            
            // quad buffering
            if(LIKELY(size >= Batch_4)) {
                
                // first buffer
                spu_mfcdma32(src_buffer_0,src_pointer,sizeof(Type) * Batch,SORT_TAG_0,MFC_GET_CMD);
                src_pointer += sizeof(Type) * Batch;
                size -= Batch;
                
                // second buffer
                spu_mfcdma32(src_buffer_1,src_pointer,sizeof(Type) * Batch,SORT_TAG_1,MFC_GET_CMD);
                src_pointer += sizeof(Type) * Batch;
                size -= Batch;
                
                // third buffer
                spu_mfcdma32(src_buffer_2,src_pointer,sizeof(Type) * Batch,SORT_TAG_2,MFC_GET_CMD);
                src_pointer += sizeof(Type) * Batch;
                size -= Batch;
                
                // fourth buffer
                spu_mfcdma32(src_buffer_3,src_pointer,sizeof(Type) * Batch,SORT_TAG_3,MFC_GET_CMD);
                src_pointer += sizeof(Type) * Batch;
                size -= Batch;
                
                while(LIKELY(size >= Batch_4)) {
                    
                    // first buffer
                    spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
                    spu_mfcstat(MFC_TAG_UPDATE_ALL);
                    
                    radix_sort_global_index<Type,Byte>(index,pointers_buffer_0,dest_pointer,dest_buffer_0,src_buffer_0,Batch);
                    spu_mfcdma32(dest_buffer_0,(size_t)pointers_buffer_0,8 * Batch,SORT_TAG_0,MFC_PUTL_CMD);
                    
                    spu_mfcdma32(src_buffer_0,src_pointer,sizeof(Type) * Batch,SORT_TAG_0,MFC_GET_CMD);
                    src_pointer += sizeof(Type) * Batch;
                    size -= Batch;
                    
                    // second buffer
                    spu_writech(MFC_WrTagMask,SORT_TAG_MASK_1);
                    spu_mfcstat(MFC_TAG_UPDATE_ALL);
                    
                    radix_sort_global_index<Type,Byte>(index,pointers_buffer_1,dest_pointer,dest_buffer_1,src_buffer_1,Batch);
                    spu_mfcdma32(dest_buffer_1,(size_t)pointers_buffer_1,8 * Batch,SORT_TAG_1,MFC_PUTL_CMD);
                    
                    spu_mfcdma32(src_buffer_1,src_pointer,sizeof(Type) * Batch,SORT_TAG_1,MFC_GET_CMD);
                    src_pointer += sizeof(Type) * Batch;
                    size -= Batch;
                    
                    // third buffer
                    spu_writech(MFC_WrTagMask,SORT_TAG_MASK_2);
                    spu_mfcstat(MFC_TAG_UPDATE_ALL);
                    
                    radix_sort_global_index<Type,Byte>(index,pointers_buffer_2,dest_pointer,dest_buffer_2,src_buffer_2,Batch);
                    spu_mfcdma32(dest_buffer_2,(size_t)pointers_buffer_2,8 * Batch,SORT_TAG_2,MFC_PUTL_CMD);
                    
                    spu_mfcdma32(src_buffer_2,src_pointer,sizeof(Type) * Batch,SORT_TAG_2,MFC_GET_CMD);
                    src_pointer += sizeof(Type) * Batch;
                    size -= Batch;
                    
                    // fourth buffer
                    spu_writech(MFC_WrTagMask,SORT_TAG_MASK_3);
                    spu_mfcstat(MFC_TAG_UPDATE_ALL);
                    
                    radix_sort_global_index<Type,Byte>(index,pointers_buffer_3,dest_pointer,dest_buffer_3,src_buffer_3,Batch);
                    spu_mfcdma32(dest_buffer_3,(size_t)pointers_buffer_3,8 * Batch,SORT_TAG_3,MFC_PUTL_CMD);
                    
                    spu_mfcdma32(src_buffer_3,src_pointer,sizeof(Type) * Batch,SORT_TAG_3,MFC_GET_CMD);
                    src_pointer += sizeof(Type) * Batch;
                    size -= Batch;
                }
                
                // first buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                radix_sort_global_index<Type,Byte>(index,pointers_buffer_0,dest_pointer,dest_buffer_0,src_buffer_0,Batch);
                spu_mfcdma32(dest_buffer_0,(size_t)pointers_buffer_0,8 * Batch,SORT_TAG_0,MFC_PUTL_CMD);
                
                // second buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_1);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                radix_sort_global_index<Type,Byte>(index,pointers_buffer_1,dest_pointer,dest_buffer_1,src_buffer_1,Batch);
                spu_mfcdma32(dest_buffer_1,(size_t)pointers_buffer_1,8 * Batch,SORT_TAG_1,MFC_PUTL_CMD);
                
                // third buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_2);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                radix_sort_global_index<Type,Byte>(index,pointers_buffer_2,dest_pointer,dest_buffer_2,src_buffer_2,Batch);
                spu_mfcdma32(dest_buffer_2,(size_t)pointers_buffer_2,8 * Batch,SORT_TAG_2,MFC_PUTL_CMD);
                
                // fourth buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_3);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                radix_sort_global_index<Type,Byte>(index,pointers_buffer_3,dest_pointer,dest_buffer_3,src_buffer_3,Batch);
                spu_mfcdma32(dest_buffer_3,(size_t)pointers_buffer_3,8 * Batch,SORT_TAG_3,MFC_PUTL_CMD);
            }
            
            // double buffering
            if(LIKELY(size >= Batch_2)) {
                
                // first buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                spu_mfcdma32(src_buffer_0,src_pointer,sizeof(Type) * Batch,SORT_TAG_0,MFC_GET_CMD);
                src_pointer += sizeof(Type) * Batch;
                size -= Batch;
                
                // second buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_1);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                spu_mfcdma32(src_buffer_1,src_pointer,sizeof(Type) * Batch,SORT_TAG_1,MFC_GET_CMD);
                src_pointer += sizeof(Type) * Batch;
                size -= Batch;
                
                // first buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                radix_sort_global_index<Type,Byte>(index,pointers_buffer_0,dest_pointer,dest_buffer_0,src_buffer_0,Batch);
                spu_mfcdma32(dest_buffer_0,(size_t)pointers_buffer_0,8 * Batch,SORT_TAG_0,MFC_PUTL_CMD);
                
                // second buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_1);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                radix_sort_global_index<Type,Byte>(index,pointers_buffer_1,dest_pointer,dest_buffer_1,src_buffer_1,Batch);
                spu_mfcdma32(dest_buffer_1,(size_t)pointers_buffer_1,8 * Batch,SORT_TAG_1,MFC_PUTL_CMD);
            }
            
            // single buffering
            if(LIKELY(size >= Batch)) {
                
                // first buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                spu_mfcdma32(src_buffer_0,src_pointer,sizeof(Type) * Batch,SORT_TAG_0,MFC_GET_CMD);
                src_pointer += sizeof(Type) * Batch;
                size -= Batch;
                
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                radix_sort_global_index<Type,Byte>(index,pointers_buffer_0,dest_pointer,dest_buffer_0,src_buffer_0,Batch);
                spu_mfcdma32(dest_buffer_0,(size_t)pointers_buffer_0,8 * Batch,SORT_TAG_0,MFC_PUTL_CMD);
            }
            
            if(LIKELY(size >= 1)) {
                
                // second buffer
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_1);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                spu_mfcdma32(src_buffer_1,src_pointer,sizeof(Type) * size,SORT_TAG_1,MFC_GET_CMD);
                
                spu_writech(MFC_WrTagMask,SORT_TAG_MASK_1);
                spu_mfcstat(MFC_TAG_UPDATE_ALL);
                
                radix_sort_global_index<Type,Byte>(index,pointers_buffer_1,dest_pointer,dest_buffer_1,src_buffer_1,size);
                spu_mfcdma32(dest_buffer_1,(size_t)pointers_buffer_1,8 * size,SORT_TAG_1,MFC_PUTL_CMD);
            }
            
            spu_writech(MFC_WrTagMask,SORT_TAG_MASK_0 | SORT_TAG_MASK_1 | SORT_TAG_MASK_2 | SORT_TAG_MASK_3);
            spu_mfcstat(MFC_TAG_UPDATE_ALL);
        }
        
        //
        template <class Type,s32 Buffer,s32 Batch,class Compare> sv_inline void radix_sort_global(size_t pointer,s32 size,Compare compare) {
            
            assert(IS_ALIGNED16(pointer) && "radix_sort_global(): bad pointer align");
            assert(sizeof(Type) >= 16 && "radix_sort_global(): bad type size");
            assert(Batch >= 16 && (Batch & 0x03) == 0 && "radix_sort_global(): bad buffer size");
            
            if(LIKELY(size < Buffer)) {
                Type buffer[Buffer * 2];
                sort_get_buffer(buffer,pointer,size);
                radix_sort_local(buffer,size,compare);
                sort_put_buffer(buffer,pointer,size);
                return;
            }
            
            vec_uint4 index[256];
            vec_uint4 distribution[256];
            for(u16 i = 0; LIKELY(i < 256); i += 8) {
                distribution[i + 0] = (vec_uint4)spu_splats(0);
                distribution[i + 1] = (vec_uint4)spu_splats(0);
                distribution[i + 2] = (vec_uint4)spu_splats(0);
                distribution[i + 3] = (vec_uint4)spu_splats(0);
                distribution[i + 4] = (vec_uint4)spu_splats(0);
                distribution[i + 5] = (vec_uint4)spu_splats(0);
                distribution[i + 6] = (vec_uint4)spu_splats(0);
                distribution[i + 7] = (vec_uint4)spu_splats(0);
            }
            
            radix_sort_global_distribution<Type,Batch>(distribution,pointer,size,compare);
            
            index[0] = (vec_uint4)spu_splats(0);
            index[1] = distribution[0];
            index[2] = spu_add(index[1],distribution[1]);
            index[3] = spu_add(index[2],distribution[2]);
            index[4] = spu_add(index[3],distribution[3]);
            index[5] = spu_add(index[4],distribution[4]);
            index[6] = spu_add(index[5],distribution[5]);
            index[7] = spu_add(index[6],distribution[6]);
            for(u16 i = 8; LIKELY(i < 256); i += 8) {
                index[i + 0] = spu_add(index[i - 1],distribution[i - 1]);
                index[i + 1] = spu_add(index[i + 0],distribution[i + 0]);
                index[i + 2] = spu_add(index[i + 1],distribution[i + 1]);
                index[i + 3] = spu_add(index[i + 2],distribution[i + 2]);
                index[i + 4] = spu_add(index[i + 3],distribution[i + 3]);
                index[i + 5] = spu_add(index[i + 4],distribution[i + 4]);
                index[i + 6] = spu_add(index[i + 5],distribution[i + 5]);
                index[i + 7] = spu_add(index[i + 6],distribution[i + 6]);
            }
            
            size_t src_pointer = pointer;
            size_t dest_pointer = pointer + sizeof(Type) * size;
            radix_sort_global_index<Type,Batch,3>(index,dest_pointer,src_pointer,size);
            radix_sort_global_index<Type,Batch,2>(index,src_pointer,dest_pointer,size);
            radix_sort_global_index<Type,Batch,1>(index,dest_pointer,src_pointer,size);
            radix_sort_global_index<Type,Batch,0>(index,src_pointer,dest_pointer,size);
        }
        
        //
        template <class Type,s32 Buffer,s32 Batch> void radixSortGlobal(size_t pointer,s32 size) {
            if(UNLIKELY(size < 2)) return;
            radix_sort_default_compare<Type> compare;
            radix_sort_global<Type,Buffer,Batch>(pointer,size,compare);
        }
        
        template <class Type,s32 Buffer,s32 Batch,class Compare> void radixSortGlobal(size_t pointer,s32 size,Compare compare) {
            if(UNLIKELY(size < 2))
                return;
            radix_sort_global<Type,Buffer,Batch>(pointer,size,compare);
        }
        
    }//!namespace util
    
}//namespace sv



#endif /* SV_UTILS_SPU_H */
