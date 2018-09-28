//
//  SDUISys.h
//  SVEDemo
//
//  Created by 李晓帆 on 2018/9/3.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "SDMainVC.h"
@interface SDUISys : NSObject
@property (strong, nonatomic) SDMainVC *pMainVC;
+(instancetype) getInst;
- (void)initSys;
@end
