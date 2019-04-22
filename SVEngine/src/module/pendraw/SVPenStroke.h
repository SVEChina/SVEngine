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
#include "SVPenCurve.h"
#include "../../node/SVMultPassNode.h"
namespace sv{
    
    struct SVStrokePoint {
        FVec3 point;
        FVec3 normal;
        FVec3 ext0;
        FVec3 ext1;
    };
    
    class SVPenStroke : public SVGameBase {
    public:
        SVPenStroke(SVInst* _app);
        
        ~SVPenStroke();
        
        void update(f32 _dt);
        
        void begin(f32 _px,f32 _py,f32 _pz);
        
        void end(f32 _px,f32 _py,f32 _pz);
        
        void draw(f32 _px,f32 _py,f32 _pz);
        
        void setStrokeWidth(f32 _width);
        
        void setDrawBox(bool _drawBox);
        
    protected:
        void _screenPointToWorld(FVec2 &_point, SVStrokePoint &_worldPoint);
        //
        void _genPolygon();
        //生成面片
        void _genMesh();
        //
        void _genBox(FVec3& _pt);
        //
        void _drawMesh();
        //
        typedef SVArray<SVStrokePoint> PTPOOL;
        
        PTPOOL m_ptPool;
        SVMultPassNodePtr m_pPassNode;
        SVPassPtr m_pass;
        SVPenCurvePtr m_penCurve;
        //数据域
        SVDataSwapPtr m_pVertData;  //mesh
        SVRenderMeshPtr m_pMesh;
        SVRenderObjectPtr m_pRenderObj;
        SVMtlStrokeBasePtr m_pMtl;
        SVTexturePtr m_pTex;
        SVLockPtr m_lock;
        SVBoundBox m_aabbBox;   //AABB包围盒
        FMat4 m_localMat;
        s32 m_vertexNum;
        s32 m_lastVertexIndex;
        f32 m_density;
        f32 m_point_dis_dert;
        f32 m_pen_width;
        f32 m_plane_dis;
        LERPMETHOD m_lerpMethod;
        bool m_drawBox;
    };
    
}//!namespace sv

#endif //SV_PENSTROKE_H
