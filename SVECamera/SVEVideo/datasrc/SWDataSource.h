//
//  SWDataSource.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "../media/Camera.h"
#import "SWDetection.h"
#import "SWInStream.h"
#import "SWOutStream.h"
//数据源

@interface SWDataSource : NSObject
{
    SWDATASRCTYPE m_srcType;
}

+ (SWDataSource *)createDataSrcType:(SWDATASRCTYPE)dataSrcType;
- (void)initModule;
- (void)destroyModule;
- (void)start;
- (void)stop;
- (SWDATASRCTYPE)getDataSrcType;
@end
