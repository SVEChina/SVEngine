//
// SVResFBO.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SVResFBO_h
#define SVResFBO_h

#include "SVRObjBase.h"
#include "../base/SVPreDeclare.h"

namespace sv {
    
    namespace render{
        
        class SVResFBO: public SVRObjBase{
        public:
            SVResFBO(SVInst* _app);
            
            virtual ~SVResFBO();
            
            virtual void create(SVRendererPtr _renderer);
            
            virtual void destroy(SVRendererPtr _renderer);
            
        protected:
            u32     m_uid;          //唯一ID对象
            SVLockPtr m_texLock;
        };
        
    }//!namespace render
}//!namespace sv


#endif /* SVResFBO_h */
