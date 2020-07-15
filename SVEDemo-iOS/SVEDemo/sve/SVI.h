//
//  SV.h
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVInterfaceBase.h"

#if TARGET_OS_IPHONE

#import <Metal/MTLDevice.h>
#import <GLKit/GLKit.h>
#import <Foundation/Foundation.h>

@class SVICamera;

@class SVIEffect;

@interface SVI : SVInterfaceBase
typedef enum {
    SV_AUTO_MODE = 0,
    SV_MANUAL_MODE
}SVSYNCMODE;

//
@property(nonatomic, strong) SVICamera *pCamera;
@property(nonatomic, strong) SVIEffect *pEffect;
/**
 创建引擎模式

 @param _mode SV_AUTO_MODE自动模式 SV_MANUAL_MODE手动模式
 @param _tag 引擎的tag
 */
- (instancetype)initMode:(SVSYNCMODE)_mode Tag:(NSString *)_tag;
//返回引擎实例tag
- (NSString *)getTag;
//设置资源路径
- (void)addResPath:(NSString*)_path;
//开启引擎
- (void)startEngine;
//关闭引擎
- (void)stopEngine;
//设置引擎时间状态
- (void)setEngineTimeState:(int)_timeState;
/**
 引擎更新,在手动模式下需要调用.
 */
- (void)update;
//引擎挂起
-(void)suspend;
//引擎唤醒
-(void)resume;
//创建渲染器(GL,DX,Vulkan,Metal) 和 size
- (void)createRendererGL:(int)glVer Context:(EAGLContext*)_context Width:(int)_w Height:(int)_h;
//销毁渲染器
- (void)destroyRendererGL;
//创建Metal渲染器
- (void)createRendererMetal:(void*)_device Width:(int)_w Height:(int)_h;
//销毁渲染器
- (void)destroyRendererMetal;
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
/**
 获取引擎同步模式

 @return SVSYNCMODE
 */
- (SVSYNCMODE)getSyncMode;

//推送简单的点击数据
- (void)pushTouchX:(float)_x Y:(float)_y WorldPtX:(float *)_worldX WorldPtY:(float *)_worldY WorldPtZ:(float *)_worldZ State:(int)_state;

- (void)pushTouchWorldX:(float)_x Y:(float)_y Z:(float)_z State:(int)_state;

- (void)pushCameraMatrix:(void *)_matrixData len:(int)_len;

- (void)pushProjectMatrix:(void *)_matrixData len:(int)_len;

- (void)pushARAnchorModelMatrix:(void *)_matrixData;

- (void)pushARAnchorProjPosX:(float)_x PosY:(float)_y;
@end

#endif
