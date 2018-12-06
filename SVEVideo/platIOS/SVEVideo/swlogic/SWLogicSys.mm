//
//  SWLogicSys.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWLogicSys.h"
#import "../swbasic/SWBasicSys.h"
#import "SSZipArchive.h"
#import "../datasrc/SWDataSourceARCamera.h"
#import "../datasrc/SWDataSourceCamera.h"
#import "src/app/SVInst.h"
#import "src/work/SVThreadPool.h"
#import "src/work/SVThreadMain.h"
#import "src/work/SVThreadHelp.h"
#import "src/file/SVFileMgr.h"
#import "src/operate/SVOpRender.h"
#import "src/operate/SVOpCreate.h"
#import "src/basesys/SVBasicSys.h"
#import "src/basesys/SVStreamIn.h"
#import "src/basesys/SVStreamOut.h"
#import "src/operate/SVOpOutCamera.h"
#import "src/operate/SVOpFilter.h"
static SWLogicSys *mInst;

@interface SWLogicSys (){
    SVInst *m_pApp;
    EAGLContext* m_pGLContext;
}

@end

@implementation SWLogicSys

void effect_op_callback(const char* _info){
    NSString *msg = [NSString stringWithUTF8String:_info];
}

+(instancetype) getInst{
    if(mInst == nil){
        mInst = [SWLogicSys new];
    }
    return mInst;
}

- (void)initSys{
    m_pApp = nullptr;
    //创建设备上下文
    m_pGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    //
    self.pSWState = [[SWState alloc] init];
    //注册监听消息
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didEnterBackground) name:@"didEnterBackground" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didEnterForeground) name:@"didEnterForeground" object:nil];
    //平台系统
    self.pLogicPlatform = [SWLogicPlatform new];
    [self.pLogicPlatform initModule];
    //用户系统
    self.pLogicUser = [SWLogicUser new];
    [self.pLogicUser initModule];
    //ANI
    self.pLogicAni = [SWLogicAni new];
    [self.pLogicAni initModule];
    //滤镜
    self.pLogicFilter = [SWLogicFilter new];
    [self.pLogicFilter initModule];
    //功能
    self.pLogicFunction = [SWLogicFunction new];
    [self.pLogicFunction initModule];
    
    //
    [self initSVE];
    //
    [[SWBasicSys getInst].m_outStream openStream];
    
    [self initAirdropFilePath];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(addEffect:) name:@"effectPath" object:nil];
}

- (void)initAirdropFilePath{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    NSString *path = [paths lastObject];
    self.m_airdropFilePath = [path stringByAppendingPathComponent:@"airdropeffect"];
    if (![[NSFileManager defaultManager] fileExistsAtPath:self.m_airdropFilePath]) {
        [[NSFileManager defaultManager] createDirectoryAtPath:self.m_airdropFilePath withIntermediateDirectories:YES attributes:nil error:nil];
    }
}

- (void)dealloc{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)destroySys{
    //开启相机
    [[SWBasicSys getInst].m_pDataSrc stop];
    //
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"didEnterBackground" object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"didEnterForeground" object:nil];
    //停止引擎并析构
    [self destroySVE];
    //
    [self.pLogicAni destroyModule];
    self.pLogicAni = nil;
    //
    [self.pLogicUser destroyModule];
    self.pLogicUser = nil;
    //
    [self.pLogicPlatform destroyModule];
    self.pLogicPlatform = nil;
}

