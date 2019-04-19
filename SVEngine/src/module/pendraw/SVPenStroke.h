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
#include "../../base/SVMat4.h"
#include "../../rendercore/SVRenderDeclare.h"
#include "../../mtl/SVMtlDeclare.h"
#include "../../base/SVBounds.h"
namespace sv{
    
    class SVPenStroke : public SVGameBase {
    public:
        typedef SVStrokePoint
    
        SVPenStroke(SVInst* _app);
        
        ~SVPenStroke();
        
        void update(f32 _dt);
        
        void begin(f32 _px,f32 _py,f32 _pz);
        
        void end(f32 _px,f32 _py,f32 _pz);
        
        void draw(f32 _px,f32 _py,f32 _pz);
        
        void setStrokeWidth(f32 _width);
        
        void setDrawBox(bool _drawBox);
    protected:
        void _genPoints();
        
        void _updatePtPool(SVArray<FVec2> &_inPtPool, SVArray<FVec3> &_outPtPool);
        
        void _screenPointToWorld(FVec2 &_point, FVec3 &_worldPoint);
        //
        void _fixRectVertexPoints();
        //
        void _genPolygon();
        //生成面片
        void _genMesh();
        
        void _drawMesh();
        //
        void _drawBoundBox();
        //
        typedef SVArray<SVStrokePoint> PTPOOL;
        
        PTPOOL m_screenPtPool;
        
        PTPOOL m_ptPool;
        
        PTPOOL m_ptCachePool;
        
        PTPOOL m_rectVertexPool;
        
        SVPenCurvePtr m_penCurve;
        //数据域
        SVDataSwapPtr m_pVertData;
        SVRenderMeshPtr m_pMesh;
        SVRenderObjectPtr m_pRenderObj;
        SVMtlStrokeBasePtr m_pMtl;
        SVTexturePtr m_pTex;
        SVLockPtr m_lock;
        SVBoundBox m_aabbBox;   //AABB包围盒
        FMat4 m_localMat;
        f32 m_stroke;
        f32 m_density;
        s32 m_vertexNum;
        bool m_drawBox;
        bool m_isFirstTouch;
    };
    
}//!namespace sv

#endif //SV_PENSTROKE_H
