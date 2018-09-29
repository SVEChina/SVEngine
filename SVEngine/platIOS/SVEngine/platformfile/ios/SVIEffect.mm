//
//  SVEffect.m
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVIEffect.h"
#import "SVInst.h"
#import "SVGlobalMgr.h"
#import "SVThreadMain.h"
#import "SVThreadPool.h"
#import "SVOpFilter.h"
#import "SVDataSwap.h"
#import "SVOpCreate.h"
#import "SVOpDestroy.h"
#if TARGET_OS_IPHONE

@interface SVIEffect(){
}
@end


@implementation SVIEffect

- (instancetype)init{
    self = [super init];
    if (self) {
    }
    return self;
}

//设置滤镜
-(void)setBeautyFilter:(NSString*)_filter level:(int)_level OP:(cb_func_op)_cb msg:(NSString*)_msg{
    int t_isLevel = 1;
    if (_level == 0) {
        t_isLevel = 0;
    }
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpSetBeautyFilterPtr t_op = MakeSharedPtr<SVOpSetBeautyFilter>(t_app,"","",t_isLevel);
    t_op->setCallBack(_cb, [_msg UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

//关闭美颜滤镜
-(void)closeBeautyFilterOP:(cb_func_op)_cb msg:(NSString*)_msg{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpCloseBeautyFilterPtr t_op = MakeSharedPtr<SVOpCloseBeautyFilter>(t_app);
    t_op->setCallBack(_cb, [_msg UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

-(void)updateFilterOP:(cb_func_op)_cb msg:(NSString*)_msg smooth:(float)_smooth filtertype:(SVIEFILTERTYPE) _type{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpUpdateFilterSmoothPtr t_op = MakeSharedPtr<SVOpUpdateFilterSmooth>(t_app,_smooth,_type);
    t_op->setCallBack(_cb, [_msg UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

-(void)updateFaceShapeOP:(cb_func_op)_cb msg:(NSString*)_msg face:(float) _face eye:(float) _eye {
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpShapeFaceSmoothFilterPtr t_op = MakeSharedPtr<SVOpShapeFaceSmoothFilter>(t_app,_face,_eye);
    t_op->setCallBack(_cb, [_msg UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

-(void)updateFilterBSplineOP:(cb_func_op)_cb msg:(NSString*)_msg dataTex:(unsigned char*)_data{
    SVInst* t_app = (SVInst*)m_pApp;
    SVDataSwapPtr t_pDataSwap = MakeSharedPtr<SVDataSwap>();
    t_pDataSwap->writeData(_data, 256*4);
    SVOpUpdateBSplineFilterPtr t_op = MakeSharedPtr<SVOpUpdateBSplineFilter>(t_app,t_pDataSwap);
    t_op->setCallBack(_cb, [_msg UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    
}

//加载粒子特效
-(void)loadParticle:(NSString*)_particeFile OP:(cb_func_op)_cb msg:(NSString*)_msg{
    SVInst* t_app = (SVInst*)m_pApp;
//    SVOpCreateEffcet* t_op = new SVOpCreateEffcet(m_pApp,[_particeFile UTF8String]);
//    t_op->setCallBack(_cb, [_msg UTF8String]);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//    t_op->release();
}

- (void)loadEffect:(NSString *)_effect OP:(cb_func_op)_cb msg:(NSString*)_msg{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpCreateEffcetPtr t_op = MakeSharedPtr<SVOpCreateEffcet>(t_app,[_effect UTF8String]);
    t_op->setCallBack(_cb, [_msg UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

- (void)removeEffect:(NSString *)_effect OP:(cb_func_op)_cb msg:(NSString *)_msg{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpDestroyEffectPtr t_op = MakeSharedPtr<SVOpDestroyEffect>(t_app,[_effect UTF8String]);
    t_op->setCallBack(_cb, [_msg UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}


@end

#endif
