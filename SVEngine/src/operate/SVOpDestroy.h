//
// SVOpDestroy.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#ifndef SV_OPDESTROY_H
#define SV_OPDESTROY_H

#include "SVOpBase.h"
#include "../base/SVDatatDef.h"

namespace sv {
    class SVOpDestroyEffect : public SVOpBase {
    public:
        SVOpDestroyEffect(SVInst *_app, cptr8 _name);
        
    protected:
        void _process(f32 dt);
        
        SVString m_name;
    };

    //
    class SVOpDestroyDivision : public SVOpBase {
    public:
        SVOpDestroyDivision(SVInst *_app);
        
    protected:
        void _process(f32 dt);
    };
    
    //
    class SVOpDestroyMark : public SVOpBase {
    public:
        SVOpDestroyMark(SVInst *_app);
        
    protected:
        void _process(f32 dt);
    };
    //
    class SVOpClosePen : public SVOpBase {
    public:
        SVOpClosePen(SVInst *_app);
        
    protected:
        void _process(f32 dt);
    };
}//!namespace sv



#endif //SV_OPDESTROY_H
