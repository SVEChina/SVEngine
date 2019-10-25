//
// SVMtlDef.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLDEF_H
#define SV_MTLDEF_H

#include "../base/SVDef.h"
#include "../rendercore/SVRenderDef.h"
#include "SVMtlDeclare.h"

#define SV_MTL_CLONE(classname) \
virtual SVMtlCorePtr clone() {\
    return MakeSharedPtr<classname>(this);\
}

namespace sv {

//暂时用这个

/* AccumOp */
#define SV_ACCUM                          0x0100
#define SV_LOAD                           0x0101
#define SV_RETURN                         0x0102
#define SV_MULT                           0x0103
#define SV_ADD                            0x0104

/* AlphaFunction */
#define SV_NEVER                          0x0200
#define SV_LESS                           0x0201
#define SV_EQUAL                          0x0202
#define SV_LEQUAL                         0x0203
#define SV_GREATER                        0x0204
#define SV_NOTEQUAL                       0x0205
#define SV_GEQUAL                         0x0206
#define SV_ALWAYS                         0x0207

#define SV_KEEP                           0x1E00
#define SV_REPLACE                        0x1E01
#define SV_INCR                           0x1E02
#define SV_DECR                           0x1E03
    
    class SVInst;
    
    typedef SVMtlCore *(*mtl_new_func)(SVInst *_app);

    enum TEXTUREPARAM{
        E_T_PARAM_FILTER_MAG = 0,
        E_T_PARAM_FILTER_MIN,
        E_T_PARAM_WRAP_S,
        E_T_PARAM_WRAP_T
    };
    
    //纹理过滤器
    enum TEXTUREFILTER{
        E_T_FILTER_NEAREST = 0,
        E_T_FILTER_LINEAR
    };
    
    //纹理环绕规则
    enum TEXTUREWRAP{
        E_T_WRAP_CLAMP = 0,
        E_T_WRAP_CLAMP_TO_EDAGE,
        E_T_WRAP_REPEAT,
        E_T_WRAP_MIRROR
    };
    
    struct BlendParam{
        bool enable;
        s32 srcParam;
        s32 dstParam;
    };
    
    struct CullFaceParam{
        bool enable;
        s32 frontFace;
        s32 cullFace;
    };
    
    //深度参数
    struct DepthParam{
        bool clear;
        bool enable;        //开启深度测试
        s32 testMethod;     //深度测试方法(GL_NEVER,GL_ALWAYS,GL_LESS,GL_LEQUAL,GL_GREATER,GL_GEQUAL,GL_NOTEQUAL)
    };
    
    //透明测试参数
    struct AlphaParam{
        bool enable;        //开启alpha测试
        s32 testMethod;    //alpha测试方法(GL_NEVER,GL_ALWAYS,GL_LESS,GL_LEQUAL,GL_GREATER,GL_GEQUAL,GL_NOTEQUAL)
    };
    
    //模板参数
    struct StencilParam{
        bool clear;         //
        bool enable;        //开启模板测试
        s32 passMethod;     //通过模板测试的规则(GL_NEVER,GL_LESS < ,GL_LEQUAL <= ,GL_GREATER > ,GL_GEEQUAL >= ,GL_EQUAL == ,GL_ALWAYS)
        s32 refValue;       //通过模板测试的参考值(0-255)
        s32 maskValue;      //掩码( 模板值&掩码 * 参考值&掩码 )
        s32 sfail;          //模板测试失败     (GL_KEEP,GL_ZERO,GL_REPLACE(参考值代替写入),GL_INCR,GL_DECR,GL_INVERT(按位取反))
        s32 zfail;          //模板通过 深度失败 (GL_KEEP,GL_ZERO,GL_REPLACE(参考值代替写入),GL_INCR,GL_DECR,GL_INVERT(按位取反))
        s32 zpass;          //模板通过 深度通过 (GL_KEEP,GL_ZERO,GL_REPLACE(参考值代替写≤
    };
    
    //材质标记
    enum MTLFLAG0{
        MTL_F0_MAT_NULL     = 0x00000000,
        MTL_F0_MAT_M        = 0x00000001,
        MTL_F0_MAT_V        = 0x00000002,
        MTL_F0_MAT_P        = 0x00000004,
        MTL_F0_MAT_VP       = 0x00000008,
        MTL_F0_TEX0         = 0x00000010,
        MTL_F0_TEX1         = 0x00000020,
        MTL_F0_TEX2         = 0x00000040,
        MTL_F0_TEX3         = 0x00000080,
        MTL_F0_TEX4         = 0x00000100,
        MTL_F0_TEX5         = 0x00000200,
        MTL_F0_TEX6         = 0x00000400,
        MTL_F0_TEX7         = 0x00000800,
        MTL_F0_TEX_FLIP     = 0x00001000,
        MTL_F0_POINT_SIZE   = 0x00002000,
        MTL_F0_LINE_SIZE    = 0x00004000,
        MTL_F0_BLEND        = 0x00010000,
        MTL_F0_CULL         = 0x00020000,
        MTL_F0_STENCIL      = 0x00040000,
        MTL_F0_ALPHA        = 0x00080000,
        MTL_F0_DEPTH        = 0x00100000,
        MTL_F0_TEX_SIZE     = 0x01000000,
        MTL_F1_TEX_SIZE     = 0x02000000,
        MTL_F2_TEX_SIZE     = 0x04000000,
        MTL_F3_TEX_SIZE     = 0x08000000,
        MTL_F0_BLENDMODE    = 0x10000000,
        MTL_F0_ZOFF         = 0x20000000
    };
    
