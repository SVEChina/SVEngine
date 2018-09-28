//
//  SVInterface.h
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVIHead.h"

#if TARGET_OS_IPHONE

@interface SVInterfaceBase : NSObject{
    void*  m_pApp;
}

-(void*)getSVE;

-(void)setSVE:(void*)_sve;

@end

#endif
