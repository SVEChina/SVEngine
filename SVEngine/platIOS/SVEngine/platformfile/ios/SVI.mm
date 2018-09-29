//
//  SV.m
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVI.h"
#import "SVICamera.h"
#import "SVIDetect.h"
#import "SVIEffect.h"
#import "SVIGame.h"
#import "SVIGameRedPacket.h"
#import "SVIGameHeartFlutter.h"
#import "SVIVideo.h"
#import "SVIDivision.h"
#import "SVThreadPool.h"
#import "SVThreadMain.h"
#import "SVThreadHelp.h"
#import "SVFileMgr.h"
#import "SVContextIOS.h"
#import "SVOpRender.h"
#import "SVOpCreate.h"
#import "SVBasicSys.h"
#import "SVOpEvent.h"
#import "SVEventMgr.h"
#if TARGET_OS_IPHONE
#import <UIKit/UIApplication.h>
@interface SVI(){
    int             glesVersion;
    NSString*       m_scenename;
    int             m_designWidth;
    int             m_designHeight;
    
    
    NSMutableArray *m_bundleIDArray;
    NSString       *m_bundleID;
}
@end

void callback_op(const char* info){
    NSLog(@"callback_op %s \n",info);
}

@implementation SVI

- (instancetype)init{
    self = [super init];
    if (self) {
        //包名检查
        if (![self checkBundleID]) {
            //检查包名不通过
            return self;
        }
        
        m_pApp = new SVInst();
        
        self.pCamera = [[SVICamera alloc] init];
        [self.pCamera setSVE: m_pApp];
        
        self.pDetect = [[SVIDetect alloc] init];
        [self.pDetect setSVE: m_pApp];
        
        self.pEffect = [[SVIEffect alloc] init];
        [self.pEffect setSVE: m_pApp];
        
        self.pGame = [[SVIGame alloc] init];
        [self.pGame setSVE: m_pApp];
        
        self.pGameRedPacket = [[SVIGameRedPacket alloc]init];
        [self.pGameRedPacket setSVE:m_pApp];
        
        self.pGameHeartFlutter = [[SVIGameHeartFlutter alloc]init];
        [self.pGameHeartFlutter setSVE:m_pApp];
        
        self.pVideo = [[SVIVideo alloc] init];
        [self.pVideo setSVE: m_pApp];

        self.pDivison = [[SVIDivision alloc] init];
        [self.pDivison setSVE:m_pApp];
        
        SVInst* t_app = (SVInst*)m_pApp;
        if(t_app){
            t_app->init();
        }
    }
    return self;
}

//对象析构
-(void)dealloc{
    //
    SVInst* t_app = (SVInst*)m_pApp;
    if(t_app){
        t_app->destroy();
        delete t_app;
        m_pApp = nullptr;
    }
    //
    [self.pCamera setSVE: nullptr];
    [self.pDetect setSVE: nullptr];
    [self.pEffect setSVE: nullptr];
    [self.pGame setSVE: nullptr];
    [self.pGameRedPacket setSVE:nullptr];
    [self.pGameHeartFlutter setSVE:nullptr];
    [self.pDivison setSVE:nullptr];
    [self.pVideo setSVE: nullptr];
    self.pCamera = nil;
    self.pDetect = nil;
    self.pEffect = nil;
    self.pGame = nil;
    self.pGameRedPacket = nullptr;
    self.pGameHeartFlutter = nullptr;
    self.pVideo = nil;
    self.pDivison = nil;
}

//设置资源搜索路径
- (void)addResPath:(NSString*)_path{
    if (m_pApp) {
        SVInst* t_app = (SVInst*)m_pApp;
        if(t_app){
            t_app->getFileMgr()->addRespath([_path UTF8String]);
        }
    }
}


//开启引擎
- (void)startEngine{
    if (m_pApp) {
        SVInst* t_app = (SVInst*)m_pApp;
        if(t_app){
            t_app->startSVE();
        }
    }
}

//关闭引擎
- (void)stopEngine{
    if (m_pApp) {
        SVInst* t_app = (SVInst*)m_pApp;
        if(t_app){
            t_app->stopSVE();
        }
    }

}
//挂起
-(void)suspend{
    if (m_pApp) {
        SVInst* t_app = (SVInst*)m_pApp;
        if(t_app){
            t_app->svSuspend();
        }
    }

}

- (void)delaySuspend:(float)_s OP:(cb_func_op)_cb{
    if (m_pApp) {
        SVInst* t_app = (SVInst*)m_pApp;
        if(t_app){
            if (_s <= 0 ) {
                if (_cb) {
                    (*_cb)("sv_suspend");
                }
                [self suspend];
            }else {
                SVOpOpenDelaySuspendPtr t_op = MakeSharedPtr<SVOpOpenDelaySuspend>(t_app, _s);
                t_op->setCallBack(_cb, "");
                t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
                t_app->svWillSuspend();
            }
        }
    }
}

- (void)willSuspend{
    if (m_pApp) {
        SVInst* t_app = (SVInst*)m_pApp;
        if(t_app){
            
        }
    }
}

//唤醒
-(void)resume{
    if (m_pApp) {
        SVInst* t_app = (SVInst*)m_pApp;
        if(t_app){
            t_app->svResume();
        }
    }

}

- (void)setEngineTimeState:(int)_timeState{
    if(_timeState < 0 || _timeState > 2) return;
    if (m_pApp) {
        SVInst* t_app = (SVInst*)m_pApp;
        if(t_app){
            t_app->setTimeState((SV_ENG_TIMESTATE)_timeState);
        }
    }
}

