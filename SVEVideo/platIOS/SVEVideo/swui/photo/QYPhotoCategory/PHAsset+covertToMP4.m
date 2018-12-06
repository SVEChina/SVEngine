//
//  PHAsset+LivePhotoCovertToMP4.m
//  NXFramework
//
//  Created by liuming on 2017/11/2.
//

#import "PHAsset+covertToMP4.h"
#import "NXFileManager.h"

@implementation PHAsset (LivePhotoCovertToMP4)
- (void)getLivePhotoData:(void (^)(PHLivePhoto *data))block
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        PHImageManager *manager = [PHImageManager defaultManager];
        PHLivePhotoRequestOptions *livePhotoOptions = [[PHLivePhotoRequestOptions alloc] init];
        livePhotoOptions.deliveryMode = PHImageRequestOptionsDeliveryModeHighQualityFormat;
        livePhotoOptions.networkAccessAllowed = YES;
        [manager requestLivePhotoForAsset:self
                               targetSize:CGSizeMake(960, 720)
                              contentMode:PHImageContentModeAspectFit
                                  options:livePhotoOptions
                            resultHandler:^(PHLivePhoto *_Nullable livePhoto, NSDictionary *_Nullable info) {
                                
                                NSLog(@"live photo info dic %@", info);
                                
                                if (block) block(livePhoto);
                            }];
        
    });
}
- (void)getLivePhotoOfMP4Data:(void (^)(NSData *data, NSString *filePath, UIImage *coverImage,NSError * error))block
{
    if ([self canOutputVideo])
    {
        __weak typeof(self) weakSelf = self;
        NSString *newFilePath = [NSString
                                 stringWithFormat:@"%@/%@", [self getSaveLivePhotoMP4Path], [[self getOriginalVedioName] lastPathComponent]];
        
        if ([[NSFileManager defaultManager] fileExistsAtPath:newFilePath])
        {
            //存在不进行转换 直接读取
            NSData *data = [NSData dataWithContentsOfURL:[NSURL fileURLWithPath:newFilePath]];
            UIImage *covertImge = nil;
            if (block)
            {
                block(data, newFilePath, covertImge,nil);
            }
        }
        else
        {
            [weakSelf livePhotoData:block];
        }
    }
    else
    {
        NSLog(@"非 livePhoto 或者 video 转换失败");
        if (block)
        {
            NSError * error = [NSError errorWithDomain:@"livePhoto转换出错" code:-10001 userInfo:@{@"errorInfo":@"非 livePhoto 或者 video 转换失败"}];
            block(nil, nil, nil,error);
        }
    }
}
- (void)livePhotoData:(void (^)(NSData *data, NSString *filePath, UIImage *corvetImage,NSError * error))block
{
    NSArray *assetResources = [PHAssetResource assetResourcesForAsset:self];
    PHAssetResource *resource;
    for (PHAssetResource *assetRes in assetResources)
    {
        if (assetRes.type == PHAssetResourceTypePairedVideo || assetRes.type == PHAssetResourceTypeVideo)
        {
            resource = assetRes;
        }
    }
    if (resource == nil)
    {
        if (block)
        {
            NSError * error = [NSError errorWithDomain:@"livePhoto资源文件获取失败" code:-10002 userInfo:@{@"errorInfo":@"livePhoto资源文件获取失败"}];
            block(nil, @"", nil,error);
        }
        return;
    }
    
    NSString *fileName = @"tempAssetVideo.mov";
    if (resource.originalFilename)
    {
        fileName = resource.originalFilename;
    }
    if ([self canOutputVideo])
    {
        [NXFileManager validateDir:[self getSaveLivePhotoMP4Path]];
        
        PHAssetResourceRequestOptions *options = [[PHAssetResourceRequestOptions alloc] init];
        options.networkAccessAllowed = NO;
        options.progressHandler = ^(double progress) {
            
            NSLog(@"export progress -> %f", progress);
        };
        NSString *path_movie_file = [[self getSaveLivePhotoMP4Path] stringByAppendingPathComponent:fileName];
        [[NSFileManager defaultManager] removeItemAtPath:path_movie_file error:nil];
        [[PHAssetResourceManager defaultManager] writeDataForAssetResource:resource
                                                                    toFile:[NSURL fileURLWithPath:path_movie_file]
                                                                   options:options
                                                         completionHandler:^(NSError *_Nullable error) {
                                                             if (error)
                                                             {
                                                                 block(nil, nil, nil,error);
                                                             }
                                                             else
                                                             {
                                                                 if (block)
                                                                 {
                                                                     block(nil, path_movie_file, nil,nil);
                                                                 }
                                                             }
                                                         }];
    }
    else
    {
        NSLog(@"非 livePhoto 或者 video 转换失败");
        NSError * error = [NSError errorWithDomain:@"livePhoto转换出错" code:-10001 userInfo:@{@"errorInfo":@"非 livePhoto 或者 video 转换失败"}];
        block(nil, nil, nil,error);
    }
}

