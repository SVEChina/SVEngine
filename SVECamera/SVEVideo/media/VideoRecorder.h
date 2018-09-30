//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreMedia/CMFormatDescription.h>
#import <CoreMedia/CMSampleBuffer.h>
@class VideoRecorder;
@protocol VideoRecorderDelegate <NSObject>
@required
- (void)movieRecorderDidFinishPreparing:(VideoRecorder *)recorder;
- (void)movieRecorder:(VideoRecorder *)recorder didFailWithError:(NSError *)error;
- (void)movieRecorderDidFinishRecording:(VideoRecorder *)recorder;
@end

@interface VideoRecorder : NSObject
- (instancetype)initWithURL:(NSURL *)URL delegate:(id<VideoRecorderDelegate>)delegate callbackQueue:(dispatch_queue_t)queue; // delegate is weak referenced

// Only one audio and video track each are allowed.
- (void)addVideoTrackWithSourceFormatDescription:(CMFormatDescriptionRef)formatDescription transform:(CGAffineTransform)transform settings:(NSDictionary *)videoSettings; // see AVVideoSettings.h for settings keys/values
- (void)addAudioTrackWithSourceFormatDescription:(CMFormatDescriptionRef)formatDescription settings:(NSDictionary *)audioSettings; // see AVAudioSettings.h for settings keys/values

// Asynchronous, might take several hundred milliseconds.
// When finished the delegate's recorderDidFinishPreparing: or recorder:didFailWithError: method will be called.
- (void)prepareToRecord;

//- (void)appendVideoSampleBuffer:(CMSampleBufferRef)sampleBuffer;
- (void)appendVideoPixelBuffer:(CVPixelBufferRef)pixelBuffer withPresentationTime:(CMTime)presentationTime;
- (void)appendAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer;

// Asynchronous, might take several hundred milliseconds.
// When finished the delegate's recorderDidFinishRecording: or recorder:didFailWithError: method will be called.
- (void)finishRecording;


@end
