//
//  SWDataSourceCamera.h
//  SVEVideo
//
//  Created by 李晓帆 on 2018/7/5.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import "SWDataSource.h"

@interface SWDataSourceCamera : SWDataSource
- (instancetype)init;

- (Camera*)getCamera;

- (void)swipCamera;

- (void)chageCameraExposure:(CGPoint) point isContinuousAuto:(bool) flag;

- (void)autoContinuousWhiteMode:(BOOL) flag;

- (void)changeCameraFocusing:(CGPoint)point;

- (void)setExposureTargetBias:(CGFloat)bias;
@end
