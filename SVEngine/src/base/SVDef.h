//
// SVDef.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BASEDEF_H
#define SV_BASEDEF_H

#include "SVTypes.h"
#include "svstr.h"
#include "SVArray.h"

#define SV_DEBUG_RENDERTIME 1
#define SV_SUPPORT_MAX_PERSON_NUM 4
#define SV_PI       3.14159265359f
#define SV_HALF_PI  SV_PI/2.0f

//默认一个小单位格子是64 ，64*64为一个tile 一个单一世界由
#define SV_WORLD_SIZE 64.0*64.0*8
#define SV_WORLD_DEPTH 3

enum SV_STATE{
    SV_ST_NULL = 0,
    SV_ST_WAIT,         //等待
    SV_ST_RUN,          //运行
    SV_ST_WILLSUSPEND,  //将要挂起
    SV_ST_SUSPEND       //挂起
};

enum SV_ENGCORE {
    SV_E_CORE_GL = 0,
    SV_E_CORE_METAL,
    SV_E_CORE_VULKAN
};

//引擎状态
enum SV_ENG_TIMESTATE {
    ENG_TS_NOR = 0,
    ENG_TS_FIX,
    ENG_TS_STOP
};

//sv线程阶段 回调
enum SV_THREAD_STAGE {
    THREAD_STAGE_BEGIN = 0,
    THREAD_STAGE_END,
    THREAD_STAGE_SIGN_SUSPEND,
    THREAD_STAGE_UPDATE_BEGIN,
    THREAD_STAGE_UPDATE_END
};

//sv线程运行状态
enum SV_THREAD_STATE {
    THREAD_STATE_NULL = 0,
    THREAD_STATE_START,
    THREAD_STATE_SUSPEND,
    THREAD_STATE_RUN,
    THREAD_STATE_DESTROY
};

//sv渲染器类型
enum SV_RENDERER_TYPE {
    E_RENDERER_GLES,
    E_RENDERER_VUNKAN,
    E_RENDERER_METAL
};

//适配模式
enum ADAPT_MODE {
    E_APT_M_SCALE=1,  //缩放模式
    E_APT_M_BODER,  //留黑边模式
    E_APT_M_FULL    //全屏无黑边
};

enum SV_OUTSTEAMFORMATE {
    SV_OUT_STEAM_RGB2YUVNV12 = 0,
    SV_OUT_STEAM_RGBA,
    SV_OUT_STEAM_RGB2YUVNV21,
    SV_OUT_STEAM_RGB2YUVYV12,
    SV_OUT_STEAM_RGB2YUVI420,
    SV_OUT_STEAM_BGRA,
    SV_OUT_STEAM_NULL,
};

//bitfont 绑定枚举
enum BITFONT_ATCH_PT {
    ATCH_LB,
    ATCH_LC,
    ATCH_LT,
    ATCH_MB,
    ATCH_MC,
    ATCH_MT,
    ATCH_RB,
    ATCH_RC,
    ATCH_RT
};

//SV 支持的纹理类型
enum SVTEXTYPE {
    E_TEX_MAIN = 0,
    E_TEX_CAMERA = 1,
    E_TEX_INSTREAM = 10,
    E_TEX_OUTSTREAM = 20,
    E_TEX_HELP0 = 21,
    E_TEX_HELP1,
    E_TEX_HELP2,
    E_TEX_HELP3,
    E_TEX_HELP4,
    E_TEX_HELP5,
    E_TEX_HELP6,
    E_TEX_HELP7,
    E_TEX_VIDEO0,
    E_TEX_VIDEO1,
    E_TEX_VIDEO2,
    E_TEX_VIDEO3,
    E_TEX_VIDEO4,
    E_TEX_FILTER_DEFORM01,
    E_TEX_FILTER_DEFORM02,
    E_TEX_FILTER_DEFORM03,
    E_TEX_FILTER_LUT_OUT,
    E_TEX_FILTER_LUT,
    E_TEX_FILTER_GENLUT_OUT,
    E_TEX_FILTER_GENLUT_H1,
    E_TEX_FILTER_GENLUT_H2,
    E_TEX_FILTER_GENLUT_H3,
    E_TEX_FILTER_GENLUT_H4,
    E_TEX_FILTER_MIP_1,
    E_TEX_FILTER_MIP_2,
    E_TEX_FILTER_GOF_1,
    E_TEX_FILTER_GOF_2,
    E_TEX_FILTER_1,
    E_TEX_FILTER_2,
    E_TEX_FILTER_3,
    E_TEX_FILTER_4,
    E_TEX_END = 64
};

//相机图片格式
enum PICFORMATE {
    SV_PF_GRAY8 = 0,
    SV_PF_YV12,
    SV_PF_I420,
    SV_PF_NV12,
    SV_PF_NV21,
    SV_PF_BGRA,
    SV_PF_RGBA,
    SV_PF_RGB,
    SV_PF_NULL
};


//创建外部GL环境
namespace sv {
    namespace render{
        class SVContextBase;
        typedef void (SVContextBase::*glEnvCreate)();
        typedef void (SVContextBase::*glEnvDestroy)();
    }
}

namespace sv {
    class SVInst;
    class SVThread;
}
//回调函数
typedef void (*ThreadCallback)(sv::SVInst* _app,sv::SVThread* _thread,SV_THREAD_STAGE _stage);
//百分比回调
typedef void (*cb_func_percent)(f32 percent);
//百分比回调
typedef void (*cb_func_record)(s32 _frames);
//操作回调
typedef void (*cb_func_op)(cptr8 _info);
//游戏信息回调
typedef void (*cb_func_game)(s32 _type,cptr8 _info);
//输出回调
typedef void (*cb_out_stream)(s32 _w,s32 _h,s32 _formate,void* _pdata,long _timtap);
//输出状态
typedef void (*cb_outstream_state)(s32 _state);

//最大同时支持的视频解析线程数目
#define SV_MAX_VIDEOLOAD_NUM 6

#endif //SV_BASEDEF_H
