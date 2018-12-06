//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "Camera.h"
#import <AssetsLibrary/AssetsLibrary.h>
#import <Photos/PhotosDefines.h>
#import <AVFoundation/AVFoundation.h>
#import <ImageIO/ImageIO.h>


@interface Camera()<AVCaptureVideoDataOutputSampleBufferDelegate, AVCaptureAudioDataOutputSampleBufferDelegate>{
    NSLock *lock;
    bool    pauseCamera;
    AVCaptureDevice *_audioDevice;
    AVCaptureDeviceInput *_audioIn;
   
}

@property(nonatomic, strong) AVCaptureDevice *backCameraDevice;
@property(nonatomic, strong) AVCaptureDevice *frontCameraDevcie;
@property(nonatomic, strong) AVCaptureDevice *currentCameraDevcie;
@property(nonatomic, strong) AVCaptureSession *captureSession;
@property(nonatomic, strong) dispatch_queue_t sessionQueue;

@property(nonatomic, strong) AVCaptureDeviceInput *currentCameraInput;
@property(nonatomic, strong) CIContext *ciContext;
@property(nonatomic, strong) dispatch_queue_t videoQueue;
@property(nonatomic, strong) dispatch_queue_t audioQueue;
@property (nonatomic,assign)int isMirrored;
@end

@implementation Camera
- (instancetype)init {
    self = [super init];
    if (self) {
        pauseCamera = false;
        [self initializeCamera];
       
    }
    return self;
}

- (void)initializeCamera {
    lock = [[NSLock alloc]init];
    self.videoQueue = dispatch_queue_create("com.appmagic.capture.video", DISPATCH_QUEUE_SERIAL);
    
    NSError *error;
    self.captureSession = [[AVCaptureSession alloc] init];
    
#ifdef RECORD_AUDIO
    /* Audio */
    _audioDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeAudio];
    _audioIn = [[AVCaptureDeviceInput alloc] initWithDevice:_audioDevice error:nil];
    if ( [_captureSession canAddInput:_audioIn] ) {
        [_captureSession addInput:_audioIn];
    }
    self.audioOutput = [[AVCaptureAudioDataOutput alloc] init];
    // Put audio on its own queue to ensure that our video processing doesn't cause us to drop audio
    self.audioQueue = dispatch_queue_create( "com.appmagic.capture.audio", DISPATCH_QUEUE_SERIAL );
    [self.audioOutput setSampleBufferDelegate:self queue:self.audioQueue];
    
    if ( [_captureSession canAddOutput:self.audioOutput] ) {
        [_captureSession addOutput:self.audioOutput];
    }
    _audioConnection = [self.audioOutput connectionWithMediaType:AVMediaTypeAudio];
#endif // RECORD_AUDIO
    
    for(AVCaptureDevice *device in [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo]) {
        if(device.position == AVCaptureDevicePositionBack) {
            self.backCameraDevice = device;
        } else if(device.position == AVCaptureDevicePositionFront) {
            self.frontCameraDevcie = device;
        }
        [device lockForConfiguration:&error];
        //[self.captureSession beginConfiguration];
        device.exposureMode = AVCaptureExposureModeContinuousAutoExposure;
        //device.focusMode = AVCaptureFocusModeContinuousAutoFocus;
        [device unlockForConfiguration];
        //[self.captureSession commitConfiguration];
    }
    self.currentCameraDevcie = self.frontCameraDevcie;
    
    _currentCameraInput = [AVCaptureDeviceInput deviceInputWithDevice:self.currentCameraDevcie error:&error];
    
    if([self.captureSession canAddInput:_currentCameraInput]) {
        [self.captureSession addInput:_currentCameraInput];
    }
    
    self.videoOutput = [[AVCaptureVideoDataOutput alloc] init];
    [self.videoOutput setAlwaysDiscardsLateVideoFrames:NO];
    [self.videoOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id)kCVPixelBufferPixelFormatTypeKey]]; //
    [self.videoOutput setSampleBufferDelegate:self queue:self.videoQueue];
    if([self.captureSession canAddOutput:self.videoOutput]) {
        [self.captureSession addOutput:self.videoOutput];
    }
    self.captureSession.sessionPreset = AVCaptureSessionPresetiFrame1280x720;;//AVCaptureSessionPresetPhoto;//AVCaptureSessionPresetiFrame1280x720;//AVCaptureSessionPresetiFrame960x540; //
    _videoConnection = [self.videoOutput connectionWithMediaType:AVMediaTypeVideo];
    if ([_videoConnection isVideoOrientationSupported])
    {
        AVCaptureVideoOrientation orientation = AVCaptureVideoOrientationPortrait;
        [_videoConnection setVideoOrientation:orientation];
        [_videoConnection setVideoMirrored:YES];
    }
    [self setFrameRate:30];
    [self.captureSession commitConfiguration];
}

