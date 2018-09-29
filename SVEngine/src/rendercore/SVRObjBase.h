//
// SVRObjBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ROBJBASE_H
#define SV_ROBJBASE_H

#include "../base/SVGBase.h"
#include "../base/SVPreDeclare.h"
#include "SVRenderDef.h"

//渲染中间件基类

namespace sv {
    
    namespace render{
        
        class SVRObjBase: public SVGBase {
        public:
            SVRObjBase(SVInst* _app);
            
            virtual ~SVRObjBase();
            
            virtual void create(SVRendererBasePtr _renderer);
            
            virtual void destroy(SVRendererBasePtr _renderer);
            
            virtual void setData(SVDataSwapPtr _pdata);
            
            virtual void process();     //处理
            
        protected:
            SVDataSwapPtr m_pData;      //数据
        };
        
    }//!namespace render
    
}//!namespace sv



//
#endif //SV_ROBJBASE_H
