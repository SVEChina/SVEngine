//
//  SDLogicSys.m
//  SVEDemo
//
//  Created by 李晓帆 on 2018/9/3.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import "SDLogicSys.h"

@interface SDLogicSys(){
    SVInst *m_pInst;
    EAGLContext* m_pGLContext;
}
@end

static SDLogicSys *mInst;

@implementation SDLogicSys

+(instancetype) getInst{
    if(mInst == nil){
        mInst = [SDLogicSys new];
    }
    return mInst;
}

- (void)initSys{
    m_pInst = new SVInst();
    m_pInst->init();
    //创建设备上下文
    m_pGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    //注册监听消息（退到后台和重返前台）
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didEnterBackground) name:@"didEnterBackground" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didEnterForeground) name:@"didEnterForeground" object:nil];
    //初始化引擎
    [self initSVE];
    //创建ViewControl
    self.m_pVC = [[ViewController alloc] init];
}

- (void)destroySys{
    //开启相机
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"didEnterBackground" object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"didEnterForeground" object:nil];
    //停止引擎并析构
    [self destroySVE];
}

-(SVInst*)getSVE {
    return m_pInst;
}

- (void)initSVE {
    if(m_pInst) {
        //设置资源路径
        NSString *bundlePath = [[NSBundle mainBundle] pathForResource:@"sve" ofType:@"bundle"];
        bundlePath = [NSString stringWithFormat:@"%@/",bundlePath];
        //
        m_pInst->addRespath([bundlePath UTF8String]);
        m_pInst->addRespath("");
        //开启引擎
        m_pInst->startSVE();
        //创建渲染器
        //[m_pInst-> createRendererGL:3 Context:m_pGLContext Width:720 Height:1280];
        //创建场景
        //[m_pInst-> createScene:NULL msg:@""];
    }
}

- (void)destroySVE {
    //停止引擎并析构
    if( m_pInst ) {
        m_pInst->stopSVE();
        m_pInst->destroy();
        m_pInst = nil;
    }
}

-(EAGLContext*)getGLContext{
    return m_pGLContext;
}

- (void)didEnterBackground{
    //如果有预览视频正在播放，那么暂停。
    if(m_pInst) {
        m_pInst->svSuspend();
    }
}

- (void)didEnterForeground{
    //如果有预览视频正在播放，那么继续播放。
    if(m_pInst) {
        m_pInst->svResume();
    }
}

@end
