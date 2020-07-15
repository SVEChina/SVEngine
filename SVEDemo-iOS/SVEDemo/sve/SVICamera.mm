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
#import "mtl/SVTexMgr.h"
#import "rendercore/SVRendererBase.h"
#import "SVIThread.h"

#if TARGET_OS_IPHONE

@interface SVICamera(){
    GLubyte *m_frame_cpy;
    CVOpenGLESTextureCacheRef m_glTextureCache;
    CVOpenGLESTextureRef      m_glTexture;
    CVOpenGLESTextureRef      m_glLumaTexture;
    CVOpenGLESTextureRef      m_glChromaTexture;
}
@end


@implementation SVICamera

- (instancetype)init{
    self = [super init];
    if (self) {
        m_frame_cpy = NULL;
        m_glTexture = NULL;
        m_glTextureCache = NULL;
    }
    return self;
}

- (void)dealloc{
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_frame_cpy) {
            free(self->m_frame_cpy);
            self->m_frame_cpy = NULL;
        }
        if (self->m_glTexture) {
            CFRelease(self->m_glTexture);
            self->m_glTexture = NULL;
        }
        if (self->m_glTextureCache) {
            CFRelease(self->m_glTextureCache);
            self->m_glTextureCache = NULL;
        }
        if (self->m_glLumaTexture) {
            CFRelease(self->m_glLumaTexture);
            self->m_glLumaTexture = NULL;
        }
        if (self->m_glChromaTexture) {
            CFRelease(self->m_glChromaTexture);
            self->m_glChromaTexture = NULL;
        }
    }];
}

//创建一个IOS输入流
- (void)createInStream:(NSString*)_name formate:(int)_formate width:(int)_w height:(int)_h angle:(float)_angle OP:(cb_func_op)_cb msg:(NSString*)_msg{
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        SVInst* t_app = (SVInst*)self->m_pApp;
        SVOpCreateIOSInstreamPtr t_op = MakeSharedPtr<SVOpCreateIOSInstream>(t_app,[_name UTF8String], PICFORMATE(_formate), _w, _h, _angle, true);
        t_op->setCallBack(_cb, obj, [_msg UTF8String]);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }];
}

//删除一个IOS输出流
- (void)destroyInstream:(NSString*)_name OP:(cb_func_op)_cb msg:(NSString*)_msg {
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        SVInst* t_app = (SVInst*)self->m_pApp;
        SVOpDestroyIOSInstreamPtr t_op = MakeSharedPtr<SVOpDestroyIOSInstream>(t_app,[_name UTF8String]);
        t_op->setCallBack(_cb, obj, [_msg UTF8String]);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }];
}

//处理照片
-(void)pushInStream:(NSString*)_name angle:(float)_angle img:(CVPixelBufferRef)_pixelBuf{
    [m_thread globalSyncProcessingQueue:^{
        //拷贝相机数据到纹理数据
        SVInst* t_app = (SVInst*)self->m_pApp;
        if( t_app && (t_app->getState() == SV_ST_RUN)) {
            if(CVPixelBufferLockBaseAddress(_pixelBuf, 0) == kCVReturnSuccess){
                size_t width = CVPixelBufferGetWidth(_pixelBuf);
                size_t height = CVPixelBufferGetHeight(_pixelBuf);
                size_t bytesPerRow = CVPixelBufferGetBytesPerRowOfPlane(_pixelBuf, 0);
                OSType pixelFormat = CVPixelBufferGetPixelFormatType(_pixelBuf);
                if (pixelFormat == kCVPixelFormatType_32BGRA) {
                    GLubyte* bufferPtr = (GLubyte *)CVPixelBufferGetBaseAddress(_pixelBuf);
                    //判断从 CVImageBufferRef 里取出来的数据是否带panding
                    if (bytesPerRow/4 - width !=0) {
                        //带panding，要处理
                        if (self->m_frame_cpy == NULL) {
                            self->m_frame_cpy = (GLubyte *)malloc(width * height *4);
                        }
                        int a = 0;
                        for (int i = 0; i < height; i++) {
                            memcpy(self->m_frame_cpy + i * width * 4 , bufferPtr + a, width*4);
                            a += bytesPerRow;
                        }
                        SVStreamInPtr t_streamIn = t_app->getBasicSys()->getStreamIn();
                        if(t_streamIn){
                            t_streamIn->pushStreamData([_name UTF8String],(u8*)self->m_frame_cpy, (s32)width, (s32)height,pixelFormat, _angle);
                        }
                    }else{
                        SVStreamInPtr t_streamIn = t_app->getBasicSys()->getStreamIn();
                        if(t_streamIn){
                            t_streamIn->pushStreamData([_name UTF8String],(u8*)bufferPtr, (s32)width, (s32)height,pixelFormat, _angle);
                        }
                    }
                }else {
                    //YUV 数据要这么取！！！！！！！！！！！！！！！！！！！
                    GLubyte* bufferPtr0 = (GLubyte  *)CVPixelBufferGetBaseAddressOfPlane(_pixelBuf, 0);
                    SVStreamInPtr t_streamIn = t_app->getBasicSys()->getStreamIn();
                    if(t_streamIn){
                        t_streamIn->pushStreamData([_name UTF8String], bufferPtr0, (s32)width, (s32)height, pixelFormat, _angle);
                    }
                }
                CVPixelBufferUnlockBaseAddress(_pixelBuf, 0);
            }
        }
    }];
}


