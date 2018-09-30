//
//  SWLogicUser.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWLogicUser.h"
#import "SWLogicSys.h"
#import "SWLogicPlatform.h"
#import "../utility/SWFileUtil.h"

//用户系统

@interface SWLogicUser (){
    enum USERSTATE m_userState;     //用户状态
}
@end

@implementation SWLogicUser

- (void)initModule{
    self.m_userid = @"default";
    self.m_name = @"default";
    self.m_plattype = @"default";
    self.m_gold = 0;
    //
    self.m_magicB = 0;
    self.m_password = @"default";
    self.m_head = @"default";
    self.m_hasLogon = false;
    

    NSDictionary* t_info = [[NSUserDefaults standardUserDefaults] objectForKey:@"userinfo"];
    if(!t_info){
        //第一次使用 需要写入默认的信息
        NSMutableDictionary* t_info = [[NSMutableDictionary alloc] init];
        [t_info setObject:@"default" forKey:@"userid"];
        [t_info setObject:@"点击登录" forKey:@"username"];
        [t_info setObject:@"default" forKey:@"platType"];
        [t_info setObject:@"default" forKey:@"headimg"];
        [t_info setObject:@"99999999" forKey:@"gold"];
        [[NSUserDefaults standardUserDefaults] setObject:t_info forKey:@"userinfo"];
        if( [[NSUserDefaults standardUserDefaults] synchronize] ){
            NSLog(@"write sucess!\n");
        }else{
            NSLog(@"write failure!\n");
        }
    }else{
        //非第一次使用 读取信息
        self.m_userid = [t_info objectForKey:@"userid"];
        self.m_name = [t_info objectForKey:@"username"];
        self.m_head = [t_info objectForKey:@"headimg"];
        self.m_plattype = [t_info objectForKey:@"platType"];
        NSString* gold = [t_info objectForKey:@"gold"];
        if(gold){
            self.m_gold = [gold intValue];
        }
        self.m_hasLogon = false;
        self.m_magicB = 0;
    }
}

- (void)destroyModule{
}

//保存数据到本地
- (void)saveLocal{
    NSMutableDictionary* t_info = [[NSMutableDictionary alloc] init];
    //用户ID
    [t_info setObject:self.m_userid forKey:@"userid"];
    //用户名字
    [t_info setObject:self.m_name forKey:@"username"];
    //平台
    [t_info setObject:self.m_plattype forKey:@"platType"];
    //金币
    NSString* t_gold = [[NSString alloc] initWithFormat:@"%d",self.m_gold];
    [t_info setObject:t_gold forKey:@"gold"];
    //头像名称  相对路径
    [t_info setObject:self.m_head forKey:@"headimg"];
    [[NSUserDefaults standardUserDefaults] setObject:t_info forKey:@"userinfo"];
    if( [[NSUserDefaults standardUserDefaults] synchronize] ){
        NSLog(@"write sucess!\n");
    }else{
        NSLog(@"write failure!\n");
    }
}

-(void)userLogout{
    NSString *userInfoDirectory = [SWFileUtil pathForLibraryDirectoryWithPath:@"userinfo"];
    NSString *userHeadImgPath = [userInfoDirectory stringByAppendingPathComponent:self.m_head];
    [SWFileUtil removeItemAtPath:userHeadImgPath];
    //重置登录信息
    NSMutableDictionary* t_info = [[NSMutableDictionary alloc] init];
    [t_info setObject:@"default" forKey:@"userid"];
    [t_info setObject:@"点击登录" forKey:@"username"];
    [t_info setObject:@"default" forKey:@"platType"];
    [t_info setObject:@"default" forKey:@"headimg"];
    [t_info setObject:@"99999999" forKey:@"gold"];
    [[NSUserDefaults standardUserDefaults] setObject:t_info forKey:@"userinfo"];
    if( [[NSUserDefaults standardUserDefaults] synchronize] ){
        NSLog(@"write sucess!\n");
    }else{
        NSLog(@"write failure!\n");
    }
    
    self.m_userid = t_info[@"userid"];
    self.m_name = t_info[@"username"];
    self.m_plattype = t_info[@"platType"];
    self.m_gold = [t_info[@"gold"] intValue];
    self.m_magicB = [t_info[@"gold"] intValue];;
    self.m_password = @"default";
    self.m_head = t_info[@"headimg"];
    self.m_hasLogon = false;
}

@end
