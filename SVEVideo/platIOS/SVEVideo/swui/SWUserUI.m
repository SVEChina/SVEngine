//
//  SWUserUI.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWUserUI.h"
#import "SWUISys.h"
#import "SWSetupUI.h"
#import "../swlogic/SWLogicSys.h"
#import "../utility/SWFileUtil.h"

@interface SWUserUI(){
    UIImageView* pImgBg;
    UIImageView* pImgHead;
    UIButton* pBtnLogon;
    //
    UIImageView* pImgGold;
    UILabel* pLableGold;
    UIImageView* pImgGoldLine;
    //
    UIImageView* pImgMofabi;
    UILabel* pLableMofabi;
    UIImageView* pImgMofabiLine;
    //
    UIButton* pBtnSetting;
    UIButton* pBtnFriend;
    UIButton* pBtnLogout;
    
}
@end

@implementation SWUserUI

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        int t_w = frame.size.width;
        int t_h = frame.size.height;
        int t_view_px,t_view_py,t_view_w,t_view_h;
        //背景
        t_view_px = TRANS_BY_WIDTH(0,t_w);
        t_view_py = TRANS_BY_WIDTH(0,t_w);
        t_view_w = TRANS_BY_WIDTH(750,t_w);
        t_view_h = TRANS_BY_WIDTH(1624,t_w);
        pImgBg = [[UIImageView alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pImgBg setImage:[UIImage imageNamed:@"com_bg"]];
        [self addSubview:pImgBg];
        //头像
        t_view_px = TRANS_BY_WIDTH(233,t_w);
        t_view_py = TRANS_BY_WIDTH(162,t_w);
        t_view_w = TRANS_BY_WIDTH(126,t_w);
        t_view_h = TRANS_BY_WIDTH(126,t_w);
        
        pImgHead = [[UIImageView alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pImgHead setImage:[UIImage imageNamed:@"user_head_nor"]];
        pImgHead.userInteractionEnabled = YES;
        UITapGestureRecognizer *tapHead = [[UITapGestureRecognizer alloc] init];
        [tapHead addTarget:self action:@selector(eventLogon)];
        [pImgHead addGestureRecognizer:tapHead];
        [self addSubview:pImgHead];
        //未登录
        t_view_px = TRANS_BY_WIDTH(194,t_w);
        t_view_py = TRANS_BY_WIDTH(322,t_w);
        t_view_w = TRANS_BY_WIDTH(200,t_w);
        t_view_h = TRANS_BY_WIDTH(27,t_w);
        pBtnLogon = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnLogon setTitle:@"点击登录" forState:UIControlStateNormal];
        //[pBtnLogon setImage:[UIImage imageNamed:@"user_name"] forState:UIControlStateNormal];
        [pBtnLogon addTarget:self action:@selector(eventLogon) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnLogon];
        //金币图像
        t_view_px = TRANS_BY_WIDTH(188,t_w);
        t_view_py = TRANS_BY_WIDTH(416,t_w);
        t_view_w = TRANS_BY_WIDTH(45,t_w);
        t_view_h = TRANS_BY_WIDTH(45,t_w);
        pImgGold = [[UIImageView alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pImgGold setImage:[UIImage imageNamed:@"com_gold_nor"]];
        [self addSubview:pImgGold];
        //金币数字
        t_view_px = TRANS_BY_WIDTH(254,t_w);
        t_view_py = TRANS_BY_WIDTH(422,t_w);
        t_view_w = TRANS_BY_WIDTH(480,t_w);
        t_view_h = TRANS_BY_WIDTH(32,t_w);
        pLableGold = [[UILabel alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pLableGold setText:@"0"];
        [pLableGold setTextColor:[UIColor whiteColor]];
        [self addSubview:pLableGold];
        //金币线
        t_view_px = TRANS_BY_WIDTH(124,t_w);
        t_view_py = TRANS_BY_WIDTH(482,t_w);
        t_view_w = TRANS_BY_WIDTH(360,t_w);
        t_view_h = TRANS_BY_WIDTH(5,t_w);
        pImgGoldLine = [[UIImageView alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pImgGoldLine setImage:[UIImage imageNamed:@"com_line0"]];
        [self addSubview:pImgGoldLine];
        //魔法币
        t_view_px = TRANS_BY_WIDTH(188,t_w);
        t_view_py = TRANS_BY_WIDTH(519,t_w);
        t_view_w = TRANS_BY_WIDTH(43,t_w);
        t_view_h = TRANS_BY_WIDTH(43,t_w);
        pImgMofabi = [[UIImageView alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pImgMofabi setImage:[UIImage imageNamed:@"com_mofabi_nor"]];
        [self addSubview:pImgMofabi];
        //魔法币数字
        t_view_px = TRANS_BY_WIDTH(254,t_w);
        t_view_py = TRANS_BY_WIDTH(524,t_w);
        t_view_w = TRANS_BY_WIDTH(480,t_w);
        t_view_h = TRANS_BY_WIDTH(32,t_w);
        pLableMofabi = [[UILabel alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        //[pLableMofabi setBackgroundColor:[UIColor whiteColor]];
        [pLableMofabi setText:@"0"];
        [pLableMofabi setTextColor:[UIColor whiteColor]];
        [self addSubview:pLableMofabi];
        //魔法币线
        t_view_px = TRANS_BY_WIDTH(124,t_w);
        t_view_py = TRANS_BY_WIDTH(591,t_w);
        t_view_w = TRANS_BY_WIDTH(360,t_w);
        t_view_h = TRANS_BY_WIDTH(5,t_w);
        pImgMofabiLine = [[UIImageView alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pImgMofabiLine setImage:[UIImage imageNamed:@"com_line0"]];
        [self addSubview:pImgMofabiLine];
        //设置
        t_view_px = TRANS_BY_WIDTH(53,t_w);
        t_view_py = TRANS_BY_WIDTH(874,t_w);
        t_view_w = TRANS_BY_WIDTH(495,t_w);
        t_view_h = TRANS_BY_WIDTH(82,t_w);
        pBtnSetting = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnSetting setImage:[UIImage imageNamed:@"user_set"] forState:UIControlStateNormal];
        [pBtnSetting addTarget:self action:@selector(eventSetting:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnSetting];
        //推荐给好友
        t_view_px = TRANS_BY_WIDTH(53,t_w);
        t_view_py = TRANS_BY_WIDTH(983,t_w);
        t_view_w = TRANS_BY_WIDTH(495,t_w);
        t_view_h = TRANS_BY_WIDTH(82,t_w);
        pBtnFriend = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnFriend setImage:[UIImage imageNamed:@"user_tuijian"] forState:UIControlStateNormal];
        [pBtnFriend addTarget:self action:@selector(eventSendFriends:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnFriend];
        //退出登录
        t_view_px = TRANS_BY_WIDTH(53,t_w);
        t_view_py = TRANS_BY_WIDTH(1090,t_w);
        t_view_w = TRANS_BY_WIDTH(495,t_w);
        t_view_h = TRANS_BY_WIDTH(82,t_w);
        pBtnLogout = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnLogout setImage:[UIImage imageNamed:@"user_logout"] forState:UIControlStateNormal];
        [pBtnLogout addTarget:self action:@selector(eventLogout:) forControlEvents:UIControlEventTouchUpInside];
        NSDictionary *userinfo = [[NSUserDefaults standardUserDefaults] objectForKey:@"userinfo"];
        if (![userinfo[@"userid"] isEqualToString:@"default"]) {
            pBtnLogout.enabled = YES;
        }else{
            pBtnLogout.enabled = NO;
        }
        [self addSubview:pBtnLogout];
    }
    return self;
}

//切换
- (void)changeToShow{
}

//刷新UI
- (void)refreshData{
    //头像数据
    NSString *avatarPath = [SWFileUtil pathForLibraryDirectoryWithPath:@"userinfo"];
    avatarPath = [avatarPath stringByAppendingPathComponent:[SWLogicSys getInst].pLogicUser.m_head];
    UIImage *avatarImage = [UIImage imageWithContentsOfFile:avatarPath];
    if (avatarImage) {
        pImgHead.image = avatarImage;
    }else{
        [pImgHead setImage:[UIImage imageNamed:@"user_head_nor"]];
    }
    //姓名
    [pBtnLogon setTitle:[SWLogicSys getInst].pLogicUser.m_name forState:UIControlStateNormal];
    NSDictionary *userinfo = [[NSUserDefaults standardUserDefaults] objectForKey:@"userinfo"];
    if (![userinfo[@"userid"] isEqualToString:@"default"]) {
        pBtnLogout.enabled = YES;
    }else{
        pBtnLogout.enabled = NO;
    }
    //金币数量
    NSString *t_gold = @"0";
    [pLableGold setText:t_gold];
    //魔法币数量
    NSString *t_magics = @"0";
    [pLableMofabi setText:t_magics];
}

- (void)eventLogon{
    NSLog(@"eventLogon\n");
}

- (void)eventLogout:(UIButton *)btn{
    NSLog(@"eventLogout\n");
    [[SWLogicSys getInst].pLogicUser userLogout];
    [self refreshData];
}

- (void)eventSendFriends:(UIButton *)btn{
    NSLog(@"eventSendFriends\n");
}

- (void)eventSetting:(UIButton *)btn{
    NSLog(@"eventSetting\n");
}

@end
