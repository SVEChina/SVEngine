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
        SVOpTexAttachment(SVInst *_app, cptr8 _strPath, s32 _channel, void * _data, s32 _width, s32 _height);
        
        ~SVOpTexAttachment();
        
    protected:
        void _process(f32 dt);
        
        void * m_data;
        SVString m_strPath;
        s32   m_width;
        s32   m_height;
        s32   m_channel;
    };
    
    //恢复
    class SVOpEngineResume : public SVOpBase {
    public:
        SVOpEngineResume(SVInst *_app);
        
        ~SVOpEngineResume();
        
    protected:
        void _process(f32 dt);
    };
    
    //挂起
    class SVOpEngineSuspend : public SVOpBase {
    public:
        SVOpEngineSuspend(SVInst *_app);
        
        ~SVOpEngineSuspend();
        
    protected:
        void _process(f32 dt);
    };

    //推送人脸识别点
    class SVOpPushFaceData : public SVOpBase {
    public:
        SVOpPushFaceData(SVInst *_app, void *_data);
        
        ~SVOpPushFaceData();
        
    protected:
        void _process(f32 dt);
        
        void *m_data;
    };
    
    //测试操作
    class SVOpCreateTest : public SVOpBase {
    public:
        SVOpCreateTest(SVInst *_app);
        
        ~SVOpCreateTest();
        
    protected:
        void _process(f32 dt);
    };
    
    //测试操作
    class SVOpCreateDebugFacePt : public SVOpBase {
    public:
        SVOpCreateDebugFacePt(SVInst *_app);
        
        ~SVOpCreateDebugFacePt();
        
    protected:
        void _process(f32 dt);
    };
   
    //测试画线
    class SVOpCreateTestLine : public SVOpBase {
    public:
        SVOpCreateTestLine(SVInst *_app);
        
        ~SVOpCreateTestLine();
        
    protected:
        void _process(f32 dt);
    };
    
    //测试画补丁网格
    class SVOpCreateTestPatch : public SVOpBase {
    public:
        SVOpCreateTestPatch(SVInst *_app);
        
        ~SVOpCreateTestPatch();
        
    protected:
        void _process(f32 dt);
        
        SVActionUnitPtr m_actUnit;
    };

    //测试面纱
    class SVOpCreateTestMianSha : public SVOpBase {
    public:
        SVOpCreateTestMianSha(SVInst *_app);
        
        ~SVOpCreateTestMianSha();
        
    protected:
        void _process(f32 dt);
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
    
    class SVOpMarkScale : public SVOpBase {
    public:
        SVOpMarkScale(SVInst *_app, f32 _scale);
        
        ~SVOpMarkScale();
        
        f32 m_scale;
    protected:
        void _process(f32 dt);
    };
    
    class SVOpMarkRotation : public SVOpBase {
    public:
        SVOpMarkRotation(SVInst *_app, f32 _rotation);
        
        ~SVOpMarkRotation();
        
        f32 m_rotation;
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
    
    //
    class SVOpOpenPen : public SVOpBase {
    public:
        SVOpOpenPen(SVInst *_app);
        
        ~SVOpOpenPen();
        
    protected:
        void _process(f32 dt);
    };
    //
    class SVOpSetPenEffcet : public SVOpBase {
    public:
        SVOpSetPenEffcet(SVInst *_app, cptr8 pStrPath);
        
        ~SVOpSetPenEffcet();
        
    protected:
        void _process(f32 dt);
        
        SVString m_strPath;
    };
    //
    class SVOpSavePenEffcet : public SVOpBase {
    public:
        SVOpSavePenEffcet(SVInst *_app, cptr8 pStrPath);
        
        ~SVOpSavePenEffcet();
        
    protected:
        void _process(f32 dt);
        
        SVString m_strPath;
    };
    //
    class SVOpClearPen : public SVOpBase {
    public:
        SVOpClearPen(SVInst *_app);
        
        ~SVOpClearPen();
        
    protected:
        void _process(f32 dt);
    };
    //
    class SVOpPenUndo : public SVOpBase {
    public:
        SVOpPenUndo(SVInst *_app);
        
        ~SVOpPenUndo();
        
    protected:
        void _process(f32 dt);
    };
    //
    class SVOpPenRedo : public SVOpBase {
    public:
        SVOpPenRedo(SVInst *_app);
        
        ~SVOpPenRedo();
        
    protected:
        void _process(f32 dt);
    };
    //
    class SVOpPenUndoIsEnable : public SVOpBase {
    public:
        SVOpPenUndoIsEnable(SVInst *_app);
        
        ~SVOpPenUndoIsEnable();
        
    protected:
        void _process(f32 dt);
    };
    //
    class SVOpPenRedoIsEnable : public SVOpBase {
    public:
        SVOpPenRedoIsEnable(SVInst *_app);
        
        ~SVOpPenRedoIsEnable();
        
    protected:
        void _process(f32 dt);
    };
    //
    class SVOpPenMode : public SVOpBase {
    public:
        SVOpPenMode(SVInst *_app, s32 _mode);
        
        ~SVOpPenMode();
        
    protected:
        void _process(f32 dt);
        s32 m_mode;
    };

    class SVOpCreateGameZCMgr : public SVOpBase {
    public:
        SVOpCreateGameZCMgr(SVInst *_app);
        
        ~SVOpCreateGameZCMgr();
        
    protected:
        void _process(f32 dt);
    };
}//!namespace sv



#endif //SV_OPERATECREATE_H
