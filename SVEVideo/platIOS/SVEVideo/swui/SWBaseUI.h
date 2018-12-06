//
//  SWBaseUI.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <UIKit/UIKit.h>

//基础UI

@interface SWBaseUI : UIView

- (void)destroy;

- (void)changeToShow;

- (void)refreshData;

- (void)appear:(int)Dir;

- (void)disappear:(int)Dir AtOnce:(bool)Flag;

@property (assign, nonatomic) bool m_uihide;

@end