    //材质标记
    enum MTLFLAG_BASEON{
        MTL_F_BASEON_NONE         = 0x00000000,
        MTL_F_BASEON_SHADOW       = 0x00000001,
        MTL_F_BASEON_HL           = 0x00000002,
        MTL_F_BASEON_CONSTRAST    = 0x00000004,
        MTL_F_BASEON_SATURATION   = 0x00000008,
        MTL_F_BASEON_ACUTANCE     = 0x00000010,
        MTL_F_BASEON_BRIGHTNESS   = 0x00000020,
        MTL_F_BASEON_WHITENING    = 0x00000040,
        MTL_F_BASEON_GAMMA        = 0x00000080,
        MTL_F_BASEON_REDSHIFT     = 0x00000100,
        MTL_F_BASEON_GREENSHIFT   = 0x00000200,
        MTL_F_BASEON_BLUESHIFT    = 0x00000400,
        MTL_F_BASEON_SDREDSHIFT   = 0x00000800,
        MTL_F_BASEON_SDGREENSHIFT = 0x00001000,
        MTL_F_BASEON_SDBLUESHIFT  = 0x00002000,
        MTL_F_BASEON_HHREDSHIFT   = 0x00004000,
        MTL_F_BASEON_HHGREENSHIFT = 0x00008000,
        MTL_F_BASEON_HHBLUESHIFT  = 0x00010000,
        MTL_F_BASEON_TEMPERATURE  = 0x00020000,
        MTL_F_BASEON_TINT         = 0x00040000,
    };
    
    //美颜滤镜(FTB filter beauty)
    enum  FTBTYPE{
        FTB_ORIGIN = 0,
        FTB_ORIGIN02,
        FTB_VALENCIA,
        FTB_VALENCIA01,
        FTB_NASHVILLE,
        FTB_WALDEN,
        FTB_WALDEN01,
        FTB_BRANNAN,
        FTB_INKWELL,
        FTB_INKWELL01,
        FTB_INKWELL02,
        FTB_INKWELL03,
        FTB_PINK,
        FTB_PINK01,
        FTB_GUASS,
        FTB_GUASS01,
        FTB_GRASS,
        FTB_BEACH,
        FTB_BEACH01,
        FTB_NEW3,
        FTB_SWEETY,
        FTB_SWEETY01,
        FTB_SWEETY02,
        FTB_NATURE,
        FTB_NATURE01,
        FTB_NATURE02,
        FTB_CLEAN,
        FTB_CLEAN01,
        FTB_CLEAN02,
        FTB_FRESH,
        FTB_FRESH01,
        FTB_CORAL,
        FTB_CORAL01,
        FTB_SUNSET,
        FTB_VIVID,
        FTB_LOLITA,
        FTB_ROSY,
        FTB_URBAN,
        FTB_VINTAGE,
        FTB_KISSKISS,
        FTB_SNOW,
        FTB_GLOSSY
    };
    
    //基于时间的滤镜(FTT filter time)
    enum  FTTTYPE{
        FTT_EDAGE_DETECTION = 0,
        FTT_BASIC_DEFORM,
        FTT_CHRO_ABE,
        FTT_EM_INTERFERENCE,
        FTT_CONSTRAST
    };

    //shader编译头
    //版本
    static c8 SHADER_HEAD_GL[] = {
        "#version 150 \n"
    };
    
    static c8 SHADER_HEAD_GLES_20[] = {
        "#define GL_ES 1 \n"
    };
    
    static c8 SHADER_HEAD_GLES_30[] = {
        "precision lowp float;\n"
    };
    
    //精度
    static c8 SHADER_PRCCISION_LOW[] = {
        "precision lowp float;\n"
    };
    
    static c8 SHADER_PRCCISION_MEDIUMP[] = {
        "precision mediump float;\n"
    };
    
    static c8 SHADER_PRCCISION_HIGH[] = {
        "precision highp float;\n"
    };
    
    
}



#endif //SV_MTLDEF_H
