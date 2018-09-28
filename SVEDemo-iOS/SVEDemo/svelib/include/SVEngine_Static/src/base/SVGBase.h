//
// Created by yizhou Fu on 2017/4/29.
//

#ifndef SV_GBASE_H
#define SV_GBASE_H

//管理基类

#include "SVObject.h"
#include "SVPreDeclare.h"

namespace sv {
    
    class SVGBase: public SVObject {
    public:
        SVGBase(SVInst* _app);
        
        ~SVGBase();
        
    protected:
        SVInst* mApp;
    };
    
}//!namespace


#endif //SV_GBASE_H
