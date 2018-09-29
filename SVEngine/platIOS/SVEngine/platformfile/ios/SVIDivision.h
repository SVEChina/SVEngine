//
//  SVIDivision.h
//  SVEngine
//
//  Created by 李晓帆 on 2018/9/7.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import "SVInterfaceBase.h"
#if TARGET_OS_IPHONE

@interface SVIDivision : SVInterfaceBase
- (void)openDivision;
- (void)closeDivision;
- (void)switchDivision:(int)_type;
@end
#endif
