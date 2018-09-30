//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "WBRoundHUD.h"

#define S_W [[UIScreen mainScreen] bounds].size.width
#define S_H [[UIScreen mainScreen] bounds].size.height

#define LoadingWidth 50   // 默认大小

#define MAX_ROUND 6   // 小球最大数量

@interface WBRoundHUD () <CAAnimationDelegate>
{
    WBRoundHUDType _type;  // 运动类型
    NSInteger _animationCount;  // 已运动的小球个数
}

@property (nonatomic, strong)  NSMutableArray<CALayer *> *caLayers;

@end

@implementation WBRoundHUD

#pragma makr - Show To View
+(instancetype) showToView:(UIView *)view type:(WBRoundHUDType)type animated:(BOOL)animated {
    if (!type) {
        type = WBRoundHUDTypeUniform;
    }
    CGRect rect = CGRectMake((view.bounds.size.width - LoadingWidth ) / 2, (view.bounds.size.height - LoadingWidth ) / 2, LoadingWidth, LoadingWidth);
    WBRoundHUD *hud = [[WBRoundHUD alloc] initWithFrame:rect type:type roundColor:nil];
    for (UIView *subView in view.subviews) {
        if ([subView isKindOfClass:[WBRoundHUD class]]) {
            if (!animated) {
                [(WBRoundHUD *)subView stop];
            }else{
                [(WBRoundHUD *)subView start];
            }
            return (WBRoundHUD *)subView;
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
        self = [self initWithFrame:CGRectZero type:WBRoundHUDTypeUniform roundColor:nil];
    }
    return self;
}

-(instancetype) initWithFrame:(CGRect)frame type:(WBRoundHUDType)type roundColor:(UIColor *)roundColor {
    if (self == [super initWithFrame:frame]) {
        self.roundColor = roundColor;
        // 设置默认动画时长
        self.duration = 1.0;
        _type = type;
        
        if (!roundColor) {
            self.roundColor = [UIColor whiteColor];
        }
        if (!type) {
            _type = WBRoundHUDTypeUniform;
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
    
    if (!self.caLayers) {
        self.caLayers = [NSMutableArray array];
    }
    
    CGFloat width = self.bounds.size.width;
    CGPoint layerCenter = CGPointMake(width / 6 * 5, self.bounds.size.height / 2);
    
    for (int index = 0; index < MAX_ROUND; index ++) {
        CALayer *_layer = [CALayer layer];
        _layer.position = layerCenter;
        _layer.bounds = CGRectMake(0, 0, width / 10 , width / 10);
        _layer.backgroundColor = self.roundColor.CGColor;
        _layer.cornerRadius = width / 20;
        _layer.hidden = YES;
        [self.layer addSublayer:_layer];
        
        [self.caLayers addObject:_layer];
    }
}

#pragma mark - Start Animations
-(void) start {
    if (self.caLayers.firstObject.animationKeys) {
        // 有动画，直接返回
        return;
    }
    
    CGFloat width = self.bounds.size.width;
    CGPoint theCenter = CGPointMake(width / 2, self.bounds.size.height / 2);
    UIBezierPath *path = [UIBezierPath bezierPathWithArcCenter:theCenter radius:width / 3 startAngle:0 endAngle:2 * M_PI clockwise:YES];
    
    // 帧动画
    CAKeyframeAnimation *animation = [CAKeyframeAnimation animation];
    animation.keyPath = @"position";
    animation.fillMode = kCAFillModeForwards;
    animation.removedOnCompletion = NO;
    // 小球动画匀速运动
    if (_type ==  WBRoundHUDTypeUniform) {
        animation.path = path.CGPath;
        animation.duration = self.duration;
        animation.calculationMode = kCAAnimationCubicPaced;
        animation.repeatCount = CGFLOAT_MAX;
        animation.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];
        
        // 为每一个Layer添加动画
        for (int index = 0; index < self.caLayers.count; index ++) {
            // 使第一个小球将要运动到最初的位置的时候，最后一个小球刚开始动画
            animation.beginTime = self.duration / self.caLayers.count * (index + 1);
            self.caLayers[index].hidden = NO;
            [self.caLayers[index] addAnimation:animation forKey:@"Layer_animation"];
        }
    }else{
        // 小球运行做渐变动画<前半圈动画>
        path = [UIBezierPath bezierPathWithArcCenter:theCenter radius:width / 3 startAngle:0 endAngle:M_PI clockwise:YES];
        animation.path = path.CGPath;
        animation.duration = self.duration / 2;
        animation.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
        
        path = [UIBezierPath bezierPathWithArcCenter:theCenter radius:width / 3 startAngle:-M_PI endAngle:0 clockwise:YES];
        // 帧动画
        CAKeyframeAnimation *upAnimation = [CAKeyframeAnimation animation];
        upAnimation.keyPath = @"position";
        upAnimation.fillMode = kCAFillModeForwards;
        upAnimation.removedOnCompletion = NO;
        // 小球运行做渐变动画<后半圈动画>
        upAnimation.path = path.CGPath;
        upAnimation.duration = self.duration / 2;
        upAnimation.beginTime = self.duration / 2;
        upAnimation.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
        
        CAAnimationGroup *group = [CAAnimationGroup animation];
        group.duration = self.duration;
        group.delegate = self;
        group.animations = @[animation, upAnimation];
        
        // 为每一个Layer添加动画
        for (int index = 0; index < self.caLayers.count; index ++) {
            // 使第一个小球将要运动到对面(即半个圈)位置的时候，最后一个小球刚开始动画
            CFTimeInterval time = self.duration / 2 / self.caLayers.count * (index + 1);
            // 延时到相对应的时间再开始下一个小球动画
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(time * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                self.caLayers[index].hidden = NO;
                [self.caLayers[index] addAnimation:group forKey:@"index_Layer_animation"];
            });
        }
    }
}

#pragma mark - CAAnimationDelegate
-(void)animationDidStop:(CAAnimation *)anim finished:(BOOL)flag {
    if (!flag) return ;
    // 小球动画运动完成之后，对其隐藏并移除动画
    CALayer *layer = self.caLayers[_animationCount];
    layer.hidden = YES;
    [layer removeAllAnimations];
    _animationCount += 1;
    // 到最后一个小球运动完成，开始下一轮的动画
    if (_animationCount == self.caLayers.count){
        _animationCount = 0;
        // 稍微延迟0.1s，使其最后一个隐藏
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            [self start];
        });
    }
}

#pragma mark - Stop Animations
-(void) stop {
    for (CALayer *layer in self.caLayers) {
        [layer removeAllAnimations];
    }
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

#pragma mark - Setter
-(void)setRoundColor:(UIColor *)roundColor {
    if (_roundColor == roundColor) {
        return ;
    }
    _roundColor = roundColor;
    for (CALayer *layer in self.caLayers) {
        layer.backgroundColor = roundColor.CGColor;
    }
}

@end
