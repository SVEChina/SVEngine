//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "RecordMedia.h"
#import "VideoRecorder.h"
#import "Camera.h"
typedef  void (^VideoRecorderError)(NSError *error);
typedef  void (^VideoRecorderDidStart)();
typedef  void (^VideoRecorderWillStop)();
typedef  void (^VideoRecorderDidStop)();

@interface RecordVideo : RecordMedia
@property(copy,nonatomic)VideoRecorderError videoRecorderError;
@property(copy,nonatomic)VideoRecorderDidStart videoRecorderDidStart;
@property(copy,nonatomic)VideoRecorderWillStop videoRecorderWillStop;
@property(copy,nonatomic)VideoRecorderDidStop videoRecorderDidStop;
@property(nonatomic, copy)NSURL *moveAssetPath;
- (void)setAudioFormatDescription:(CMSampleBufferRef)sampleBuffer;
- (void)setVideoFormatDescription:(CMSampleBufferRef)sampleBuffer;
- (void)setVideoSampleBuffer:(CMSampleBufferRef)sampleBuffer;
- (void)setAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer;
- (void)setReocrdURL:(NSURL *)url;
- (NSURL *)getRecordUrl;
- (void)setupWithCameraSite:(Camera *)camera;
- (void)startRecord;
- (void)finishRecord;
- (void)saveVideoToAssetsLibrary;
@end
