//
//  SWDetection.h
//  SVEVideo
//
//  Created by 李晓帆 on 2018/7/7.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import <Foundation/Foundation.h>
@interface SWDetection : NSObject
{
    SWDETECTION m_detectionType;
    unsigned char* m_detectData;
    bool        m_enable;
}
+ (SWDetection *)createDetectionEngine:(SWDETECTION)type;
- (void)initModule;
- (void)detectBuffer:(void *)buffer;
- (SWDETECTION)getDetectionType;
- (void)enableDetect;
- (void)disableDetect;
@end
