//
//  SWOutStreamSampleBuffer.m
//  SVEVideo
//
//  Created by 李晓帆 on 2018/7/5.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import "SWOutStreamSampleBuffer.h"
#import "../swbasic/SWBasicSys.h"
#import "SWDataSourceCamera.h"
@implementation SWOutStreamSampleBuffer

- (void)initModule{
    [super initModule];
    NSURL *imageURL = [[NSURL alloc] initFileURLWithPath:[NSString pathWithComponents:@[NSTemporaryDirectory(), @"showImage.jpg"]]];
    [self.m_pRecordImg setImagedURL:imageURL];
    __weak SWOutStreamSampleBuffer *weakSelf = self;
    self.m_pRecordImg.saveImageToFileResult = ^(NSString *msg) {
        if ([msg isEqualToString:@"success"]) {
            [weakSelf.m_pRecordImg saveImageToAssetsLibrary];
        }else{
            
        }
        
    };
    //
    self.m_pRecordImg.saveImageToAssetsResult = ^(NSError *error) {
        
    };
    
    
    
    //
    if ([[SWBasicSys getInst].m_pDataSrc getDataSrcType] == DATASRCCAMERA) {
        SWDataSourceCamera *camera = (SWDataSourceCamera *)[SWBasicSys getInst].m_pDataSrc;
        [self.m_pRecordVideo setupWithCameraSite: [camera getCamera]];
        NSString *fileName = [NSString stringWithFormat:@"fm2Movie.mp4"];
        NSURL *videoURL = [[NSURL alloc] initFileURLWithPath:[NSString pathWithComponents:@[NSTemporaryDirectory(), fileName]]];
        [self.m_pRecordVideo setReocrdURL:videoURL];

        self.m_pRecordVideo.videoRecorderError = ^(NSError *error) {
            
        };
        
        self.m_pRecordVideo.videoRecorderDidStart = ^{
//            if ([weakSelf.delegate respondsToSelector:@selector(videoRecordDidStart)]) {
//                [weakSelf.delegate videoRecordDidStart];
//            }
            //正在录制
        };
        self.m_pRecordVideo.videoRecorderWillStop = ^{
            //录制将要结束
//            if ([weakSelf.delegate respondsToSelector:@selector(videoRecordWillStop)]) {
//                [weakSelf.delegate videoRecordWillStop];
//            }
        };
        self.m_pRecordVideo.videoRecorderDidStop = ^{
            //        //录制成功并且已经停止
            [weakSelf.m_pRecordVideo saveVideoToAssetsLibrary];
//            if ([weakSelf.delegate respondsToSelector:@selector(videoRecordDidStop)]) {
//                [weakSelf.delegate videoRecordDidStop];
//            }
        };
        
    }
    
}

- (void)destroyModule{
    [super destroyModule];
}

- (void)setOutPutBuffer:(void *)buffer{
    struct opaqueCMSampleBuffer *sampleBuffer = (struct opaqueCMSampleBuffer *)buffer;
    if (self.m_pRecordImg) {
        [self.m_pRecordImg setImageBuffer:sampleBuffer];
    }
    if (self.m_pRecordVideo) {
        [self.m_pRecordVideo setVideoFormatDescription:sampleBuffer];
        [self.m_pRecordVideo setVideoSampleBuffer:sampleBuffer];
    }
}

@end
