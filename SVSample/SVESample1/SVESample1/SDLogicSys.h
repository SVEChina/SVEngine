//
//  SDLogicSys.h
//  SVEDemo
//
//  Created by 李晓帆 on 2018/9/3.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ViewController.h"
#include "app/SVInst.h"

@interface SDLogicSys : NSObject

+(instancetype) getInst;

- (void)initSys;

- (void)destroySys;

-(EAGLContext*)getGLContext;

-(SVInst*)getSVE;

@end
