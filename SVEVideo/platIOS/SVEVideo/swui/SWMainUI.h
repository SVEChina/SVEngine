//
//  SWMainUI.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "SWBaseUI.h"

#import "SWFilterTblUI.h"
#import "SWFunctionTblUI.h"
#import "SWFilterSlider.h"
#import "SWFilterRGBW.h"
#import "SWFilterColorBalanceSlider.h"
//主UI

@interface SWMainUI : SWBaseUI

- (void)refreshUI;

- (void)refreshFilterBar;

- (void)refreshFunctionBar;

- (void)changeToShow;

- (void)hideFrame;

- (void)showFrame;

- (void)openMoudle:(NSString*)_moudlename;

- (void)closeMoudle:(NSString*)_moudlename AtOnce:(bool)_flag;

- (void)openFunction:(NSString*)_functionname;

- (void)closeFunction:(NSString*)_functionname AtOnce:(bool)_flag;

- (void)setSliderValue:(float)_value;

//当前模块(bottom tools)
@property (copy, nonatomic) NSString* m_curMoudle;
//当前功能
@property (copy, nonatomic) NSString* m_curFunction;
//滤镜列表
@property (strong, nonatomic) SWFilterTblUI* pFilterView;
//功能列表
@property (strong, nonatomic) SWFunctionTblUI* pFunctionView;
//滑动条
@property (strong, nonatomic) SWFilterSlider* pFilterSlider;
//色彩平衡页面
@property (strong, nonatomic) SWFilterColorBalanceSlider* pFilterColorBalance;
//RGB界面
@property (strong, nonatomic) SWFilterRGBW* pFilterRGBW;

@property(strong, nonatomic)UIImageView *m_imageView;

@end
