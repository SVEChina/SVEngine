#ifndef __MAP_H__
#define __MAP_H__

#include "SVCoreTree.h"

namespace sv {
    
    namespace util {
        
        //
        template <class Type> struct MapData {
            Type data;
        };
        
        //
        template <class Key,class Type> class SVMap : public SVCoreTree<Key,MapData<Type> > {
        public:
            SVMap() {
            }
            
            ~SVMap() {
            }

            typename SVMap<Key,Type>::Iterator findData(const Type &t) const {
                typename SVMap<Key,Type>::Iterator end_it = SVCoreTree<Key,MapData<Type> >::end();
                for(typename SVMap<Key,Type>::Iterator it = SVCoreTree<Key,MapData<Type> >::begin(); it != end_it; ++it) {
                    if(it->data == t) {
                         return it;
                    }
                }
                return end_it;
            }
            
            sv_inline Type &operator[](const Key &key) {
                s32 change = 0;
                typename SVMap<Key,Type>::Node *parent = 0;
                typename SVMap<Key,Type>::Node *node = SVCoreTree<Key,MapData<Type> >::append_proc(key,SVCoreTree<Key,MapData<Type> >::root,parent,change);
                return node->data;
            }
            
            sv_inline Type &get(const Key &key) {
                s32 change = 0;
                typename SVMap<Key,Type>::Node *parent = 0;
                typename SVMap<Key,Type>::Node *node = SVCoreTree<Key,MapData<Type> >::append_proc(key,SVCoreTree<Key,MapData<Type> >::root,parent,change);
                return node->data;
            }
            
            sv_inline Type &append(const Key &key) {
                s32 change = 0;
                typename SVMap<Key,Type>::Node *parent = 0;
                typename SVMap<Key,Type>::Node *node = SVCoreTree<Key,MapData<Type> >::append_proc(key,SVCoreTree<Key,MapData<Type> >::root,parent,change);
                return node->data;
            }
            
            sv_inline void append(const Key &key,const Type &t) {
                s32 change = 0;
                typename SVMap<Key,Type>::Node *parent = 0;
                typename SVMap<Key,Type>::Node *node = SVCoreTree<Key,MapData<Type> >::append_proc(key,SVCoreTree<Key,MapData<Type> >::root,parent,change);
                node->data = t;
            }
            
            void append(const SVMap<Key,Type> &m) {
                typename SVMap<Key,Type>::Iterator end_it = m.end();
                for(typename SVMap<Key,Type>::Iterator it = m.begin(); it != end_it; ++it) {
                    append(it->key,it->data);
                }
            }
        };
        
    }//!namespace util
    
}//!namespace sv



#endif /* __MAP_H__ */
