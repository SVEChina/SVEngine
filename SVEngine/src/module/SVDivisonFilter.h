//
// SVDivisonFilter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_DIVISIONFILTER_H
#define SV_DIVISIONFILTER_H

#include "SVModuleBase.h"
#include "SVModuleDef.h"
#include "../event/SVEventProc.h"
#include "../base/SVDef.h"
#include "../node/SVNodeDeclare.h"
#include "../basesys/SVStaticData.h"
namespace sv{
    
    class SVDivisonFilter : public SVModuleBase {
    public:
        SVDivisonFilter(SVInst* _app);
        
        virtual ~SVDivisonFilter();
        
        virtual void init();
        
        virtual void destroy();
        
        virtual void update(f32 _dt);
        
        virtual void open();
        
        virtual void close();
        
        virtual bool isOpen();
    protected:
        SVSpriteNodePtr m_backgroundNode;
    };
    
}//!namespace sv





#endif //SV_DIVISIONFILTER_H
