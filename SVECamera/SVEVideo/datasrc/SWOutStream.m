//
//  SWOutStream.m
//  SVEVideo
//
//  Created by 李晓帆 on 2018/7/5.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import "SWOutStream.h"
#import "SWOutStreamSampleBuffer.h"
#import "SWOutStreamBuffer.h"
#import "../swlogic/SWLogicSys.h"
unsigned char *s_outDataBuffer;
@implementation SWOutStream

void data_out_stream(int _w,int _h,int _formate,void* _pdata,long _timtap) {
    if (s_outDataBuffer != NULL) {
        memcpy(s_outDataBuffer, _pdata, _w*_h*4);
    }
}

+ (SWOutStream *)createOutStream:(SWSTREAMTYPE)type{
    if (type == SWSTREAMSAMPLE) {
        return [[SWOutStreamSampleBuffer alloc] init];
    }else if (type == SWSTREAMBUFFER){
        return [[SWOutStreamBuffer alloc] init];
    }
    return nil;
}

- (instancetype)init{
    self = [super init];
    if (self) {
        s_outDataBuffer = NULL;
        self.m_outStreaming = false;
        self.m_pRecordImg = nil;
        self.m_pRecordVideo = nil;
    }
    return self;
}

- (void)initModule{
    self.m_pRecordImg = [[RecordImage alloc] init];
    self.m_pRecordVideo = [[RecordVideo alloc] init];
}

- (void)destroyModule{
    
}

- (void)setOutPutBuffer:(void *)buffer{
    
}

- (void)openStream {
    if (s_outDataBuffer == NULL) {
        s_outDataBuffer = (unsigned char *)malloc([SWLogicSys getInst].pSWState.svOutW*[SWLogicSys getInst].pSWState.svOutH*4);
    }
    self.m_outStreaming = true;
    [[SWLogicSys getInst].pSVI.pCamera openOutStream:data_out_stream Mode:1];
}

- (void)closeStream {
    if (s_outDataBuffer) {
        free(s_outDataBuffer);
    }
    self.m_outStreaming = false;
    [[SWLogicSys getInst].pSVI.pCamera closeOutStream];
}

@end
