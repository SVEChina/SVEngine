//
//  SVInterface.h
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVIHead.h"

#if TARGET_OS_IPHONE
@class SVIThread;
@interface SVInterfaceBase : NSObject{
    void*  m_pApp;
    SVIThread *m_thread;
}
- (instancetype)initWithSVE:(void *)_sve Thread:(SVIThread *)_thread;

-(void*)getSVE;
@end

#endif
