//
// heapsort.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
// copy from irr
//

#ifndef SV_HEAPSORT_H
#define SV_HEAPSORT_H

#include "SVTypes.h"

namespace sv {
    
    //! Sinks an element into the heap.
    template<class T>
    inline void heapsink(T *array, s32 element, s32 max) {
        while ((element << 1) < max){
            // there is a left child
            s32 j = (element << 1);
            if (j + 1 < max && array[j] < array[j + 1])
                j = j + 1; // take right child
            if (array[element] < array[j]) {
                T t = array[j]; // swap elements
                array[j] = array[element];
                array[element] = t;
                element = j;
            } else
                return;
        }
    }
    
    
    //! Sorts an array with size 'size' using heapsort.
    template<class T>
    inline void heapsort(T *array_, s32 size) {
        // for heapsink we pretend this is not c++, where
        // arrays start with index 0. So we decrease the array pointer,
        // the maximum always +2 and the element always +1
        T *virtualArray = array_ - 1;
        s32 virtualSize = size + 2;
        s32 i;
        // build heap
        for (i = ((size - 1) / 2); i >= 0; --i){
            heapsink(virtualArray, i + 1, virtualSize - 1);
        }
        // sort array, leave out the last element (0)
        for (i = size - 1; i > 0; --i) {
            T t = array_[0];
            array_[0] = array_[i];
            array_[i] = t;
            heapsink(virtualArray, 1, i + 1);
        }
    }
    
    //! Sinks an element into the heap.
    template<class T>
    inline void heapsinkobject(T *array, s32 element, s32 max) {
        while ((element << 1) < max){
            // there is a left child
            s32 j = (element << 1);
            if (j + 1 < max && (*array[j]) < (*array[j + 1])){
                j = j + 1; // take right child
            }
            if ((*array[element]) < (*array[j])) {
                T t = array[j]; // swap elements
                array[j] = array[element];
                array[element] = t;
                element = j;
            } else
                return;
        }
    }
    //! Sorts an array with size 'size' using heapsort.
    template<class T>
    inline void heapsortobject(T *array_, s32 size) {
        // for heapsink we pretend this is not c++, where
        // arrays start with index 0. So we decrease the array pointer,
        // the maximum always +2 and the element always +1
        
        T *virtualArray = array_ - 1;
        s32 virtualSize = size + 2;
        s32 i;
        
        // build heap
        
        for (i = ((size - 1) / 2); i >= 0; --i)
            heapsinkobject(virtualArray, i + 1, virtualSize - 1);
        
        // sort array, leave out the last element (0)
        for (i = size - 1; i > 0; --i) {
            T t = array_[0];
            array_[0] = array_[i];
            array_[i] = t;
            heapsinkobject(virtualArray, 1, i + 1);
        }
    }
    
    template<class T>
    inline void popsortobject(T *array_, s32 size) {
        s32 low = 0;
        s32 high = size-1;
        while(low<high){
            
            for(int i=low;i<high;++i){   //正向冒泡，确定最大值
                if((*array_[i])>(*array_[i+1])){  //如果前一位大于后一位，交换位置
                    T t = array_[i];
                    array_[i]= array_[i+1];
                    array_[i+1]= t;
                }
            }
            --high;
            
            for(int j=high;j>low;--j){   //反向冒泡，确定最小值
                if((*array_[j])<(*array_[j-1])){  //如果前一位大于后一位，交换位置
                    T t = array_[j];
                    array_[j]= array_[j-1];
                    array_[j-1]= t;
                }
            }
            ++low;
        }
    }
    
} //namespace sv

#endif

