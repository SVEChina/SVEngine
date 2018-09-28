//
// SVMtlRGB2YUV.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTL_RGB2YUV_H
#define SV_MTL_RGB2YUV_H

#include "SVMtlCore.h"

namespace sv {
    namespace mtl{
        
        class SVMtlRGB2YUV : public SVMtlCore {
        public:
            SVMtlRGB2YUV(SVInst *_app, SV_OUTSTEAMFORMATE formate);
            
            SVMtlRGB2YUV(SVMtlRGB2YUV* _mtl);
            
            ~SVMtlRGB2YUV();
            
            virtual SVMtlCorePtr clone();
            
            void setTex(f32 _w, f32 _h);
            
            void _submitMtl();
            
        private:
            s32 m_tex_w;
            s32 m_tex_h;
        };
        
    }//!namespace mtl
}//!namespace sv



#endif //SV_MTL_RGB2YUV_H
