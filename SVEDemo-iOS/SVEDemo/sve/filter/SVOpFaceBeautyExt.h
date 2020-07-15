//
// SVOpFaceBeautyExt.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_OPFACEBEAUTYEXT_H
#define SV_OPFACEBEAUTYEXT_H


#include <operate/SVOpBase.h>

namespace sv {
    //设置美颜滤镜
    class SVOpFaceBeautyExt : public SVOpBase {
    public:
        SVOpFaceBeautyExt(SVInst* _app,cptr8 _scenename,cptr8 _filter, int _lows);
    protected:
        void _process(f32 _dt);
        SVString m_scenename;
        SVString m_filter;
        s32 m_lows;
    };


}//!namespace sv


#endif //SV_OPFACEBEAUTYEXT_H
