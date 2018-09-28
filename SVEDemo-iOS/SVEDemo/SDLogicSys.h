//
//  SDLogicSys.h
//  SVEDemo
//
//  Created by 李晓帆 on 2018/9/3.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SVI.h"
#import "SDState.h"
#import "SDCamera.h"
@interface SDLogicSys : NSObject
@property (strong, nonatomic) SVI *pSVI;
@property (strong, nonatomic) SDState *pState;
@property (strong, nonatomic)SDCamera *pCamera;
+(instancetype) getInst;
- (void)initSys;
- (void)destroySys;
-(EAGLContext*)getGLContext;
@end
