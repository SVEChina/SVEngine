//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <AVFoundation/AVFoundation.h>

//#define RECORD_AUDIO

@protocol CameraDelegate<NSObject>
@optional
- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection;
@end

//
@interface Camera : NSObject

@property(weak,nonatomic)id<CameraDelegate> delegate;
@property(nonatomic, strong) AVCaptureVideoDataOutput *videoOutput;
@property(nonatomic, strong) AVCaptureAudioDataOutput *audioOutput;
@property(nonatomic, strong) AVCaptureConnection *audioConnection;
@property(nonatomic, strong) AVCaptureConnection *videoConnection;

- (void)changeCamera;
- (void)startCapture;
- (void)stopCapture;
-(void) chageCameraExposure:(CGPoint) point isContinuousAuto:(bool) flag;
-(void) autoContinuousWhiteMode:(BOOL) flag;
-(void) changeCameraFocusing:(CGPoint)point;
-(void)setExposureTargetBias:(CGFloat)bias;

@end
