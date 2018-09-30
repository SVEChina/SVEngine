//
//  SWMediaPlayer.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWMediaPlayer.h"
#import <MediaPlayer/MediaPlayer.h>

@interface SWMediaPlayer(){
//    AVPlayer *pPlayer;
//    AVPlayerItem *pPlayerItem;
//    AVPlayerLayer *pPlayerLayer;
}
@end

@implementation SWMediaPlayer

- (void)initModule{
    
}

- (void)destroyModule{
//    [self stop];
//    pPlayer = nil;
}

- (void)preparePlay:(NSURL*)_url  Frame:(CGRect)_frame View:(UIView*)_view{
    //NSURL *url = [NSURL fileURLWithPath:videoFilePath];
    NSMutableDictionary* t_set_dic = [[NSMutableDictionary alloc] init];
    AVURLAsset *t_media_set = [AVURLAsset URLAssetWithURL:_url options:t_set_dic];
    NSError *error = nil;
    AVAssetReader *asset_reader = [[AVAssetReader alloc] initWithAsset:t_media_set error:&error];
    AVAssetTrack* video_track=[[t_media_set tracksWithMediaType:AVMediaTypeVideo] objectAtIndex:0];
    NSMutableDictionary *t_track_dic = [[NSMutableDictionary alloc]init];
    [t_track_dic setObject: [NSNumber numberWithInt:kCVPixelFormatType_32BGRA]
                   forKey:(NSString *)kCVPixelBufferPixelFormatTypeKey];
    AVAssetReaderTrackOutput *asset_reader_output=[ [AVAssetReaderTrackOutput alloc] initWithTrack:video_track outputSettings:t_track_dic];
    
    if([asset_reader canAddOutput:asset_reader_output]){
        [asset_reader addOutput:asset_reader_output];
    }
    //
    if([asset_reader startReading]) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
            CMSampleBufferRef buffer=NULL;
            while ( [asset_reader status]==AVAssetReaderStatusReading ) {
                buffer=[asset_reader_output copyNextSampleBuffer];
                if(buffer){
                    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(buffer);
                    CVPixelBufferLockBaseAddress(imageBuffer,0);
                    uint8_t *baseAddress = (uint8_t *)CVPixelBufferGetBaseAddress(imageBuffer);
                    size_t bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer);
                    size_t width = CVPixelBufferGetWidth(imageBuffer);
                    size_t height = CVPixelBufferGetHeight(imageBuffer);
                    CVPixelBufferUnlockBaseAddress(imageBuffer,0);
//                    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
//                    CGContextRef newContext = CGBitmapContextCreate(baseAddress,
//                                                                    width, height,
//                                                                    8,
//                                                                    bytesPerRow,
//                                                                    colorSpace,
//                                                                    kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst);
//                    CGImageRef newImage = CGBitmapContextCreateImage(newContext);
//                    CGContextRelease(newContext);
//                    CGColorSpaceRelease(colorSpace);
//                    //                    [layer performSelectorOnMainThread:@selector(setContents:) withObject:(id)newImage waitUntilDone:YES];
//                    CFRelease(newImage);
//                    CFRelease(buffer);
                }
                [NSThread sleepForTimeInterval:0.035];
            }
        });
    }
    
//    if(pPlayer){
//        [self close];
//    }
//    if(_view){
//        // 2.创建AVPlayerItem
//        pPlayerItem = [AVPlayerItem playerItemWithURL:_url];
//        // 3.创建AVPlayer
//        pPlayer = [AVPlayer playerWithPlayerItem:pPlayerItem];
//        // 4.添加AVPlayerLayer
//        //UI
//        pPlayerLayer = [AVPlayerLayer playerLayerWithPlayer:pPlayer];
//        pPlayerLayer.frame = _frame;
//        pPlayerLayer.backgroundColor = [UIColor clearColor].CGColor;
//        [_view.layer insertSublayer:pPlayerLayer atIndex:0];
//        [self addNotification];
//        [self play];
//    }
}

- (void)close{
//    //
//    [self removeNotification];
//    //
//    if(pPlayerLayer && pPlayer){
//        [pPlayerLayer removeFromSuperlayer];
//        [pPlayer pause];
//        pPlayer = nil;
//        pPlayerLayer = nil;
//    }
}

- (void)play{
//    if(pPlayer){
//        [pPlayer play];
//    }
}

- (void)pause{
//    if(pPlayer){
//        [pPlayer pause];
//    }
}

- (void)stop{
//    if(pPlayer){
//        [pPlayer pause];
//    }
}

/**
 *  添加播放器通知
 */
-(void)addNotification{
//    //给AVPlayerItem添加播放完成通知
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playbackFinished:) name:AVPlayerItemDidPlayToEndTimeNotification object:pPlayer.currentItem];
}

-(void)removeNotification{
//    [[NSNotificationCenter defaultCenter] removeObserver:self];
}
/**
 *  播放完成通知
 *
 *  @param notification 通知对象
 */
-(void)playbackFinished:(NSNotification *)notification{
//    NSLog(@"视频播放完成.");
//    // 播放完成后重复播放
//    // 跳到最新的时间点开始播放
//    [pPlayer seekToTime:CMTimeMake(0, 1)];
//    [pPlayer play];
}

- (void)doit:(NSURL *)moviePath{
    
//    static CALayer *layer=nil;
//
//    if(!layer){
//
//        layer=[CALayer layer];
//
//        layer.frame=CGRectMake(20, 0, 320, 320);
//
//        layer.contentsGravity=kCAGravityBottomLeft;
//
//        layer.contentsScale=[UIScreen mainScreen].scale;
//
//        [self.view.layer addSublayer:layer];
//
//        [CATransaction setDisableActions:YES];
//
//    }
}


@end

