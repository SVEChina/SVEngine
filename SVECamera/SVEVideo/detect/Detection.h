//
//  Detection.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>




@interface Detection : NSObject
- (void)createSTEngine;
-(unsigned char *) detectFace:(CMSampleBufferRef)sampleBuffer;
@end
