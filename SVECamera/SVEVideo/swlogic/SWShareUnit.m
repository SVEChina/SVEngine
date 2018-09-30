//
//  SWShareUnit.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWShareUnit.h"

//分享单元
@interface SWShareUnit(){
}
@end

@implementation SWShareUnit

- (instancetype) init{
    self = [super init];
    if(self){
        self.m_vioKey = nil;
        self.m_imgKey = nil;
        self.m_vioRes = nil;
        self.m_imgRes = nil;
        self.m_url = nil;
        self.m_score = 0;
    }
    return self;
}

@end


