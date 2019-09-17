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
#include "../base/SVVec4.h"

namespace sv {
    namespace mtl{
        
        class SVMtlNocolor : public SVMtlCore {
        public:
            SVMtlNocolor(SVInst *_app);
            
            SVMtlNocolor(SVMtlNocolor *_mtl);
            
            ~SVMtlNocolor();
            
            SV_MTL_CLONE(SVMtlNocolor);
            
            virtual void update(f32 dt);
            
        protected:
            virtual void _submitMtl(SVRendererBasePtr _render); 
            f32 m_lerp;
            f32 m_transTime;
            f32 m_accTime;
        };
        
        //3D图元
        class SVMtlGeo3d : public SVMtlCore {
        public:
            SVMtlGeo3d(SVInst *_app);
            
            SVMtlGeo3d(SVMtlGeo3d *_mtl);
            
            ~SVMtlGeo3d();
            
            SV_MTL_CLONE(SVMtlGeo3d);
            
            void setColor(f32 _r,f32 _g,f32 _b,f32 _a);
            
        protected:
            virtual void _submitMtl(SVRendererBasePtr _render); 
            SVColor m_color;
        };
        
        //坐标
        class SVMtlCoord : public SVMtlCore {
        public:
            SVMtlCoord(SVInst *_app);
            
            SVMtlCoord(SVMtlCoord *_mtl);
            
            ~SVMtlCoord();
            
            SV_MTL_CLONE(SVMtlCoord);
            
        protected:
            virtual void _submitMtl(SVRendererBasePtr _render);
        };
        
        //网格
        class SVMtlNetGrid : public SVMtlCore {
        public:
            SVMtlNetGrid(SVInst *_app);
            
            SVMtlNetGrid(SVMtlNetGrid *_mtl);
            
            ~SVMtlNetGrid();
            
            SV_MTL_CLONE(SVMtlNetGrid);
            
            void setGridSize(s32 _width,s32 _height);
            
        protected:
            virtual void _submitMtl(SVRendererBasePtr _render);
            f32 m_width;
            f32 m_height;
        };
        
        //粒子带动画的
        class SVMtlParticleAni : public SVMtlCore {
        public:
            SVMtlParticleAni(SVInst *_app);
            
            SVMtlParticleAni(SVMtlParticleAni *_mtl);
            
            ~SVMtlParticleAni();
            
            SV_MTL_CLONE(SVMtlParticleAni);

        protected:
            virtual void _submitMtl(SVRendererBasePtr _render);

        public:
            FMat4 m_p_transform;
            FVec4 m_ani_trans;
            f32 m_p_radius;
            f32 m_p_fade;
            f32 m_diffuse_scale;
            FVec4 m_diffuse_color;
            FVec4 m_ambient_color;
        };
        
    }//!namespace mtl
}//!namespace sv



#endif //SV_MTL_NOCOLOR_H
