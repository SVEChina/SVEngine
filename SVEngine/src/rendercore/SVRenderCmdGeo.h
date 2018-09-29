//
// SVRenderCmdGeo.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERCMD_GEO_H
#define SV_RENDERCMD_GEO_H


#include "SVRenderCmd.h"
#include "../base/SVBounds.h"
#include "../base/SVRect.h"
#include "../base/SVPreDeclare.h"

namespace sv {
    
    namespace render{
        
        //线绘制
        class SVRenderCmdGeo : public SVRenderCmd {
        public:
            SVRenderCmdGeo();
            
            ~SVRenderCmdGeo();
            
            virtual void render();
            
            void setMtl(SVMtlCorePtr _material);
            
        protected:
            virtual void _render();
            
            SVMtlCorePtr m_pMtl;
        };
        
        //线绘制
        class SVRenderCmdLine : public SVRenderCmdGeo {
        public:
            void setLine(FVec3& _start,FVec3& _end);
            
        protected:
            virtual void _render();
            
            FVec3 m_start;
            FVec3 m_end;
        };
        
        //矩形绘制
        class SVRenderCmdRect : public SVRenderCmdGeo {
        public:
            void setRect(SVRect& _rect);
            
        protected:
            virtual void _render();
            
            SVRect m_rect;
        };
        
        //曲线绘制
        class SVRenderCmdCurve : public SVRenderCmdGeo {
        public:
            
        protected:
            virtual void _render();
        
        public:
            SVArray<FVec3> m_ptList;
        };
        
        //原型绘制
        class SVRenderCmdCircle : public SVRenderCmdGeo {
        public:
            
        protected:
            virtual void _render();
            FVec3 m_center;
            f32 m_radius;
            f32 m_angle;
        };
        
        //圆固体绘制
        class SVRenderCmdCircleSolid : public SVRenderCmdGeo {
        public:
            
        protected:
            virtual void _render();
            FVec3 m_center;
            f32 m_radius;
            f32 m_angle;
        };
        
        //AABB绘制
        class SVRenderCmdAABB : public SVRenderCmdGeo {
        public:
            void setAABB(SVBoundBox& _aabb);
            
        protected:
            virtual void _render();
            
            SVBoundBox m_aabb;
        };
        
    }//!namespace render
    
}//!namespace sv




#endif //SV_RENDERCMD_GEO_H
