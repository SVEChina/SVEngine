//
//  SVInterface.m
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVInterfaceBase.h"

#include "SVInst.h"

#if TARGET_OS_IPHONE

@interface SVInterfaceBase()
{
}
@end

//
@implementation SVInterfaceBase

- (instancetype)init{
    self = [super init];
    if (self) {
    }
    return self;
}

-(void*)getSVE{
    return m_pApp;
}


-(void)setSVE:(void*)_sve{
    m_pApp = _sve;
}

@end

#endif
