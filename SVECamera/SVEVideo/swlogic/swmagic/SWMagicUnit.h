//
//  SWMagicUnit.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SWMagicUnit : NSObject
@property (copy, nonatomic)NSString *categoryID;
@property (copy, nonatomic)NSString *ctime;
@property (copy, nonatomic)NSString *cutime;
@property (copy, nonatomic)NSString *magicEffectID;
@property (copy, nonatomic)NSString *mark;
@property (copy, nonatomic)NSString *downloadNum;
@property (copy ,nonatomic)NSString *ownerUid;
@property (copy, nonatomic)NSString *score;
@property (copy, nonatomic)NSString *status;
@property (copy, nonatomic)NSString *sutime;
@property (copy, nonatomic)NSString *categoryName;
@property (copy, nonatomic)NSString *cover;
@property (copy, nonatomic)NSString *name;
@property (copy, nonatomic)NSString *source;
@property (copy, nonatomic)NSString *tags;
@property (copy, nonatomic)NSString *isDownload;
@property (copy, nonatomic)NSString *price;
- (void)refreshData:(NSDictionary *)dic;
- (NSDictionary *)convertUnitToDictionary;
@end
