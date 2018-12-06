//
//  SWUIBezier.h
//  UIBezierPathDemo
//
//  Created by EL on 2017/6/22.
//  Copyright © 2017年 etouch. All rights reserved.
//

#import <UIKit/UIKit.h>

@class SWFilterRGBW;

@interface SWUIBezier : UIView

@property (strong, nonatomic) UIColor* lineColor;

-(void)linkRGBW:(SWFilterRGBW*)_rgbw;

-(void)delCtrlPt;

-(void)active;

-(void)unactive;

-(float*)getBezierData;

@end
