//
//  SWState.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>

//单一变量集合

@interface SWState : NSObject

@property (assign, nonatomic) int svDesignW;
@property (assign, nonatomic) int svDesignH;
@property (assign, nonatomic) int svOutW;
@property (assign, nonatomic) int svOutH;
@property (assign, nonatomic) int svAdaptMode;
@property (assign, nonatomic) float svAdaptRate;
@property (assign, nonatomic) float svBeautyRadius;
@property (assign, nonatomic) float svShadows;
@property (assign, nonatomic) float svHighlights;
@property (assign, nonatomic) float svContrast;
@property (assign, nonatomic) float svSaturation;
@property (assign, nonatomic) float svAcutance;
@property (assign, nonatomic) float svBrightness;
@property (assign, nonatomic) float svWhitening;
@property (assign, nonatomic) float svGamma;
@property (assign, nonatomic) float svRedShift;
@property (assign, nonatomic) float svGreenShift;
@property (assign, nonatomic) float svBlueShift;
@property (assign, nonatomic) float svSDRedShift;
@property (assign, nonatomic) float svSDGreenShift;
@property (assign, nonatomic) float svSDBlueShift;
@property (assign, nonatomic) float svHHRedShift;
@property (assign, nonatomic) float svHHGreenShift;
@property (assign, nonatomic) float svHHBlueShift;
@property (assign, nonatomic) float svFaceEye;
@property (assign, nonatomic) float svFaceShape;
@property (assign, nonatomic) float svTemperature;
@property (assign, nonatomic) float svTint;
@property (assign, nonatomic) float svExposure;

@end
