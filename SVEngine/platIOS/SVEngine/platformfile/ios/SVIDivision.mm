
//
//  SVIDivision.m
//  SVEngine
//
//  Created by 李晓帆 on 2018/9/7.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import "SVIDivision.h"
#if TARGET_OS_IPHONE
#import "SVInst.h"
#import "SVGlobalMgr.h"
#import "SVThreadMain.h"
#import "SVThreadPool.h"
#import "SVOpFilter.h"
#import "SVDataSwap.h"
#import "SVOpCreate.h"
#import "SVOpDestroy.h"
#import "SVModuleSys.h"
#import "SVDivisonFilter.h"
#import "SVStaticData.h"
@interface SVIDivision (){
    
}
@end

@implementation SVIDivision
- (instancetype)init{
    self = [super init];
    if (self) {
        
    }
    return self;
}
- (void)openDivision{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpCreateDivisionPtr t_op = MakeSharedPtr<SVOpCreateDivision>(t_app);
    t_op->setCallBack(nullptr, "");
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}
- (void)closeDivision{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpDestroyDivisionPtr t_op = MakeSharedPtr<SVOpDestroyDivision>(t_app);
    t_op->setCallBack(nullptr, "");
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}
- (void)switchDivision:(int)_type{
    SVInst* t_app = (SVInst*)m_pApp;
    SVModuleBasePtr t_modulePtr = t_app->getModuleSys()->getModule("sv_filter_division");
    SVDivisonFilterPtr t_division = std::dynamic_pointer_cast<SVDivisonFilter>(t_modulePtr);
    t_division->setDivisionFilterType((SVStaticData::EFFICACYMESHTYPE)_type);
}
@end
#endif
