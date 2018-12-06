//
//  SWMagicsUI.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SWMagicsEffectsUI.h"
#import "SWMagicsFiltersUI.h"
#import "SWFilterSlider.h"
#import "SWBaseUI.h"
//帮助UI

@interface SWMagicsUI : SWBaseUI
@property(strong, nonatomic)SWMagicsEffectsUI *magicsEffectUI;
@property(strong, nonatomic)SWMagicsFiltersUI *magicsFilterUI;
@property(strong, nonatomic)SWFilterSlider  *magicsBeautyUI;
@property(copy, nonatomic)NSString            *whereFrom;//从哪个UI页面跳转过来的
- (void)changeToShow;
- (void)eventEffects:(UIButton *)btn;
@end
