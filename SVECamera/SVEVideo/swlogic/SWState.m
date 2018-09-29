//
//  SWState.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWState.h"

@interface SWState(){
}
@end

@implementation SWState

-(instancetype)init {
    if (self == [super init]) {
        self.svDesignW = 750;
        self.svDesignH = 1624;
        self.svOutW = 720;
        self.svOutH = 1280;
        self.svAdaptMode = 1;       //1 全屏(等比) 2.像素不裁剪(等比)
        self.svAdaptRate = 1.0f;
        self.svBeautyRadius=0.0;
        self.svShadows=0.0;
        self.svHighlights=0.0;
        self.svContrast=0.0;
        self.svSaturation=0.0;
        self.svAcutance=0.0;
        self.svBrightness=0.0;
        self.svWhitening=0.0;
        self.svGamma=0.0;
        self.svRedShift=0.0;
        self.svGreenShift=0.0;
        self.svBlueShift=0.0;
        self.svSDRedShift=0.0;
        self.svSDGreenShift=0.0;
        self.svSDBlueShift=0.0;
        self.svHHRedShift=0.0;
        self.svHHGreenShift=0.0;
        self.svHHBlueShift=0.0;
        self.svFaceEye=0.0;
        self.svFaceShape=0.0;
        self.svTemperature=0.0;
        self.svTint=0.0;
        self.svExposure=0.0;
    }
    return self;
}

@end


