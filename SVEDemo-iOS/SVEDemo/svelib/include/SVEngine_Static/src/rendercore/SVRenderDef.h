//
// SVRenderDef.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERDEF_H
#define SV_RENDERDEF_H

#include "../base/SVDef.h"
#include "SVRenderDeclare.h"

enum RENDERSTREAMTYPE {
    RST_BEGIN = 0,
    RST_IMG_TRANS,
    RST_COLORSELECT_BEGIN,  //拾取开始
    RST_COLORSELECT,        //拾取渲染流
    RST_COLORSELECT_END,    //拾取结束
    RST_SCENE_BEGIN,        //场景渲染是在设计分辨率下玩的内容
    RST_MAGIC_BEGIN,
    RST_BACKGROUND,
    RST_FACEMORPH,
    RST_SKY,
    RST_PREFILTER,
    RST_MASK2D,
    RST_ANIMATE,
    RST_FREETYPE,
    RST_DEBUG_INNER,
    RST_FRAMECOPY,          //主纹理拷贝
    RST_LASTFILTER,
    RST_MAGIC_END,
    RST_SCENE_END,          //场景之后是在界面尺寸(真分辨率)下玩的内容
    RST_DATATRANS,          //数据转换流
    RST_ADAPT_SCENE,        //scene单独使用，将设计分辨率的内容 适配给真是分辨率
    RST_LOGO,
    RST_DEBUG,
    RST_END
};

#endif //SV_RENDERDEF_H