- (void)createInTextureStream:(NSString*)_name formate:(int)_formate width:(int)_w height:(int)_h angle:(float)_angle context:(EAGLContext *)_context OP:(cb_func_op)_cb msg:(NSString*)_msg{
    if (!_context) {
        return;
    }
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, _context, NULL, &self->m_glTextureCache);
        if (err) {
            NSLog(@"error:%d CVOpenGLESTextureCacheCreate",err);
        }
        SVInst* t_app = (SVInst*)self->m_pApp;
        SVOpCreateIOSTexIDInstreamPtr t_op = MakeSharedPtr<SVOpCreateIOSTexIDInstream>(t_app,[_name UTF8String], 0, 0, 0, PICFORMATE(_formate), _w, _h, _angle, true);
        t_op->setCallBack(_cb, obj, [_msg UTF8String]);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }];
}


- (void)cleanTextureCache{
    [m_thread globalSyncProcessingQueue:^{
        if (self->m_glTexture){
            CFRelease(self->m_glTexture);
            self->m_glTexture = NULL;
        }
        if (self->m_glLumaTexture){
            CFRelease(self->m_glLumaTexture);
            self->m_glLumaTexture = NULL;
        }
        if (self->m_glChromaTexture){
            CFRelease(self->m_glChromaTexture);
            self->m_glChromaTexture = NULL;
        }
        if (self->m_glTextureCache) {
            CVOpenGLESTextureCacheFlush(self->m_glTextureCache, 0);
        }
    }];
}


