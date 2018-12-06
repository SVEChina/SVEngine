//
//  SWMotionManager.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreMotion/CoreMotion.h>
struct DEVICEPOSE{
    float roll;
    float yaw;
    float pitch;
};

struct DEVICEACCELEROMETER{
    double x;
    double y;
    double z;
};

struct DEVICEGYRO{
    double x;
    double y;
    double z;
};

struct DEVICEGRAVITY{
    double x;
    double y;
    double z;
};
@interface SWMotionManager : NSObject
- (void)initModule;
- (void)destroyModule;
- (void)startSensor;
- (void)stopSensor;
- (CMRotationMatrix)getRotationMatrix;
- (struct DEVICEPOSE *)getDevicePose;
- (struct DEVICEACCELEROMETER *)getDeviceAccelerometer;
- (struct DEVICEGRAVITY *)getDeviceGravity;
- (struct DEVICEGYRO *)getDeviceGyro;
@end
