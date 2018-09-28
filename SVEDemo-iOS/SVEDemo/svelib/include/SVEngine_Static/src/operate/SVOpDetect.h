//
// SVOpDetect.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#ifndef SV_OPERATEDETECT_H
#define SV_OPERATEDETECT_H

#include "SVOpBase.h"
#include "../base/SVDatatDef.h"

namespace sv {
    
    //
    class SVOpOpenDetect : public SVOpBase {
    public:
        SVOpOpenDetect(SVInst *_app, s32 detecttype);
        
    protected:
        void _process(f32 dt);
        
        DETECTTYPE m_detectType;
    };
    
    class SVOpCloseDetect : public SVOpBase {
    public:
        SVOpCloseDetect(SVInst *_app);
        
    protected:
        void _process(f32 dt);
    };
    
}//!namespace sv



#endif //SV_OPERATEDETECT_H