-(AVCaptureWhiteBalanceGains)clampGains:(AVCaptureWhiteBalanceGains)gains toMinVal:(CGFloat)minVal andMaxVal:(CGFloat)maxVal {
    AVCaptureWhiteBalanceGains tmpGains = gains;
    tmpGains.blueGain = MAX(MIN(tmpGains.blueGain, maxVal), minVal);
    tmpGains.redGain = MAX(MIN(tmpGains.redGain, maxVal), minVal);
    tmpGains.greenGain = MAX(MIN(tmpGains.greenGain, maxVal), minVal);
    return tmpGains;
}

-(void) whtiletoTemperature:(CGFloat)currents{
    [_currentCameraDevcie lockForConfiguration:nil];
    if ([_currentCameraDevcie isWhiteBalanceModeSupported:AVCaptureWhiteBalanceModeLocked]) {
        AVCaptureWhiteBalanceGains currentGains = _currentCameraDevcie.deviceWhiteBalanceGains;
        if(currents>8000){
            currents=8000;
        }
        if(currents<3000){
            currents=3000;
        }
        CGFloat currentTint = [_currentCameraDevcie temperatureAndTintValuesForDeviceWhiteBalanceGains:currentGains].tint;
        AVCaptureWhiteBalanceTemperatureAndTintValues tempAndTintValues;
        tempAndTintValues.temperature = currents;
        tempAndTintValues.tint = currentTint;
        AVCaptureWhiteBalanceGains deviceGains = [_currentCameraDevcie deviceWhiteBalanceGainsForTemperatureAndTintValues:tempAndTintValues];
        //        CGFloat maxWhiteBalanceGain = videoDevice.maxWhiteBalanceGain;
        //        CGFloat mixWhiteBalanceGain = videoDevice.mixWhiteBalanceGain;
        //   deviceGains = [self clampGains:deviceGains toMinVal:mixWhiteBalanceGain andMaxVal:maxWhiteBalanceGain];
        [_currentCameraDevcie setWhiteBalanceModeLockedWithDeviceWhiteBalanceGains:deviceGains completionHandler:nil];
    }
    [_currentCameraDevcie unlockForConfiguration];
}

-(CGFloat)getTemperature{
    CGFloat temperature=0;
    [_currentCameraDevcie lockForConfiguration:nil];
    if ([_currentCameraDevcie isWhiteBalanceModeSupported:AVCaptureWhiteBalanceModeLocked]) {
        AVCaptureWhiteBalanceGains currentGains = _currentCameraDevcie.deviceWhiteBalanceGains;
        temperature = [_currentCameraDevcie temperatureAndTintValuesForDeviceWhiteBalanceGains:currentGains].temperature;
    }
    [_currentCameraDevcie unlockForConfiguration];
    return temperature;
}

//设置相机采集帧率
-(void) setFrameRate:(int)rate;{
    if ([self.currentCameraDevcie respondsToSelector:@selector(activeVideoMinFrameDuration)]) {
        [self.currentCameraDevcie lockForConfiguration:nil];
        self.currentCameraDevcie.activeVideoMinFrameDuration = CMTimeMake(1, rate);
        self.currentCameraDevcie.activeVideoMaxFrameDuration = CMTimeMake(1, rate);
        [self.currentCameraDevcie unlockForConfiguration];
    }else{
        AVCaptureConnection *conn = [[_captureSession.outputs lastObject] connectionWithMediaType:AVMediaTypeVideo];
        if (conn.supportsVideoMinFrameDuration)
            conn.videoMinFrameDuration = CMTimeMake(1,rate);
        if (conn.supportsVideoMaxFrameDuration)
            conn.videoMaxFrameDuration = CMTimeMake(1,rate);
    }
}

- (void) changeCameraFocusing:(CGPoint)point{
    NSError *error = nil;
    [_currentCameraDevcie lockForConfiguration:&error];
    if ([_currentCameraDevcie isFocusPointOfInterestSupported] &&[_currentCameraDevcie isFocusModeSupported:AVCaptureFocusModeContinuousAutoFocus]) {
        [_currentCameraDevcie setFocusMode:AVCaptureFocusModeContinuousAutoFocus];
        [_currentCameraDevcie setFocusPointOfInterest:point];
    }
    [_currentCameraDevcie unlockForConfiguration];
}

-(void) openHDR{
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0)
    {
        [_currentCameraDevcie setAutomaticallyAdjustsVideoHDREnabled:true];
        if (_currentCameraDevcie.activeFormat.isVideoHDRSupported) {
            [_currentCameraDevcie setVideoHDREnabled:YES];
        }
    }
    
}

- (void)setExposureTargetBias:(CGFloat)bias
{
    [_currentCameraDevcie lockForConfiguration:nil];
    [_currentCameraDevcie setExposureTargetBias:(float)bias completionHandler:nil];
    [_currentCameraDevcie unlockForConfiguration];
}

-(void) chageCameraExposure:(CGPoint) point isContinuousAuto:(bool) flag{
        NSError *error = nil;
        [_currentCameraDevcie lockForConfiguration:&error];
        if(flag){
            if ([_currentCameraDevcie isExposureModeSupported:AVCaptureExposureModeContinuousAutoExposure ])
            {
                [_currentCameraDevcie setExposurePointOfInterest:point];
                _currentCameraDevcie.exposureMode = AVCaptureExposureModeContinuousAutoExposure;
            }
        }else{
            if ([_currentCameraDevcie isExposureModeSupported:AVCaptureExposureModeAutoExpose ])
            {
                [_currentCameraDevcie setExposurePointOfInterest:point];
                _currentCameraDevcie.exposureMode = AVCaptureExposureModeAutoExpose;
            }
        }
        [_currentCameraDevcie unlockForConfiguration];
}

