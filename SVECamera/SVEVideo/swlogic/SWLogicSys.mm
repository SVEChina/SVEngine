//
//  SWLogicSys.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWLogicSys.h"
#import "../swbasic/SWBasicSys.h"
static SWLogicSys *mInst;

@interface SWLogicSys (){
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
    //
    self.pSVI = nil;
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
}

- (void)destroySys{
    //开启相机
    [[SWBasicSys getInst].m_pDataSrc stop];
    //
    //[[NSNotificationCenter defaultCenter] removeObserver:self name:@"sensorActive" object:nil];
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
    if(!self.pSVI) {
        //创建sv引擎对象
        self.pSVI = [[SVI alloc] init];
        //设置资源路径
        NSString *bundlePath = [[NSBundle mainBundle] pathForResource:@"sve" ofType:@"bundle"];
        bundlePath = [NSString stringWithFormat:@"%@/",bundlePath];
        [self.pSVI addResPath:bundlePath];
        [self.pSVI addResPath:@""];
        //开启引擎
        [self.pSVI startEngine];
        //创建渲染器
        [self.pSVI createRendererGL:3 Context:m_pGLContext Width:self.pSWState.svOutW Height:self.pSWState.svOutH];
        //创建场景
        [self.pSVI createScene:NULL msg:@""];
        //创建相机节点
        [self.pSVI.pCamera createInStream:SCENENAME Type:0 width:self.pSWState.svOutW height:self.pSWState.svOutH OP:NULL msg:@""];
        //
        [self.pSVI.pCamera createOutStream:SCENENAME Type:0 StreamType:0 OP:NULL msg:@""];
        //开启相机
        [[SWBasicSys getInst].m_pDataSrc start];
        //
//        [self.pSVI setFPS:15];
        //
        [self.pSVI.pEffect setBeautyFilter:@"" level:1 OP:nil msg:nil];
        //
        [self.pSVI.pEffect updateFilterOP:NULL msg:@"" smooth:50.0f filtertype:SVI_EBEAUTY_FILTER];
        
        [self.pSVI.pEffect updateFilterOP:NULL msg:@"" smooth:20.0f filtertype:SVI_WHITENING_FILTER];
        
        [self.pSVI.pEffect updateFilterOP:NULL msg:@"" smooth:10.0 filtertype:SVI_ACUTANCE_FILTER];
        //
        [self.pSVI.pEffect updateFilterOP:NULL msg:@"" smooth:18.0 filtertype:SVI_CONTRAST_FILTER];
        
        [self.pSVI.pDivison openDivision];
        NSString *t_path = [[NSBundle mainBundle] pathForResource:@"qiuqian" ofType:@"bundle"];
//        [self.pSVI.pEffect loadEffect:t_path OP:effect_op_callback msg:nil];
    }
}

- (void)destroySVE {
    //停止引擎并析构
    if( self.pSVI ) {
        //析构引擎
        [self.pSVI stopEngine];
        self.pSVI = nil;
    }
}

-(EAGLContext*)getGLContext{
    return m_pGLContext;
}

- (void)initDetection{
    
}

- (void)didEnterBackground{
    //如果有预览视频正在播放，那么暂停。
    [self.pSVI suspend];
}

- (void)didEnterForeground{
    //如果有预览视频正在播放，那么继续播放。
    [self.pSVI resume];
}


@end
