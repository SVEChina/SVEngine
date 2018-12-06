//
//  SWFilterSlider.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWBaseUI.h"

//滤镜滑条

@interface SWFilterSlider : SWBaseUI

@property (assign, nonatomic) BOOL showed;

@property (assign, nonatomic) int Mode;

- (float)getSliderValue;

- (void)setSliderMode:(int)_mode;

- (void)setSliderValue:(float)value;

@end
