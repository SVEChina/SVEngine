//
//  SVIGameHeartFlutter.m
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVIGameHeartFlutter.h"
#import "SVInst.h"
#import "SVOpLoadBase.h"
#import "SVGlobalMgr.h"
#import "SVThreadMain.h"
#import "SVThreadPool.h"
#import "SVOpFilter.h"
#import "SVDataSwap.h"
#import "SVOpCreate.h"
#import "SVOpDestroy.h"

#if TARGET_OS_IPHONE
void heartflutter_op_callback(const char* _info) {
    printf("heartflutter_op_callback %s\n", _info);
    NSDictionary *t_dic = @{@"operate":[NSString stringWithUTF8String:_info]};
    [[NSNotificationCenter defaultCenter] postNotificationName:@"heartflutter_op_callback" object:nil userInfo:t_dic];
}

@interface SVIGameHeartFlutter(){

}
@end

@implementation SVIGameHeartFlutter
- (instancetype)init{
    self = [super init];
    if (self) {
    }
    return self;
}

- (void)loadGameResource{
    NSString *t_path = [[NSBundle mainBundle] pathForResource:@"heartflutter" ofType:@"bundle"];
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpLoadHeartFlutterPtr t_op = MakeSharedPtr<SVOpLoadHeartFlutter>(t_app,[t_path UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

- (void)startGame{
    NSString *t_path = [[NSBundle mainBundle] pathForResource:@"heartflutter" ofType:@"bundle"];
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpBeginHeartFlutterPtr t_op = MakeSharedPtr<SVOpBeginHeartFlutter>(t_app,[t_path UTF8String]);
    t_op->setCallBack(heartflutter_op_callback,"");
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    
}

- (void)stopGame{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpDestroyHeartFlutterPtr t_op = MakeSharedPtr<SVOpDestroyHeartFlutter>(t_app);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

- (void)disappear{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpDisappearHeartFlutterPtr t_op = MakeSharedPtr<SVOpDisappearHeartFlutter>(t_app);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}
@end


#endif
