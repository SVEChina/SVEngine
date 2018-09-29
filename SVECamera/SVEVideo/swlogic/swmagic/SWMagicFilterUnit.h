//
//  SWMagicFilterUnit.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SWMagicFilterUnit : NSObject
@property (copy, nonatomic)NSString *filterLogo;
@property (copy, nonatomic)NSString *filterName;
@property (copy, nonatomic)NSString *filterPackage;
- (void)refreshUnit:(NSDictionary *)dic;
@end