-(void)autoContinuousWhiteMode:(BOOL) flag{
    NSError *error = nil;
    [_currentCameraDevcie lockForConfiguration:&error];
    if(flag){
        if ([_currentCameraDevcie isWhiteBalanceModeSupported:AVCaptureWhiteBalanceModeContinuousAutoWhiteBalance]) {
            [_currentCameraDevcie setWhiteBalanceMode:AVCaptureWhiteBalanceModeContinuousAutoWhiteBalance];
        }
    }else{
        if ([_currentCameraDevcie isWhiteBalanceModeSupported:AVCaptureWhiteBalanceModeAutoWhiteBalance]) {
            [_currentCameraDevcie setWhiteBalanceMode:AVCaptureWhiteBalanceModeAutoWhiteBalance];
        }
    }
    [_currentCameraDevcie unlockForConfiguration];
}

- (void)startCapture {
    dispatch_queue_t sessionQueue = dispatch_queue_create("com.example.camera.capture_session", DISPATCH_QUEUE_SERIAL);
    self.sessionQueue = sessionQueue;
    dispatch_async(sessionQueue, ^(void) {
        [self.captureSession startRunning];
    });
}

- (void)stopCapture {
    dispatch_async(self.sessionQueue, ^(void) {
        [self.captureSession stopRunning];
    });
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection{
    if (!pauseCamera) {
        if ([self.delegate respondsToSelector:@selector(captureOutput:didOutputSampleBuffer:fromConnection:)]) {
            [self.delegate captureOutput:captureOutput didOutputSampleBuffer:sampleBuffer fromConnection:connection];
        }
    }
}

- (UIImage *)rgbPixelBufferToImage:(unsigned char *)rgbBuffer W:(int)w H:(int)h{
    NSDictionary *pixelAttributes = @{(id)kCVPixelBufferIOSurfacePropertiesKey : @{}};
    CVPixelBufferRef pixelBuffer = NULL;
    CVReturn result = CVPixelBufferCreate(kCFAllocatorDefault,
                                          w,
                                          h,
                                          kCVPixelFormatType_32BGRA,
                                          (__bridge CFDictionaryRef)(pixelAttributes),
                                          &pixelBuffer);
    CVPixelBufferLockBaseAddress(pixelBuffer,0);
    unsigned char *destPlane = (unsigned char *)CVPixelBufferGetBaseAddress(pixelBuffer);
    memcpy(destPlane, rgbBuffer, w * h*4);
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    if (result != kCVReturnSuccess) {
        NSLog(@"Unable to create cvpixelbuffer %d", result);
    }
    CIImage *coreImage = [CIImage imageWithCVPixelBuffer:pixelBuffer];//CIImage Conversion DONE!!!!
    CIContext *MytemporaryContext = [CIContext contextWithOptions:nil];
    CGImageRef MyvideoImage = [MytemporaryContext
                               createCGImage:coreImage
                               fromRect:CGRectMake(0, 0,
                                                   w,
                                                   h)];
    UIImage *Mynnnimage = [[UIImage alloc] initWithCGImage:MyvideoImage scale:1.0 orientation:UIImageOrientationUp];
    CVPixelBufferRelease(pixelBuffer);
    CGImageRelease(MyvideoImage);
    return Mynnnimage;
}

#pragma mark - camera control
- (void)changeCamera{
    dispatch_async(self.sessionQueue, ^(void) {
        [self.captureSession beginConfiguration ];
        pauseCamera = true;
        if(self.currentCameraDevcie == self.frontCameraDevcie) {
            NSError *error;
            AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:self.backCameraDevice error:&error];
            [self.captureSession removeInput:_currentCameraInput];
            if([self.captureSession canAddInput:input]) {
                [self.captureSession addInput:input];
                _currentCameraInput = input;
                self.currentCameraDevcie = self.backCameraDevice;
            }
            self.isMirrored = NO;
        } else {
            NSError *error;
            AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:self.frontCameraDevcie error:&error];
            [self.captureSession removeInput:_currentCameraInput];
            if([self.captureSession canAddInput:input]) {
                [self.captureSession addInput:input];
                _currentCameraInput = input;
                self.currentCameraDevcie = self.frontCameraDevcie;
            }
            self.isMirrored = YES;
        }
        _videoConnection = [self.videoOutput connectionWithMediaType:AVMediaTypeVideo];
        if ([_videoConnection isVideoOrientationSupported]){
            AVCaptureVideoOrientation orientation = AVCaptureVideoOrientationPortrait;
            [_videoConnection setVideoOrientation:orientation];
            [_videoConnection setVideoMirrored:self.isMirrored];
        }
        // Changes take effect once the outermost commitConfiguration is invoked.
        [self.captureSession commitConfiguration];
        pauseCamera = false;
    });
}

@end
