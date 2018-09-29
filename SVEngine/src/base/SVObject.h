//
// SVObject.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_OBJECT_H
#define SV_OBJECT_H

#include "SVDef.h"
#include "SVPreDeclare.h"

namespace sv {
    
    //对象
    class SVObject : public ShareFromThis<SVObject>{
    public:
        SVObject();
        
        virtual ~SVObject();
        
        virtual void listenData(SVObjectPtr datagen) {};
    };
    
    //数据监听函数指针
    typedef void (SVObject::*dataListenFunc)(SVObjectPtr datagen);
    
}



#endif //SV_OBJECT_H
