//
// SVPenDraw.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PENDRAW_H
#define SV_PENDRAW_H

#include "../SVGameBase.h"
#include "../../base/SVVec4.h"
#include "../../event/SVEvent.h"
#include "../../mtl/SVMtlDeclare.h"
#include "../../rendercore/SVRenderDeclare.h"
#include "SVPenStroke.h"
namespace sv{
    class SVPenDraw : public SVGameBase {
    public:
        SVPenDraw(SVInst* _app);
        
        ~SVPenDraw();
        
        virtual void init(SVGameReadyPtr _ready,SVGameRunPtr _run,SVGameEndPtr _end);
        
        virtual void destroy();
        
        virtual void update(f32 _dt);
        
        virtual void open();
        
        virtual void close();
        
        virtual bool procEvent(SVEventPtr _event);
        
        void clear();
        
        void redo();
        
        void undo();
        
        bool isRedoEnable();
        
        bool isUndoEnable();
        
        void setStrokeWidth(f32 _width);
        
        void setStrokeColor(FVec4 &_color);
        
        void setGlowWidth(f32 _width);
        
        void setGlowColor(FVec4 &_color);
        
        void setPenMode(SVPENMODE _mode);
        
        bool save(cptr8 _path);
        //序列化接口
        void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue, cptr8 _path);
        
        void fromJSON(RAPIDJSON_NAMESPACE::Value &_item, cptr8 _path);
    protected:
        void _fromJSONBase(RAPIDJSON_NAMESPACE::Value &_item);
        void _drawStroke();//画笔触
        void _drawGlow();//画荧光
        void _drawReback();//画回主纹理
        void _updateFaceParam();
        typedef SVArray<SVPenStrokePtr> STROKEPOOL;
        STROKEPOOL m_strokes;
        STROKEPOOL m_strokesCache;
        SVLockPtr m_lock;
        SVPenStrokePtr m_curStroke;
        SVRenderTexturePtr m_fbo1;
        SVRenderTexturePtr m_fbo2;
        SVMultMeshMtlRenderObjectPtr m_pRenderObj;
        SVMtlCorePtr m_mtl1;
        SVRenderMeshPtr m_mesh1;
        SVMtlCorePtr m_mtl2;
        SVRenderMeshPtr m_mesh2;
        SVTexturePtr m_pTex1;
        SVTexturePtr m_pTex2;
        SVFilterGlowPtr m_glowFilter;
        SVFilterBlurPtr m_blurFilter;
        f32 m_strokeWidth;
        f32 m_glowWidth;
        FVec4 m_strokeColor;
        FVec4 m_glowColor;
        FVec3 m_noseCenter;
        FVec3 m_faceRot;
        f32   m_faceEyeDis;
        SVPENMODE m_mode;
    };
    
}//!namespace sv





#endif //SV_PENDRAW_H
