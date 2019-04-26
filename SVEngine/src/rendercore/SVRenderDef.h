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

/*
 .背景（sky）
 .美颜（磨皮，美白，红润等）
 .美妆（纹眉，腮红，嘴唇等面具技术）
 .面部微整（大眼，瘦脸，尖下巴，隆鼻等）
 .风格化（老照片，青色，阳光等）
 .面具（饰品类）
 .动画（头饰类，跟人）
 .AR特效
 .平面特效（动画，全屏幕动画）
 .ui类
 .debug类
 */

enum RENDERSTREAMTYPE {
    RST_BEGIN = 0,
    RST_SCENE_BEGIN,        //场景渲染是在设计分辨率下玩的内容
    RST_SKY,                //background
    RST_BEAUTY,             //美颜
    RST_FACEACCELENT,       //面部增强
    RST_FACEMORPH,          //面部微整
    RST_IMGFILTER,          //风格化
    RST_IMGFILTER_OUT,      //lut数据输出
    RST_MASK2D,             //面具+头饰
    RST_FREETYPE,
    RST_SOLID_2D,           //跟人2d先绘制
    RST_AR_BEGIN,           //ar开始
    RST_AR,                 //ar相关
    RST_AR1,
    RST_AR_END,             //ar结束
    RST_SOLID_3D,           //正常固体
    RST_ALPHA_3D,           //正常透明
    RST_DEBUG_INNER,        //图像内部测试
    RST_LASTFILTER,         //图形后处理滤镜
    RST_MAGIC_OUT,          //图像输出
    RST_SCENE_END,          //场景之后是在界面尺寸(真分辨率)下玩的内容
    RST_DATATRANS,          //数据转换流
    RST_ADAPT_SCENE,        //scene单独使用，将设计分辨率的内容 适配给真是分辨率
    RST_2D_BEGIN,
    RST_UI,
    RST_2D_END,
    RST_LOGO,
    RST_DEBUG,
    RST_END
};

/* Buffer mask
 */
enum {
    BUFFER_NONE = 0,
    BUFFER_RED = 1 << 0,
    BUFFER_GREEN = 1 << 1,
    BUFFER_BLUE = 1 << 2,
    BUFFER_ALPHA = 1 << 3,
    BUFFER_DEPTH = 1 << 4,
    BUFFER_STENCIL = 1 << 5,
    BUFFER_COLOR = (BUFFER_RED | BUFFER_GREEN | BUFFER_BLUE | BUFFER_ALPHA),
    BUFFER_DEPTH_STENCIL = (BUFFER_DEPTH | BUFFER_STENCIL),
    BUFFER_ALL = (BUFFER_COLOR | BUFFER_DEPTH_STENCIL),
};

/* Depth testing
 */
enum {
    DEPTH_NONE = 0,
    DEPTH_NEVER,
    DEPTH_LESS,
    DEPTH_EQUAL,
    DEPTH_LEQUAL,
    DEPTH_GREATER,
    DEPTH_NOTEQUAL,
    DEPTH_GEQUAL,
    DEPTH_ALWAYS,
    NUM_DEPTH_FUNC,
};

/* Blending
 */
enum {
    BLEND_NONE = 0,
    BLEND_ZERO,
    BLEND_ONE,
    BLEND_SRC_COLOR,
    BLEND_ONE_MINUS_SRC_COLOR,
    BLEND_SRC_ALPHA,
    BLEND_ONE_MINUS_SRC_ALPHA,
    BLEND_DEST_COLOR,
    BLEND_ONE_MINUS_DEST_COLOR,
    BLEND_DEST_ALPHA,
    BLEND_ONE_MINUS_DEST_ALPHA,
    BLEND_SRC1_COLOR,
    BLEND_ONE_MINUS_SRC1_COLOR,
    BLEND_SRC1_ALPHA,
    BLEND_ONE_MINUS_SRC1_ALPHA,
    NUM_BLEND_FUNC,
};

/* Downsize
 */
enum {
    DOWNSIZE_NONE = 0,
    DOWNSIZE_SCALE,
    DOWNSIZE_FILTER,
    DOWNSIZE_BACKGROUND_SCALE,
    DOWNSIZE_BACKGROUND_FILTER,
    NUM_DOWNSIZE_MODES,
};

/* Alpha Func
 */
enum {
    ALPHA_NONE,
    ALPHA_COVERAGE,
    NUM_ALPHA_FUNC,
};

/* Front face
 */
enum {
    FRONT_CCW = 0,
    FRONT_CW,
    FRONT_INVERT,
    NUM_FRONT_MODES,
};

/* Face culling
 */
enum {
    CULL_NONE = 0,
    CULL_BACK,
    CULL_FRONT,
    NUM_CULL_MODES,
};

/* Polygon fill
 */
enum {
    FILL_SOLID = 0,
    FILL_WIREFRAME,
    NUM_FILL_MODES,
};

/* Shader types
 */
enum {
    SHADER_VERTEX = 0,
    SHADER_CONTROL,
    SHADER_EVALUATE,
    SHADER_GEOMETRY,
    SHADER_FRAGMENT,
    SHADER_COMPUTE,
    NUM_SHADERS,
};

#endif //SV_RENDERDEF_H
