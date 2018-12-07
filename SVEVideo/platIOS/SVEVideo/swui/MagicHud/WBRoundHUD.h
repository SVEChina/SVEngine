//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <UIKit/UIKit.h>

/**
 小球运动的类型
 */
typedef NS_ENUM(NSInteger, WBRoundHUDType) {
    
    WBRoundHUDTypeUniform,    /// 匀速运动
    
    WBRoundHUDTypeGradient   /// 渐变运动
};

@interface WBRoundHUD : UIView

/**
 小球颜色，默认为 [UIColor whiteColor]
 */
@property(nonatomic, strong) UIColor *roundColor;

/**
 动画一圈的时长，默认为1s
 */
@property CFTimeInterval duration;

/**
 初始化HUD
 
 @param frame 视图frame，默认中心为屏幕中心，大小为 (50, 50)
 @param type 动画运动的类型，默认为 WBRoundHUDTypeUniform
 @param roundColor 小球颜色，默认为 [UIColor whiteColor]
 @return WBRoundHUD
 */
-(instancetype) initWithFrame:(CGRect)frame type:(WBRoundHUDType)type roundColor:(UIColor *)roundColor;

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
 @param type 动画的类型 ，默认为 WBRoundHUDTypeUniform
 @param animated 是否启动动画
 @return WBRoundHUD
 */
+(instancetype) showToView:(UIView *)view type:(WBRoundHUDType)type animated:(BOOL)animated;

@end
