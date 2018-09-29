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
}

- (void)startGame{
}

- (void)stopGame{
}

- (void)disappear{
}

@end


#endif
