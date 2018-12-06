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
    struct DEVICEGYRO  *mGyro;
    struct DEVICEGRAVITY  *mGravity;
    struct DEVICEACCELEROMETER *mAccelerometer;
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
    mAccelerometer = new DEVICEACCELEROMETER;
    mGyro = new DEVICEGYRO;
    mGravity = new DEVICEGRAVITY;
    [self initMotionManager];
}

- (void)destroyModule{
    delete mPose;
    delete mAccelerometer;
    delete mGyro;
    delete mGravity;
}
- (void)initMotionManager {
    if (!mCmmotionManager) {
        mCmmotionManager = [[CMMotionManager alloc] init];
        mCmmotionManager.deviceMotionUpdateInterval = 1/ 30.0;
        CMDeviceMotion *deviceMotion = mCmmotionManager.deviceMotion;
        mDeviceCMAttitude = deviceMotion.attitude;
        [mCmmotionManager stopDeviceMotionUpdates];
    }
    sensorActive = false;
}

- (void)startSensor{
    if (!sensorActive) {
        sensorActive = true;
//        [self startGyro];
        [self startDeviceMotion];
//        [self startAccelerometer];
    }
}

- (void)stopSensor{
    if (sensorActive) {
        [mCmmotionManager stopDeviceMotionUpdates];
//        [mCmmotionManager stopAccelerometerUpdates];
//        [mCmmotionManager stopGyroUpdates];
        sensorActive = false;
    }
}

- (void)startAccelerometer{
    if (mCmmotionManager.isAccelerometerAvailable) {
        [mCmmotionManager startAccelerometerUpdatesToQueue:[NSOperationQueue mainQueue] withHandler:^(CMAccelerometerData * _Nullable accelerometerData, NSError * _Nullable error) {
            CMAcceleration acceleration = accelerometerData.acceleration;
            self->mAccelerometer->x = acceleration.x;
            self->mAccelerometer->y = acceleration.y;
            self->mAccelerometer->z = acceleration.z;
        }];
    }
}

- (void)startGyro{
    if (mCmmotionManager.isGyroAvailable) {
        [mCmmotionManager startGyroUpdatesToQueue:[NSOperationQueue mainQueue] withHandler:^(CMGyroData * _Nullable gyroData, NSError * _Nullable error) {
            CMRotationRate rotationRate = [gyroData rotationRate];
            self->mGyro->x = rotationRate.x;
            self->mGyro->y = rotationRate.y;
            self->mGyro->z = rotationRate.z;
        }];
    }
}

- (void)startDeviceMotion{
    if (mCmmotionManager.isDeviceMotionAvailable) {
        [mCmmotionManager startDeviceMotionUpdatesToQueue:[NSOperationQueue mainQueue] withHandler:^(CMDeviceMotion * _Nullable motion, NSError * _Nullable error) {
            CMAttitude *attitude = [motion attitude];
            
            CMQuaternion quat = attitude.quaternion;
            float roll = radiansToDegrees(atan2(2*(quat.y*quat.w - quat.x*quat.z), 1 - 2*quat.y*quat.y - 2*quat.z*quat.z)) ;
            float pitch = radiansToDegrees(atan2(2*(quat.x*quat.w + quat.y*quat.z), 1 - 2*quat.x*quat.x - 2*quat.z*quat.z));
            float yaw = radiansToDegrees(asin(2*quat.x*quat.y + 2*quat.w*quat.z));
            self->mPose->roll = roll;
            self->mPose->pitch = pitch;
            self->mPose->yaw = yaw;
            //
            self->mGravity->x = [motion gravity].x;
            self->mGravity->y = [motion gravity].y;
            self->mGravity->z = [motion gravity].z;
            //
            self->mAccelerometer->x = [motion userAcceleration].x;
            self->mAccelerometer->y = [motion userAcceleration].y;
            self->mAccelerometer->z = [motion userAcceleration].z;
            //
            self->mGyro->x = [motion rotationRate].x;
            self->mGyro->y = [motion rotationRate].y;
            self->mGyro->z = [motion rotationRate].z;
        }];
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

- (struct DEVICEGYRO *)getDeviceGyro{
    if (sensorActive && mGyro) {
        return mGyro;
    }
    return nullptr;
}

- (struct DEVICEPOSE *)getDevicePose{
    if (sensorActive && mPose) {
        return mPose;
    }
    return nullptr;
}

- (struct DEVICEGRAVITY *)getDeviceGravity{
    if (sensorActive && mGravity) {
        return mGravity;
    }
    return nullptr;
}

- (struct DEVICEACCELEROMETER *)getDeviceAccelerometer{
    if (sensorActive && mAccelerometer) {
        return mAccelerometer;
    }
    return nullptr;
}
@end
