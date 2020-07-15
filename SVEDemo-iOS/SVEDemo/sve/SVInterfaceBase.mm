//
//  SVInterface.m
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVInterfaceBase.h"
#import "SVIThread.h"
#include "app/SVInst.h"

#if TARGET_OS_IPHONE

@interface SVInterfaceBase()
{
}
@end

//
@implementation SVInterfaceBase

- (instancetype)initWithSVE:(void *)_sve Thread:(SVIThread *)_thread{
    self = [super init];
    if (self) {
        m_pApp = _sve;
        m_thread = _thread;
    }
    return self;
}

- (void)dealloc{
    m_pApp = NULL;
    m_thread = nil;
}

-(void*)getSVE{
    return m_pApp;
}
@end

#endif
