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
    //加载红包资源
    class SVOpLoadRedPacket : public SVOpBase {
    public:
        SVOpLoadRedPacket(SVInst *_app, cptr8 pStrPath);
        
        ~SVOpLoadRedPacket();
        
    protected:
        void _process(f32 dt);
        
        SVString m_strPath;
    };
    //创建红包
    class SVOpCreateRedPacket : public SVOpBase {
    public:
        SVOpCreateRedPacket(SVInst *_app, cptr8 pStrPath, s32 _totalPacketNum, s32 _mode = 0);
        
        ~SVOpCreateRedPacket();
        
    protected:
        void _process(f32 dt);
        
        SVString m_strPath;
        
        s32 m_totalPacketNum;
        
        s32 m_mode;
    };
    
    //创建红包
    class SVOpOpenRedPacket : public SVOpBase {
    public:
        SVOpOpenRedPacket(SVInst *_app, s32 _value, s32 _x, s32 _y);
        
        ~SVOpOpenRedPacket();
        
    protected:
        void _process(f32 dt);
        
        s32 m_value;
        
        s32 m_x;
        
        s32 m_y;
    };
    
    class SVOpRemoveOnePacket : public SVOpBase {
    public:
        SVOpRemoveOnePacket(SVInst *_app, s32 _id);
        
        ~SVOpRemoveOnePacket();
        
    protected:
        void _process(f32 dt);
        
        s32 m_id;
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
    
}//!namespace sv



#endif //SV_OPERATECREATE_H
