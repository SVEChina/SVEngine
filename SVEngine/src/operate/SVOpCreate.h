//
// SVOpCreate.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li，daming Li
//

#ifndef SV_OPERATECREATE_H
#define SV_OPERATECREATE_H


#include "SVOpBase.h"
#include "../rendercore/SVRenderDef.h"
#include "../node/SVNodeDef.h"
#include "../node/SVScene.h"
#include "../node/SVSpineNode.h"

namespace sv {
    
    //创建场景OP
    class SVOpCreateScene : public SVOpBase {
    public:
        SVOpCreateScene(SVInst *_app, cptr8 name);
        
    protected:
        void _process(f32 dt);
        
        SVString m_scenename;
    };
    
    //删除逻辑场景
    class SVOpDestroyScene : public SVOpBase {
    public:
        SVOpDestroyScene(SVInst *_app,cptr8 name);
        
    protected:
        void _process(f32 dt);
        
        SVString m_scenename;
    };
    
    //
    class SVOpCameraControl : public SVOpBase {
    public:
        SVOpCameraControl(SVInst *_app, bool bOpen, s32 iResId);
        
        ~SVOpCameraControl();
        
    protected:
        void _process(f32 dt);
        
        bool m_bOpen;
        s32 m_iResId;
    };
    
    //创建天空
    class SVOpCreateSkyDome : public SVOpBase {
    public:
        SVOpCreateSkyDome(SVInst *_app, cptr8 pStrPath,s32 resId);
        
        ~SVOpCreateSkyDome();
        
    protected:
        void _process(f32 dt);
        
        SVString m_strPath;
        f32 m_iResId;
    };
    //创建粒子
    class SVOpCreateParticle : public SVOpBase {
    public:
        SVOpCreateParticle(SVInst *_app, cptr8 pStrPath);
        
        ~SVOpCreateParticle();
        
    protected:
        void _process(f32 dt);
        
        SVString m_strPath;
    };
    
    //创建特效
    class SVOpCreateEffcet : public SVOpBase {
    public:
        SVOpCreateEffcet(SVInst *_app, cptr8 pStrPath);
        
        ~SVOpCreateEffcet();
        
    protected:
        void _process(f32 dt);
        
        SVString m_strPath;
    };
    
    class SVOpTexAttachment: public SVOpBase {
    public:
        SVOpTexAttachment(SVInst *_app, cptr8 _strPath, s32 _channel, cptr8 _data, s32 _width, s32 _height);
        
        ~SVOpTexAttachment();
        
    protected:
        void _process(f32 dt);
        
        cptr8 m_data;
        SVString m_strPath;
        s32   m_width;
        s32   m_height;
        s32   m_channel;
    };
    
    //打开延时挂起
    class SVOpOpenDelaySuspend : public SVOpBase {
    public:
        SVOpOpenDelaySuspend(SVInst *_app, f32 _time);
        
        ~SVOpOpenDelaySuspend();
        
    protected:
        void _process(f32 dt);
        
        f32 m_time;
    };

    
    //测试操作
    class SVOpCreateTest : public SVOpBase {
    public:
        SVOpCreateTest(SVInst *_app);
        
        ~SVOpCreateTest();
        
    protected:
        void _process(f32 dt);
    };
    //操作测试spine
    class SVOpCreateTestSpine : public SVOpBase {
    public:
        SVOpCreateTestSpine(SVInst *_app, cptr8 _path);
        
        ~SVOpCreateTestSpine();
        
    protected:
        void _process(f32 dt);
        
        SVString m_path;
    };
    //分屏
    class SVOpCreateDivision : public SVOpBase {
    public:
        SVOpCreateDivision(SVInst *_app);
        
        ~SVOpCreateDivision();
        
    protected:
        void _process(f32 dt);
    };
    //mark
    class SVOpCreateMark : public SVOpBase {
    public:
        SVOpCreateMark(SVInst *_app);
        
        ~SVOpCreateMark();
        
    protected:
        void _process(f32 dt);
    };
    
    class SVOpMarkContent : public SVOpBase {
    public:
        SVOpMarkContent(SVInst *_app, SVString _content);
        
        ~SVOpMarkContent();
        
        SVString m_content;
    protected:
        void _process(f32 dt);
    };
    
    class SVOpMarkAlpha : public SVOpBase {
    public:
        SVOpMarkAlpha(SVInst *_app, f32 _alpha);
        
        ~SVOpMarkAlpha();
        
        f32 m_alpha;
    protected:
        void _process(f32 dt);
    };
    
    class SVOpMarkAlphaTime : public SVOpBase {
    public:
        SVOpMarkAlphaTime(SVInst *_app, f32 _alphaTime);
        
        ~SVOpMarkAlphaTime();
        
        f32 m_alphaTime;
    protected:
        void _process(f32 dt);
    };
    
    class SVOpMarkAlphaRange : public SVOpBase {
    public:
        SVOpMarkAlphaRange(SVInst *_app, f32 _srcAlpha, f32 _tarAlpha);
        
        ~SVOpMarkAlphaRange();
        
        f32 m_srcAlpha;
        
        f32 m_tarAlpha;
    protected:
        void _process(f32 dt);
    };
    
    class SVOpMarkAppearTime : public SVOpBase {
    public:
        SVOpMarkAppearTime(SVInst *_app, f32 _appearTime);
        
        ~SVOpMarkAppearTime();
        
        f32 m_appearTime;
    protected:
        void _process(f32 dt);
    };
    
    class SVOpMarkPosition : public SVOpBase {
    public:
        SVOpMarkPosition(SVInst *_app, FVec3 _pos);
        
        ~SVOpMarkPosition();
        
        FVec3 m_pos;
    protected:
        void _process(f32 dt);
    };
    
    class SVOpMarkEnableRandom : public SVOpBase {
    public:
        SVOpMarkEnableRandom(SVInst *_app, bool _enable);
        
        ~SVOpMarkEnableRandom();
        
        bool m_enable;
    protected:
        void _process(f32 dt);
    };
    
}//!namespace sv



#endif //SV_OPERATECREATE_H