- (void)initSVE {
    if(!m_pApp) {
//        //创建sv引擎对象
//        m_pApp = new SVInst();
//        m_pApp->init();
//        //设置资源路径
//        NSString *bundlePath = [[NSBundle mainBundle] pathForResource:@"sve" ofType:@"bundle"];
//        bundlePath = [NSString stringWithFormat:@"%@/",bundlePath];
//        m_pApp->getFileMgr()->addRespath([bundlePath UTF8String]);
//        m_pApp->getFileMgr()->addRespath("");
//        //开启引擎
//        m_pApp->startSVE();
//        //创建渲染器
//        SVOpCreateRenderderPtr t_op_renderder = MakeSharedPtr<SVOpCreateRenderder>(m_pApp);
//        t_op_renderder->setGLParam(3,(__bridge_retained void *)m_pGLContext,self.pSWState.svOutW,self.pSWState.svOutH);
//        m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op_renderder);
//        //创建场景
//        SVOpCreateScenePtr t_op_scene = MakeSharedPtr<SVOpCreateScene>(m_pApp,"showScene");
//        m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op_scene);
//        //开启相机
//        [[SWBasicSys getInst].m_pDataSrc start];
//        if ([[SWBasicSys getInst].m_pDataSrc isKindOfClass:[SWDataSourceCamera class]]) {
//            //创建相机节点
//            SVOpCreateIOSInstreamPtr t_op_iosin = MakeSharedPtr<SVOpCreateIOSInstream>(m_pApp, [SCENENAME UTF8String], 1, self.pSWState.svOutW, self.pSWState.svOutH, 0.0f, true);
//            m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op_iosin);
//            //相机输出
//            SVOpCreateIOSOutstreamPtr t_op_iosout = MakeSharedPtr<SVOpCreateIOSOutstream>(m_pApp,[SCENENAME UTF8String], 1, 0);
//            m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op_iosout);
//
//        }else if ([[SWBasicSys getInst].m_pDataSrc isKindOfClass:[SWDataSourceARCamera class]]){
//            SVOpCreateYUVInstreamPtr t_op_yuv = MakeSharedPtr<SVOpCreateYUVInstream>(m_pApp,[SCENENAME UTF8String], SV_PF_NV12, 1280, 720, 90.0f);
//            m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op_yuv);
//        }
        //美颜
//        SVOpUpdateFilterSmoothPtr t_op_beauty_filter = MakeSharedPtr<SVOpUpdateFilterSmooth>(m_pApp, 50.0f, u32(SVI_EBEAUTY_FILTER));
//        m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op_beauty_filter);
//
//        [self.pSVI.pEffect updateFilterOP:NULL msg:@"" smooth:50.0f filtertype:SVI_EBEAUTY_FILTER];
//
//        [self.pSVI.pEffect updateFilterOP:NULL msg:@"" smooth:20.0f filtertype:SVI_WHITENING_FILTER];
//
//        [self.pSVI.pEffect updateFilterOP:NULL msg:@"" smooth:10.0 filtertype:SVI_ACUTANCE_FILTER];
//        //
//        [self.pSVI.pEffect updateFilterOP:NULL msg:@"" smooth:18.0 filtertype:SVI_CONTRAST_FILTER];
        //
    }
}

- (void *)getEngineInst{
    return m_pApp;
}

- (void)destroySVE {
    //停止引擎并析构
//    if(m_pApp){
//        m_pApp->stopSVE();
//        m_pApp->destroy();
//        delete m_pApp;
//        m_pApp = nullptr;
//    }
}

-(EAGLContext*)getGLContext{
    return m_pGLContext;
}

- (void)addEffect:(NSNotification*)notifi{
//    if (m_pApp) {
//        NSString* path = notifi.object;
//        NSString *copy_path = [self.m_airdropFilePath stringByAppendingPathComponent:[path lastPathComponent]];
//        SVOpCreateEffcetPtr t_op = MakeSharedPtr<SVOpCreateEffcet>(m_pApp,[copy_path UTF8String]);
//        m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//    }
}

- (void)initDetection{
    
}

- (void)didEnterBackground{
    //如果有预览视频正在播放，那么暂停。
//    if(m_pApp){
//        m_pApp->svSuspend();
//    }
}

- (void)didEnterForeground{
    //如果有预览视频正在播放，那么继续播放。
//    if (m_pApp) {
//        m_pApp->svResume();
//    }
}


@end
