//
//  SDCamera.h
//  SVEDemo
//
//  Created by 李晓帆 on 2018/9/3.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <AVFoundation/AVFoundation.h>
@protocol CameraDelegate<NSObject>
@optional
- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection;
@end
@interface SDCamera : NSObject
@property(weak,nonatomic)id<CameraDelegate> delegate;
@property(nonatomic, strong) AVCaptureVideoDataOutput *videoOutput;
@property(nonatomic, strong) AVCaptureAudioDataOutput *audioOutput;
@property(nonatomic, strong) AVCaptureConnection *audioConnection;
@property(nonatomic, strong) AVCaptureConnection *videoConnection;

- (void)changeCamera;
- (void)startCapture;
- (void)stopCapture;
@end
