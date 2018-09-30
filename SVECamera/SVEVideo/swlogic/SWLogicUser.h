//
//  SWLogicUser.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>

//分享UI
enum USERSTATE{
    USER_NONE = 0,
    USER_GUEST = 1,
    USER_WX,
    USER_QQ,
    USER_WB
};

@interface SWLogicUser : NSObject

- (void)initModule;

- (void)destroyModule;

- (void)saveLocal;

- (void)userLogout;

@property (copy, nonatomic) NSString* m_userid;     //用户ID
@property (copy, nonatomic) NSString* m_password;   //密码
@property (copy, nonatomic) NSString* m_plattype;   //第三方平台类型
@property (assign, nonatomic) bool m_hasLogon;      //是否登录
@property (assign, nonatomic) int m_gold;           //金币
@property (assign, nonatomic) int m_magicB;         //魔法币
@property (copy, nonatomic) NSString* m_head;       //头像
@property (copy, nonatomic) NSString* m_name;       //名字

@end
