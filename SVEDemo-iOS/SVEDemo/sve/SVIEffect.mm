//
//  SVEffect.m
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVIEffect.h"
#import "SVIThread.h"
#import "app/SVInst.h"
#import "app/SVGlobalMgr.h"
#import "work/SVThreadMain.h"
#import "work/SVThreadPool.h"
#import "SVOpFaceBeautyExt.h"
#import "SVFilterExtDef.h"
#import "operate/SVOpFilter.h"
#import "operate/SVOpCreate.h"
#import "operate/SVOpDestroy.h"
//test
#include "basesys/filter/SVADFilterBase.h"
#include "mtl/SVMtlShinning.h"
#include "mtl/SVMtlBlackWhite.h"
#include "mtl/SVMtlStreak.h"
#include "mtl/SVMtlRainbowColor.h"
#include "mtl/SVMtlFlash.h"
#include "mtl/SVMtlGlitch.h"
#include "mtl/SVMtlDistorted.h"
#include "basesys/SVPictureProcess.h"
#include "basesys/SVBasicSys.h"
#import "basesys/SVConfig.h"
#import "SVADFilter.h"
#import "SVOpADFilter.h"
#import "SVADFilterDef.h"

#if TARGET_OS_IPHONE
void gifteffect_op_callback(const char* _info, void *_obj) {
    if (strlen(_info) > 0) {
        SVIEffect* t_self = (__bridge SVIEffect *)_obj;
        
    }
}

@interface SVIEffect(){

}
@end


@implementation SVIEffect

- (instancetype)initWithSVE:(void *)_sve Thread:(SVIThread *)_thread{
    self = [super initWithSVE:_sve Thread:_thread];
    if (self) {

    }
    return self;
}

- (void)dealloc{

}


-(void)setBeautyFilter:(NSString*)_filter level:(int)_level OP:(cb_func_op)_cb msg:(NSString*)_msg{
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        int t_isLevel = 0;
        if (_level == 0) {
            t_isLevel = 0;
        }
        SVInst* t_app = (SVInst*)self->m_pApp;
        SVOpFaceBeautyExtPtr t_op = MakeSharedPtr<SVOpFaceBeautyExt>(t_app,"","",t_isLevel);
        t_op->setCallBack(_cb, obj, [_msg UTF8String]);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }];
}

//关闭美颜滤镜
-(void)closeBeautyFilterOP:(cb_func_op)_cb msg:(NSString*)_msg{
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        SVInst* t_app = (SVInst*)self->m_pApp;
        SVOpCloseBeautyFilterPtr t_op = MakeSharedPtr<SVOpCloseBeautyFilter>(t_app);
        t_op->setCallBack(_cb, obj, [_msg UTF8String]);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }];
}

-(void)updateFilterOP:(cb_func_op)_cb msg:(NSString*)_msg smooth:(float)_smooth filtertype:(SVIEFILTERTYPE) _type{
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        SVInst* t_app = (SVInst*)self-> m_pApp;
        SVOpUpdateFilterSmoothPtr t_op = MakeSharedPtr<SVOpUpdateFilterSmooth>(t_app,_smooth,_type);
        t_op->setCallBack(_cb, obj, [_msg UTF8String]);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }];
}

-(void)updateFaceShapeOP:(cb_func_op)_cb msg:(NSString*)_msg face:(float) _face eye:(float) _eye {
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        SVInst* t_app = (SVInst*)self->m_pApp;
        SVOpShapeFaceSmoothFilterPtr t_op = MakeSharedPtr<SVOpShapeFaceSmoothFilter>(t_app,_face,_eye);
        t_op->setCallBack(_cb, obj, [_msg UTF8String]);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }];
}

-(void)updateFilterBSplineOP:(cb_func_op)_cb msg:(NSString*)_msg dataTex:(unsigned char*)_data{
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        SVInst* t_app = (SVInst*)self->m_pApp;
        SVDataSwapPtr t_pDataSwap = MakeSharedPtr<SVDataSwap>();
        t_pDataSwap->writeData(_data, 256*4);
        SVOpUpdateBSplineFilterPtr t_op = MakeSharedPtr<SVOpUpdateBSplineFilter>(t_app,t_pDataSwap);
        t_op->setCallBack(_cb, obj, [_msg UTF8String]);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }];
}

