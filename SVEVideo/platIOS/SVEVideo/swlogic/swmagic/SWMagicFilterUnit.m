//
//  SWMagicFilterUnit.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWMagicFilterUnit.h"

@implementation SWMagicFilterUnit
- (instancetype)init{
    self = [super init];
    if (self) {
        
    }
    return self;
}

- (void)refreshUnit:(NSDictionary *)dic {
    self.filterLogo = dic[@"filterlogo"];
    self.filterName = dic[@"filtername"];
    self.filterPackage = dic[@"filterpackage"];
}
@end
