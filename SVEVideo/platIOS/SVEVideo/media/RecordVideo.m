//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "RecordVideo.h"
#import <AVFoundation/AVFoundation.h>
#import <AssetsLibrary/AssetsLibrary.h>
#import "Camera.h"
typedef NS_ENUM( NSInteger,VideoRecordingStatus )
{
    VideoRecordingStatusIdle = 0,
    VideoRecordingStatusStartingRecording,
    VideoRecordingStatusRecording,
    VideoRecordingStatusStoppingRecording,
}; // internal state machine
@interface RecordVideo()<VideoRecorderDelegate>
{
    VideoRecordingStatus _recordingStatus;
    NSURL                *_recordingURL;
    VideoRecorder        *_recorder;
    NSDictionary *_videoCompressionSettings;
    NSDictionary *_audioCompressionSettings;
    AVCaptureVideoOrientation _videoBufferOrientation;
}
@property(atomic) AVCaptureVideoOrientation recordingOrientation;
// Because we specify __attribute__((NSObject)) ARC will manage the lifetime of the backing ivars even though they are CF types.
@property(nonatomic, strong) __attribute__((NSObject)) CVPixelBufferRef currentPreviewPixelBuffer;
@property(nonatomic, strong) __attribute__((NSObject)) CMFormatDescriptionRef outputVideoFormatDescription;
@property(nonatomic, strong) __attribute__((NSObject)) CMFormatDescriptionRef outputAudioFormatDescription;
@end
@implementation RecordVideo

- (instancetype)init{
    self = [super init];
    if (self) {
        _recordingOrientation = AVCaptureVideoOrientationPortrait;
        self.outputVideoFormatDescription = NULL;
        self.outputAudioFormatDescription = NULL;
    }
    return self;
}

- (void)setVideoFormatDescription:(CMSampleBufferRef)sampleBuffer{
    if ( self.outputVideoFormatDescription == NULL ) {
        // Don't render the first sample buffer.
        // This gives us one frame interval (33ms at 30fps) for setupVideoPipelineWithInputFormatDescription: to complete.
        // Ideally this would be done asynchronously to ensure frames don't back up on slower devices.
        CMFormatDescriptionRef formatDescription = CMSampleBufferGetFormatDescription( sampleBuffer );
        self.outputVideoFormatDescription = formatDescription;
    }
}

- (void)setAudioFormatDescription:(CMSampleBufferRef)sampleBuffer{
    if (self.outputAudioFormatDescription == NULL) {
        CMFormatDescriptionRef formatDescription = CMSampleBufferGetFormatDescription( sampleBuffer );
        self.outputAudioFormatDescription = formatDescription;
    }
}

- (void)setVideoSampleBuffer:(CMSampleBufferRef)sampleBuffer{
    CMTime timestamp = CMSampleBufferGetPresentationTimeStamp( sampleBuffer );
    CVPixelBufferRef pixelBuffer = CMSampleBufferGetImageBuffer( sampleBuffer );
    if (pixelBuffer) {
        if ( _recordingStatus == VideoRecordingStatusRecording ) {
            [_recorder appendVideoPixelBuffer:pixelBuffer withPresentationTime:timestamp];
        }
    }
    
}

- (void)setAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer{
   
    if ( _recordingStatus == VideoRecordingStatusRecording ) {
        [_recorder appendAudioSampleBuffer:sampleBuffer];
    }
}

- (void)setupWithCameraSite:(Camera *)camera {
#ifdef RECORD_AUDIO
    _audioCompressionSettings = [[camera.audioOutput recommendedAudioSettingsForAssetWriterWithOutputFileType:AVFileTypeMPEG4] copy];
#endif
    NSMutableDictionary *t_dic = [[NSMutableDictionary alloc]initWithDictionary:[camera.videoOutput recommendedVideoSettingsForAssetWriterWithOutputFileType:AVFileTypeMPEG4]];
    
     [t_dic setValue:camera.videoOutput.videoSettings[@"Width"] forKey:AVVideoWidthKey];
     [t_dic setValue:camera.videoOutput.videoSettings[@"Height"] forKey:AVVideoHeightKey];
     _videoCompressionSettings = [[NSDictionary alloc] initWithDictionary:t_dic];
//   _videoCompressionSettings = [[camera.videoOutput recommendedVideoSettingsForAssetWriterWithOutputFileType:AVFileTypeQuickTimeMovie] copy];
    AVCaptureConnection *videoConnection = [camera.videoOutput connectionWithMediaType:AVMediaTypeVideo];
    _videoBufferOrientation = videoConnection.videoOrientation;
}

