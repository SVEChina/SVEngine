//
//  SVEffect.m
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVIEffect.h"
#import "app/SVInst.h"
#import "app/SVGlobalMgr.h"
#import "work/SVThreadMain.h"
#import "work/SVThreadPool.h"
#import "operate/SVOpFilter.h"
#import "operate/SVOpCreate.h"
#import "operate/SVOpDestroy.h"
#include "module/SVModuleSys.h"
#include "module/SVEffectPackage.h"
#include "core/SVSpine.h"
#include "node/SVSpineNode.h"
#include "app/SVGlobalMgr.h"
#if TARGET_OS_IPHONE

@interface SVIEffect(){
    NSString *curEffect;
    NSString *lastEffect;
}
@end


@implementation SVIEffect

- (instancetype)init{
    self = [super init];
    if (self) {
    }
    return self;
}

- (void)loadEffectPath:(NSString *)_effect OP:(cb_func_op)_cb msg:(NSString*)_msg{
    curEffect = [_effect lastPathComponent];
    curEffect = [curEffect stringByDeletingPathExtension];
    //
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpCreateEffcetPtr t_op = MakeSharedPtr<SVOpCreateEffcet>(t_app,[_effect UTF8String]);
    t_op->setCallBack(_cb, [_msg UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

- (void)removeEffectOP:(cb_func_op)_cb msg:(NSString *)_msg{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpDestroyEffectPtr t_op = MakeSharedPtr<SVOpDestroyEffect>(t_app,[curEffect UTF8String]);
    t_op->setCallBack(_cb, [_msg UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

- (NSArray *)getAnimations{
    SVInst* t_app = (SVInst*)m_pApp;
    SVModuleBasePtr t_module = t_app->m_pGlobalMgr->m_pModuleSys->getModule([curEffect UTF8String]);
    SVEffectPackagePtr t_effectPackage = std::dynamic_pointer_cast<SVEffectPackage>(t_module);
    if (t_effectPackage) {
        SVSpineNodePtr t_spine = std::dynamic_pointer_cast<SVSpineNode>(t_effectPackage->getNode("ani2d_0"));
        SVArray<SVString> t_anis;
        t_spine->getSpine()->getAllAnimationName(t_anis);
        NSMutableArray *anis = [[NSMutableArray alloc] init];
        for (int i=0; i<t_anis.size(); i++) {
            SVString t_ani = t_anis[i];
            [anis addObject:[NSString stringWithUTF8String:t_ani.c_str()]];
        }
        return anis;
    }
    return nil;
}

- (void)playAnimation:(NSString *)_ani{
    SVInst* t_app = (SVInst*)m_pApp;
    SVModuleBasePtr t_module = t_app->m_pGlobalMgr->m_pModuleSys->getModule([curEffect UTF8String]);
    SVEffectPackagePtr t_effectPackage = std::dynamic_pointer_cast<SVEffectPackage>(t_module);
    if (t_effectPackage) {
        SVSpineNodePtr t_spine = std::dynamic_pointer_cast<SVSpineNode>(t_effectPackage->getNode("ani2d_0"));
        t_spine->stop();
        t_spine->play([_ani UTF8String]);
    }
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

@end

#endif
