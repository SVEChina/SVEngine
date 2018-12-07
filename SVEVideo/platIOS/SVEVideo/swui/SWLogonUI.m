//
//  SWLogonUI.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWLogonUI.h"
#import "SWUISys.h"
#import "../swlogic/SWLogicSys.h"
#import "../swlogic/SWLogicLogon.h"
#import "../utility/Utility.h"

@interface SWLogonUI()<SWLogicLogonDelegate>{
    UIImageView* pImgBg;
    UIImageView* pImgHead;
    UILabel* pUserName;
    UIButton* pBtnLogonWB;
    UIButton* pBtnLogonWX;
    UIButton* pBtnLogonQQ;
    UIButton* pBtnClose;
    SWLogicLogon *pLogoicLogon;
    UIView       *backView;
    UIActivityIndicatorView *activityView;
}
@end

@implementation SWLogonUI

+ (SWLogonUI *)createWithFrame:(CGRect)frame{
    return [[SWLogonUI alloc] initWithFrame:frame];
}

- (void)dealloc{
    NSLog(@"destory SWLogonUI");
}

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        pLogoicLogon = [[SWLogicLogon alloc] init];
        pLogoicLogon.delegate = self;
        int t_w = frame.size.width;
        int t_h = frame.size.height;
        int t_view_px,t_view_py,t_view_w,t_view_h;
        //背景
        t_view_px = TRANS_BY_WIDTH(0,t_w);
        t_view_py = TRANS_BY_WIDTH(0,t_w);
        t_view_w = TRANS_BY_WIDTH(750,t_w);
        t_view_h = TRANS_BY_WIDTH(1334,t_w);
        pImgBg = [[UIImageView alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pImgBg setImage:[UIImage imageNamed:@"com_bg"]];
        [self addSubview:pImgBg];
        //关闭
        t_view_px = TRANS_BY_WIDTH(25,t_w);
        t_view_py = TRANS_BY_WIDTH(25,t_w);
        t_view_w = TRANS_BY_WIDTH(85,t_w);
        t_view_h = TRANS_BY_WIDTH(85,t_w);
        pBtnClose = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnClose setImage:[UIImage imageNamed:@"common_close"] forState:UIControlStateNormal];
        [pBtnClose setImage:[UIImage imageNamed:@"common_close_press"] forState:UIControlStateHighlighted];
        [pBtnClose addTarget:self action:@selector(eventCloseVideo:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnClose];
        //头像
        t_view_px = TRANS_BY_WIDTH(312,t_w);
        t_view_py = TRANS_BY_WIDTH(165,t_w);
        t_view_w = TRANS_BY_WIDTH(126,t_w);
        t_view_h = TRANS_BY_WIDTH(126,t_w);
        pImgHead = [[UIImageView alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pImgHead setImage:[UIImage imageNamed:@"user_head_nor"]];
        [self addSubview:pImgHead];
        //用户名称
        t_view_px = TRANS_BY_WIDTH(250,t_w);
        t_view_py = TRANS_BY_WIDTH(321,t_w);
        t_view_w = TRANS_BY_WIDTH(250,t_w);
        t_view_h = TRANS_BY_WIDTH(35,t_w);
        pUserName = [[UILabel alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pUserName setTextAlignment:NSTextAlignmentCenter];
        [pUserName setText:@"anonymous"];
        pUserName.font = [UIFont systemFontOfSize:12];
        [pUserName setTextColor:[UIColor whiteColor]];
        [self addSubview:pUserName];
        //QQ登录
        t_view_px = TRANS_BY_WIDTH(127,t_w);
        t_view_py = TRANS_BY_WIDTH(416,t_w);
        t_view_w = TRANS_BY_WIDTH(496,t_w);
        t_view_h = TRANS_BY_WIDTH(85,t_w);
        pBtnLogonQQ = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnLogonQQ setImage:[UIImage imageNamed:@"logon_qq"] forState:UIControlStateNormal];
        [pBtnLogonQQ addTarget:self action:@selector(eventLogonQQ:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnLogonQQ];
        //微信登录
        t_view_px = TRANS_BY_WIDTH(127,t_w);
        t_view_py = TRANS_BY_WIDTH(528,t_w);
        t_view_w = TRANS_BY_WIDTH(496,t_w);
        t_view_h = TRANS_BY_WIDTH(85,t_w);
        pBtnLogonWX = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnLogonWX setImage:[UIImage imageNamed:@"logon_wx"] forState:UIControlStateNormal];
        [pBtnLogonWX addTarget:self action:@selector(eventLogonWX:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnLogonWX];
        //微博登录
        t_view_px = TRANS_BY_WIDTH(127,t_w);
        t_view_py = TRANS_BY_WIDTH(638,t_w);
        t_view_w = TRANS_BY_WIDTH(496,t_w);
        t_view_h = TRANS_BY_WIDTH(85,t_w);
        pBtnLogonWB = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnLogonWB setImage:[UIImage imageNamed:@"logon_wb"] forState:UIControlStateNormal];
        [pBtnLogonWB addTarget:self action:@selector(eventLogonWB:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnLogonWB];
        
        //
        backView = [[UIView alloc] initWithFrame:self.bounds];
        backView.backgroundColor = [UIColor colorWithRed:0.0f green:0.0f blue:0.0f alpha:0.5f];
        activityView = [[UIActivityIndicatorView alloc]initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
        activityView.center = backView.center;
        [activityView setHidesWhenStopped:YES];
        backView.hidden = YES;
        [backView addSubview:activityView];
        [self addSubview:backView];
    }
    return self;
}
- (void)beginIndicator{
    backView.hidden = NO;
    [activityView startAnimating];
}

- (void)stopIndicator{
    backView.hidden = YES;
    [activityView stopAnimating];
}



- (void)eventLogonWX:(UIButton *)btn{
    NSLog(@"eventLogonWX\n");
    [self beginIndicator];
    [pLogoicLogon logonWX];
}

- (void)eventLogonWB:(UIButton *)btn{
    NSLog(@"eventLogonWB\n");
    [self beginIndicator];
    [pLogoicLogon logonWB];
}


- (void)eventLogonQQ:(UIButton *)btn{
    NSLog(@"eventLogonQQ\n");
    [self beginIndicator];
    [pLogoicLogon logonQQ];
}

- (void)eventCloseVideo:(UIButton *)btn{
    NSLog(@"share ui eventCloseVideo\n");
    UIViewController *targetVC = [Utility findViewController:self];
    [targetVC dismissViewControllerAnimated:YES completion:^{
        
    }];
}

#pragma mark --- SWLogicLogonDelegate
- (void)logonFailed{
//    [self stopIndicator];
//    [self makeToast:@"登录失败" duration:1.5f position:CSToastPositionCenter];
}

- (void)logonSuccess{
//    [self stopIndicator];
//    [self makeToast:@"登录成功" duration:1.0f position:CSToastPositionCenter ];
//    dispatch_time_t delayTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1.0/*延迟执行时间*/ * NSEC_PER_SEC));
//    dispatch_after(delayTime, dispatch_get_main_queue(), ^{
//        [self eventCloseVideo:nil];
//    });
}
@end
