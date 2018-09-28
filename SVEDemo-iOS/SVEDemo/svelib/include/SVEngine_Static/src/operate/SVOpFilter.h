//
// SVOpFilter.h 
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_OPERATEFILTER_H
#define SV_OPERATEFILTER_H


#include "SVOpBase.h"

namespace sv {
    //设置美颜滤镜
    class SVOpSetBeautyFilter : public SVOpBase {
    public:
        SVOpSetBeautyFilter(SVInst* _app,cptr8 _scenename,cptr8 _filter, int _lows);
    protected:
        void _process(f32 _dt);
        SVString m_scenename;
        SVString m_filter;
        s32 m_lows;
    };
    //设置美颜滤镜
    class SVOpUpdateFilterSmooth : public SVOpBase {
    public:
        f32 m_smooth;
        u32 m_type;
        SVOpUpdateFilterSmooth(SVInst* _app,f32 _smooth,u32 _type);
    protected:
        void _process(f32 _dt);
      
    };
    
    class SVOpUpdateBSplineFilter : public SVOpBase {
    public:
        SVDataSwapPtr m_pDataSwap;
        SVOpUpdateBSplineFilter(SVInst* _app,SVDataSwapPtr _pDataSwap);
    protected:
        void _process(f32 _dt);
        
    };
    
    //关闭美颜滤镜
    class SVOpCloseBeautyFilter : public SVOpBase {
    public:
        SVOpCloseBeautyFilter(SVInst* _app);
    protected:
        void _process(f32 _dt);
    };
    
    //
    class SVOpShapeFaceSmoothFilter : public SVOpBase {
    public:
        SVOpShapeFaceSmoothFilter(SVInst* _app,f32 _face,f32 _eye);
    protected:
        void _process(f32 _dt);
        f32 m_face;
        f32 m_eye;
    };
    

    
}//!namespace sv


#endif //SV_OPERATEFILTER_H
