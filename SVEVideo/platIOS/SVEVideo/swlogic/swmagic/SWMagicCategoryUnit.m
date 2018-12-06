//
//  SWMagicCategoryUnit.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWMagicCategoryUnit.h"
#import "SWMagicUnit.h"
#import "SWLogicSys.h"
@implementation SWMagicCategoryUnit
- (instancetype)init{
    self = [super init];
    if (self) {
        _categoryID = @"";
        _number = @"";
        _name = @"";
        _spines = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)dealloc{
    NSLog(@"destory SWMagicCategoryUnit");
}

- (void)refreshData:(NSDictionary *)dic{
    if (dic) {
    }
}

- (NSDictionary *)convertUnitToDictionary{
    NSMutableArray *spines = [[NSMutableArray alloc] init];
    for (SWMagicUnit *magicUnit in _spines) {
        NSDictionary *spineDic = [magicUnit convertUnitToDictionary];
        [spines addObject:spineDic];
    }
    NSDictionary *spineDic = @{@"id":_categoryID,
                               @"number":_number,
                               @"name":_name,
                               @"spines":spines
                               };
    return spineDic;
}

@end
