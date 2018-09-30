//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <UIKit/UIKit.h>

/**
 加载中的动画，默认为视图中心，大小为(100, 100)
 */
@interface WBLoadingHUD : UIView

/**
 线条颜色，默认为 [UIColor lightGrayColor]
 */
@property(nonatomic, strong) UIColor *lineColor;

/**
 动画内圈运动的时长，默认为1s
 */
@property CFTimeInterval inDuration;

/**
 动画中圈运动的时长，默认为1.25s
 */
@property CFTimeInterval centerDuration;

/**
 动画外圈运动的时长，默认为1.5s
 */
@property CFTimeInterval outDuration;

/**
 开始动画
 */
-(void) start;

/**
 只停止动画
 */
-(void) stop;

/**
 停止动画并隐藏

 @param animated 是否开启动画隐藏
 */
-(void) hideAnimated:(BOOL)animated;

/**
 实例化添加到视图上

 @param view 需要添加的view, 默认为视图中心点
 @param animated 是否启动动画
 @return WBLoadingViewHUD
 */
+(instancetype) showToView:(UIView *)view animated:(BOOL)animated;

@end