- (void)setReocrdURL:(NSURL *)url{
    _recordingURL = url;
}

- (NSURL *)getRecordUrl{
    return _recordingURL;
}

- (void)startRecord{
    dispatch_async(dispatch_get_main_queue(), ^{
        [self startRecording];
    });
}

- (void)finishRecord{
    dispatch_async(dispatch_get_main_queue(), ^{
        [self finishRecording];
    });
}

- (void)startRecording{
    if (self.outputVideoFormatDescription == NULL) {
        NSLog(@"outputVideoFormatDescription 为空");
        return;
    }
#ifdef RECORD_AUDIO
    if (self.outputAudioFormatDescription == NULL) {
        NSLog(@"outputAudioFormatDescription 为空");
        return;
    }
#endif
    @synchronized( self )
    {
        if ( _recordingStatus != VideoRecordingStatusIdle ) {
            @throw [NSException exceptionWithName:NSInternalInconsistencyException reason:@"Already recording" userInfo:nil];
            return;
        }
        
        [self updateRecordingStatus:VideoRecordingStatusStartingRecording error:nil];
    }
    
    dispatch_queue_t callbackQueue = dispatch_queue_create( "com.apple.sample.recordvideo.recordercallback", DISPATCH_QUEUE_SERIAL ); // guarantee ordering of callbacks with a serial queue
    VideoRecorder *recorder = [[VideoRecorder alloc] initWithURL:_recordingURL delegate:self callbackQueue:callbackQueue];
    
#ifdef RECORD_AUDIO
    [recorder addAudioTrackWithSourceFormatDescription:self.outputAudioFormatDescription settings:_audioCompressionSettings];
#endif // RECORD_AUDIO
    
    CGAffineTransform videoTransform = [self transformFromVideoBufferOrientationToOrientation:self.recordingOrientation withAutoMirroring:NO];
    [recorder addVideoTrackWithSourceFormatDescription:self.outputVideoFormatDescription transform:videoTransform settings:_videoCompressionSettings];
    _recorder = recorder;
    
    [recorder prepareToRecord]; // asynchronous, will call us back with recorderDidFinishPreparing: or recorder:didFailWithError: when done
}

// Auto mirroring: Front camera is mirrored; back camera isn't
- (CGAffineTransform)transformFromVideoBufferOrientationToOrientation:(AVCaptureVideoOrientation)orientation withAutoMirroring:(BOOL)mirror
{
    CGAffineTransform transform = CGAffineTransformIdentity;
    
    // Calculate offsets from an arbitrary reference orientation (portrait)
    CGFloat orientationAngleOffset = angleOffsetFromPortraitOrientationToOrientation( orientation );
    CGFloat videoOrientationAngleOffset = angleOffsetFromPortraitOrientationToOrientation( _videoBufferOrientation );
    
    // Find the difference in angle between the desired orientation and the video orientation
    CGFloat angleOffset = orientationAngleOffset - videoOrientationAngleOffset;
    transform = CGAffineTransformMakeRotation( angleOffset );
//    transform = CGAffineTransformRotate( transform, M_PI_2 );
//    if ( _videoDevice.position == AVCaptureDevicePositionFront )
//    {
//        if ( mirror ) {
//            transform = CGAffineTransformScale( transform, -1, 1 );
//        }
//        else {
//            if ( UIInterfaceOrientationIsPortrait( (UIInterfaceOrientation)orientation ) ) {
//                transform = CGAffineTransformRotate( transform, M_PI );
//            }
//        }
//    }
    
    return transform;
}

