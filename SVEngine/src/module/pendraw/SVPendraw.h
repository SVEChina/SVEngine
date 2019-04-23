//
// SVPendraw.h
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

    class SVPendraw : public SVGameBase {
    public:
        SVPendraw(SVInst* _app);
        
        ~SVPendraw();
        
        virtual void init(SVGameReadyPtr _ready,SVGameRunPtr _run,SVGameEndPtr _end);
        
        virtual void destroy();
        
        virtual void update(f32 _dt);
        
        virtual void open();
        
        virtual void close();
        
        virtual bool procEvent(SVEventPtr _event);
        
        void setStrokeWidth(f32 _width);
        
        void setStrokeColor(FVec4 &_color);
        
    protected:
        typedef SVArray<SVPenStrokePtr> STROKEPOOL;
        STROKEPOOL m_strokes;
        SVPenStrokePtr m_curStroke;
        SVRenderTexturePtr m_fbo;
        SVRenderObjectPtr m_pRenderObj;
        SVTexturePtr m_pInTex;
        SVTexturePtr m_pOutTex;
        f32 m_strokeWidth;
        FVec4 m_strokeColor;
    };
    
}//!namespace sv





#endif //SV_PENDRAW_H
