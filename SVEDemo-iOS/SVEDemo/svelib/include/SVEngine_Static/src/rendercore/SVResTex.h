//
// SVResTex.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SVResTex_h
#define SVResTex_h

#include "SVRObjBase.h"

namespace sv {
    
    namespace render{
        class SVResTex: public SVRObjBase{
        public:
            SVResTex(SVInst* _app);
            
            virtual ~SVResTex();
            
            virtual void create(SVRendererBasePtr _renderer);
            
            virtual void destroy(SVRendererBasePtr _renderer);
            
            virtual void setTexData(void *_data, s32 _len);
            
            virtual void commit();  //数据提交到显卡
            
            virtual bool getbLoad();
        
        protected:
            u32     m_uid;          //唯一ID对象
            SVLockPtr m_texLock;
        };
        
    }//!namespace render
}//!namespace sv


#endif /* SVResTex_h */
