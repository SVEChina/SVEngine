//
//  SWMediaPlayer.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SWDataSource.h"
#import "../datasrc/SWInStreamSampleBuffer.h"

//作为一个视频流的数据源

@interface SWMediaPlayer : SWDataSource

- (void)initModule;

- (void)destroyModule;

- (void)preparePlay:(NSURL*)_url  Frame:(CGRect)_frame View:(UIView*)_view;

- (void)close;

- (void)play;

- (void)stop;

- (void)pause;

@end
