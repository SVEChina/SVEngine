//
//  SVI.m
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVI.h"
#import "SVIThread.h"
#import "SVICamera.h"
#import "SVIEffect.h"
#import "work/SVThreadPool.h"
#import "work/SVThreadMain.h"
#import "work/SVThreadHelp.h"
#import "file/SVFileMgr.h"
#import "rendercore/SVContextIOS.h"
#import "operate/SVOpRender.h"
#import "operate/SVOpCreate.h"
#import "basesys/SVBasicSys.h"
#import "basesys/SVConfig.h"
#import "basesys/SVPickProcess.h"
#import "event/SVOpEvent.h"
#import "event/SVEventMgr.h"
#if TARGET_OS_IPHONE
#import <UIKit/UIApplication.h>

@interface SVI(){
    SVIThread*      m_thread;
    SVSYNCMODE      m_syncMode;
    bool            m_first;
    timeval         m_lastT;
    int             glesVersion;
    int             m_designWidth;
    int             m_designHeight;
    //
    float           m_lastRoll;
    float           m_lastYaw;
    float           m_lastPitch;
    //
    float           m_lastAccX;
    float           m_lastAccY;
    float           m_lastAccZ;
}
@end

void sv_callback_op(const char* _info, void *_obj){
    if (strlen(_info) > 0) {
        SVI* t_self = (__bridge SVI*)_obj;
        if (t_self) {
            NSDictionary *t_dic = @{@"operate":[NSString stringWithUTF8String:_info]};
            dispatch_async(dispatch_get_main_queue(), ^{
                [[NSNotificationCenter defaultCenter] postNotificationName:@"svengine_op_callback" object:nil userInfo:t_dic];
            });
        }
    }
}

@implementation SVI

- (instancetype)initMode:(SVSYNCMODE)_mode Tag:(NSString *)_tag{
    self = [super init];
    if (self) {
        //包名检查
        m_thread = [[SVIThread alloc] init];
        [m_thread globalSyncProcessingQueue:^{
            self->m_pApp = new SVInst();
            
            SVInst* t_app = (SVInst*)self->m_pApp;
            if(t_app){
                t_app->init(_mode);
                if (_tag.length > 0) {
                    t_app->getConfig()->m_tag = [_tag UTF8String];
                }
            }
            
            self.pCamera = [[SVICamera alloc] initWithSVE:self->m_pApp Thread:self->m_thread];
            
            self.pEffect = [[SVIEffect alloc] initWithSVE:self->m_pApp Thread:self->m_thread];
            
            self->m_syncMode = _mode;
            self->m_first = true;
        }];
    }
    return self;
}

- (instancetype)init{
    return [self initMode:SV_AUTO_MODE Tag:@"svengine"];
}

- (NSString *)getTag{
    if (self->m_pApp) {
        SVInst* t_app = (SVInst*)self->m_pApp;
        SVString t_enginetag = t_app->getConfig()->m_tag;
        return [NSString stringWithUTF8String:t_enginetag.c_str()];
    }
    return nil;
}

//对象析构
-(void)dealloc{
    //
    [m_thread globalSyncProcessingQueue:^{
        self.pCamera = nil;
        self.pEffect = nil;
        self->m_first = false;
        SVInst* t_app = (SVInst*)self->m_pApp;
        if(t_app){
            t_app->destroy();
            delete t_app;
            self->m_pApp = nullptr;
        }
    }];
    m_thread = nil;
}

//设置资源搜索路径
- (void)addResPath:(NSString*)_path{
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            t_app->getFileMgr()->addRespath([_path UTF8String]);
        }
    }];
}

//开启引擎
- (void)startEngine{
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            if(t_app){
                t_app->startSVE();
                (*sv_callback_op)("sv_enginestart", obj);
            }
        }
    }];
}

//关闭引擎
- (void)stopEngine{
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            if(t_app){
                t_app->stopSVE();
                (*sv_callback_op)("sv_enginestop", obj);
            }
        }
    }];
}

- (float)getDert{
    f32 dt = 0;
    if (m_first) {
        m_first = false;
        timeval t_curT;
        gettimeofday(&t_curT, NULL);
        m_lastT = t_curT;
        dt = 0.033f;    //按照30帧算的
    } else {
        timeval t_curT;
        gettimeofday(&t_curT, NULL);
        s32 t_ms = s32( (t_curT.tv_sec - m_lastT.tv_sec)*1000+(t_curT.tv_usec - m_lastT.tv_usec) * 0.001f);
        dt = t_ms * 0.001f;
        m_lastT = t_curT;
        SV_LOG_INFO("svi time dert %f \n",dt);
    }
    return dt;
}

//更新
- (void)update{
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_syncMode == SV_MANUAL_MODE) {
            if (self->m_pApp) {
                SVInst* t_app = (SVInst*)self->m_pApp;
                if(t_app){
                    t_app->updateSVE([self getDert]);
                }
            }
        }
    }];
}

