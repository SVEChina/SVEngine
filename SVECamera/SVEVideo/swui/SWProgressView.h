//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

@interface SWProgressView : UIView
@property (weak, nonatomic) id delegate;
@property (strong, nonatomic) UIColor *progressBarColor;
@property (strong, nonatomic) UIColor *wrapperColor;
@property (assign, nonatomic) CGFloat progressBarShadowOpacity;
@property (assign, nonatomic) CGFloat progressBarArcWidth;
@property (assign, nonatomic) CGFloat wrapperArcWidth;
@property (assign, nonatomic) CFTimeInterval duration;
@property (assign, nonatomic) CGFloat currentProgress;

- (void)run:(CGFloat)progress;
- (void)pause;
- (void)reset;
@end
