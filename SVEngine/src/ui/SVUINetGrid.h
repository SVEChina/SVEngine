//
// SVUINetGrid.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_UINETGRID_H
#define SV_UINETGRID_H

#include "../node/SVNode.h"

namespace sv {
    
    namespace node{
       
        //网格节点
        class SVUINetGrid : public SVNode {
            public :
            SVUINetGrid(SVInst *_app);
            
            ~SVUINetGrid();
            
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


#endif //SV_UINETGRID_H
