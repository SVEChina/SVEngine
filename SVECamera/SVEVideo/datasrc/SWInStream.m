//
//  SWInStream.m
//  SVEVideo
//
//  Created by 李晓帆 on 2018/7/5.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import "SWInStream.h"
#import "SWInStreamSampleBuffer.h"
#import "SWInStreamBuffer.h"
@implementation SWInStream

+ (SWInStream *)createInStream:(SWSTREAMTYPE)type{
    if (type == SWSTREAMSAMPLE) {
        return [[SWInStreamSampleBuffer alloc] init];
    }else if (type == SWSTREAMBUFFER){
        return [[SWInStreamBuffer alloc] init];
    }
    return nil;
}

- (instancetype)init{
    self = [super init];
    if (self) {
        
        
    }
    return self;
}

- (void)dealloc {
    
}

- (void)initModule {
    
}

- (void)destroyModule{
    
}

- (void)renderSampleBuffer:(void *)buffer{
    
}

@end
