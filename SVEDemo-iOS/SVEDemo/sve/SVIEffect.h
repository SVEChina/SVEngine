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

- (void)loadEffectPath:(NSString *)_effect OP:(cb_func_op)_cb msg:(NSString*)_msg;

- (void)removeEffectOP:(cb_func_op)_cb msg:(NSString *)_msg;

- (NSArray *)getAnimations;

- (void)playAnimation:(NSString *)_ani;
@end

#endif
