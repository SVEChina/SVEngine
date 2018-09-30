//
//  SWStartupUI.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWStartupUI.h"
#import "SWUISys.h"
#import "../swlogic/SWLogicSys.h"
#import "../swbasic/SWBasicSys.h"
#import "../utility/Utility.h"

@interface SWStartupUI(){
    int m_backTime;
}

@end

@implementation SWStartupUI

- (void)dealloc{
    NSLog(@"destory SWStartupUI");
}

- (void)viewDidLoad{
    [super viewDidLoad];
    float t_w = self.view.bounds.size.width;
    float t_h = self.view.bounds.size.height;
    //
    float t_view_px = TRANS_BY_WIDTH(0,t_w);
    float t_view_py = TRANS_BY_WIDTH(0,t_w);
    float t_view_w = TRANS_BY_WIDTH(750,t_w);
    float t_view_h = TRANS_BY_WIDTH(1624,t_w);
    UIImageView* t_imgview = [[UIImageView alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
    t_imgview.image = [UIImage imageNamed:@"startup"];
    [self.view addSubview:t_imgview];
    m_backTime = 3;
}

- (void)gcdTimer {
    // 延迟2s
    dispatch_time_t delayTime = dispatch_time(DISPATCH_TIME_NOW, 2 * NSEC_PER_SEC);
    dispatch_after(delayTime, dispatch_get_main_queue(), ^(void){
        NSLog(@"延迟2s后执行");
        //[[SWUISys getInst].pNavi show];
        [[SWUISys getInst].pNavi pushViewController:[SWUISys getInst].pMainVC animated:false];
    });
    NSLog(@"start");
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}



@end
