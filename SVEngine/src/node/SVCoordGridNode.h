//
// SVCoordGridNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_COORD_GRID_NODE_H
#define SV_COORD_GRID_NODE_H

#include "SVNode.h"

namespace sv {
    
    namespace node{
        
        //笛卡尔坐标系
        
        class SVCoordGridNode : public SVNode {
        public :
            SVCoordGridNode(SVInst *_app);
            
            ~SVCoordGridNode();
            
            void update(f32 dt);
            
            void render();
            
            f32 getUnit();
            
        protected:
            void _refreshUnit();
            
            f32 m_unit;
            
            SVRenderObjectPtr m_pRenderObj;
            
            SVRenderMeshPtr m_pMeshX;
            SVRenderMeshPtr m_pMeshY;
            SVRenderMeshPtr m_pMeshZ;
            
            SVTexturePtr m_gridTex;
        };
        
        //网格节点
        class SVNetGridNode : public SVNode {
        public :
            SVNetGridNode(SVInst *_app);
            
            ~SVNetGridNode();
            
            void update(f32 dt);
            
            void render();
            
            f32 getWidth();
            
            f32 getHeight();
            
            f32 getGridUnit(f32 _h);
            
            void setMaxGridX(s32 _max_gx);
            
            void setMaxGridY(s32 _max_gy);
            
            void setGridUnit(f32 _unit);
            
        protected:
            f32 m_grid_unit;
            s32 m_grid_x;
            s32 m_grid_y;
            bool m_refresh;

            SVRenderObjectPtr m_pRenderObj;
            SVRenderMeshPtr m_pMesh;
            SVTexturePtr m_gridTex;
        };
        
    }//!namespace node
    
}//!namespace sv



#endif /* SV_COORD_GRID_NODE_H */