//初始化GL渲染器
- (void)createRendererGL:(int)glVer Context:(EAGLContext*)_context Width:(int)_w Height:(int)_h{
    if (m_pApp) {
        SVInst* t_app = (SVInst*)m_pApp;
        SVOpCreateRenderderPtr t_op = MakeSharedPtr<SVOpCreateRenderder>(t_app);
        t_op->setGLParam(glVer,(__bridge_retained void *)_context,_w,_h);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }

}

//销毁渲染器
- (void)destroyRendererGL {
    if (m_pApp) {
        SVInst* t_app = (SVInst*)m_pApp;
        SVOpDestroyRenderderPtr t_op = MakeSharedPtr<SVOpDestroyRenderder>(t_app);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }
}

//创建渲染环境
-(void)createRenderTarget:(int)_fboid Color:(int)_colorid Width:(int)_w Height:(int)_h Mirror:(bool)_mirror{
    if (m_pApp) {
        SVInst* t_app = (SVInst*)m_pApp;
        SVOpSetRenderTargetPtr t_op = MakeSharedPtr<SVOpSetRenderTarget>(t_app);
        t_op->setTargetParam(_w,_h,_fboid,_colorid, _mirror);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }
}

//销毁渲染环境
-(void)destroyRenderTarget{
    if (m_pApp) {
        SVInst* t_app = (SVInst*)m_pApp;
        SVOpDestroyRenderTargetPtr t_op = MakeSharedPtr<SVOpDestroyRenderTarget>(t_app);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }
}

- (void)setMirror:(bool)_mirror{
    if (m_pApp) {
        SVInst* t_app = (SVInst*)m_pApp;
        SVOpSetRenderMirrorPtr t_op = MakeSharedPtr<SVOpSetRenderMirror>(t_app);
        t_op->setTargetParam(_mirror);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }
}

//创建场景
- (void)createScene:(cb_func_op)_cb msg:(NSString*)_msg{
    if (m_pApp) {
        SVInst* t_app = (SVInst*)m_pApp;
        SVOpCreateScenePtr t_op = MakeSharedPtr<SVOpCreateScene>(t_app,"showScene");
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
        //测试包
//        SVOpCreateTestPtr t_op_test = MakeSharedPtr<SVOpCreateTest>(t_app);
//        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op_test);
//        NSString *path = [[NSBundle mainBundle] pathForResource:@"sve" ofType:@"bundle"];
//        path = [path stringByAppendingPathComponent:@"svres"];
//        SVOpCreateTestSpinePtr t_op_test = MakeSharedPtr<SVOpCreateTestSpine>(t_app, [path UTF8String]);
//        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op_test);
    }
}

//销毁场景
-(void)destroyScene{
    if (m_pApp) {
        SVInst* t_app = (SVInst*)m_pApp;
        SVOpDestroyScenePtr t_op = MakeSharedPtr<SVOpDestroyScene>(t_app,"showScene");
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }
}

//推送简单的点击数据
-(void)pushTouchX:(float)_x Y:(float)_y State:(int)_state{
    if (m_pApp) {
        if (_state == 1) {
            SVInst* t_app = (SVInst*)m_pApp;
            SVTouchEventPtr t_touch = MakeSharedPtr<SVTouchEvent>();
            t_touch->eventType = EVN_T_TOUCH_BEGIN;
            t_touch->x = _x;
            t_touch->y = _y;
            t_app->getEventMgr()->pushEvent(t_touch);
        }
        if (_state == 2) {
            SVInst* t_app = (SVInst*)m_pApp;
            SVTouchEventPtr t_touch = MakeSharedPtr<SVTouchEvent>();
            t_touch->eventType = EVN_T_TOUCH_MOVE;
            t_touch->x = _x;
            t_touch->y = _y;
            t_app->getEventMgr()->pushEvent(t_touch);
        }
        if (_state == 3) {
            SVInst* t_app = (SVInst*)m_pApp;
            SVTouchEventPtr t_touch = MakeSharedPtr<SVTouchEvent>();
            t_touch->eventType = EVN_T_TOUCH_END;
            t_touch->x = _x;
            t_touch->y = _y;
            t_app->getEventMgr()->pushEvent(t_touch);
        }
    }
}

//切换工作模式
-(void)setWorkMode:(int)_mode OP:(cb_func_op)_cb msg:(NSString*)_msg{
    
}
//
- (void)setFPS:(int)_fps{
    if (_fps >= 0) {
        if (m_pApp) {
            SVInst* t_app = (SVInst*)m_pApp;
            t_app->m_pTPool->getMainThread()->setFPS(_fps);
            t_app->m_pTPool->getHelpThread()->setFPS(_fps);
        }
    }
}
//
- (EAGLContext *)getContext{
    return nullptr;
}

- (BOOL)checkBundleID{
    m_bundleID = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleIdentifier"];
    m_bundleIDArray = [[NSMutableArray alloc] initWithObjects:@"one.SHOW.Live", @"test.showtools.SVEShow",@"test.showtools.SVEVideo", nil];
    BOOL t_result = NO;
    for (NSInteger i = 0; i<m_bundleIDArray.count; i++) {
        NSString *t_bundleID = m_bundleIDArray[i];
        if ([[t_bundleID lowercaseString] isEqualToString:[m_bundleID lowercaseString]]) {
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
