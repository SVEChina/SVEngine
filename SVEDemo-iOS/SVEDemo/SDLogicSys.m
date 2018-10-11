//
//  SDLogicSys.m
//  SVEDemo
//
//  Created by 李晓帆 on 2018/9/3.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import "SDLogicSys.h"
#import "SVICamera.h"
#import "SDPreDef.h"
#import "SVIEffect.h"
@interface SDLogicSys()<CameraDelegate>{
    EAGLContext* m_pGLContext;
}
@end
unsigned char *s_outDataBuffer;
void data_out_stream(int _w,int _h,int _formate,void* _pdata,long _timtap) {
    if (s_outDataBuffer != NULL) {
        memcpy(s_outDataBuffer, _pdata, _w*_h*4);
    }
}
static SDLogicSys *mInst;
@implementation SDLogicSys

+(instancetype) getInst{
    if(mInst == nil){
        mInst = [SDLogicSys new];
    }
    return mInst;
}

- (void)dealloc{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)initSys{
    //
    self.pSVI = nil;
    //创建设备上下文
    m_pGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    //
    self.pState = [[SDState alloc] init];
    //注册监听消息
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didEnterBackground) name:@"didEnterBackground" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didEnterForeground) name:@"didEnterForeground" object:nil];
    //
    [self initSVE];
    //
    [self initCamera];
    
    [self initAirdropFilePath];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(addEffect:) name:@"effectPath" object:nil];
}

- (void)destroySys{
    //开启相机
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"didEnterBackground" object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"didEnterForeground" object:nil];
    //停止引擎并析构
    [self destroySVE];
}

- (void)initCamera{
    self.pCamera = [[SDCamera alloc] init];
    self.pCamera.delegate = self;
    [self.pCamera startCapture];
}


- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection{
    if (connection == self.pCamera.videoConnection) {
        //这里可以做detect
        //.......................
        //渲染
        if (self.pSVI) {
            [self.pSVI.pCamera pushInStream:SCENENAME Img:sampleBuffer];
        }
        

        
    }else if (connection == self.pCamera.audioConnection) {
        
        
    }
}

- (void)initAirdropFilePath{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    NSString *path = [paths lastObject];
    self.m_airdropFilePath = [path stringByAppendingPathComponent:@"airdropeffect"];
    if (![[NSFileManager defaultManager] fileExistsAtPath:self.m_airdropFilePath]) {
        [[NSFileManager defaultManager] createDirectoryAtPath:self.m_airdropFilePath withIntermediateDirectories:YES attributes:nil error:nil];
    }
}

- (void)initSVE {
    if(!self.pSVI) {
        //创建sv引擎对象
        self.pSVI = [[SVI alloc] init];
        //设置资源路径
        NSString *bundlePath = [[NSBundle mainBundle] pathForResource:@"sve" ofType:@"bundle"];
        bundlePath = [NSString stringWithFormat:@"%@/",bundlePath];
        [self.pSVI addResPath:bundlePath];
        [self.pSVI addResPath:@""];
        //开启引擎
        [self.pSVI startEngine];
        //创建渲染器
        [self.pSVI createRendererGL:3 Context:m_pGLContext Width:self.pState.svOutW Height:self.pState.svOutH];
        //创建场景
        [self.pSVI createScene:NULL msg:@""];
        //创建相机节点
        [self.pSVI.pCamera createInStream:SCENENAME Type:0 width:self.pState.svOutW height:self.pState.svOutH OP:NULL msg:@""];
        //创建输出流节点
        [self.pSVI.pCamera createOutStream:SCENENAME Type:0 StreamType:0 OP:NULL msg:@""];
        //
//        [self.pSVI.pEffect setBeautyFilter:@"" level:1 OP:nil msg:nil];
//        //
//        [self.pSVI.pEffect updateFilterOP:NULL msg:@"" smooth:50.0f filtertype:SVI_EBEAUTY_FILTER];
//        //
//        [self.pSVI.pEffect updateFilterOP:NULL msg:@"" smooth:20.0f filtertype:SVI_WHITENING_FILTER];
//        //
//        [self.pSVI.pEffect updateFilterOP:NULL msg:@"" smooth:10.0 filtertype:SVI_ACUTANCE_FILTER];
//        //
//        [self.pSVI.pEffect updateFilterOP:NULL msg:@"" smooth:18.0 filtertype:SVI_CONTRAST_FILTER];

    }
}

- (void)destroySVE {
    //
    
    [self.pCamera stopCapture];
    //停止引擎并析构
    if( self.pSVI ) {
        [self.pSVI.pCamera closeOutStream];
        //析构引擎
        [self.pSVI stopEngine];
        self.pSVI = nil;
    }
}

-(EAGLContext*)getGLContext{
    return m_pGLContext;
}

- (void)didEnterBackground{
    //如果有预览视频正在播放，那么暂停。
    [self.pSVI suspend];
}

- (void)didEnterForeground{
    //如果有预览视频正在播放，那么继续播放。
    [self.pSVI resume];
}

- (void)addEffect:(NSNotification*)notifi{
    NSString* path = notifi.object;
    NSString *copy_path = [self.m_airdropFilePath stringByAppendingPathComponent:[path lastPathComponent]];
    [self.pSVI.pEffect loadEffectPath:copy_path OP:NULL msg:@""];
}

@end
