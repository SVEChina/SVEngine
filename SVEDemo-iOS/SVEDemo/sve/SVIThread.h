//
//  SVIThread.h
//
//  Created by 李晓帆 on 2017/3/14.
//  Copyright © 2017年 李晓帆. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SVIThread : NSObject

//
-(void)destroy;

//同步队列
- (void)globalSyncProcessingQueue:(void (^)(void))block;

//异步队列
- (void)globalAsyncProcessingQueue:(void (^)(void))block;

@end
