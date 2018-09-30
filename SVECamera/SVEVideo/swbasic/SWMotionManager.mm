//
//  SWMotionManager.mm
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWMotionManager.h"
#define radiansToDegrees(x) (180/M_PI)*x
@interface SWMotionManager(){
    CMMotionManager *mCmmotionManager;
    CMAttitude      *mDeviceCMAttitude;
    BOOL            sensorActive;
    struct DEVICEPOSE  *mPose;
}
@end

@implementation SWMotionManager
- (instancetype)init{
    self = [super init];
    if (self) {
        
    }
    return self;
}

- (void)initModule{
    mPose = new DEVICEPOSE;
    [self initMotionManager];
}

- (void)destroyModule{
    delete mPose;
}
- (void)initMotionManager {
    if (!mCmmotionManager) {
        mCmmotionManager = [[CMMotionManager alloc] init];
        CMDeviceMotion *deviceMotion = mCmmotionManager.deviceMotion;
        mDeviceCMAttitude = deviceMotion.attitude;
        [mCmmotionManager stopDeviceMotionUpdates];
    }
    sensorActive = false;
}

- (void)startSensor{
    if (!sensorActive) {
        [mCmmotionManager startDeviceMotionUpdates];
        sensorActive = true;
    }
}

- (void)stopSensor{
    if (sensorActive) {
        [mCmmotionManager stopDeviceMotionUpdates];
        sensorActive = false;
    }
}

- (CMRotationMatrix)getRotationMatrix{
    CMRotationMatrix rotation;
    CMDeviceMotion *deviceMotion = mCmmotionManager.deviceMotion;
    CMAttitude *attitude = deviceMotion.attitude;
    if (mDeviceCMAttitude != nil) {
        [attitude multiplyByInverseOfAttitude:mDeviceCMAttitude];
    }
    rotation = attitude.rotationMatrix;
    return rotation;
}

- (struct DEVICEPOSE *)getDevicePose{
    if (sensorActive) {
        CMDeviceMotion *deviceMotion = mCmmotionManager.deviceMotion;
        CMAttitude *attitude = deviceMotion.attitude;
        CMQuaternion quat = attitude.quaternion;
        float roll = radiansToDegrees(atan2(2*(quat.y*quat.w - quat.x*quat.z), 1 - 2*quat.y*quat.y - 2*quat.z*quat.z)) ;
        float pitch = radiansToDegrees(atan2(2*(quat.x*quat.w + quat.y*quat.z), 1 - 2*quat.x*quat.x - 2*quat.z*quat.z));
        float yaw = radiansToDegrees(asin(2*quat.x*quat.y + 2*quat.w*quat.z));
        mPose->roll = roll;
        mPose->pitch = pitch;
        mPose->yaw = yaw;
        return mPose;
    }
    return nullptr;
    
}

@end
