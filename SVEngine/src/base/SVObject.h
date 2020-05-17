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

/* c++ 11 标准 sharepter
 * share_ptr是一个模版类
 * share_ptr
 * unique_ptr
 * weak_pter
 * 本质是模版，重载了->和*
 * 转换类型
 * static_point_case<T>(), const_pointer_case<T>(),dynamic_pointer_cast<T>()
 * 创建 make_shared 而不是new
 */


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
