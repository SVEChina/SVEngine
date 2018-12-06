//
//  SWLogicAni.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWLogicAni.h"
#import "SWLogicSys.h"
#import "swmagic/SWMagicCategoryUnit.h"
#import "../swui/SWUISys.h"

#define DOWNLOADEFFECT @"DownloadEffect"
#define CATEGORYLIST @"CategoryList"

@interface SWLogicAni (){
    dispatch_source_t _timer;
    //主UI动画心事
    float m_mainUIState;
    float m_mainUICurState;
}
@end

@implementation SWLogicAni

- (void)initModule{
    [self startTimer];
    m_mainUIState = 1;
    m_mainUICurState = 1;
}

- (void)destroyModule{
    [self stopTimer];
}

- (void)startTimer {
    // 延迟2s
    NSTimeInterval period = 0.03; //设置时间间隔
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    _timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue);
    dispatch_source_set_timer(_timer, dispatch_walltime(NULL, 0), period * NSEC_PER_SEC, 0); //每秒执行
    dispatch_source_set_event_handler(_timer, ^{
        //[self MainToolsShow];
    });
    dispatch_resume(_timer);
}

-(void)stopTimer{
    if(_timer) {
        dispatch_cancel(_timer);
    }
}

-(void)setMainUiState:(float)_value{
    m_mainUIState = _value;
}

-(void)MainToolsShow {
}

@end
