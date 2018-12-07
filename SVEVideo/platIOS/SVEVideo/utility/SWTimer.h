//
//  SWTimer.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>
@protocol FMTimeDelegate<NSObject>
@optional
- (void)timerEnd;
- (void)timerUpdate:(int)time;
@end
@interface SWTimer : NSObject
@property(weak, nonatomic)id<FMTimeDelegate>delegate;
- (void)setTimeOut:(int)time;
- (int)getTimeOut;
- (void)play;
@end
