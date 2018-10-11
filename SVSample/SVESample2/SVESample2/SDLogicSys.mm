//
//  SDLogicSys.m
//  SVEDemo
//
//  Created by 李晓帆 on 2018/9/3.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import "SDLogicSys.h"
#import "SDCamera.h"
#import "SVTest.h"
#import "operate/SVOpRender.h"
#import "operate/SVOpCreate.h"
#import "operate/SVOpOutCamera.h"
#import "basesys/SVBasicSys.h"
#import "work/SVThreadPool.h"
#import "work/SVThreadMain.h"
#import "work/SVThreadHelp.h"
#import "basesys/SVBasicSys.h"
#import "basesys/SVStreamIn.h"
#define SCENENAME @"SAMPLE2"
@interface SDLogicSys()<CameraDelegate>{
    SVInst *m_pInst;
    EAGLContext* m_pGLContext;
    NSInteger m_outW;
    NSInteger m_outH;
    SDCamera *m_pCamera;
    GLubyte *m_frame_cpy;
}
@end

static SDLogicSys *mInst;

@implementation SDLogicSys

+(instancetype) getInst{
    if(mInst == nil){
        mInst = [SDLogicSys new];
    }
    return mInst;
}

- (void)dealloc{
    if (m_frame_cpy) {
        free(m_frame_cpy);
    }
}

- (void)initSys{
    m_frame_cpy = NULL;
    m_outW = 720;
    m_outH = 1280;
    m_pInst = new SVInst();
    m_pInst->init();
    //创建设备上下文
    m_pGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    //注册监听消息（退到后台和重返前台）
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didEnterBackground) name:@"didEnterBackground" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didEnterForeground) name:@"didEnterForeground" object:nil];
    //初始化引擎
    [self initSVE];
    [self initCamera];
}

- (void)destroySys{
    //开启相机
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"didEnterBackground" object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"didEnterForeground" object:nil];
    //停止引擎并析构
    [self destroySVE];
}

-(SVInst*)getSVE {
    return m_pInst;
}

- (void)initSVE {
    if(m_pInst) {
        //设置资源路径
        NSString *bundlePath = [[NSBundle mainBundle] pathForResource:@"sve" ofType:@"bundle"];
        bundlePath = [NSString stringWithFormat:@"%@/",bundlePath];
        m_pInst->addRespath([bundlePath UTF8String]);
        m_pInst->addRespath("");
        //开启引擎
        m_pInst->startSVE();
        //创建GL渲染器
        SVOpCreateRenderderPtr t_op_renderer = MakeSharedPtr<SVOpCreateRenderder>(m_pInst);
        t_op_renderer->setGLParam(3,(__bridge_retained void *)m_pGLContext,m_outW,m_outH);
        m_pInst->m_pTPool->getMainThread()->pushThreadOp(t_op_renderer);
        //创建场景
        SVOpCreateScenePtr t_op_scene = MakeSharedPtr<SVOpCreateScene>(m_pInst,[SCENENAME UTF8String]);
        m_pInst->m_pTPool->getMainThread()->pushThreadOp(t_op_scene);
        //创建一个iOS输入流
        SVOpCreateIOSInstreamPtr t_op_instream = MakeSharedPtr<SVOpCreateIOSInstream>(m_pInst,[SCENENAME UTF8String],1,m_outW,m_outH,0.0f);
        m_pInst->m_pTPool->getMainThread()->pushThreadOp(t_op_instream);
        
    }
}

- (void)initCamera{
    m_pCamera = [[SDCamera alloc] init];
    m_pCamera.delegate = self;
    [m_pCamera startCapture];
}

- (void)destroySVE {
    //停止引擎并析构
    if( m_pInst ) {
        m_pInst->stopSVE();
        m_pInst->destroy();
        m_pInst = nil;
    }
}

-(EAGLContext*)getGLContext{
    return m_pGLContext;
}

- (void)didEnterBackground{
    //如果有预览视频正在播放，那么暂停。
    if(m_pInst) {
        m_pInst->svSuspend();
    }
}

- (void)didEnterForeground{
    //如果有预览视频正在播放，那么继续播放。
    if(m_pInst) {
        m_pInst->svResume();
    }
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection{
    if (connection == m_pCamera.videoConnection) {
        //这里可以做detect
        //.......................
        //渲染
        if (m_pInst) {
            [self pushInStream:SCENENAME Img:sampleBuffer];
        }
        
        
        
    }
}

//处理照片
-(void)pushInStream:(NSString*)_name Img:(CMSampleBufferRef)_samplerBuf{
    //拷贝相机数据到纹理数据
    SVInst* t_app = (SVInst*)m_pInst;
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
