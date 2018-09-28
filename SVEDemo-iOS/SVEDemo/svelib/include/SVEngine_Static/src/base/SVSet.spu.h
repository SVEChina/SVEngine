#ifndef __SET_SPU_H__
#define __SET_SPU_H__

#include "SVCoreTree.spu.h"

namespace sv {
    
    namespace util {
        
        //
        class SetData {
        public:
            SetData() {
            }
            
            ~SetData() {
            }
        };
        
        //
        template <class Key,u16 Capacity = 128> class SVSet : public SVCoreTree<Key,SetData,Capacity> {
        public:
            SVSet() {
            }
            
            ~SVSet() {
            }
            
            sv_inline void append(const Key &key) {
                s16 change = 0;
                typename SVSet<Key,Capacity>::Node *parent = 0;
                append_proc(key,SVCoreTree<Key,SetData,Capacity>::root,parent,change);
            }
        };
        
    }//!namespace util
    
}//!namespace sv



#endif /* __SET_SPU_H__ */
