//
//  SWUISys.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SWNavigationController.h"
#import "SWMainVC.h"
#import "SWUserUI.h"


@interface SWUISys : NSObject

+ (instancetype) getInst;

- (void)initSys:(CGRect)_rc;

- (void)destroySys;

@property (strong, nonatomic) SWNavigationController* pNavi;
@property (strong, nonatomic) SWMainVC *pMainVC;
@property (strong, nonatomic) SWUserUI *pUserView;

@end