- (BOOL)encodeVideo:(NSString *)videoURL
          withBlock:(void (^)(NSData *data, NSString *filePath, UIImage *covertImage))block
{
    NSString *fileName = [videoURL lastPathComponent];
    
    [NXFileManager validateDir:[self getSaveLivePhotoMP4Path]];
    
    NSString *exportPath = [NSString stringWithFormat:@"%@/export%@", [self getSaveLivePhotoMP4Path], fileName];
    NSLog(@"文件路径 -- %@", exportPath);
    if (![[NSFileManager defaultManager] fileExistsAtPath:exportPath])
    {
        NSURL *url = [NSURL fileURLWithPath:[videoURL stringByExpandingTildeInPath]];
        AVURLAsset *asset = [[AVURLAsset alloc] initWithURL:url options:nil];
        
        UIImage *covertImage = [self getVideoCovertImage:videoURL];
        AVMutableComposition *composition = [AVMutableComposition composition];
        AVMutableCompositionTrack *videoTrack =
        [composition addMutableTrackWithMediaType:AVMediaTypeVideo preferredTrackID:kCMPersistentTrackID_Invalid];
        AVMutableCompositionTrack *audioTrack =
        [composition addMutableTrackWithMediaType:AVMediaTypeAudio preferredTrackID:kCMPersistentTrackID_Invalid];
        NSArray *assetVideoTracks = [asset tracksWithMediaType:AVMediaTypeVideo];
        if (assetVideoTracks.count <= 0)
        {
            NSLog(@"Error reading the transformed video track");
            return NO;
        }
        
        // Insert the tracks in the composition's tracks
        AVAssetTrack *assetVideoTrack = [assetVideoTracks firstObject];
        [videoTrack insertTimeRange:assetVideoTrack.timeRange
                            ofTrack:assetVideoTrack
                             atTime:CMTimeMake(0, 1)
                              error:nil];
        [videoTrack setPreferredTransform:assetVideoTrack.preferredTransform];
        
        AVAssetTrack *assetAudioTrack = [[asset tracksWithMediaType:AVMediaTypeAudio] objectAtIndex:0];
        [audioTrack insertTimeRange:assetAudioTrack.timeRange
                            ofTrack:assetAudioTrack
                             atTime:CMTimeMake(0, 1)
                              error:nil];
        
        double sysVersion = [[[UIDevice currentDevice] systemVersion] doubleValue];
        // Export to mp4
        NSString *mp4Quality = sysVersion > 6.0 ? AVAssetExportPresetHighestQuality : AVAssetExportPresetPassthrough;
        
        NSURL *exportUrl = [NSURL fileURLWithPath:exportPath];
        AVAssetExportSession *exportSession =
        [[AVAssetExportSession alloc] initWithAsset:composition presetName:mp4Quality];
        exportSession.outputURL = exportUrl;
        CMTime start = CMTimeMakeWithSeconds(0.0, 0);
        CMTimeRange range = CMTimeRangeMake(start, [asset duration]);
        exportSession.timeRange = range;
        exportSession.outputFileType = AVFileTypeMPEG4;
        [exportSession exportAsynchronouslyWithCompletionHandler:^{
            switch ([exportSession status])
            {
                case AVAssetExportSessionStatusCompleted:
                {
                    NSLog(@"MP4 Successful!");
                    
                    NSData *data = [NSData dataWithContentsOfURL:[NSURL fileURLWithPath:exportPath]];
                    if (block)
                    {
                        block(data, exportPath, covertImage);
                    }
                }
                    break;
                case AVAssetExportSessionStatusFailed:
                {
                    NSLog(@"Export failed: %@", [[exportSession error] localizedDescription]);
                    
                    if (block)
                    {
                        block(nil, nil, nil);
                    }
                }
                    
                    break;
                case AVAssetExportSessionStatusCancelled:
                {
                    NSLog(@"Export canceled");
                    if (block)
                    {
                        block(nil, nil, nil);
                    }
                }
                    break;
                default:
                {
                    if (block)
                    {
                        block(nil, nil, nil);
                    }
                }
                    break;
            }
        }];
    }
    else
    {
        NSLog(@"文件已经存在!不需要转格式");
        NSData *data = [NSData dataWithContentsOfURL:[NSURL fileURLWithPath:exportPath]];
        if (block)
        {
            UIImage *covertImage = [self getVideoCovertImage:exportPath];
            block(data, exportPath, covertImage);
        }
    }
    
    return YES;
}

