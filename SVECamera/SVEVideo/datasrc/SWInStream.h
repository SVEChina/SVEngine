//
//  SWInStream.h
//  SVEVideo
//
//  Created by 李晓帆 on 2018/7/5.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
@interface SWInStream : NSObject
+ (SWInStream *)createInStream:(SWSTREAMTYPE)type;
- (void)initModule;
- (void)destroyModule;
- (void)renderSampleBuffer:(void *)buffer;
@end
