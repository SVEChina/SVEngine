//
//  SWLogicFilter.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>

@class SWFilter;

@interface SWLogicFilter : NSObject
- (void)initModule;

- (void)destroyModule;

- (NSMutableArray *)getFilterList;

@end
