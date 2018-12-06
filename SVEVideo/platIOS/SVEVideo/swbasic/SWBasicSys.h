//
//  SWBasicSys.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SWRecord.h"
#import "SWMediaPlayer.h"
#import "../swbasic/SWMotionManager.h"
#import "../datasrc/SWDataSource.h"
#import "../datasrc/SWOutStream.h"
#import "../datasrc/SWInStream.h"
#import "../detect/SWDetection.h"
//基础系统

@interface SWBasicSys : NSObject

+ (instancetype) getInst;

- (void)initSys;

- (void)destroySys;

@property (strong, nonatomic)SWDetection *m_detection;
@property (strong, nonatomic)SWInStream *m_inStream;
@property (strong, nonatomic)SWOutStream *m_outStream;
@property (strong, nonatomic) SWDataSource *m_pDataSrc;
//@property (strong, nonatomic) SWMediaPlayer *pSWMediaPlayer;
@property (strong, nonatomic) SWMotionManager *m_pFMMotion;

@end
