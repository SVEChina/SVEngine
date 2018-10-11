//
//  SVICamera.m
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVICamera.h"
#import "app/SVInst.h"
#import "app/SVGlobalMgr.h"
#import "basesys/SVBasicSys.h"
#import "basesys/SVStreamIn.h"
#import "basesys/SVStreamOut.h"
#import "operate/SVOpOutCamera.h"
#import "work/SVThreadMain.h"
#import "work/SVThreadPool.h"


#if TARGET_OS_IPHONE

@interface SVICamera(){
    GLubyte *m_frame_cpy;
}
@end


@implementation SVICamera

- (instancetype)init{
    self = [super init];
    if (self) {
        m_frame_cpy = NULL;
    }
    return self;
}

- (void)dealloc{
    if (m_frame_cpy) {
        free(m_frame_cpy);
    }
}

//创建一个IOS输入流
- (void)createInStream:(NSString*)_name Type:(int)_type width:(int)_w height:(int)_h OP:(cb_func_op)_cb msg:(NSString*)_msg{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpCreateIOSInstreamPtr t_op = MakeSharedPtr<SVOpCreateIOSInstream>(t_app,[_name UTF8String],1,_w,_h,0.0f, true);
    t_op->setCallBack(_cb, [_msg UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

//删除一个IOS输出流
- (void)destroyInstream:(NSString*)_name OP:(cb_func_op)_cb msg:(NSString*)_msg {
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpDestroyIOSInstreamPtr t_op = MakeSharedPtr<SVOpDestroyIOSInstream>(t_app,[_name UTF8String]);
    t_op->setCallBack(_cb, [_msg UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

//处理照片
-(void)pushInStream:(NSString*)_name Img:(CMSampleBufferRef)_samplerBuf{
    //拷贝相机数据到纹理数据
    SVInst* t_app = (SVInst*)m_pApp;
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

- (GLubyte *)convertUIImageToBuffer:(UIImage*)image width:(int)width height:(int)height
{
    
    GLubyte *imageData = NULL;
    
    CFDataRef dataFromImageDataProvider = CGDataProviderCopyData(CGImageGetDataProvider(image.CGImage));
    
    imageData = (GLubyte *)CFDataGetBytePtr(dataFromImageDataProvider);
    
    //CFRelease(dataFromImageDataProvider);//最后别忘了release
    
    return imageData;
    
}

//用于一段一段的处理并保存图片
-(void)pushInStream:(NSString*)_name Img:(CMSampleBufferRef)_samplerBuf Save:(bool)_save{
//    if( m_pApp->getSVMode() == SV_ST_RECORD){
//        //拷贝相机数据到纹理数据
//        CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(_samplerBuf);
//        if(CVPixelBufferLockBaseAddress(imageBuffer, 0) == kCVReturnSuccess){
//            size_t width = CVPixelBufferGetWidth(imageBuffer);
//            size_t height = CVPixelBufferGetHeight(imageBuffer);
//            int pixelFormat = CVPixelBufferGetPixelFormatType(imageBuffer);
//            unsigned char* bufferPtr = (unsigned char*)CVPixelBufferGetBaseAddress(imageBuffer);
//            m_pApp->getSceneMgr()->pushStreamData(bufferPtr,(s32)width,(s32)height,pixelFormat,true);
//            //拷贝纹理数据到相机数据
//            CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
//        }
//    }
}

- (void)createOutStream:(NSString*)_name Type:(int)_type StreamType:(int)_streamType OP:(cb_func_op)_cb msg:(NSString*)_msg{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpCreateIOSOutstreamPtr t_op = MakeSharedPtr<SVOpCreateIOSOutstream>(t_app,[_name UTF8String],1, _streamType);
    t_op->setCallBack(_cb, [_msg UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

- (void)destroyOutStream:(NSString *)_name OP:(cb_func_op)_cb msg:(NSString*)_msg{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpDestroyIOSOutstreamPtr t_op = MakeSharedPtr<SVOpDestroyIOSOutstream>(t_app,[_name UTF8String]);
    t_op->setCallBack(_cb, [_msg UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

-(void) openOutStream:(cb_out_stream)_cb Mode:(int)_mode {
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpOpenIOSOutstreamPtr t_op = MakeSharedPtr<SVOpOpenIOSOutstream>(t_app, _cb);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

-(void) closeOutStream{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpCloseIOSOutstreamPtr t_op = MakeSharedPtr<SVOpCloseIOSOutstream>(t_app);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}





@end

#endif
