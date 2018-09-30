//
//  SWLogicAni.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>

//动画 启动定时器 做各种动画用

@interface SWLogicAni : NSObject

- (void)initModule;

- (void)destroyModule;

-(void)setMainUiState:(float)_value;

@end
