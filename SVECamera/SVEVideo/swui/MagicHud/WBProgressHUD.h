//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <UIKit/UIKit.h>

/**
 视图的动画类型

 - WBProgressHUDTypeSingle: 单线条
 - WBProgressHUDTypeFishHook: 标准八卦形
 - WBProgressHUDTypeGossip: 类八卦形
 */
typedef NS_ENUM(NSInteger, WBProgressHUDType) {
    
    WBProgressHUDTypeSingle,
    
    WBProgressHUDTypeFishHook,
    
    WBProgressHUDTypeGossip
};

@interface WBProgressHUD : UIView

/**
  线条颜色，默认为 [UIColor lightGrayColor]
 */
@property (nonatomic, strong) UIColor *lineColor;

/**
 动画内圈运动的时长，默认为1s
 */
@property CFTimeInterval inDuration;

/**
 动画外圈运动的时长，默认为1.5s
 */
@property CFTimeInterval outDuration;

/**
  初始化HUD

 @param frame 视图frame，默认中心为屏幕中心，大小为 (50, 50)
 @param type 动画的类型
 @param lineColor 线条颜色，默认为 [UIColor whiteColor]
 @return WBProgressHUD
 */
-(instancetype) initWithFrame:(CGRect)frame type:(WBProgressHUDType)type lineColor:(UIColor *)lineColor;

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

 @param view 需要添加的view， 默认为视图中心点
 @param type 动画的类型， 默认为 WBProgressHUDTypeSingle
 @param animated 是否启动动画
 @return WBLoadingViewHUD
 */
+(instancetype) showToView:(UIView *)view type:(WBProgressHUDType)type animated:(BOOL)animated;

@end
