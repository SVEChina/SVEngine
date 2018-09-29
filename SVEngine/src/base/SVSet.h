#ifndef __SET_H__
#define __SET_H__

#include "SVCoreTree.h"

namespace sv {
    
    namespace util {
        //
        template <class Key> class SVSet : public SVCoreTree<Key,SetData> {
        public:
            SVSet() {
            }

            ~SVSet() {
            }
            
            sv_inline void append(const Key &key) {
                s32 change = 0;
                typename SVSet<Key>::Node *parent = 0;
                SVCoreTree<Key,SetData>::append_proc(key,SVCoreTree<Key,SetData>::root,parent,change);
            }
            
            void append(const SVSet<Key> &s) {
                typename SVSet<Key>::Iterator end_it = s.end();
                for(typename SVSet<Key>::Iterator it = s.begin(); it != end_it; ++it) {
                    append(it->key);
                }
            }
        };
        
        //
        struct SetData {
        };
        
    }//!namespace util
    
}//!namespace sv


#endif /* __SET_H__ */