//挂起
-(void)suspend{
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            if(t_app && t_app->getState() != SV_ST_SUSPEND){
                t_app->svSuspend();
                t_app->clearCache();
                (*sv_callback_op)("sv_enginesuspend", obj);
            }
        }
    }];
}

//唤醒
-(void)resume{
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            if(t_app){
                t_app->svResume();
                (*sv_callback_op)("sv_engineresume", obj);
            }
        }
        self->m_first = true;
    }];
}

- (void)setEngineTimeState:(int)_timeState{
    if(_timeState < 0 || _timeState > 2) return;
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            if(t_app){
                t_app->setTimeState((SV_ENG_TIMESTATE)_timeState);
            }
        }
    }];
}

//初始化GL渲染器
- (void)createRendererGL:(int)glVer Context:(EAGLContext*)_context Width:(int)_w Height:(int)_h{
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            SVOpCreateRenderderGLIOSPtr t_op = MakeSharedPtr<SVOpCreateRenderderGLIOS>(t_app,
                                                                                       glVer,
                                                                                       (__bridge_retained void *)_context,
                                                                                       _w,
                                                                                       _h);
            t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
        }
    }];
}

//销毁渲染器
- (void)destroyRendererGL {
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            SVOpDestroyRenderderPtr t_op = MakeSharedPtr<SVOpDestroyRenderder>(t_app);
            t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
        }
    }];
}

//创建Metal渲染器
- (void)createRendererMetal:(void*)_device Width:(int)_w Height:(int)_h {
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            SVOpCreateRenderderMTLPtr t_op = MakeSharedPtr<SVOpCreateRenderderMTL>(t_app,_device);
            t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
        }
    }];
}

//销毁渲染器
- (void)destroyRendererMetal {
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            SVOpDestroyRenderderPtr t_op = MakeSharedPtr<SVOpDestroyRenderder>(t_app);
            t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
        }
    }];
}

//创建渲染环境
-(void)createRenderTarget:(int)_fboid Color:(int)_colorid Width:(int)_w Height:(int)_h Mirror:(bool)_mirror{
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            SVOpSetRenderTargetPtr t_op = MakeSharedPtr<SVOpSetRenderTarget>(t_app);
            t_op->setTargetParam(_w,_h,_fboid,_colorid, _mirror);
            t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
        }
    }];
}

//销毁渲染环境
-(void)destroyRenderTarget{
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            SVOpDestroyRenderTargetPtr t_op = MakeSharedPtr<SVOpDestroyRenderTarget>(t_app);
            t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
        }
    }];
}

- (void)setMirror:(bool)_mirror{
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            SVOpSetRenderMirrorPtr t_op = MakeSharedPtr<SVOpSetRenderMirror>(t_app);
            t_op->setTargetParam(_mirror);
            t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
        }
    }];
}

//创建场景
- (void)createScene:(cb_func_op)_cb msg:(NSString*)_msg{
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            SVOpCreateScenePtr t_op = MakeSharedPtr<SVOpCreateScene>(t_app,"showScene");
            t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
            //////
//                    SVOpCreateTestLinePtr t_op_test = MakeSharedPtr<SVOpCreateTestLine>(t_app);
//                    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op_test);
        }
    }];
}

//销毁场景
-(void)destroyScene{
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            SVOpDestroyScenePtr t_op = MakeSharedPtr<SVOpDestroyScene>(t_app,"showScene");
            t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
        }
    }];
}

//推送简单的点击数据
-(void)pushTouchX:(float)_x Y:(float)_y WorldPtX:(float *)_worldX WorldPtY:(float *)_worldY WorldPtZ:(float *)_worldZ State:(int)_state{
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            SVPickProcessPtr t_pickModule = t_app->getBasicSys()->getPickModule();
            SVTouchEventPtr t_touch = MakeSharedPtr<SVTouchEvent>();
            t_touch->x = _x;
            t_touch->y = _y;
            FVec3 t_worldPt = FVec3(0.0f, 0.0f, 0.0f);
            if (t_pickModule) {
                FVec2 t_screenPt = FVec2(_x, _y);
                t_pickModule->transScreenPtToWorld(t_screenPt, t_worldPt);
                *_worldX = t_worldPt.x;
                *_worldY = t_worldPt.y;
                *_worldZ = t_worldPt.z;
            }
            switch (_state) {
                case 1:{
                    t_touch->eventType = EVN_T_TOUCH_BEGIN;
                    break;
                }
                case 2:{
                    t_touch->eventType = EVN_T_TOUCH_MOVE;
                    break;
                }
                case 3:{
                    t_touch->eventType = EVN_T_TOUCH_END;
                    break;
                }
                default:
                    break;
            }
            t_app->getEventMgr()->pushEvent(t_touch);
        }
    }];
}

