//
//  SWDataSourceARCamera.h
//  SVEVideo
//
//  Created by 李晓帆 on 2018/7/5.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import "SWDataSource.h"

@interface SWDataSourceARCamera : SWDataSource
- (instancetype)init;
- (void)hitPoint:(CGPoint)_point;
@end