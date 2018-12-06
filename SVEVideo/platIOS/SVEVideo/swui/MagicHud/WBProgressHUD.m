//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "WBProgressHUD.h"

#define S_W [[UIScreen mainScreen] bounds].size.width
#define S_H [[UIScreen mainScreen] bounds].size.height

#define LoadingWidth 50   // 默认大小

@interface WBProgressHUD ()
{
    WBProgressHUDType _type;   // 旋转动画的类型
    CAGradientLayer *_inGradientLayer;  // 内圈渐变层
    CAGradientLayer *_outGradientLayer;   // 外圈渐变层
    CAShapeLayer *_inShapeLayer;  // 内圈模板
    CAShapeLayer *_outShapeLayer;  // 外圈模板
}

/**
 线圈的颜色条
 */
@property(nonatomic, strong) CALayer  *inLayer;  // 内圈layer
@property(nonatomic, strong) CALayer  *outLayer;  // 外圈layer

@end

@implementation WBProgressHUD

#pragma makr - Show To View
+(instancetype) showToView:(UIView *)view type:(WBProgressHUDType)type animated:(BOOL)animated {
    if (!type) {
        type = WBProgressHUDTypeSingle;
    }
    CGRect rect = CGRectMake((view.bounds.size.width - LoadingWidth ) / 2, (view.bounds.size.height - LoadingWidth ) / 2, LoadingWidth, LoadingWidth);
    WBProgressHUD *hud = [[WBProgressHUD alloc] initWithFrame:rect type:type lineColor:nil];
    for (UIView *subView in view.subviews) {
        if ([subView isKindOfClass:[WBProgressHUD class]]) {
            if (!animated) {
                [(WBProgressHUD *)subView stop];
            }else{
                [(WBProgressHUD *)subView start];
            }
            return (WBProgressHUD *)subView;
        }
    }
    [view addSubview:hud];
    [view bringSubviewToFront:hud];
    if (!animated) {
        [hud stop];
    }else {
        [hud start];
    }
    return hud;
}

#pragma mark - Clicye Life
-(instancetype)init {
    if (self == [super init]) {
        self = [self initWithFrame:CGRectZero type:WBProgressHUDTypeSingle lineColor:nil];
    }
    return self;
}

-(instancetype) initWithFrame:(CGRect)frame type:(WBProgressHUDType)type lineColor:(UIColor *)lineColor {
    if (self == [super initWithFrame:frame]) {
        self.lineColor = lineColor;
        // 默认时长
        self.inDuration = 1.0;
        self.outDuration = 1.5;
        _type = type;
        
        if (!lineColor) {
            self.lineColor = [UIColor whiteColor];
        }
        if (!type) {
            _type = WBProgressHUDTypeSingle;
        }
        
        [self initializeInterface];
    }
    return self;
}

-(void) initializeInterface {
    
    if (CGRectGetWidth(self.frame) == 0 || CGRectGetHeight(self.frame) == 0) {
        self.frame = CGRectMake((S_W - LoadingWidth) / 2, (S_H - LoadingWidth) / 2, LoadingWidth, LoadingWidth);
    }
    self.layer.cornerRadius  = 5;
    self.backgroundColor = [UIColor colorWithWhite:0.0 alpha:0.95];
    
    [self.layer addSublayer:self.inLayer];
    _inGradientLayer = [self createGradientLayer:YES];
    _inShapeLayer = [self createCaShapeLayer:YES];
    [self.inLayer addSublayer: _inGradientLayer];
    self.inLayer.mask = _inShapeLayer;
    
    [self.layer addSublayer:self.outLayer];
    _outGradientLayer = [self createGradientLayer:NO];
    _outShapeLayer = [self createCaShapeLayer:NO];
    [self.outLayer addSublayer: _outGradientLayer];
    self.outLayer.mask = _outShapeLayer;
}

/**
  创建颜色渐变层

 @param isInOrOut 是否在内圈
 @return 创建好的渐变层
 */
