//
// SVPenStroke.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PENSTROKE_H
#define SV_PENSTROKE_H

#include "../SVGameBase.h"
#include "../../base/SVVec3.h"
#include "../../rendercore/SVRenderDeclare.h"
namespace sv{
    
    class SVPenStroke : public SVGameBase {
    public:
        SVPenStroke(SVInst* _app);
        
        ~SVPenStroke();
        
        void update(f32 _dt);
        
        void begin(f32 _px,f32 _py,f32 _pz);
        
        void end(f32 _px,f32 _py,f32 _pz);
        
        void draw(f32 _px,f32 _py,f32 _pz);
        
    protected:
        //生成面片
        void _genMesh();
        
        void _drawMesh();
        //
        typedef SVArray<FVec3> PTPOOL;
        
        PTPOOL m_ptPool;
        //数据域
        SVDataSwapPtr m_pVertData;
        SVRenderMeshPtr m_pMesh;
        SVRenderObjectPtr m_pRenderObj;
//        //
//        static PTPOOL m_ptCache;
        f32 m_pointSize;
        s32 m_vertexNum;
    };
    
}//!namespace sv

#endif //SV_PENSTROKE_H
