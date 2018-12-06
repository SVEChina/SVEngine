//
//  SWARCamera.h
//  SVEVideo
//
//  Created by 李晓帆 on 2018/11/6.
//  Copyright © 2018 付一洲. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ARKit/ARKit.h>
NS_ASSUME_NONNULL_BEGIN
@protocol ARCameraDelegate<NSObject>
@optional
- (void)session:(ARSession *)session updateFrame:(ARFrame *)frame;
@end

@interface SWARCamera : NSObject
@property(weak,nonatomic)id<ARCameraDelegate> delegate;
- (void)setupCamera;
- (void)runCamera;
- (void)pauseCamera;
- (ARSession *)getSession;
@end

NS_ASSUME_NONNULL_END
