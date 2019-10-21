//
// SVUINetGrid.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_UINETGRID_H
#define SV_UINETGRID_H

#include "SVUIBase.h"
#include "../base/SVMap.h"
#include "../base/SVArray.h"

namespace sv {
    
    namespace node{
        
        struct ElemCoord {
            s32 m_row;
            s32 m_col;
        };
    
        //网格元素
        class SVUINetElem : public SVGBase {
        public:
            SVUINetElem(SVInst* _app);
            
            ~SVUINetElem();
            
            bool hasData(s32 _row,s32 _col);
            
            void pushData(s32 _row,s32 _col);
            
            void popData();
            
            void clearData();
            
            bool delData(s32 _row,s32 _col);
            
            //刷新数据
            void refreshData(s32 _unit);
            
            void update(f32 _dt,f32 *_mat);
            
            void render(RENDERSTREAMTYPE _rsType);
            
            //标记
            s32 m_tag;
            
            //纹理
            SVTexturePtr m_pTex;
            
            //数据池
            SVArray<ElemCoord> m_elemPool;
            
        protected:
            bool m_dirty;
            //一堆格子的mesh
            SVRenderMeshPtr m_pMesh;
            SVRenderObjectPtr m_pRObj;
        };
        
        //网格节点
        class SVUINetGrid : public SVUIBase {
        public :
            SVUINetGrid(SVInst *_app);
            
            ~SVUINetGrid();
            
            void update(f32 dt);
            
            void render();
            
            f32 getWidth();
            
            f32 getHeight();
            
            f32 getGridUnit(f32 _h);
            
            s32 getMaxGridX();
            
            s32 getMaxGridY();
            
            void setMaxGridX(s32 _max_gx);
            
            void setMaxGridY(s32 _max_gy);
            
            void setGridUnit(f32 _unit);
            
            void setArcho(EUIARCHO _cho);
            //元素操作
            //增加一种元素
            bool addElem(s32 _type,cptr8 _texname);
            
            //删除一种元素
            bool delElem(s32 _type);
            
            //查找一种元素
            bool hasElem(s32 _type);
            
            SVUINetElemPtr getElem(s32 _type);
            
            //增加某一元素的数据
            bool addElemData(s32 _type,s32 _row,s32 _col);
            
            //删除某一元素的数据
            void delElemData(s32 _type,s32 _row,s32 _col);
            
            //清楚某一元素的数据
            void clearElemData(s32 _type);
            
            //弹出某一元素的数据
            void popElemData(s32 _type);
            
        protected:
            f32 m_grid_unit;
            s32 m_grid_x;
            s32 m_grid_y;
            bool m_refresh;
            
            //偏移值 主要用于横滚
            f32 m_off_x;
            f32 m_off_y;
            
            //
            SVRenderObjectPtr m_pRenderObj;
            SVRenderMeshPtr m_pMesh;
            SVTexturePtr m_gridTex;
            
            //元素表
            typedef SVArray<SVUINetElemPtr> ELEMTBL;
            ELEMTBL m_elemTbl;
            
            void _updateElemShow(s32 _type);
            
            void _updateAllElemShow();
            
            void _updateElemShow(SVUINetElemPtr _elem);
        };

    }//!namespace node
    
}//!namespace sv


#endif //SV_UINETGRID_H