-(void)pushTouchWorldX:(float)_x Y:(float)_y Z:(float)_z State:(int)_state{
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            if (_state == 1) {
                SVTouchWorldEventPtr t_touch = MakeSharedPtr<SVTouchWorldEvent>();
                t_touch->eventType = EVN_T_TOUCH_WORLD_BEGIN;
                t_touch->x = _x;
                t_touch->y = _y;
                t_touch->z = _z;
                t_app->getEventMgr()->pushEvent(t_touch);
            }
            if (_state == 2) {
                SVTouchWorldEventPtr t_touch = MakeSharedPtr<SVTouchWorldEvent>();
                t_touch->eventType = EVN_T_TOUCH_WORLD_MOVE;
                t_touch->x = _x;
                t_touch->y = _y;
                t_touch->z = _z;
                t_app->getEventMgr()->pushEvent(t_touch);
            }
            if (_state == 3) {
                SVTouchWorldEventPtr t_touch = MakeSharedPtr<SVTouchWorldEvent>();
                t_touch->eventType = EVN_T_TOUCH_WORLD_END;
                t_touch->x = _x;
                t_touch->y = _y;
                t_touch->z = _z;
                t_app->getEventMgr()->pushEvent(t_touch);
            }
        }
    }];
}

- (void)pushCameraMatrix:(void *)_matrixData len:(int)_len{
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            SVCameraMatrixEventPtr t_cameraMatrix = MakeSharedPtr<SVCameraMatrixEvent>();
            t_cameraMatrix->eventType = EVN_T_CAMERA_MATRIX;
            t_cameraMatrix->m_matData->writeData(_matrixData, _len);
            t_app->getEventMgr()->pushEvent(t_cameraMatrix);
        }
    }];
}

- (void)pushProjectMatrix:(void *)_matrixData len:(int)_len{
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            SVProjectMatrixEventPtr t_projectMatrix = MakeSharedPtr<SVProjectMatrixEvent>();
            t_projectMatrix->eventType = EVN_T_PROJECT_MATRIX;
            t_projectMatrix->m_matData->writeData(_matrixData, _len);
            t_app->getEventMgr()->pushEvent(t_projectMatrix);
        }
    }];
}

- (void)pushARAnchorModelMatrix:(void *)_matrixData{
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            SVARAnchorEventPtr t_anchorPoint = MakeSharedPtr<SVARAnchorEvent>();
            t_anchorPoint->eventType = EVN_T_ANCHOR_AR;
            t_anchorPoint->m_matData->writeData(_matrixData, 16*sizeof(float));
            t_app->getEventMgr()->pushEvent(t_anchorPoint);
        }
    }];
}

- (void)pushARAnchorProjPosX:(float)_x PosY:(float)_y{
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_pApp) {
            SVInst* t_app = (SVInst*)self->m_pApp;
            SVARAnchorProjPosEventPtr t_anchorPoint = MakeSharedPtr<SVARAnchorProjPosEvent>();
            t_anchorPoint->eventType = EVN_T_ANCHORPOINT_AR;
            t_anchorPoint->m_x = _x;
            t_anchorPoint->m_y = _y;
            t_app->getEventMgr()->pushEvent(t_anchorPoint);
        }
    }];
}

//切换工作模式
-(void)setWorkMode:(int)_mode OP:(cb_func_op)_cb msg:(NSString*)_msg{
    
}

- (void)setFPS:(int)_fps{
    [m_thread globalSyncProcessingQueue:^{
        if (_fps >= 0) {
            if (self->m_pApp) {
                SVInst* t_app = (SVInst*)self->m_pApp;
                t_app->m_pTPool->getMainThread()->setFPS(_fps);
                t_app->m_pTPool->getHelpThread()->setFPS(_fps);
            }
        }
    }];
}

- (SVSYNCMODE)getSyncMode{
    return m_syncMode;
}

- (BOOL)checkBundleID{
    NSString *bundleID = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleIdentifier"];
    NSMutableArray *bundleIDArray = [[NSMutableArray alloc] initWithObjects:@"one.SHOW.Live", @"test.showtools.SVEShow",@"test.showtools.SVEVideo", nil];
    BOOL t_result = NO;
    for (NSInteger i = 0; i<bundleIDArray.count; i++) {
        NSString *t_bundleID = bundleIDArray[i];
        if ([[t_bundleID lowercaseString] isEqualToString:[bundleID lowercaseString]]) {
            t_result = YES;
            break;
        }
    }
    //弹窗
    if (!t_result) {
        id<UIApplicationDelegate> t_delegate = [UIApplication sharedApplication].delegate;
        [t_delegate.window makeKeyAndVisible];
        UIAlertController * alertController = [UIAlertController alertControllerWithTitle:@"SVEEngine Error" message:@"检查包名不通过，请联系wechat:freeze1123" preferredStyle:UIAlertControllerStyleAlert];//UIAlertControllerStyleAlert视图在中央
        UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleCancel handler:nil];
        [alertController addAction:cancelAction];
        [t_delegate.window.rootViewController presentViewController:alertController animated:YES completion:nil];
    }
    return t_result;
}
@end

#endif
