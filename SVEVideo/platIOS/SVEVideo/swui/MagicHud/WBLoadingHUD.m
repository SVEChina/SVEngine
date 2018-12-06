//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "WBLoadingHUD.h"

#define S_W [[UIScreen mainScreen] bounds].size.width
#define S_H [[UIScreen mainScreen] bounds].size.height

#define LoadingWidth 100   // 默认大小

#define Max_Line 5  // 一共有多少段

@interface WBLoadingHUD ()
{
    CAShapeLayer *_inShapLayer;
    CAShapeLayer *_centerShapLayer;
    CAShapeLayer *_outShapLayer;
}
@property(nonatomic, strong) CALayer *inLayer; // 最里层layer
@property(nonatomic, strong) CALayer *centerLayer; // 中间一层layer
@property(nonatomic, strong) CALayer *outLayer; // 最外层layer

@end

@implementation WBLoadingHUD

#pragma makr - Show To View
+(instancetype) showToView:(UIView *)view animated:(BOOL)animated {
    
    CGRect rect = CGRectMake((view.bounds.size.width - LoadingWidth ) / 2, (view.bounds.size.height - LoadingWidth ) / 2, LoadingWidth, LoadingWidth);
    WBLoadingHUD *hud = [[WBLoadingHUD alloc] initWithFrame:rect];
    for (UIView *subView in view.subviews) {
        if ([subView isKindOfClass:[WBLoadingHUD class]]) {
            if (!animated) {
                [(WBLoadingHUD *)subView stop];
            }else{
                [(WBLoadingHUD *)subView start];
            }
            return (WBLoadingHUD *)subView;
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
-(instancetype)initWithFrame:(CGRect)frame {
    if (self == [super initWithFrame:frame]) {
        self.inDuration = 1.0;
        self.centerDuration = 1.25;
        self.outDuration = 1.5;
        [self initializeInterface];
    }
    return self;
}

-(void) initializeInterface {
    
    if (CGRectGetWidth(self.frame) == 0 || CGRectGetHeight(self.frame) == 0) {
        self.frame = CGRectMake((S_W - LoadingWidth) / 2, (S_H - LoadingWidth) / 2, LoadingWidth, LoadingWidth);
    }
    self.layer.cornerRadius  = 5;
    self.backgroundColor = [UIColor colorWithWhite:0.0 alpha:0.05];
    
    CGFloat radiusCoefficient = 10; // 半径系数
    
    [self.layer addSublayer:self.inLayer];
    _inShapLayer = [self createShapeLayerWithRadius:radiusCoefficient];
    [self.inLayer addSublayer: _inShapLayer];
    
    [self.layer addSublayer:self.centerLayer];
    radiusCoefficient -= 4;
    _centerShapLayer = [self createShapeLayerWithRadius:radiusCoefficient];
    [self.centerLayer addSublayer: _centerShapLayer];
    
    [self.layer addSublayer:self.outLayer];
    radiusCoefficient -= 1.75;
    _outShapLayer = [self createShapeLayerWithRadius:radiusCoefficient];
    [self.outLayer addSublayer:_outShapLayer];
}

-(CAShapeLayer *) createShapeLayerWithRadius:(CGFloat)radiusCoefficient {
    CGFloat width = CGRectGetWidth(self.bounds);
    
    CGFloat lineWidth = width / 20;
    
    CGFloat roundLength = 0;  // 圈全长
    
    // 视图中心
    CGPoint theCenter = CGPointMake(width / 2, CGRectGetHeight(self.bounds) / 2);
    
    UIBezierPath *path = [UIBezierPath bezierPathWithArcCenter:theCenter radius: width / radiusCoefficient startAngle:0 endAngle:2 * M_PI clockwise:NO];
    roundLength = 2 * M_PI * width / radiusCoefficient;
    CAShapeLayer *_shapeLayer = [CAShapeLayer layer];
    _shapeLayer.path = [path bezierPathByReversingPath].CGPath;
    _shapeLayer.fillColor = [UIColor clearColor].CGColor;
    _shapeLayer.strokeColor = [UIColor lightGrayColor].CGColor;
    _shapeLayer.lineCap = kCALineCapButt;
    _shapeLayer.lineWidth = lineWidth;
    _shapeLayer.lineDashPattern = @[@((roundLength - 1.5 * lineWidth * Max_Line) / Max_Line),[NSNumber numberWithFloat:1.5 * lineWidth]];
    return _shapeLayer;
}

#pragma mark - Start Animation
-(void) start {
    
    if (self.inLayer.animationKeys) {
        // 有动画，直接返回
        return;
    }
    
    // 旋转动画
    CABasicAnimation *animation = [CABasicAnimation animation];
    animation.keyPath = @"transform.rotation.z";
    animation.fromValue = @0;
    animation.toValue = @(2 * M_PI);
    animation.duration = self.inDuration;
    animation.cumulative = YES;
    animation.fillMode = kCAFillModeForwards;
    animation.repeatCount = CGFLOAT_MAX;
    animation.removedOnCompletion = NO;
    
    // 内圈动画
    [self.inLayer addAnimation:animation forKey:@"inLayer_animation"];
    
    // 外圈动画
    animation.duration = self.outDuration;
    [self.outLayer addAnimation:animation forKey:@"outLayer_animation"];
    
    // 中圈动画
    animation.fromValue = @(2 * M_PI);
    animation.toValue = @0;
    animation.duration = self.centerDuration;
    [self.centerLayer addAnimation:animation forKey:@"centerLayer_animation"];
}

#pragma mark - Stop Animation
-(void) stop {
    [self.inLayer removeAllAnimations];
    [self.centerLayer removeAllAnimations];
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

#pragma mark - Lazy Loading
-(CALayer *)inLayer {
    if (_inLayer) {
        return _inLayer;
    }
    _inLayer = [CALayer layer];
    _inLayer.backgroundColor = [UIColor clearColor].CGColor;
    _inLayer.frame = self.bounds;
    return _inLayer;
}

-(CALayer *)centerLayer {
    if (_centerLayer) {
        return _centerLayer;
    }
    _centerLayer = [CALayer layer];
    _centerLayer.backgroundColor = [UIColor clearColor].CGColor;
    _centerLayer.frame = self.bounds;
    return _centerLayer;
}

-(CALayer *)outLayer {
    if (_outLayer) {
        return _outLayer;
    }
    _outLayer = [CALayer layer];
    _outLayer.backgroundColor = [UIColor clearColor].CGColor;
    _outLayer.frame = self.bounds;
    return _outLayer;
}

#pragma mark - Setter
-(void)setLineColor:(UIColor *)lineColor {
    if (_lineColor == lineColor) {
        return;
    }
    _lineColor = lineColor;
    
    _inShapLayer.strokeColor = lineColor.CGColor;
    _centerShapLayer.strokeColor = lineColor.CGColor;
    _outShapLayer.strokeColor = lineColor.CGColor;
}

@end
