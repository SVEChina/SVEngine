//
//  SWFunctionTblUI.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SWBaseUI.h"
#import "../swlogic/SWFunction.h"

@interface SWFunctionTblUI : SWBaseUI

- (void)refreshData;

- (void)changeToShow;

- (void)disappear:(int)Dir AtOnce:(bool)Flag;

@end
