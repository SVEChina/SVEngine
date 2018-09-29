//
//  SVIDetect.m
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVIDetect.h"
#import "SVOpDetect.h"
#import "SVInst.h"
#import "SVGlobalMgr.h"
#import "SVThreadMain.h"
#import "SVThreadPool.h"
#import "SVOpFilter.h"
#import "SVDataSwap.h"
#import "SVDetectST.h"
#import "SVDetectMgr.h"

#if TARGET_OS_IPHONE

@interface SVIDetect(){
}
@end


@implementation SVIDetect

- (instancetype)init{
    self = [super init];
    if (self) {
    }
    return self;
}

//开启识别算法
-(void)enableDetect:(int)_type{
}

//关闭识别算法
-(void)closeDetect{
}

-(void)pushDetectDataFace:(void *)_faceData{
    SVInst* t_app = (SVInst*)m_pApp;
    if( t_app->getDetectMgr() ) {
        SVDetectBasePtr t_detect = t_app->getDetectMgr()->getDetect();
        if( t_detect ) {
            t_detect->pushData(_faceData);
        }
    }
}

@end

#endif
