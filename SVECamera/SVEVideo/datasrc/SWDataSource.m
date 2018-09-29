//
//  SWDataSource.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWDataSource.h"
#import "../swlogic/SWLogicSys.h"
#import "../swbasic/SWBasicSys.h"
#import "SWDataSourceCamera.h"
#import "SWDataSourcePic.h"
#import "SWDataSourceVideo.h"
#import "SWDataSourceNet.h"
#import <SVEngine/SVICamera.h>
@interface SWDataSource ()<CameraDelegate>{
}

@end

@implementation SWDataSource
+ (SWDataSource *)createDataSrcType:(SWDATASRCTYPE)dataSrcType{
    if (dataSrcType == DATASRCCAMERA) {
        return [[SWDataSourceCamera alloc] init];
    }else if (dataSrcType == DATASRCPIC){
        return [[SWDataSourcePic alloc] init];
    }else if (dataSrcType == DATASRCVIDEO){
        return [[SWDataSourceVideo alloc] init];
    }else if (dataSrcType == DATASRCNET){
        return [[SWDataSourceNet alloc] init];
    }
    return nil;
}

- (instancetype)init{
    self = [super init];
    if (self) {
        m_srcType = DATASRCNONE;
    }
    return self;
}

- (void)dealloc{
    
}

- (void)initModule{
    
}

- (void)destroyModule{
    
}

- (void)start{
    
}

- (void)stop{
    
}

- (SWDATASRCTYPE)getDataSrcType{
    return m_srcType;
}

@end
