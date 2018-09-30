//
//  SWInStreamCamera.m
//  SVEVideo
//
//  Created by 李晓帆 on 2018/7/5.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import "SWInStreamSampleBuffer.h"
#import "../swlogic/SWLogicSys.h"
@interface SWInStreamSampleBuffer(){
    
}
@end

@implementation SWInStreamSampleBuffer

- (instancetype)init{
    self = [super init];
    if (self) {
        
        
    }
    return self;
}

- (void)initModule{
    [super initModule];
}

- (void)destroyModule{
    [super destroyModule];
}

- (void)renderSampleBuffer:(void *)buffer{
    struct opaqueCMSampleBuffer *sampleBuffer = (struct opaqueCMSampleBuffer *)buffer;
    [[SWLogicSys getInst].pSVI.pCamera pushInStream:SCENENAME Img:sampleBuffer];
}


@end
