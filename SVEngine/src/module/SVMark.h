//
// SVMark.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MARK_H
#define SV_MARK_H


#include "SVModuleBase.h"
#include "../act/SVActDeclare.h"
namespace sv{
    class SVMark : public SVModuleBase {
    public:
        SVMark(SVInst* _app);
        
        ~SVMark();
        
        virtual void init();
        
        virtual void destroy();
        
        virtual void update(f32 _dt);
        
        virtual void open();
        
        virtual void close();
        
        virtual bool isOpen();
        
        virtual bool procEvent(SVEventPtr _event);
        
        void setContent(SVString _content);
        
        void setAlpha(f32 _alpha);
        
        void setPosition(f32 _posX, f32 _posY, f32 _posZ);
        
        void setEnableRandomPosition(bool _enable);
        
        void setAlphaRange(f32 _src, f32 _tar);
        
        void setAlphaTime(f32 _time);
        
        void setAppearRate(f32 _time);
    protected:
        SVBMFontNodePtr m_bmFontNode;
        SVActionUnitPtr m_actAlphaUnit;
        SVActionUnitPtr m_actPositionUnit;
        SVString m_content;
        f32 m_alphaTime;
        f32 m_appearTime;
        f32 m_alpha;
        f32 m_srcAlpha;
        f32 m_tarAlpha;
    };
    
    
}//!namespace sv

#endif /* SV_MARK_H */
