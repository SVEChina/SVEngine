//
//  SWLogicLogon.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>
@protocol SWLogicLogonDelegate<NSObject>
@required
- (void)logonSuccess;
- (void)logonFailed;
@end
@interface SWLogicLogon : NSObject
@property (weak, nonatomic)id<SWLogicLogonDelegate>delegate;
- (void)logonWB;
- (void)logonWX;
- (void)logonQQ;
@end
