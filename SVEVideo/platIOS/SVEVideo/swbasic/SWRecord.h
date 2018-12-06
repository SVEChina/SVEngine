//
//  SWRecord.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "../datasrc/SWInStreamSampleBuffer.h"
#import "../media/RecordImage.h"
#import "../media/RecordVideo.h"

@protocol SWRecordDelegate<NSObject>
@optional
- (void)videoRecordError:(NSError *)error;
- (void)videoRecordDidStart;
- (void)videoRecordWillStop;
- (void)videoRecordDidStop;
- (void)saveVideoToAssetsResult:(NSError *)error;
- (void)saveImageOver;
- (void)saveImageToAssetsResult:(NSError *)error;
@end


@interface SWRecord : NSObject
@property(weak, nonatomic)id<SWRecordDelegate>delegate;
- (void)initModule;

- (void)destroyModule;

- (void)initializeRecorder:(SWInStreamSampleBuffer *)fmcamera;

- (void)saveImageToAssetsLibrary;
//保存图片到文件夹里
- (void)saveImageToFileCompress:(float)compress;

//保存视频到相册
- (void)saveVideoToAssetsLibrary;

- (void)startRecording;

- (void)stopRecording;

- (void)RecordingImage:(CMSampleBufferRef)sampleBuffer;

- (void)RecordingVideo:(CMSampleBufferRef)sampleBuffer;

- (void)RecordingAudio:(CMSampleBufferRef)sampleBuffer;

- (void)setAudioFormatDescription:(CMSampleBufferRef)sampleBuffer;

- (void)setVideoFormatDescription:(CMSampleBufferRef)sampleBuffer;

@property (strong, nonatomic) RecordImage *pRecordImage;
@property (strong, nonatomic) RecordVideo *pRecordVideo;

@end
