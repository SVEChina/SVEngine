//
// SVMtlNocolor.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#ifndef SV_MTL_NOCOLOR_H
#define SV_MTL_NOCOLOR_H

#include "SVMtlCore.h"
#include "../base/SVColor.h"
#include "../base/SVPreDeclare.h"

namespace sv {
    namespace mtl{
        
        class SVMtlNocolor : public SVMtlCore {
        public:
            SVMtlNocolor(SVInst *_app);
            
            SVMtlNocolor(SVMtlNocolor *_mtl);
            
            ~SVMtlNocolor();
            
            virtual SVMtlCorePtr clone();
            
            virtual void update(f32 dt);
            
        protected:
            virtual void _submitMtl();
            f32 m_lerp;
            f32 m_transTime;
            f32 m_accTime;
        };
        
        //3d图元
        class SVMtlGeo3dParam : public SVMtlCoreParam {
        public:
            SVMtlGeo3dParam();
            
            virtual SVMtlCorePtr genMtl(SVInst *_app);
            
            f32 m_r;
            f32 m_g;
            f32 m_b;
            f32 m_a;
        };
        
        class SVMtlGeo3d : public SVMtlCore {
        public:
            SVMtlGeo3d(SVInst *_app);
            
            SVMtlGeo3d(SVMtlGeo3d *_mtl);
            
            ~SVMtlGeo3d();
            
            virtual SVMtlCorePtr clone();
            
            void setColor(f32 _r,f32 _g,f32 _b,f32 _a);
            
        protected:
            virtual void _submitMtl();
            SVColor m_color;
        };
        
    }//!namespace mtl
}//!namespace sv



#endif //SV_MTL_NOCOLOR_H