-(CAGradientLayer *)createGradientLayer:(BOOL) isInOrOut {
    CAGradientLayer *_gradientLayer = [CAGradientLayer layer];
    _gradientLayer.frame = self.bounds;
    _gradientLayer.colors = @[(__bridge id)[UIColor clearColor].CGColor, (__bridge id)self.lineColor.CGColor];
    if (_type ==  WBProgressHUDTypeGossip) {
        if (isInOrOut) {
            _gradientLayer.startPoint = CGPointMake(0.5, 1.0);
            _gradientLayer.endPoint = CGPointMake(0.5, 0.0);
        }else{
            _gradientLayer.startPoint = CGPointMake(1.0, 0.5);
            _gradientLayer.endPoint = CGPointMake(0.0, 0.5);
        }
    }else if (_type == WBProgressHUDTypeFishHook) {
        if (isInOrOut) {
            _gradientLayer.startPoint = CGPointMake(0.5, 1.0);
            _gradientLayer.endPoint = CGPointMake(0.5, 0.0);
        }else{
            _gradientLayer.startPoint = CGPointMake(0.5, 0.0);
            _gradientLayer.endPoint = CGPointMake(0.5, 1.0);
        }
    }else{
        _gradientLayer.startPoint = CGPointMake(0.0, 0.5);
        _gradientLayer.endPoint = CGPointMake(1.0, 0.5);
    }
    return _gradientLayer;
}

/**
 创建mask层

 @param isInOrOut 是否是内圈
 @return maskLayer
 */
-(CAShapeLayer *)createCaShapeLayer:(BOOL) isInOrOut {
    CGFloat width = CGRectGetWidth(self.bounds);
    CGFloat height = CGRectGetHeight(self.bounds);
    CGFloat lineWidth = width / 15;  // 线宽
    CGPoint theCenter = CGPointMake(width / 2, height / 2);  // 视图中心点
    
    UIBezierPath *path = [UIBezierPath bezierPath];
    
    if (_type ==  WBProgressHUDTypeGossip) {
        if (isInOrOut) {
            [path moveToPoint:theCenter];
            // 第一个半圆
            CGPoint firstPoint = CGPointMake(width / 2, height / 8 * 3);
            [path appendPath:[UIBezierPath bezierPathWithArcCenter:firstPoint radius:width / 8 startAngle:M_PI_2 endAngle:M_PI_2 + M_PI clockwise:YES]];
            [path appendPath:[UIBezierPath bezierPathWithArcCenter:theCenter radius:width / 4 startAngle:-M_PI_2 endAngle:M_PI_2 clockwise:YES]];
            CGPoint secondPoint = CGPointMake(width / 2, height / 8 * 5);
            [path appendPath:[UIBezierPath bezierPathWithArcCenter:secondPoint radius:width / 8 startAngle:M_PI_2 endAngle:- M_PI_2 clockwise:NO]];
            [path closePath];
        }else{
            [path moveToPoint:CGPointMake(0, height / 2)];
            // 第二个半圆
            CGPoint firstPoint = CGPointMake(width / 8, height / 2);
            [path appendPath:[UIBezierPath bezierPathWithArcCenter:firstPoint radius:width / 8 startAngle:M_PI endAngle:2 * M_PI clockwise:YES]];
            CGPoint secondPoint = CGPointMake(width / 8 * 5, height / 2);
            [path appendPath:[UIBezierPath bezierPathWithArcCenter:secondPoint radius:width / 8 * 3 startAngle:M_PI endAngle:0 clockwise:NO]];
            [path appendPath:[UIBezierPath bezierPathWithArcCenter:theCenter radius:width / 2 startAngle:0 endAngle:M_PI clockwise:YES]];
            [path closePath];
        }
    }else if (_type == WBProgressHUDTypeFishHook) {
        if (isInOrOut) {
            [path moveToPoint:theCenter];
            // 第一个半圆
            CGPoint firstPoint = CGPointMake(width / 2, height / 4 * 3);
            [path appendPath:[UIBezierPath bezierPathWithArcCenter:firstPoint radius:width / 4 startAngle:M_PI_2 + M_PI endAngle:M_PI_2 clockwise:NO]];
             [path appendPath:[UIBezierPath bezierPathWithArcCenter:theCenter radius:width / 2 startAngle:M_PI_2 endAngle:M_PI_2 + M_PI clockwise:YES]];
            CGPoint secondPoint = CGPointMake(width / 2, height / 4);
            [path appendPath:[UIBezierPath bezierPathWithArcCenter:secondPoint radius:width / 4 startAngle:-M_PI_2 endAngle:M_PI_2 clockwise:YES]];
            [path closePath];
        }else{
            [path moveToPoint:theCenter];
            // 第二个半圆
            CGPoint firstPoint = CGPointMake(width / 2, height / 4 * 3);
            [path appendPath:[UIBezierPath bezierPathWithArcCenter:firstPoint radius:width / 4 startAngle:M_PI_2 + M_PI endAngle:M_PI_2 clockwise:NO]];
            [path appendPath:[UIBezierPath bezierPathWithArcCenter:theCenter radius:width / 2 startAngle:M_PI_2 endAngle:-M_PI_2 clockwise:NO]];
            CGPoint secondPoint = CGPointMake(width / 2, height / 4);
            [path appendPath:[UIBezierPath bezierPathWithArcCenter:secondPoint radius:width / 4 startAngle:-M_PI_2 endAngle:M_PI_2 clockwise:YES]];
            [path closePath];
        }
    }else{
        if (isInOrOut) {
            path = [UIBezierPath bezierPathWithArcCenter:theCenter radius:width / 6 startAngle:0 endAngle:M_PI clockwise:YES];
        }else{
            path = [UIBezierPath bezierPathWithArcCenter:theCenter radius:width / 4 startAngle:M_PI endAngle:2 * M_PI clockwise:YES];
        }
    }
    CAShapeLayer *_shapeLayer = [CAShapeLayer layer];
    _shapeLayer.lineCap = kCALineCapRound;
    _shapeLayer.path = path.CGPath;
    if (_type == WBProgressHUDTypeSingle) {
        _shapeLayer.lineWidth = lineWidth;
        _shapeLayer.fillColor = [UIColor clearColor].CGColor;
        _shapeLayer.strokeColor = self.lineColor.CGColor;
    }
    return _shapeLayer;
}

