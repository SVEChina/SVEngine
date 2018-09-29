//
//  SVIGameHeartFlutter.h
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVInterfaceBase.h"

#if TARGET_OS_IPHONE
@interface SVIGameHeartFlutter : SVInterfaceBase
//资源预加载
- (void)loadGameResource;

- (void)startGame;

- (void)disappear;

- (void)stopGame;
@end

#endif
