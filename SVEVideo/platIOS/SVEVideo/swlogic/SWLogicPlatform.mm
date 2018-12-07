//
//  SWLogicPlatform.mm
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWLogicPlatform.h"
#import "SWLogicSys.h"
#import "SWUISys.h"
#import "SWShareUnit.h"
#import "../utility/Utility.h"

@interface SWLogicPlatform () {
    NSString *rootMagicUrl;
    NSString *magicClientID;
    NSString *magicClientSecrect;
    //魔法商店相关
    NSNumber *mMagicsID;
    NSString *mMagicsCompany;
    NSString *mMagicsApp;
    //各种token过期时间
    NSString* m_appTokenValidTime;
    NSString* m_userTokenValidTime;
    //
    NSMutableArray* m_shareArray;
}
@end

@implementation SWLogicPlatform

- (instancetype) init{
    self = [super init];
    if(self){
        //
        m_shareArray = [[NSMutableArray alloc] init];
        //
        self.m_appToken = @"";
        self.m_userToken = @"";
        //
        m_appTokenValidTime = @"0";
        m_userTokenValidTime= @"0";
        //
        rootMagicUrl = @"";
        //fm2的账号密码
        magicClientID = @"";
        magicClientSecrect = @"";

    }
    return self;
}

- (void)initModule{
    [self initMagics];
    //注册第三方平台相关
}

- (void)initMagics{
}

- (void)destroyModule{
}

- (bool)isAppTokenExpire{
    long long t_tokentime = [m_appTokenValidTime longLongValue];
    if(t_tokentime == 0)
        return true;
    NSDate *datenow = [NSDate date];//现在时间,你可以输出来看下是什么格式
    NSString *timeSp = [NSString stringWithFormat:@"%ld", (long)[datenow timeIntervalSince1970]];
    long long t_systime = [timeSp longLongValue];
    if(t_tokentime - t_systime){
        return false;
    }
    return true;
}

- (bool)isUserTokenExpire{
    long long t_tokentime = [m_userTokenValidTime longLongValue];
    if(t_tokentime == 0)
        return true;
    NSDate *datenow = [NSDate date];//现在时间,你可以输出来看下是什么格式
    NSString *timeSp = [NSString stringWithFormat:@"%ld", (long)[datenow timeIntervalSince1970]];
    long long t_systime = [timeSp longLongValue];
    if(t_tokentime - t_systime){
        return false;
    }
    return true;
}

//
- (void)connectMagics{
}

//登陆magic
- (bool)logonMagicsThird:(NSNumber*)_plat ID:(NSString*)_id Name:(NSString*)_name{
    return true;
}

- (void)createALiClient{
}

//登录成功
- (void)logonSucess:(NSDictionary *)infoDic{
    NSString* t_name = @"aa";
    NSString* t_email = @"aa";
    NSString* t_id = @"aa";
    NSString* t_avatar = @"aa";
    NSString* t_usertoken = @"aa";
    NSString* t_rawPasswd = @"aa";
    NSNumber* t_mtype = 0;
    NSNumber* t_mark = 0;
    NSString* t_expireTime = @"aa";
    NSString* t_vtime = @"aa";
    NSString* t_origin = @"aa";
    //更新用户token
    self.m_userToken = t_usertoken;
    m_userTokenValidTime = t_expireTime;
    //更新用户数据
    [SWLogicSys getInst].pLogicUser.m_userid = t_id;
    [SWLogicSys getInst].pLogicUser.m_password = t_rawPasswd;
    if([t_mtype intValue] == 0){
        [SWLogicSys getInst].pLogicUser.m_plattype = @"WX";
    }else if([t_mtype intValue] == 1){
        [SWLogicSys getInst].pLogicUser.m_plattype = @"QQ";
    }else if([t_mtype intValue] == 2){
        [SWLogicSys getInst].pLogicUser.m_plattype = @"WB";
    }
    [SWLogicSys getInst].pLogicUser.m_name = t_name;
    NSString *t_headImgName = [NSString stringWithFormat:@"%@_%d.png",t_name, [t_mtype intValue]];
    [SWLogicSys getInst].pLogicUser.m_head = t_headImgName;
    [SWLogicSys getInst].pLogicUser.m_gold = 8888;
    [SWLogicSys getInst].pLogicUser.m_magicB = 8888;
    [SWLogicSys getInst].pLogicUser.m_hasLogon = true;
    //写入本地数据
    [[SWLogicSys getInst].pLogicUser saveLocal];
    //刷新数据
    [[SWUISys getInst].pUserView refreshData];
    [[SWUISys getInst].pUserView changeToShow];
}

- (void)shareImageToQQ:(NSString *)imgLocPath{
}

- (void)shareImageToWX:(NSString *)imgLocPath{
}

- (void)shareImageToPYQ:(NSString *)imgLocPath{
}

- (void)shareImageToWB:(NSString *)imgLocPath{
}

//是否本地存在
- (SWShareUnit*)hasShareUnit:(NSString*)_videores{
    //NSMutableArray* m_shareArray;
    for(int i=0;i<m_shareArray.count;i++){
        SWShareUnit* t_shareunit = m_shareArray[i];
        if([t_shareunit.m_vioRes isEqualToString:_videores] && (t_shareunit.m_url!=nil) ){
            return t_shareunit;
        }
    }
    return nil;
}

//视频名称规范
//用户ID_游戏名称_时间戳
//userid_gamename_timestamp.mp4

-(void)genKey:(NSString *)key Video:(SWShareUnit*)shareUnit{
}

- (void)shareVideoToQQVideo:(NSString *)_vioname Image:(NSString*)_imgname Score:(int)_score{
}

//分享朋友圈
- (void)shareVideoToPYQVideo:(NSString *)_vioname Image:(NSString*)_imgname Score:(int)_score{
}

//分享微博
- (void)shareVideoToWBVideo:(NSString *)_vioname Image:(NSString*)_imgname Score:(int)_score{
}

//微信分享
- (void)shareVideoToWXVideo:(NSString *)_vioname Image:(NSString*)_imgname Score:(int)_score{
}

-(void)shareVideoToJMVideo:(NSString *)_vioname Image:(NSString *)_imgname Score:(int)_score{
}

- (void)initHUDView{
    dispatch_async(dispatch_get_main_queue(), ^{
//        UIView *backView = [[UIView alloc]initWithFrame:SCREEN_FRAME];
//        backView.tag = 112233;
//        backView.backgroundColor = [UIColor colorWithRed:34.0/255.0 green:34.0/255.0 blue:34.0/255.0 alpha:0.5];//[UIColor redColor];//
//        [[[[UIApplication sharedApplication] delegate] window] addSubview:backView];
//
//        WBRoundHUD *hud6 = [[WBRoundHUD alloc] initWithFrame:CGRectMake(SCREEN_WIDTH/2 - 40, SCREEN_HEIGHT/2 - 40 , 80, 80) type:WBRoundHUDTypeGradient roundColor: nil];
//        hud6.duration = 2.0;
//        hud6.backgroundColor = [UIColor whiteColor];
//        hud6.roundColor = [UIColor redColor];
//        [backView addSubview:hud6];
//        [hud6 start];
    });

}

@end
