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
namespace sv{
    
    //绘制事件

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
        
    protected:
        void _drawStroke();//画笔触
        void _drawGlow();//画荧光
        void _drawReback();//画回主纹理
        typedef SVArray<SVPenStrokePtr> STROKEPOOL;
        STROKEPOOL m_strokes;
        STROKEPOOL m_strokesCache;
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
        FVec4 m_strokeColor;
        f32 m_glowWidth;
        FVec4 m_glowColor;
        SVLockPtr m_lock;
    };
    
}//!namespace sv





#endif //SV_PENDRAW_H
