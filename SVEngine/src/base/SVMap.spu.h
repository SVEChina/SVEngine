#ifndef __MAP_SPU_H__
#define __MAP_SPU_H__

#include "SVCoreTree.spu.h"

namespace sv {
    
    namespace util {
        //
        template <class Type> class MapData {
        public:
            MapData() { }
            
            ~MapData() { }
            
            Type data;
        };
        
        //
        template <class Key,class Type,u16 Capacity = 128> class Map : public SVCoreTree<Key,MapData<Type>,Capacity> {
        public:
            SVMap() { }
            
            ~SVMap() { }
            //
            typename SVMap<Key,Type>::Iterator findData(const Type &t) const {
                typename SVMap<Key,Type,Capacity>::Iterator end_it = Tree<Key,MapData<Type>,Capacity>::end();
                for(typename SVMap<Key,Type,Capacity>::Iterator it = Tree<Key,MapData<Type>,Capacity>::begin(); it != end_it; ++it) {
                    if(it->data == t)
                        return it;
                }
                return end_it;
            }
            //
            sv_inline Type &operator[](const Key &key) {
                s16 change = 0;
                typename SVMap<Key,Type,Capacity>::Node *parent = 0;
                typename SVMap<Key,Type,Capacity>::Node *node = Tree<Key,MapData<Type>,Capacity>::append_proc(key,Tree<Key,MapData<Type>,Capacity>::root,parent,change);
                return node->data;
            }
            //
            sv_inline Type &get(const Key &key) {
                s16 change = 0;
                typename SVMap<Key,Type,Capacity>::Node *parent = 0;
                typename SVMap<Key,Type,Capacity>::Node *node = Tree<Key,MapData<Type>,Capacity>::append_proc(key,Tree<Key,MapData<Type>,Capacity>::root,parent,change);
                return node->data;
            }
            //
            sv_inline Type &append(const Key &key) {
                s16 change = 0;
                typename SVMap<Key,Type,Capacity>::Node *parent = 0;
                typename SVMap<Key,Type,Capacity>::Node *node = Tree<Key,MapData<Type>,Capacity>::append_proc(key,Tree<Key,MapData<Type>,Capacity>::root,parent,change);
                return node->data;
            }
            //
            sv_inline void append(const Key &key,const Type &t) {
                s16 change = 0;
                typename SVMap<Key,Type,Capacity>::Node *parent = 0;
                typename SVMap<Key,Type,Capacity>::Node *node = Tree<Key,MapData<Type>,Capacity>::append_proc(key,Tree<Key,MapData<Type>,Capacity>::root,parent,change);
                node->data = t;
            }
        };
        
    }//!namespace util
    
}//!namespace sv


#endif /* __MAP_SPU_H__ */
