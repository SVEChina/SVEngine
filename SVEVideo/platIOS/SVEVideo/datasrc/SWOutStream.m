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
#import "src/app/SVInst.h"
#import "src/basesys/SVStreamOut.h"
#import "src/operate/SVOpOutCamera.h"
#import "src/work/SVThreadMain.h"
#import "src/work/SVThreadPool.h"
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
    void *app = [[SWLogicSys getInst] getEngineInst];
    SVInst* t_app = (SVInst*)app;
    if (t_app) {
        SVOpOpenIOSOutstreamPtr t_op = MakeSharedPtr<SVOpOpenIOSOutstream>(t_app, data_out_stream);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }
}

- (void)closeStream {
    if (s_outDataBuffer) {
        free(s_outDataBuffer);
    }
    self.m_outStreaming = false;
    SVInst* t_app = (SVInst*)[[SWLogicSys getInst] getEngineInst];
    if (t_app) {
        SVOpCloseIOSOutstreamPtr t_op = MakeSharedPtr<SVOpCloseIOSOutstream>(t_app);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }
}

@end