- (void)pushInTextureStream:(NSString*)_name angle:(float)_angle img:(CVPixelBufferRef)_pixelBuf{
    [self cleanTextureCache];
    [m_thread globalSyncProcessingQueue:^{
        //拷贝相机数据到纹理数据
        SVInst* t_app = (SVInst*)self->m_pApp;
        if( t_app && (t_app->getState() == SV_ST_RUN)) {
            if(CVPixelBufferLockBaseAddress(_pixelBuf, 0) == kCVReturnSuccess){
                size_t width = CVPixelBufferGetWidth(_pixelBuf);
                size_t height = CVPixelBufferGetHeight(_pixelBuf);
                OSType pixelFormat = CVPixelBufferGetPixelFormatType(_pixelBuf);
                if (pixelFormat == kCVPixelFormatType_32BGRA) {
                    CVReturn err = CVOpenGLESTextureCacheCreateTextureFromImage (
                                                                                 kCFAllocatorDefault,
                                                                                 self->m_glTextureCache,
                                                                                 _pixelBuf,
                                                                                 NULL, // texture attributes
                                                                                 GL_TEXTURE_2D,
                                                                                 GL_RGBA, // opengl format
                                                                                 width,
                                                                                 height,
                                                                                 GL_RGBA, // native iOS format
                                                                                 GL_UNSIGNED_BYTE,
                                                                                 0,
                                                                                 &(self->m_glTexture));
                    if (err)
                    {
                        NSLog(@"Error at CVOpenGLESTextureCacheCreateTextureFromImage %d", err);
                    }
                    GLuint texture = CVOpenGLESTextureGetName(self->m_glTexture);
                    SVStreamInPtr t_streamIn = t_app->getBasicSys()->getStreamIn();
                    if(t_streamIn){
                        t_streamIn->pushTextureStream([_name UTF8String],texture, 0, 0, (s32)width, (s32)height,pixelFormat, _angle);
                    }
                }else{
                    CVReturn err = CVOpenGLESTextureCacheCreateTextureFromImage (
                                                                                 kCFAllocatorDefault,
                                                                                 self->m_glTextureCache,
                                                                                 _pixelBuf,
                                                                                 NULL, // texture attributes
                                                                                 GL_TEXTURE_2D,
                                                                                 GL_LUMINANCE, // opengl format
                                                                                 width,
                                                                                 height,
                                                                                 GL_LUMINANCE, // native iOS format
                                                                                 GL_UNSIGNED_BYTE,
                                                                                 0,
                                                                                 &(self->m_glLumaTexture));
                    if (err)
                    {
                        NSLog(@"Error at CVOpenGLESTextureCacheCreateTextureFromImage %d", err);
                    }
                    err = CVOpenGLESTextureCacheCreateTextureFromImage (
                                                                                 kCFAllocatorDefault,
                                                                                 self->m_glTextureCache,
                                                                                 _pixelBuf,
                                                                                 NULL, // texture attributes
                                                                                 GL_TEXTURE_2D,
                                                                                 GL_LUMINANCE_ALPHA, // opengl format
                                                                                 width/2,
                                                                                 height/2,
                                                                                 GL_LUMINANCE_ALPHA, // native iOS format
                                                                                 GL_UNSIGNED_BYTE,
                                                                                 1,
                                                                                 &(self->m_glChromaTexture));
                    if (err)
                    {
                        NSLog(@"Error at CVOpenGLESTextureCacheCreateTextureFromImage %d", err);
                    }
                    GLuint texLuma = CVOpenGLESTextureGetName(self->m_glLumaTexture);
                    GLuint texChroma = CVOpenGLESTextureGetName(self->m_glChromaTexture);
                    SVStreamInPtr t_streamIn = t_app->getBasicSys()->getStreamIn();
                    if(t_streamIn){
                        t_streamIn->pushTextureStream([_name UTF8String],texLuma, texChroma, 0, (s32)width, (s32)height, pixelFormat, _angle);
                    }
                }
                CVPixelBufferUnlockBaseAddress(_pixelBuf, 0);
            }
        }
    }];
}

- (void)createOutStream:(NSString*)_name streamType:(int)_streamType width:(int)_w height:(int)_h OP:(cb_func_op)_cb msg:(NSString*)_msg{
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        SVInst* t_app = (SVInst*)self->m_pApp;
        SVOpCreateIOSOutstreamPtr t_op = MakeSharedPtr<SVOpCreateIOSOutstream>(t_app,[_name UTF8String],1, _w, _h, _streamType);
        t_op->setCallBack(_cb, obj, [_msg UTF8String]);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }];
}

- (void)destroyOutStream:(NSString *)_name OP:(cb_func_op)_cb msg:(NSString*)_msg{
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        SVInst* t_app = (SVInst*)self->m_pApp;
        SVOpDestroyIOSOutstreamPtr t_op = MakeSharedPtr<SVOpDestroyIOSOutstream>(t_app,[_name UTF8String]);
        t_op->setCallBack(_cb, obj, [_msg UTF8String]);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }];
}

- (void)openOutStream:(cb_out_stream)_cb Mode:(int)_mode {
    [m_thread globalSyncProcessingQueue:^{
        SVInst* t_app = (SVInst*)self->m_pApp;
        SVOpOpenIOSOutstreamPtr t_op = MakeSharedPtr<SVOpOpenIOSOutstream>(t_app, _cb);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }];
}

- (void)closeOutStream{
    [m_thread globalSyncProcessingQueue:^{
        SVInst* t_app = (SVInst*)self->m_pApp;
        SVOpCloseIOSOutstreamPtr t_op = MakeSharedPtr<SVOpCloseIOSOutstream>(t_app);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }];
}
@end

#endif
