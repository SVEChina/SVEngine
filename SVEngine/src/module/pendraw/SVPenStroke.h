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
        typedef FVec3 SVStrokePoint;
        typedef struct STROKEDGE{
            SVStrokePoint p0;
            SVStrokePoint p1;
        }SVStrokeEdge;
        
        typedef struct RECTANGLE{
            SVStrokeEdge edge0;
            SVStrokeEdge edge1;
        }SVStrokeRectangle;

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
        void _genPolygon();
        bool _judgePolygonLineIntersect(SVStrokeRectangle &_rectangle, f32 *_t);
        //生成矩形
        void _genRectangle();
        //生成面片
        void _genMesh();
        
        void _drawMesh();
        //
        void _drawBoundBox();
        //
        typedef SVArray<FVec3> PTPOOL;
        
        PTPOOL m_screenPtPool;
        
        PTPOOL m_ptPool;
        
        typedef SVArray<SVStrokeRectangle> RECTANGLEPOOL;
        
        RECTANGLEPOOL m_rectanglePool;
        
        SVPenCurvePtr m_penCurve;
        //数据域
        SVDataSwapPtr m_pVertData;
        SVRenderMeshPtr m_pMesh;
        SVRenderObjectPtr m_pRenderObj;
        SVMtlStrokeBasePtr m_pMtl;
        SVTexturePtr m_pTex;
        SVLockPtr m_lock;
        SVBoundBox m_aabbBox;   //AABB包围盒
        SVStrokeRectangle m_lastRectangle;
        SVStrokeEdge m_lastEdge;
        SVStrokePoint m_lastPoint;
        FMat4 m_localMat;
        f32 m_stroke;
        f32 m_density;
        s32 m_vertexNum;
        bool m_drawBox;
        bool m_isFirstTouch;
    };
    
}//!namespace sv

#endif //SV_PENSTROKE_H
