//
//  SWDataSourceCamera.m
//  SVEVideo
//
//  Created by 李晓帆 on 2018/7/5.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import "SWDataSourceCamera.h"
#import "SWInStreamSampleBuffer.h"
#import "../media/Camera.h"
#import "../detect/Detection.h"
#import "../swlogic/SWLogicSys.h"
#import "../swbasic/SWBasicSys.h"
#import "SWOutStream.h"
extern unsigned char *s_outDataBuffer;
@interface SWDataSourceCamera()<CameraDelegate>{
    Camera* m_pCamera;
    dispatch_semaphore_t  m_semaphore;
    float m_videoAngle;
}
@end

@implementation SWDataSourceCamera
- (instancetype)init{
    self = [super init];
    if (self) {
        m_srcType = DATASRCCAMERA;
        m_videoAngle = 0.0f;
        m_semaphore = dispatch_semaphore_create(1);
        m_pCamera = [[Camera alloc]init];
        m_pCamera.delegate = self;
    }
    return self;
}


- (void)initModule{
    [super initModule];
}

- (void)destroyModule{
    [super destroyModule];
}

- (void)dealloc{

}


- (int)getVideoWidth{
    return 0;
}

- (int)getVideoHeight{
    return 0;
}

- (void)start{
    if(m_pCamera){
        [m_pCamera startCapture];
    }
}

- (void)stop{
    if(m_pCamera){
        [m_pCamera stopCapture];
    }
}

- (Camera*)getCamera{
    return m_pCamera;
}

- (void)swipCamera{
    [m_pCamera changeCamera];
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection{
    
    CFDictionaryRef metadataDict = CMCopyDictionaryOfAttachments(NULL,sampleBuffer, kCMAttachmentMode_ShouldPropagate);
    NSDictionary *metadata = [[NSMutableDictionary alloc] initWithDictionary:(__bridge NSDictionary*)metadataDict];
    CFRelease(metadataDict);
    NSDictionary *exifMetadata = [[metadata objectForKey:(NSString *)kCGImagePropertyExifDictionary] mutableCopy];
    float brightnessValue = [[exifMetadata objectForKey:(NSString *)kCGImagePropertyExifBrightnessValue] floatValue];
    
    NSLog(@"光感度%f",brightnessValue);
    
    dispatch_semaphore_wait(m_semaphore, DISPATCH_TIME_FOREVER);
    if (connection == m_pCamera.videoConnection) {
        
        struct opaqueCMSampleBuffer *buffer = (struct opaqueCMSampleBuffer *)sampleBuffer;
        [[SWBasicSys getInst].m_detection detectBuffer:buffer];
        [[SWBasicSys getInst].m_inStream renderSampleBuffer:buffer];
        if ([SWBasicSys getInst].m_outStream.m_outStreaming && s_outDataBuffer != NULL) {
            [self copyToSampleBuffer:sampleBuffer];
        }
        [[SWBasicSys getInst].m_outStream setOutPutBuffer:buffer];
        
        //
//        CMRotationMatrix deviceRotation = [[SWBasicSys getInst].m_pFMMotion getRotationMatrix];
//        NSLog(@"motion pose test roll:%f yaw:%f pitch:%f",deviceRotation.m32, deviceRotation.m33,  deviceRotation.m31);
//        [[SWLogicSys getInst].pSVI pushCameraPoseRoll:deviceRotation.m32*180/3.14 Yaw:deviceRotation.m33*180/3.14 Pitch:deviceRotation.m31*180/3.14];
//
        
//        struct DEVICEPOSE *pose = [[SWBasicSys getInst].m_pFMMotion getDevicePose];
//        [[SWLogicSys getInst].pSVI pushCameraPoseRoll:pose->roll Yaw:pose->yaw Pitch:pose->pitch];
//         NSLog(@"motion pose test roll:%f yaw:%f pitch:%f",pose->roll, pose->yaw,  pose->pitch);
        
//        struct DEVICEACCELEROMETER *accelerometer = [[SWBasicSys getInst].m_pFMMotion getDeviceAccelerometer];
//        if (accelerometer) {
//            [[SWLogicSys getInst].pSVI pushAccelerometerX:accelerometer->x AY:accelerometer->y AZ:accelerometer->z];
//        }
        
    }else if (connection == m_pCamera.audioConnection) {
        
        
    }
    dispatch_semaphore_signal(m_semaphore);
}

- (void)chageCameraExposure:(CGPoint) point isContinuousAuto:(bool) flag{
    [m_pCamera chageCameraExposure:point isContinuousAuto:flag];
}

- (void)autoContinuousWhiteMode:(BOOL) flag{
    [m_pCamera autoContinuousWhiteMode:flag];
}

- (void)changeCameraFocusing:(CGPoint)point{
    [m_pCamera changeCameraFocusing:point];
    
}
- (void)setExposureTargetBias:(CGFloat)bias{
    [m_pCamera setExposureTargetBias:bias/100.0*8.0];
}

- (void)copyToSampleBuffer:(CMSampleBufferRef)sampleBuffer{
    
    CVPixelBufferRef imageBuffer = NULL;
    imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    if(CVPixelBufferLockBaseAddress(imageBuffer, 0) == kCVReturnSuccess)
    {
        size_t width = CVPixelBufferGetWidth(imageBuffer);
        size_t height = CVPixelBufferGetHeight(imageBuffer);
        size_t bytesPerRow = CVPixelBufferGetBytesPerRowOfPlane(imageBuffer, 0);
        int pixelFormat = CVPixelBufferGetPixelFormatType(imageBuffer);
        if (pixelFormat == kCVPixelFormatType_32BGRA) {
            UInt8 *bufferPtr = (UInt8*)CVPixelBufferGetBaseAddress(imageBuffer);
            //判断从 CVImageBufferRef 里取出来的数据是否带panding
            if (bytesPerRow/4 - width !=0) {
                //带panding，要处理
                int a = 0;
                for (int i = 0; i < height; i++) {
                    memcpy(bufferPtr + a , s_outDataBuffer + i * width * 4, width*4);
                    a += bytesPerRow;
                }
            }else{
                memcpy(bufferPtr, s_outDataBuffer, width*height*4);
            }
            
        }else if(pixelFormat == kCVPixelFormatType_420YpCbCr8BiPlanarFullRange){
            
            UInt8 *bufferPtr = (UInt8 *)CVPixelBufferGetBaseAddressOfPlane(imageBuffer,0);
            UInt8 *bufferPtr1 = (UInt8 *)CVPixelBufferGetBaseAddressOfPlane(imageBuffer,1);
        }
        CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
    }
    
}
@end