- (UIImage *)getVideoCovertImage:(NSString *)filePath
{
    NSURL *url = [NSURL fileURLWithPath:filePath];
    AVURLAsset *asset = [[AVURLAsset alloc] initWithURL:url options:nil];
    
    AVAssetImageGenerator *gen = [[AVAssetImageGenerator alloc] initWithAsset:asset];
    
    gen.appliesPreferredTrackTransform = YES;
    
    CMTime time = CMTimeMakeWithSeconds(0.0, 600);
    
    NSError *error = nil;
    
    CMTime actualTime;
    
    CGImageRef image = [gen copyCGImageAtTime:time actualTime:&actualTime error:&error];
    
    UIImage *thumbImg = [[UIImage alloc] initWithCGImage:image];
    
    CFRelease(image);
    return thumbImg;
}
- (NSString *)getSaveLivePhotoMP4Path
{
    NSString *path = [NSString stringWithFormat:@"%@/livePhotoMP4", [NXFileManager getCacheDir]];
    return path;
}

- (PHAssetResource *)getVideoSource
{
    NSArray *assetResources = [PHAssetResource assetResourcesForAsset:self];
    PHAssetResource *resource;
    
    for (PHAssetResource *assetRes in assetResources)
    {
        if (assetRes.type == PHAssetResourceTypePairedVideo || assetRes.type == PHAssetResourceTypeVideo)
        {
            resource = assetRes;
        }
    }
    
    return resource;
}
- (NSString *)getOriginalVedioName
{
    PHAssetResource *resource = [self getVideoSource];
    NSString *fileName = @"tempAssetVideo.mov";
    if (resource.originalFilename)
    {
        fileName = resource.originalFilename;
    }
    
    return fileName;
}

- (BOOL)canOutputVideo
{
    return self.mediaType == PHAssetMediaTypeVideo || self.mediaSubtypes == PHAssetMediaSubtypePhotoLive ||
    self.mediaSubtypes == (PHAssetMediaSubtypePhotoLive | PHAssetMediaSubtypePhotoHDR);
}

- (BOOL)checkHasVideoSource { return [self getVideoSource] == nil ? NO : YES; }
- (void)exportVideo:(void (^)(NSString *path))result progress:(void (^)(float progress))progress
{
    if (![NXFileManager validateDir:[self getSaveLivePhotoMP4Path]])
    {
        [NXFileManager createDirectoriesForPath:[self getSaveLivePhotoMP4Path]];
    }
    NSString *dateString = [NSString stringWithFormat:@"%f", [[NSDate date] timeIntervalSince1970]];
    NSString *exportPath = [NSString stringWithFormat:@"%@/export%@.mp4", [self getSaveLivePhotoMP4Path], dateString];
    NSURL *tempFileUrl = [NSURL fileURLWithPath:exportPath];
    
    PHVideoRequestOptions *options = [[PHVideoRequestOptions alloc] init];
    options.networkAccessAllowed = NO;
    options.deliveryMode = PHVideoRequestOptionsDeliveryModeHighQualityFormat;
    options.version = PHVideoRequestOptionsVersionOriginal;
    [[PHImageManager defaultManager]
     requestExportSessionForVideo:self
     options:options
     exportPreset:AVAssetExportPresetPassthrough
     resultHandler:^(AVAssetExportSession *_Nullable exportSession, NSDictionary *_Nullable info) {
         NSLog(@"Video test run on asset %@", self.localIdentifier);
         
         [exportSession setOutputFileType:AVFileTypeMPEG4];
         [exportSession setOutputURL:tempFileUrl];
         [exportSession exportAsynchronouslyWithCompletionHandler:^{
             
             switch ([exportSession status])
             {
                 case AVAssetExportSessionStatusCompleted:
                 {
                     NSLog(@"MP4 Successful!");
                     
                     if (result)
                     {
                         result(exportPath);
                     }
                 }
                     break;
                 case AVAssetExportSessionStatusFailed:
                 {
                     NSLog(@"Export failed: %@", [[exportSession error] localizedDescription]);
                     
                     if (result)
                     {
                         result(nil);
                     }
                 }
                     
                     break;
                 case AVAssetExportSessionStatusCancelled:
                 {
                     NSLog(@"Export canceled");
                     if (result)
                     {
                         result(nil);
                     }
                 }
                     break;
                 default:
                 {
                     if (result)
                     {
                         result(nil);
                     }
                 }
                     break;
             }
         }];
         while (exportSession.status == AVAssetExportSessionStatusExporting)
         {
             if (progress)
             {
                 dispatch_sync(dispatch_get_main_queue(), ^{
                     if (progress)
                     {
                         progress(exportSession.progress);
                     }
                 });
             }
         }
     }];
}
@end

