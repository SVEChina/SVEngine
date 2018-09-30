//
//  SVEffect.h
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVInterfaceBase.h"

#if TARGET_OS_IPHONE

#include "VideoToolBox/VideoToolbox.h"

@interface SVIEffect : SVInterfaceBase


//设置美颜滤镜  level 美颜级别  0 高性能设备用  1较低性能设备用
-(void)setBeautyFilter:(NSString*)_filter level:(int)_level OP:(cb_func_op)_cb msg:(NSString*)_msg;
//关闭美颜滤镜
-(void)closeBeautyFilterOP:(cb_func_op)_cb msg:(NSString*)_msg;
//
-(void)updateFilterOP:(cb_func_op)_cb msg:(NSString*)_msg smooth:(float)_smooth filtertype:(SVIEFILTERTYPE) _type;

-(void)updateFaceShapeOP:(cb_func_op)_cb msg:(NSString*)_msg face:(float) _face eye:(float) _eye;
//
-(void)updateFilterBSplineOP:(cb_func_op)_cb msg:(NSString*)_msg dataTex:(unsigned char*)_data;

- (void)loadEffectPath:(NSString *)_effect OP:(cb_func_op)_cb msg:(NSString*)_msg;

- (void)removeEffectOP:(cb_func_op)_cb msg:(NSString *)_msg;

- (NSArray *)getAnimations;

- (void)playAnimation:(NSString *)_ani;
@end

#endif
