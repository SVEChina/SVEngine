//
//  SWFrameUI.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWBaseUI.h"

//框架UI

@interface SWFrameUI :SWBaseUI

- (void)appear:(int)Dir;

- (void)disappear:(int)Dir AtOnce:(bool)Flag;

@end
