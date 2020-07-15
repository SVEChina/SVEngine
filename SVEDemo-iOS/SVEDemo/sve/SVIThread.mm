//
//  SVIThread.m
//
//  Created by 李晓帆 on 2017/3/14.
//  Copyright © 2017年 李晓帆. All rights reserved.
//

#import "SVIThread.h"

@interface SVIThread (){
    dispatch_queue_t m_execQueue;    //执行队列
}

@end

@implementation SVIThread

- (instancetype)init{
    self = [super init];
    if (self) {
    }
    return self;
}

- (void)dealloc{
    m_execQueue = NULL;
}

-(void)destroy{
}

- (void *)contextKey{
    return (__bridge void *)(self);
}

- (dispatch_queue_t)execQueue{
    if (NULL == m_execQueue){
        m_execQueue = dispatch_queue_create("org.svengine.show", DISPATCH_QUEUE_SERIAL );
        dispatch_queue_set_specific(m_execQueue, [self contextKey], (__bridge void *)self, NULL);
    }
    return m_execQueue;
}

//同步队列
- (void)globalSyncProcessingQueue:(void (^)(void))block{
    NSAssert(block != nil, @"block could not be nil!!");
    if (dispatch_get_specific( [self contextKey] ) ){
        block();
    }
    else{
        dispatch_sync([self execQueue], block);
    }
}

//异步队列
- (void)globalAsyncProcessingQueue:(void (^)(void))block{
    NSAssert(block != nil, @"block could not be nil!!");
    if (dispatch_get_specific( [self contextKey]) ){
        block();
    }
    else{
        dispatch_async([self execQueue], block);
    }
}

@end
