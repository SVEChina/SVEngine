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
namespace sv{
    
    struct SVStrokePoint {
        FVec3 point;
        FVec3 normal;
        FVec3 ext0;
        FVec3 ext1;
    };
    
    class SVPenStroke : public SVGameBase {
    public:
        SVPenStroke(SVInst* _app, f32 _strokeWidth, FVec4 &_strokeColor, f32 _glowWidth, FVec4 &_glowColor);
        
        ~SVPenStroke();
        
        void update(f32 _dt);
        
        void begin(f32 _px,f32 _py,f32 _pz);
        
        void end(f32 _px,f32 _py,f32 _pz);
        
        void draw(f32 _px,f32 _py,f32 _pz);
        
        void setDrawBox(bool _drawBox);
        
        void renderStroke();
        
        void renderGlow();
        
        void renderBoundingBox();
    protected:
        void _screenPointToWorld(FVec2 &_point, SVStrokePoint &_worldPoint);
        //
        void _genPolygon();
        //
        void _genInstances();
        //
        void _createStrokeMesh();
        //
        void _createGlowMesh();
        //
        void _drawStroke();
        //
        void _drawBoundBox();
        //
        void _drawGlow();
        //
        typedef SVArray<SVStrokePoint> PTPOOL;
        
        PTPOOL m_ptPool;
        PTPOOL m_ptGlowPool;
        SVArray<SVBillboardNodePtr> m_glowStrokes;
        SVPenCurvePtr m_penCurve;
        //盒子相关
        SVDataSwapPtr m_pInstanceOffsetData;
        SVRenderObjectPtr m_pRenderObj;
        SVRenderMeshDvidPtr m_pBoxMesh;
        SVMtlStrokeBasePtr m_pMtl;
        SVTexturePtr m_pTex;
        SVLockPtr m_lock;
        SVBoundBox m_aabbBox;   //AABB包围盒
        //画光圈相关
        SVDataSwapPtr m_pGlowInstanceOffsetData;  //mesh
        SVTexturePtr m_pGlowTex;
        SVRenderMeshDvidPtr m_pGlowMesh;
        SVMtlStrokeBasePtr m_pGlowMtl;
        FVec4 m_glowColor;
        s32 m_lastGlowInstanceIndex;
        f32 m_glowStrokeWidth;
        f32 m_glowDensity;
        s32 m_glowInstanceCount;
        //
        FMat4 m_localMat;
        FVec4 m_strokeColor;
        s32 m_instanceCount;
        s32 m_lastInstanceIndex;
        f32 m_density;
        f32 m_pen_width;
        f32 m_plane_dis;
        LERPMETHOD m_lerpMethod;
        bool m_drawBox;
    };
    
}//!namespace sv

#endif //SV_PENSTROKE_H
