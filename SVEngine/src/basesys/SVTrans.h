//
// SVTrans.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_TRANS_H
#define SV_TRANS_H

#include "../base/SVGBase.h"

namespace sv {

    namespace logic {
        
        class SVTrans : public SVGBase {
        public:
            SVTrans(SVInst *_app);
            
            virtual ~SVTrans();
            
            virtual void init(s32 _w, s32 _h,f32 _angle,PICFORMATE _picformate,SVTEXTYPE _tt);
            
            virtual void destroy();
            
            virtual void pushData(u8 *_srcPtr);
            
            virtual void update(f32 dt);
            
        protected:
            s32 m_width;
            s32 m_height;
            PICFORMATE m_picformate;
            f32 m_angle;
            SVTEXTYPE m_texTT;
        };
    }
    
}//!namespace sv

#endif //ANDROID_SVYUVINSTRESMNODE_H
