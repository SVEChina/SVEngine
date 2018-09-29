//
//  SWRecord.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWRecord.h"
#import <AssetsLibrary/AssetsLibrary.h>
#import "../swui/SWUISys.h"
#import "Utility.h"
@interface SWRecord (){
    bool mRecording;
}
@end

@implementation SWRecord

- (void)initModule{
    mRecording = false;
}

- (void)destroyModule{
    self.pRecordImage = nil;
    self.pRecordVideo = nil;
}


- (void)initializeRecorder:(SWInStreamSampleBuffer *)fmcamera{
    //
     __weak SWRecord *weakSelf = self;
    self.pRecordImage = [[RecordImage alloc] init];
    NSURL *imageURL = [[NSURL alloc] initFileURLWithPath:[NSString pathWithComponents:@[NSTemporaryDirectory(), @"fm2Image.jpg"]]];
    [self.pRecordImage setImagedURL:imageURL];
    self.pRecordImage.saveImageToFileResult = ^(NSString *msg) {
        if ([msg isEqualToString:@"success"]) {
            if ([weakSelf.delegate respondsToSelector:@selector(saveImageOver)]) {
                [weakSelf.delegate saveImageOver];
            }
            
        }else{
            
        }
    };
    //
    self.pRecordImage.saveImageToAssetsResult = ^(NSError *error) {
        if ([weakSelf.delegate respondsToSelector:@selector(saveImageToAssetsResult:)]) {
            [weakSelf.delegate saveImageToAssetsResult:error];
        }
    };
//    //
//    self.pRecordVideo = [[RecordVideo alloc] init];
//    [self.pRecordVideo setupWithCameraSite: [fmcamera getCamera] ];
//    NSString *curTime = [Utility createTimeStamp];
//    NSString *fileName = [NSString stringWithFormat:@"fm2Movie_%@.mp4",curTime];
//    NSURL *videoURL = [[NSURL alloc] initFileURLWithPath:[NSString pathWithComponents:@[NSTemporaryDirectory(), fileName]]];
//    [self.pRecordVideo setReocrdURL:videoURL];
//   
//    self.pRecordVideo.videoRecorderError = ^(NSError *error) {
//        if ([weakSelf.delegate respondsToSelector:@selector(videoRecordError:)]) {
//            [weakSelf.delegate videoRecordError:error];
//        }
//    };
//    self.pRecordVideo.videoRecorderDidStart = ^{
//        if ([weakSelf.delegate respondsToSelector:@selector(videoRecordDidStart)]) {
//            [weakSelf.delegate videoRecordDidStart];
//        }
//        //正在录制
//    };
//    self.pRecordVideo.videoRecorderWillStop = ^{
//        //录制将要结束
//        if ([weakSelf.delegate respondsToSelector:@selector(videoRecordWillStop)]) {
//            [weakSelf.delegate videoRecordWillStop];
//        }
//    };
//    self.pRecordVideo.videoRecorderDidStop = ^{
////        //录制成功并且已经停止
//        if ([weakSelf.delegate respondsToSelector:@selector(videoRecordDidStop)]) {
//            [weakSelf.delegate videoRecordDidStop];
//        }
//    };
}

- (void)saveImageToAssetsLibrary{
    if (self.pRecordImage) {
        [self.pRecordImage saveImageToAssetsLibrary];
    }
}

- (void)saveImageToFileCompress:(float)compress{
    if(self.pRecordImage){
        [self.pRecordImage savedImageToFileCompression:compress];
    }
}

- (void)saveVideoToAssetsLibrary{
    self.pRecordVideo.moveAssetPath = nil;
    ALAssetsLibrary *library = [[ALAssetsLibrary alloc] init];
    [library writeVideoAtPathToSavedPhotosAlbum:[self.pRecordVideo getRecordUrl] completionBlock:^(NSURL *assetURL, NSError *error) {
        if ([self.delegate respondsToSelector:@selector(saveVideoToAssetsResult:)]) {
            [self.delegate saveVideoToAssetsResult:error];
        }
    }];
}

- (void)startRecording{
    if(self.pRecordVideo && (!mRecording) ){
        [self.pRecordVideo startRecord];
        mRecording = true;
    }
}

- (void)stopRecording{
    if(self.pRecordVideo && mRecording){
        mRecording = false;
        [self.pRecordVideo finishRecord];
    }
}

- (void)RecordingVideo:(CMSampleBufferRef)sampleBuffer{
    if(self.pRecordVideo && mRecording){
        [self.pRecordVideo setVideoSampleBuffer:sampleBuffer];
    }
}

- (void)RecordingImage:(CMSampleBufferRef)sampleBuffer{
    if (self.pRecordImage) {
        [self.pRecordImage setImageBuffer:sampleBuffer];
    }
}

- (void)RecordingAudio:(CMSampleBufferRef)sampleBuffer{
    if(self.pRecordVideo && mRecording){
        [self.pRecordVideo setAudioSampleBuffer:sampleBuffer];
    }
}

- (void)setVideoFormatDescription:(CMSampleBufferRef)sampleBuffer{
    if(self.pRecordVideo){
        [self.pRecordVideo setVideoFormatDescription:sampleBuffer];
    }
}

- (void)setAudioFormatDescription:(CMSampleBufferRef)sampleBuffer{
    if(self.pRecordVideo){
        [self.pRecordVideo setAudioFormatDescription:sampleBuffer];
    }
}
@end
