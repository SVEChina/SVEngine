//
//  SVCamera.h
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVInterfaceBase.h"

#if TARGET_OS_IPHONE
@interface SVIGameRedPacket : SVInterfaceBase
//资源预加载
- (void)loadGameResource;
//开始游戏，要设置红包数量
//mode 0 倒计时54321   mode 1 倒计时 321
- (void)startGame:(int)_packetNum Mode:(int)_mode msg:(NSString *)_msg;

- (void)stopGame;
//打开红包（_value 显示数字）
- (void)openPacketMoney:(int)_value;
//设置钱包位置
- (void)setWalletPosX:(int)_x PosY:(int)_y;
//移除一个红包 _value 为-1
- (void)removeOnePacket:(int)_value;

- (void)setTotalMoney:(NSString *)_text;
@end

#endif
