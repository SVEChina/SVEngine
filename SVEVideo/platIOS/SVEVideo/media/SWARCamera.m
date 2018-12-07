//
//  SWARCamera.m
//  SVEVideo
//
//  Created by 李晓帆 on 2018/11/6.
//  Copyright © 2018 付一洲. All rights reserved.
//

#import "SWARCamera.h"
@interface SWARCamera()<ARSessionDelegate>{
    ARSession *m_session;
}
@end

@implementation SWARCamera
- (instancetype)init{
    self = [super init];
    if (self) {
        
    }
    return self;
}

- (void)setupCamera{
    if (@available(iOS 11.0, *)) {
        m_session = [ARSession new];
        m_session.delegate = self;
    }
}


- (void)runCamera {
    if (@available(iOS 11.0, *)) {
//        AROrientationTrackingConfiguration
        ARWorldTrackingConfiguration *config = [ARWorldTrackingConfiguration new];
        NSArray *formats = ARWorldTrackingConfiguration.supportedVideoFormats;
        for (NSInteger i=0; i<formats.count; i++) {
            ARVideoFormat *format = formats[i];
            CGSize imageSize = format.imageResolution;
            if (imageSize.width == 1280) {
                config.videoFormat = format;
            }
            int a = 0;
        }
//        config.planeDetection = ARPlaneDetectionHorizontal;
        [m_session runWithConfiguration:config];
    }
}

- (void)pauseCamera {
    if (@available(iOS 11.0, *)) {
        [m_session pause];
    }
}

- (ARSession *)getSession{
    return m_session;
}

- (void)session:(ARSession *)session didUpdateFrame:(ARFrame *)frame {
    if ([self.delegate respondsToSelector:@selector(session:updateFrame:)]) {
        [self.delegate session:session updateFrame:frame];
    }
}
@end
