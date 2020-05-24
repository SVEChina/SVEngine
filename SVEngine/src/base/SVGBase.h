//
// Created by yizhou Fu on 2017/4/29.
//

#ifndef SV_GBASE_H
#define SV_GBASE_H

//管理基类

#include "SVObject.h"
#include "SVPreDeclare.h"
#include "../third/rapidjson/rapidjson.h"
#include "../third/rapidjson/document.h"
#include "../third/rapidjson/stringbuffer.h"
#include "../third/rapidjson/writer.h"

namespace sv {
    
    class SVGBase: public SVObject {
    public:
        SVGBase(SVInst* _app);
        
        ~SVGBase();
        
    protected:
        SVInst* mApp;
    };

    class SVGBaseEx: public SVObject {
    public:
        SVGBaseEx(SVInstPtr _app);
    
        ~SVGBaseEx();
    
    protected:
        SVInstPtr mApp;
};
    
}//!namespace


#endif //SV_GBASE_H