#pragma mark - Start Animations
-(void) start {
    if (self.inLayer.animationKeys) {
        // 有动画，直接返回
        return;
    }
    
    // 旋转动画
    CABasicAnimation *animation = [CABasicAnimation animation];
    animation.keyPath = @"transform.rotation.z";
    animation.fromValue = @(2 * M_PI);
    animation.toValue = @0;
    animation.duration = self.inDuration;
    animation.cumulative = YES;
    animation.fillMode = kCAFillModeForwards;
    animation.repeatCount = CGFLOAT_MAX;
    animation.removedOnCompletion = NO;
    
    if (_type == WBProgressHUDTypeFishHook) {
        animation.fromValue = @0;
        animation.toValue = @(2 * M_PI);
        [self.inLayer addAnimation:animation forKey:@"inLayer_animation"];
        [self.outLayer addAnimation:animation forKey:@"outLayer_animation"];
        return ;
    }
    // 内圈动画
    [self.inLayer addAnimation:animation forKey:@"inLayer_animation"];
    
    // 外圈动画
    animation.fromValue = @0;
    animation.toValue = @(2 * M_PI);
    animation.duration = self.outDuration;
    [self.outLayer addAnimation:animation forKey:@"centerLayer_animation"];
}

#pragma mark - Stop Animations
-(void) stop {
    [self.inLayer removeAllAnimations];
    [self.outLayer removeAllAnimations];
}

#pragma mark - Hidden
-(void) hideAnimated:(BOOL)animated {
    if (animated) {
        [UIView animateWithDuration:0.1 animations:^{
            [self stop];
            self.alpha = 0;
        } completion:^(BOOL finished) {
            [self removeFromSuperview];
        }];
    }else{
        [self stop];
        [self removeFromSuperview];
    }
}

#pragma mark - Lazy
-(CALayer *)inLayer {
    if (_inLayer) {
        return _inLayer;
    }
    _inLayer = [CALayer layer];
    _inLayer.frame = self.bounds;
    _inLayer.backgroundColor = [UIColor clearColor].CGColor;
    return _inLayer;
}

-(CALayer *)outLayer {
    if (_outLayer) {
        return _outLayer;
    }
    _outLayer = [CALayer layer];
    _outLayer.frame = self.bounds;
    _outLayer.backgroundColor = [UIColor clearColor].CGColor;
    return _outLayer;
}

#pragma mark - Setter
-(void)setLineColor:(UIColor *)lineColor {
    if (_lineColor == lineColor) {
        return;
    }
    _lineColor = lineColor;
    // 重置颜色
    _inGradientLayer.colors = @[(__bridge id)[UIColor clearColor].CGColor, (__bridge id)lineColor.CGColor];
    _outGradientLayer.colors = @[(__bridge id)[UIColor clearColor].CGColor, (__bridge id)lineColor.CGColor];
    // 对单线条模板进行二次处理
    if (_type == WBProgressHUDTypeSingle) {
        _inShapeLayer.strokeColor = lineColor.CGColor;
        _outShapeLayer.strokeColor = lineColor.CGColor;
    }
}

@end
