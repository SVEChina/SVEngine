//
//  SWMagicCategoryUnit.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SWMagicCategoryUnit : NSObject
@property (copy, nonatomic)NSString *categoryID;
@property (copy, nonatomic)NSString *number;
@property (copy, nonatomic)NSString *name;
@property (strong, nonatomic)NSMutableArray *spines;
- (void)refreshData:(NSDictionary *)dic;
- (NSDictionary *)convertUnitToDictionary;
@end
