//
//  SWDetection.m
//  SVEVideo
//
//  Created by 李晓帆 on 2018/7/7.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import "SWDetection.h"
#import "SWDetectionSenseTime.h"
@interface SWDetection(){
    
}
@end

@implementation SWDetection

+ (SWDetection *)createDetectionEngine:(SWDETECTION)type{
    if (type == SWDETECTSENSETIME) {
        SWDetection *detection = [[SWDetectionSenseTime alloc] init];
        return detection;
    }else if (type == SWDETECTFACEPP){
        
    }
    return nil;
}

- (instancetype)init{
    self = [super init];
    if (self) {
        m_enable = true;
        m_detectionType = SWDETECTSENSETIME;
    }
    return self;
}

- (void)dealloc{
    if (m_detectData) {
        free(m_detectData);
    }
}

- (void)initModule{
    
}

- (void)detectBuffer:(void *)buffer{
    
}

- (SWDETECTION)getDetectionType{
    return m_detectionType;
}

- (void)enableDetect{
    m_enable = true;
}
- (void)disableDetect{
    m_enable = false;
}

@end
