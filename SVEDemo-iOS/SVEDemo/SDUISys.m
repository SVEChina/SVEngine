//
//  SDUISys.m
//  SVEDemo
//
//  Created by 李晓帆 on 2018/9/3.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import "SDUISys.h"

static SDUISys *mInst;

@implementation SDUISys

+(instancetype) getInst{
    if(mInst == nil){
        mInst = [SDUISys new];
    }
    return mInst;
}

- (void)initSys{
    //主Ctrl
    self.pMainVC = [[SDMainVC alloc] init];
}

- (void)destroySys{
}
@end