static CGFloat angleOffsetFromPortraitOrientationToOrientation(AVCaptureVideoOrientation orientation)
{
    CGFloat angle = 0.0;
    
    switch ( orientation )
    {
        case AVCaptureVideoOrientationPortrait:
            angle = 0.0;
            break;
        case AVCaptureVideoOrientationPortraitUpsideDown:
            angle = M_PI;
            break;
        case AVCaptureVideoOrientationLandscapeRight:
            angle = -M_PI_2;
            break;
        case AVCaptureVideoOrientationLandscapeLeft:
            angle = M_PI_2;
            break;
        default:
            break;
    }
    
    return angle;
}

// call under @synchonized( self )
- (void)updateRecordingStatus:(VideoRecordingStatus)newStatus error:(NSError *)error
{
    VideoRecordingStatus oldStatus = _recordingStatus;
    _recordingStatus = newStatus;
    
#if LOG_STATUS_TRANSITIONS
//    NSLog( @"RosyWriterCapturePipeline recording state transition: %@->%@", [self stringForRecordingStatus:oldStatus], [self stringForRecordingStatus:newStatus] );
#endif
    
    if ( newStatus != oldStatus )
    {
        if ( error && ( newStatus == VideoRecordingStatusIdle ) )
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                if (self.videoRecorderError) {
                    self.videoRecorderError(error);
                }
            });
          
        }
        else
        {
            if ( ( oldStatus == VideoRecordingStatusStartingRecording ) && ( newStatus == VideoRecordingStatusRecording ) ) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    if (self.videoRecorderDidStart) {
                        self.videoRecorderDidStart();
                    }
                });
               
            }
            else if ( ( oldStatus == VideoRecordingStatusRecording ) && ( newStatus == VideoRecordingStatusStoppingRecording ) ) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    if (self.videoRecorderWillStop) {
                        self.videoRecorderWillStop();
                    }
                });
            }
            else if ( ( oldStatus == VideoRecordingStatusStoppingRecording ) && ( newStatus == VideoRecordingStatusIdle ) ) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    if (self.videoRecorderDidStop) {
                        self.videoRecorderDidStop();
                    }
                });
                
            }
        }
    }
}

- (void)finishRecording{
    @synchronized( self )
    {
        if ( _recordingStatus != VideoRecordingStatusRecording ) {
            return;
        }
        [self updateRecordingStatus:VideoRecordingStatusStoppingRecording error:nil];
    }
    [_recorder finishRecording]; // asynchronous, will call us back with recorderDidFinishRecording: or recorder:didFailWithError: when done
}

#pragma mark MovieRecorder Delegate
- (void)movieRecorderDidFinishPreparing:(VideoRecorder *)recorder{
    @synchronized( self )
    {
        if ( _recordingStatus != VideoRecordingStatusStartingRecording ) {
            @throw [NSException exceptionWithName:NSInternalInconsistencyException reason:@"Expected to be in StartingRecording state" userInfo:nil];
            return;
        }
        [self updateRecordingStatus:VideoRecordingStatusRecording error:nil];
    }
}

- (void)movieRecorder:(VideoRecorder *)recorder didFailWithError:(NSError *)error{
    @synchronized( self )
    {
        _recorder = nil;
        [self updateRecordingStatus:VideoRecordingStatusIdle error:nil];
    }
}

- (void)movieRecorderDidFinishRecording:(VideoRecorder *)recorder{
    @synchronized( self )
    {
        if ( _recordingStatus != VideoRecordingStatusStoppingRecording ) {
            @throw [NSException exceptionWithName:NSInternalInconsistencyException reason:@"Expected to be in StoppingRecording state" userInfo:nil];
            return;
        }
        
        // No state transition, we are still in the process of stopping.
        // We will be stopped once we save to the assets library.
    }
    
    _recorder = nil;
    @synchronized( self )
    {
        if ( _recordingStatus != VideoRecordingStatusStoppingRecording ) {
            @throw [NSException exceptionWithName:NSInternalInconsistencyException reason:@"Expected to be in StoppingRecording state" userInfo:nil];
            return;
        }
        [self updateRecordingStatus:VideoRecordingStatusIdle error:nil];
    }
   
}

- (void)saveVideoToAssetsLibrary{
    ALAssetsLibrary *library = [[ALAssetsLibrary alloc] init];
    [library writeVideoAtPathToSavedPhotosAlbum:[self getRecordUrl] completionBlock:^(NSURL *assetURL, NSError *error) {
        
    }];
}

@end
