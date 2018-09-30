//
//  SWTimer.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWTimer.h"

@interface SWTimer(){
    int mSecondsCountDown;
    NSTimer *mCountDownTimer;
}
@end

@implementation SWTimer
- (instancetype)init{
    self = [super init];
    if (self) {
        mSecondsCountDown = 15;
    }
    return self;
}

- (void)dealloc{
    [mCountDownTimer invalidate];
    mCountDownTimer = nil;
}

- (void)setTimeOut:(int)time{
    if (mSecondsCountDown != time) {
        mSecondsCountDown = time;
    }
}

- (int)getTimeOut{
    return mSecondsCountDown;
}

-(void)timeFireMethod{
    mSecondsCountDown--;
    dispatch_async(dispatch_get_main_queue(), ^{
        //设置界面的按钮显示 根据自己需求设置
        if ([self.delegate respondsToSelector:@selector(timerUpdate:)]) {
            [self.delegate timerUpdate:mSecondsCountDown];
        }
        
    });
    if(mSecondsCountDown==0){
        [mCountDownTimer invalidate];
        mCountDownTimer = nil;
        dispatch_async(dispatch_get_main_queue(), ^{
            //设置界面的按钮显示 根据自己需求设置
            if ([self.delegate respondsToSelector:@selector(timerEnd)]) {
                [self.delegate timerEnd];
            }
        });
    }
}

- (void)play{
    if (mCountDownTimer != nil) {
        return;
    }
    mCountDownTimer = [NSTimer scheduledTimerWithTimeInterval:1 target:self selector:@selector(timeFireMethod) userInfo:nil repeats:YES];
}

@end
