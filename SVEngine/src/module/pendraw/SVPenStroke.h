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
    //绘制事件
    enum SVPENMODE{
        SV_ARMODE,
        SV_FACEMODE,
        SV_NORMAL
    };
    
    struct SVStrokePoint {
        FVec3 point;
        FVec3 normal;
        FVec3 ext0;
        FVec3 ext1;
    };
    
    struct SVFaceParam {
        FVec3 faceCenter;
        FVec3 faceRot;
        f32   eyeDis;
    };
    
    class SVPenStroke : public SVGameBase {
    public:
        SVPenStroke(SVInst* _app, SVPENMODE _mode);
        
        ~SVPenStroke();
        
        void update(f32 _dt);
        
        void begin(f32 _px,f32 _py,f32 _pz);
        
        void end(f32 _px,f32 _py,f32 _pz);
        
        void draw(f32 _px,f32 _py,f32 _pz);
        
        void createStrokeMesh(f32 _strokeWidth, FVec4 &_strokeColor);
        
        void createGlowMesh(f32 _glowWidth, FVec4 &_glowColor);
        
        void updateStroke(float _dt);
        
        void renderStroke();
        
        void updateGlow(float _dt);
        
        void renderGlow();
        
        void genFaceRawParam(FVec3 &_noseCenter, FVec3 &_rotation, f32 _eyeDis);//原始脸部数据
        
        void setFaceParam(FVec3 &_noseCenter, FVec3 &_rotation, f32 _eyeDis);//实时脸部数据
        
        void getStrokePt(SVDataSwapPtr _dataSwap);
        
        void getGlowPt(SVDataSwapPtr _dataSwap);
        
        void setStrokePt(SVDataSwapPtr _dataSwap, s32 _ptSize);
        
        void setGlowPt(SVDataSwapPtr _dataSwap, s32 _ptSize);
        
        void setDrawBox(bool _drawBox);
        
        void renderBoundingBox();
        
        //序列化接口
        void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue, SVPenPackDataPtr _packData, cptr8 _path);
        
        void fromJSON(RAPIDJSON_NAMESPACE::Value &_item, SVPenPackDataPtr _packData, cptr8 _path);
    protected:
        void _updateARStroke(float _dt);
        
        void _updateARFaceStroke(float _dt);
        
        void _updateARGlow(float _dt);
        
        void _updateARFaceGlow(float _dt);
        
        void _screenPointToWorld(FVec2 &_point, SVStrokePoint &_worldPoint);
        //
        void _drawBoundBox();
        //
        typedef SVArray<SVStrokePoint> PTPOOL;
        PTPOOL m_ptCachePool;//保存原始点
        PTPOOL m_ptStrokePool;
        PTPOOL m_ptGlowPool;
        SVPenCurvePtr m_penCurve;
        SVLockPtr m_lock;
        SVBoundBox m_aabbBox;   //AABB包围盒
        //画笔相关
        SVDataSwapPtr m_pInstanceOffsetData;
        SVRenderObjectPtr m_pRenderObj;
        SVRenderMeshDvidPtr m_pStrokeMesh;
        SVMtlStrokeBasePtr m_pMtl;
        SVTexturePtr m_pTex;
        s32 m_instanceCount;
        s32 m_lastInstanceIndex;
        f32 m_stroke_width;
        FVec4 m_stroke_color;
        //画光圈相关
        SVDataSwapPtr m_pGlowInstanceOffsetData;  //mesh
        SVTexturePtr m_pGlowTex;
        SVRenderMeshDvidPtr m_pGlowMesh;
        SVMtlStrokeBasePtr m_pGlowMtl;
        s32 m_lastGlowInstanceIndex;
        s32 m_glowInstanceCount;
        f32 m_glow_width;
        FVec4 m_glow_color;
        //画笔参数
        f32 m_stroke_raw_width;
        f32 m_glow_raw_width;
        f32 m_stroke_curve;
        f32 m_glow_curve;
        //
        FMat4 m_localMat;
        FMat4 m_faceTransform;
        f32 m_plane_dis;
        LERPMETHOD m_lerpMethod;
        SVPENMODE m_penMode;
        SVFaceParam m_raw_faceParam;
        SVFaceParam m_faceParam;
        bool  m_haveGenFaceCoord;
        bool  m_drawBox;
    };
    
}//!namespace sv

#endif //SV_PENSTROKE_H
