//
//  SVCamera.h
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVInterfaceBase.h"

#if TARGET_OS_IPHONE
@interface SVIGame : SVInterfaceBase
//加载游戏(游戏回调)
-(void)loadGame:(NSString*)_packname Callback:(cb_func_game)_gamecb OP:(cb_func_op)_cb msg:(NSString*)_msg;
//开始游戏
-(void)startGame:(NSString*)_packname OP:(cb_func_op)_cb msg:(NSString*)_msg;
//关闭游戏
-(void)closeGameOP:(cb_func_op)_cb msg:(NSString*)_msg;
//推送数据数据(json串)
-(void)pushGameData:(NSString*)_info;
//设置头像
-(void)setHeadRGBA:(int)_code W:(int)_w H:(int)_h Data:(void*)_data;
//设置拾取
-(void)pickX:(int)_x Y:(int)_y OP:(cb_func_op)_cb msg:(NSString*)_msg;

@end




#endif
