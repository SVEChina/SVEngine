//
//  SDState.m
//  SVEDemo
//
//  Created by 李晓帆 on 2018/9/3.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import "SDState.h"

@implementation SDState
- (instancetype)init{
    self = [super init];
    if (self) {
        self.svDesignW = 750;
        self.svDesignH = 1624;
        self.svOutW = 720;
        self.svOutH = 1280;
    }
    return self;
}
@end