//加载粒子特效
-(void)loadParticle:(NSString*)_particeFile OP:(cb_func_op)_cb msg:(NSString*)_msg{
    SVInst* t_app = (SVInst*)m_pApp;
//    SVOpCreateEffcet* t_op = new SVOpCreateEffcet(m_pApp,[_particeFile UTF8String]);
//    t_op->setCallBack(_cb, [_msg UTF8String]);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//    t_op->release();
}

- (void)loadEffect:(NSString *)_effect msg:(NSString*)_msg{
    if (!_effect) {
        return;
    }
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        SVInst* t_app = (SVInst*)self->m_pApp;
        SVOpCreateEffcetPtr t_op = MakeSharedPtr<SVOpCreateEffcet>(t_app,[_effect UTF8String]);
        t_op->setCallBack(gifteffect_op_callback, obj, [_msg UTF8String]);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }];
}

- (void)removeEffect:(NSString *)_effect msg:(NSString *)_msg{
    if (!_effect) {
        return;
    }
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        SVInst* t_app = (SVInst*)self->m_pApp;
        SVOpDestroyEffectPtr t_op = MakeSharedPtr<SVOpDestroyEffect>(t_app,[_effect UTF8String]);
        t_op->setCallBack(gifteffect_op_callback, obj, [_msg UTF8String]);
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }];
}

- (void)testADFilter{
    void *obj = (__bridge void *)self;
    [m_thread globalSyncProcessingQueue:^{
        SVInst* t_app = (SVInst*)self->m_pApp;
        SVOpLoadADFilterPtr t_op = MakeSharedPtr<SVOpLoadADFilter>(t_app);
        t_op->setCallBack(nullptr, obj, "");
        t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    }];
}

- (void)testFilter{
    [m_thread globalSyncProcessingQueue:^{
        SVInst* t_app = (SVInst*)self->m_pApp;
        SVMtlCorePtr t_mtl = nullptr;
//            t_mtl = MakeSharedPtr<SVMtlShinning>(t_app);//闪光滤镜
        
        //    t_mtl = MakeSharedPtr<SVMtlBlackWhite>(t_app);//黑白滤镜
        //
        t_mtl = MakeSharedPtr<SVMtlStreak>(t_app);//斑马纹滤镜
        //
        //    t_mtl = MakeSharedPtr<SVMtlRainbowColor>(t_app);//彩虹色滤镜
        //
        //    t_mtl = MakeSharedPtr<SVMtlFlash>(t_app);//白闪滤镜
        //
        //    t_mtl = MakeSharedPtr<SVMtlGlitch>(t_app);//毛刺滤镜
        //
        //    t_mtl = MakeSharedPtr<SVMtlDistorted>(t_app);//老电视滤镜
        //
        SVADFilterBasePtr t_filter = MakeSharedPtr<SVADFilterBase>(t_app, t_mtl);
        SVPictureProcessPtr t_picproc = t_app->getBasicSys()->getPicProc();
        if(t_filter && t_picproc){
            t_filter->create();
            t_picproc->addFilter(t_filter);
            t_picproc->openFilter(t_filter);
        }
    }];
}

- (unsigned char *)getRGBAWithImage:(UIImage *)image
{
    int RGBA = 4;
    
    CGImageRef imageRef = [image CGImage];
    
    size_t width = CGImageGetWidth(imageRef);
    size_t height = CGImageGetHeight(imageRef);
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    unsigned char *rawData = (unsigned char *) malloc(width * height * sizeof(unsigned char) * RGBA);
    NSUInteger bytesPerPixel = RGBA;
    NSUInteger bytesPerRow = bytesPerPixel * width;
    NSUInteger bitsPerComponent = 8;
    CGContextRef context = CGBitmapContextCreate(rawData, width, height, bitsPerComponent, bytesPerRow, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
    
    CGColorSpaceRelease(colorSpace);
    CGContextRelease(context);
    
    return rawData;
}

@end

#endif

