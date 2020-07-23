//
// SVOilPaintFilter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_OILPAINTFILTER_H
#define SV_OILPAINTFILTER_H
#include "module/SVGameBase.h"
//#include "SVOilPaintFilterDef.h"
namespace sv{
    class SVOilPaintFilter : public SVModuleBase {
    public:
        SVOilPaintFilter(SVInst* _app);
        
        ~SVOilPaintFilter();
        
        virtual void init();
        
        virtual void destroy();
        
        virtual void update(f32 _dt);
        
        virtual void open();
        
        virtual void close();
        
        virtual bool isOpen();
        
        virtual bool procEvent(SVEventPtr _event);
    protected:
    
    };
    
    
}//!namespace sv

#endif /* SV_OILPAINTFILTER_H */
