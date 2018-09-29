//
//  SVIDetect.h
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVInterfaceBase.h"

#if TARGET_OS_IPHONE


@interface SVIDetect : SVInterfaceBase

//开启识别
-(void)enableDetect:(int)_type;

//关闭识别
-(void)closeDetect;

//
-(void)pushDetectDataFace:(void *)_faceData;

@end

#endif
