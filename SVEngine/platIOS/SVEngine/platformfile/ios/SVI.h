//
//  SV.h
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVInterfaceBase.h"

#if TARGET_OS_IPHONE

#import <GLKit/GLKit.h>
#import <Foundation/Foundation.h>

@class SVGLView;

@class SVICamera;

@class SVIDetect;

@class SVIEffect;

@class SVIGame;

@class SVIGameRedPacket;

@class SVIGameHeartFlutter;

@class SVIVideo;

@class SVIDivision;

@interface SVI : SVInterfaceBase
//
@property(nonatomic, strong) SVICamera *pCamera;
@property(nonatomic, strong) SVIDetect *pDetect;
@property(nonatomic, strong) SVIEffect *pEffect;
@property(nonatomic, strong) SVIGame *pGame;
@property(nonatomic, strong) SVIGameRedPacket *pGameRedPacket;
@property(nonatomic, strong) SVIGameHeartFlutter *pGameHeartFlutter;
@property(nonatomic, strong) SVIVideo *pVideo;
@property(nonatomic, strong) SVIDivision *pDivison;
//设置资源路径
- (void)addResPath:(NSString*)_path;
//开启引擎
- (void)startEngine;
//关闭引擎
- (void)stopEngine;
//设置引擎时间状态
- (void)setEngineTimeState:(int)_timeState;
//引擎挂起
-(void)suspend;
//引擎唤醒
-(void)resume;
//引擎延时挂起
- (void)delaySuspend:(float)_s OP:(cb_func_op)_cb;
//创建渲染器(GL,DX,Vulkan,Metal) 和 size
- (void)createRendererGL:(int)glVer Context:(EAGLContext*)_context Width:(int)_w Height:(int)_h;
//销毁渲染器
- (void)destroyRendererGL;
//是否非镜像渲染
- (void)setMirror:(bool)_mirror;
//创建渲染环境
-(void)createRenderTarget:(int)_fboid Color:(int)_colorid Width:(int)_w Height:(int)_h Mirror:(bool)_mirror;
//销毁渲染环境
-(void)destroyRenderTarget;
//创建场景
- (void)createScene:(cb_func_op)_cb msg:(NSString*)_msg;
//销毁场景
-(void)destroyScene;
//切换工作模式
-(void)setWorkMode:(int)_mode OP:(cb_func_op)_cb msg:(NSString*)_msg;
//设置渲染帧率
- (void)setFPS:(int)_fps;
//获取设备上下文
- (EAGLContext *)getContext;
//推送简单的点击数据
-(void)pushTouchX:(float)_x Y:(float)_y State:(int)_state;


@end

#endif
