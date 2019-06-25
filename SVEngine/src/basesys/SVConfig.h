//
// SVConfig.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_CONFIG_H
#define SV_CONFIG_H

#include "../base/SVGBase.h"
#include "../base/SVDatatDef.h"
#include "../rendercore/SVRenderDef.h"
#include "../base/SVVec3.h"
#include "../base/SVVec4.h"
#define KEY_SCREEN_WIDTH  "width"
#define KEY_SCREEN_HEIGHT "height"
#define KEY_DESIGN_WIDTH  "designwidth"
#define KEY_DESIGN_HEIGHT "designheight"
#define KEY_DESIGN_IMAGE_WIDTH  "designimagewidth"
#define KEY_DESIGN_IMAGE_HEIGHT "designimageheight"
#define KEY_ADAPT_MODE "adaptmode"

#endif //SV_CONFIG_H

#define GLES30 0x00030000

namespace sv {

    struct CfgFilter{
        CfgFilter(){
            m_2dmask = 1;
            m_3dmask = 1;
            m_2dbone = 1;
            m_3dbone = 1;
            m_2dframe = 1;
            m_distorb = 1;
            m_gpumorph = 1;
            m_morph = 1;
            m_particle = 1;
            m_sky = 1;
            m_audio = 1;
        };
        s32 m_2dmask;
        s32 m_3dmask;
        s32 m_2dbone;
        s32 m_3dbone;
        s32 m_2dframe;
        s32 m_distorb;
        s32 m_gpumorph;
        s32 m_morph;
        s32 m_particle;
        s32 m_sky;
        s32 m_audio;
    };
    
    class SVConfig : public SVGBase {
    public:
        SVConfig(SVInst *_app);
        
        ~SVConfig();
        
        void init();
        
        void destroy();
        
        void loadConfig();
        
        inline s32 getWidth() { return width; }
        
        inline s32 getHeight() { return height; }
        
        inline s32 getDesignWidth() { return designWidth; }
        
        inline s32 getDesignHeight() { return designHeight; }
        
        inline s32 getDesignImageWidth() { return designImageWidth; }
        
        inline s32 getDesignImageHeight() { return designImageHeight; }
        
        inline s32 getCameraWidth() { return cameraWidth; }
        
        inline s32 getCameraHeight() { return cameraHeight; }
        
        inline s32 getCameraAngle() { return cameraAngle; }
        
        inline ADAPT_MODE getAdaptMode() { return adaptmode; }
        
        inline s32 getDetectType() { return detectType; }
        
        inline FVec3& getAdaptScale() { return m_AdaptScale; }
        
        void setCameraDsp(s32 inCameraWidth, s32 inCameraHeight, s32 inCameraAngle);
        
        s32 getRealCameraWidth();
        
        s32 getRealCameraHeight();
        
        //识别库类型
        DETECTTYPE detectType;
        //主屏幕宽高
        s32 width;
        s32 height;
        //设计分辨率
        s32 designWidth;
        s32 designHeight;
        //设计底图尺寸
        s32 designImageWidth;
        s32 designImageHeight;
        //相机分辨率
        s32 cameraWidth;
        s32 cameraHeight;
        s32 cameraAngle;        //相机转角度
        PICFORMATE cameraFormate;       //相机输出格式
        SV_OUTSTEAMFORMATE dataoutFormate;            //数据输出格式
        //画笔参数相关
        f32 m_strokeWidth;
        FVec4 m_strokeColor;
        f32 m_strokeGlowWidth;
        FVec4 m_strokeGlowColor;
        //适配模式
        ADAPT_MODE adaptmode;
        s32 glVersion;
        bool useDataSwapOnNode;
        bool cameraMirror;      //相机输入数据是否镜像
        bool loadEffectIsScreen;
        CfgFilter m_cfgFilter;
        bool mirror; //是否镜像显示,即进来什么样就是什么样，不进行翻转
        SVString m_tag;
    private:
        void _adaptScale();
        
        FVec3 m_AdaptScale;
    };

//!namespace
}
