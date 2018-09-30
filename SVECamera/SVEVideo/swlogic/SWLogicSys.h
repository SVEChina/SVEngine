//
//  SWLogicSys.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SWState.h"
#import "SWLogicPlatform.h"
#import "SWLogicUser.h"
#import "SWLogicAni.h"
#import "SWLogicFilter.h"
#import "SWLogicFunction.h"
#import <SVEngine/SVI.h>
#import <SVEngine/SVICamera.h>
#import <SVEngine/SVIDetect.h>
#import <SVEngine/SVIEffect.h>
#import <SVEngine/SVIGame.h>
#import <SVEngine/SVIGameRedPacket.h>
#import <SVEngine/SVIDivision.h>
#define DESIGN_WIDTH   750
#define DESING_HEIGHT  1624

#define TRANS_BY_WIDTH(value,width) value*1.0f/DESIGN_WIDTH*width
#define TRANS_BY_HEIGHT(value,height) value*1.0f/DESING_HEIGHT*height

//
#define TRANS_BY_PARENT_WIDHT(value,pw,width) value*1.0f/pw*width
#define TRANS_BY_PARENT_HEIGHT(value,ph,height) value*1.0f/ph*height

@interface SWLogicSys : NSObject

+ (instancetype) getInst;

- (void)initSys;

- (void)destroySys;

- (void)initSVE;

- (void)destroySVE;

-(EAGLContext*)getGLContext;

@property (strong, nonatomic) SWState *pSWState;
@property (strong, nonatomic) SWLogicPlatform *pLogicPlatform;
@property (strong, nonatomic) SWLogicUser *pLogicUser;
@property (strong, nonatomic) SWLogicAni *pLogicAni;
@property (strong, nonatomic) SWLogicFilter *pLogicFilter;      //滤镜
@property (strong, nonatomic) SWLogicFunction *pLogicFunction;  //功能
@property (strong, nonatomic) SVI *pSVI;


@end
