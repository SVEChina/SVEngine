//
//  SWLogicFunction.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>

@class SWFunction;

@interface SWLogicFunction : NSObject

//初始化模块
- (void)initModule;

//销毁模块
- (void)destroyModule;

//获取功能库
- (NSMutableArray *)getFunctionLib;

//获取选中的功能库
- (NSMutableArray *)getFunctionLibSel;

//开启功能
-(void)enableFunction:(NSString*)_name;

//关闭功能
-(void)disableFunction:(NSString*)_name;


@end
