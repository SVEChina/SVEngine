#ifndef __HASH_H__
#define __HASH_H__

#include "SVBase.h"

//
namespace sv {
    
    template <class Type,class Counter = s32> class SVHash {
    protected:
        struct Data;
    public:
        class Iterator {
        private:
            
            friend class SVHash;
            Iterator(Data *ptr,Data *end)
            : ptr(ptr)
            , end(end) {
            }
            
        public:
            typedef Type DataType;
            
            Iterator()
            : ptr(0)
            , end(0) {
            }
            
            Iterator(const Iterator &it)
            : ptr(it.ptr)
            , end(it.end) {
                
            }
            
            ~Iterator() {
                
            }
            
            Iterator &operator=(const Iterator &it) {
                ptr = it.ptr;
                end = it.end;
                return *this;
            }
            
            sv_inline s32 operator==(const Iterator &it) const { return ptr == it.ptr; }
            
            sv_inline s32 operator!=(const Iterator &it) const { return ptr != it.ptr; }
            
            sv_inline Type &operator*() { return ptr->data; }
            
            sv_inline const Type &operator*() const { return ptr->data; }
            
            sv_inline Type *operator->() { return &ptr->data; }
            
            sv_inline const Type *operator->() const { return &ptr->data; }
            
            sv_inline Type &get() { return ptr->data; }
            
            sv_inline const Type &get() const { return ptr->data; }
            
            Iterator &operator++() {
                next();
                return *this;
            }
            
            Iterator operator++(s32) {
                Iterator temp = *this;
                next();
                return temp;
            }
            
        private:
            void next() {
                if(ptr < end) {
                    while(++ptr != end && ptr->usage == 0);
                }
            }
            
            Data *ptr;
            Data *end;
        };
        
        // SVHash
        SVHash()
        : length(0)
        , capacity(0)
        , data(0)
        , dynamic_data(0)
        , id_length(0)
        , id_capacity(0)
        , id_data(0)
        , id_dynamic_data(0) {
            
        }
        
        SVHash(const SVHash &h)
        : length(h.length)
        , capacity(h.capacity)
        , data(0)
        , dynamic_data(0)
        , id_length(h.id_length)
        , id_capacity(h.id_length)
        , id_data(0)
        , id_dynamic_data(0) {
            dynamic_data = new Data[capacity];
            data = dynamic_data;
            for(s32 i = 0; i < capacity; i++) {
                data[i] = h.data[i];
            }
            id_dynamic_data = new s32[id_capacity];
            id_data = id_dynamic_data;
            for(s32 i = 0; i < id_length; i++) {
                id_data[i] = h.id_data[i];
            }
        }
        
        ~SVHash() {
            length = 0;
            capacity = 0;
            id_length = 0;
            id_capacity = 0;
            delete [] dynamic_data;
            delete [] id_dynamic_data;
            dynamic_data = 0;
            id_dynamic_data = 0;
        }
        
        SVHash &operator=(const SVHash &h) {
            if(this == &h)
                return *this;
            
            if(capacity != h.capacity) {
                capacity = h.capacity;
                delete [] dynamic_data;
                dynamic_data = new Data[capacity];
                data = dynamic_data;
            }
            
            for(s32 i = 0; i < capacity; i++) {
                data[i] = h.data[i];
            }
            
            if(id_capacity < h.id_length) {
                id_capacity = h.id_length;
                delete [] id_dynamic_data;
                id_dynamic_data = new s32[id_capacity];
                id_data = id_dynamic_data;
            }
            
            length = h.id_length;
            
            for(s32 i = 0; i < id_length; i++) {
                id_data[i] = h.id_data[i];
            }
            return *this;
        }
        
        sv_inline Type &operator[](s32 id) {
            assert((u32)id < (u32)capacity && data[id].usage && "SVHash::operator[](): bad id");
            return data[id].data;
        }
        
        sv_inline const Type &operator[](s32 id) const {
            assert((u32)id < (u32)capacity && data[id].usage && "SVHash::operator[](): bad id");
            return data[id].data;
        }
        
        sv_inline Type &get(s32 id) {
            assert((u32)id < (u32)capacity && data[id].usage && "SVHash::get(): bad id");
            return data[id].data;
        }
        
        sv_inline const Type &get(s32 id) const {
            assert((u32)id < (u32)capacity && data[id].usage && "SVHash::get(): bad id");
            return data[id].data;
        }
        
        sv_inline Iterator begin() const {
            Data *ptr = data;
            Data *end = data + capacity;
            while(ptr != end && ptr->usage == 0) ptr++;
            return Iterator(ptr,end);
        }
        
        sv_inline Iterator end() const {
            Data *end = data + capacity;
            return Iterator(end,end);
        }
        
        sv_inline s32 size() const {
            return length;
        }
        
        sv_inline s32 empty() const {
            return (length == 0);
        }
        
        void clear() {
            length = 0;
            for(s32 i = 0; i < capacity; i++) {
                data[i].usage = 0;
            }
            id_length = 0;
        }
        
        s32 append(const Type &t) {
            
            if(id_length > 0) {
                s32 id = id_data[(s32)(--id_length)];
                data[id].data = t;
                data[id].usage = 1;
                return id;
            }
            
            if(length + 1 <= capacity) {
                s32 id = length++;
                data[id].data = t;
                data[id].usage = 1;
                return id;
            }
            
            capacity = length * 2 + 1;
            Data *new_data = new Data[capacity];
            
            for(s32 i = 0; i < length; i++) {
                new_data[i] = data[i];
            }
            
            for(s32 i = length; i < capacity; i++) {
                new_data[i].usage = 0;
            }
            delete [] dynamic_data;
            dynamic_data = new_data;
            data = dynamic_data;
            s32 id = length++;
            data[id].data = t;
            data[id].usage = 1;
            return id;
        }
        
        s32 append(s32 id,const Type &t) {
            assert(id >= 0 && "SVHash::append(): bad id");
            if(id_length > 0) {
                for(s32 i = 0; i < id_length; i++) {
                    if(id_data[i] == id) {
                        id_data[i] = id_data[id_length - 1];
                        id_length--;
                        break;
                    }
                }
                data[id].data = t;
                data[id].usage = 1;
                return id;
            }
            
            if(length < id + 1) {
                length = id + 1;
            }
            
            if(id < capacity) {
                data[id].data = t;
                data[id].usage = 1;
                return id;
            }
            
            capacity = length * 2 + 1;
            Data *new_data = new Data[capacity];
            
            for(s32 i = 0; i < length; i++) {
                new_data[i] = data[i];
            }
            
            for(s32 i = length; i < capacity; i++) {
                new_data[i].usage = 0;
            }
            
            delete [] dynamic_data;
            dynamic_data = new_data;
            data = dynamic_data;
            data[id].data = t;
            data[id].usage = 1;
            return id;
        }
        
        s32 remove(s32 id) {
            assert((u32)id < (u32)capacity && data[id].usage && "SVHash::remove(): bad id");
            data[id].usage = 0;
            
            if(id_length + 1 <= id_capacity) {
                id_data[id_length++] = id;
                return id;
            }
            
            id_capacity = id_length * 2 + 1;
            s32 *new_id_data = new s32[id_capacity];
            
            for(s32 i = 0; i < id_length; i++) {
                new_id_data[i] = id_data[i];
            }
            delete [] id_dynamic_data;
            id_dynamic_data = new_id_data;
            id_data = id_dynamic_data;
            id_data[id_length++] = id;
            return id;
        }
        
        sv_inline s32 remove(const Iterator &it) {
            return remove((s32)(it.ptr - data));
        }

    protected:
        friend class Iterator;
        struct Data {
            Type data;
            s8 usage;
        };
        Counter length;
        Counter capacity;
        Data *data;
        Data *dynamic_data;
        Counter id_length;
        Counter id_capacity;
        s32 *id_data;
        s32 *id_dynamic_data;
    };
    
    /*
     */
    template <class Type,s32 Capacity = 128,class Counter = s32> class SVHashStack : public SVHash<Type,Counter> {
    public:
        SVHashStack() {
            SVHash<Type,Counter>::capacity = Capacity;
            SVHash<Type,Counter>::data = stack_data;
            SVHash<Type,Counter>::id_capacity = Capacity;
            SVHash<Type,Counter>::id_data = id_stack_data;
            SVHash<Type,Counter>::clear();
        }
        ~SVHashStack() { }
        
    private:
        typename SVHash<Type,Counter>::Data stack_data[Capacity];
        s32 id_stack_data[Capacity];
    };
    
} //!namespace sv



#endif /* __HASH_H__ */
