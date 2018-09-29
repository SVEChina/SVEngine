//
//  SVCamera.m
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVIGame.h"
#import "SVInst.h"
#import "SVOpLoadBase.h"
#import "SVGlobalMgr.h"
#import "SVThreadMain.h"
#import "SVThreadPool.h"
#import "SVOpFilter.h"
#import "SVDataSwap.h"
#import "SVOpCreate.h"

#if TARGET_OS_IPHONE

@interface SVIGame(){
    
}
@end


@implementation SVIGame

- (instancetype)init{
    self = [super init];
    if (self) {
        
    }
    return self;
}

//加载游戏
-(void)loadGame:(NSString*)_packname Callback:(cb_func_game)_gamecb OP:(cb_func_op)_cb msg:(NSString*)_msg{
}

//打开游戏
-(void)startGame:(NSString*)_packname OP:(cb_func_op)_cb msg:(NSString*)_msg{
}

//关闭游戏
-(void)closeGameOP:(cb_func_op)_cb msg:(NSString*)_msg{
}

//推送数据数据(json串)
-(void)pushGameData:(NSString*)_info{
//    SVInst* t_app = (SVInst*)m_pApp;
////    SVOpSetGameData* t_op = new SVOpSetGameData(m_pApp,[_info UTF8String]);
////    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

//设置头像
-(void)setHeadRGBA:(int)_code W:(int)_w H:(int)_h Data:(void*)_data{
}

//设置拾取
-(void)pickX:(int)_x Y:(int)_y OP:(cb_func_op)_cb msg:(NSString*)_msg {
    //
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpPickPtr t_op = MakeSharedPtr<SVOpPick>(t_app,_x,_y);
    t_op->setCallBack(_cb, [_msg UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

@end


#endif
