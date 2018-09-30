//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "RecordMedia.h"
#import <AVFoundation/AVFoundation.h>

typedef  void(^SaveImageToAssetsResult)(NSError *error);
typedef  void(^SaveImageToFileResult)(NSString *msg);
@interface RecordImage : RecordMedia
@property(copy,nonatomic)SaveImageToAssetsResult saveImageToAssetsResult;
@property(copy,nonatomic)SaveImageToFileResult saveImageToFileResult;
@property(strong, nonatomic)UIImage *m_image;
- (void)setImagedURL:(NSURL *)url;
- (NSURL*)getImageURL;
- (void)setImageBuffer:(CMSampleBufferRef)sampleBuffer;
- (void)savedImageToFileCompression:(float)compression;
- (void)saveImageToAssetsLibrary;
@end
