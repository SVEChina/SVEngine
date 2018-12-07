//
//  SWInStreamCamera.m
//  SVEVideo
//
//  Created by 李晓帆 on 2018/7/5.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import "SWInStreamSampleBuffer.h"
#import "../swlogic/SWLogicSys.h"
#import "src/app/SVInst.h"
#import "src/basesys/SVStreamOut.h"
#import "src/operate/SVOpOutCamera.h"
#import "src/work/SVThreadMain.h"
#import "src/work/SVThreadPool.h"
#import "src/basesys/SVBasicSys.h"
#import "src/basesys/SVStreamIn.h"
#import "src/basesys/SVStreamOut.h"
#import "src/operate/SVOpOutCamera.h"
@interface SWInStreamSampleBuffer(){
    GLubyte *m_frame_cpy;
}
@end

@implementation SWInStreamSampleBuffer

- (instancetype)init{
    self = [super init];
    if (self) {
        m_frame_cpy = NULL;
        
    }
    return self;
}

- (void)initModule{
    [super initModule];
}

- (void)destroyModule{
    [super destroyModule];
    if (m_frame_cpy) {
        free(m_frame_cpy);
    }
}

- (void)renderSampleBuffer:(void *)buffer{
    struct opaqueCMSampleBuffer *sampleBuffer = (struct opaqueCMSampleBuffer *)buffer;
    [self pushInStream:SCENENAME Img:sampleBuffer];
}

//处理照片
-(void)pushInStream:(NSString*)_name Img:(CMSampleBufferRef)_samplerBuf{
    //拷贝相机数据到纹理数据
    void *aa = [[SWLogicSys getInst] getEngineInst];
    SVInst* t_app = (SVInst*)aa;
    if( t_app && (t_app->getState() == SV_ST_RUN || t_app->getState() == SV_ST_WILLSUSPEND) ) {
        CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(_samplerBuf);
        if(CVPixelBufferLockBaseAddress(imageBuffer, 0) == kCVReturnSuccess){
            size_t width = CVPixelBufferGetWidth(imageBuffer);
            size_t height = CVPixelBufferGetHeight(imageBuffer);
            size_t bytesPerRow = CVPixelBufferGetBytesPerRowOfPlane(imageBuffer, 0);
            OSType pixelFormat = CVPixelBufferGetPixelFormatType(imageBuffer);
            GLubyte* bufferPtr = (GLubyte *)CVPixelBufferGetBaseAddress(imageBuffer);
            //判断从 CVImageBufferRef 里取出来的数据是否带panding
            if (bytesPerRow/4 - width !=0) {
                //带panding，要处理
                if (m_frame_cpy == NULL) {
                    m_frame_cpy = (GLubyte *)malloc(width * height *4);
                }
                int a = 0;
                for (int i = 0; i < height; i++) {
                    memcpy(m_frame_cpy + i * width * 4 , bufferPtr + a, width*4);
                    a += bytesPerRow;
                }
                SVStreamInPtr t_streamIn = t_app->getBasicSys()->getStreamIn();
                if(t_streamIn){
                    t_streamIn->pushStreamData([_name UTF8String],(u8*)m_frame_cpy, (s32)width, (s32)height,pixelFormat, 0);
                }
            }else{
                SVStreamInPtr t_streamIn = t_app->getBasicSys()->getStreamIn();
                if(t_streamIn){
                    t_streamIn->pushStreamData([_name UTF8String],(u8*)bufferPtr, (s32)width, (s32)height,pixelFormat, 0);
                }
            }
            
            
            CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
        }
    }
}
@end
