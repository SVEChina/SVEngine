//
//  SVEffect.m
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVIEffect.h"
#import "SVInst.h"
#import "SVGlobalMgr.h"
#import "SVLogicMgr.h"
#import "SVThreadMain.h"
#import "SVThreadPool.h"
#import "SVOpFilter.h"
#import "SVOpCreate.h"
#import "SVOpDestroy.h"
#include "SVEngine_Static/src/module/SVModuleSys.h"
#include "SVEngine_Static/src/module/SVEffectPackage.h"
#include "SVEngine_Static/src/core/SVSpine.h"
#include "SVEngine_Static/src/node/SVSpineNode.h"
#include "SVEngine_Static/src/app/SVGlobalMgr.h"
#include "SVEngine_Static/src/app/SVInst.h"
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
        SVArray<SVString> t_anis = t_spine->getAnimations();
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

@end

#endif
