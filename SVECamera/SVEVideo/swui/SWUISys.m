//
//  SWUISys.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWUISys.h"
#import "../swlogic/SWLogicSys.h"

static SWUISys *mInst;

@interface SWUISys(){
    
}
@end

@implementation SWUISys

+(instancetype) getInst{
    if(mInst == nil){
        mInst = [SWUISys new];
    }
    return mInst;
}

- (void)initSys:(CGRect)_rc{
    //主Ctrl
    self.pMainVC = [[SWMainVC alloc] init];
    //用户Ctrl(要改成ctrl)
    self.pUserView = [[SWUserUI alloc] initWithFrame: _rc];
    [self.pUserView setBackgroundColor:[UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:0.5]];
    [self.pUserView setAlpha:0.9];
}

- (void)destroySys{
}

@end
