//
//  SWLogicPlatform.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>

@class SWShareUnit;

@interface SWLogicPlatform : NSObject

- (void)initModule;

- (void)destroyModule;

- (void)initMagics;

- (void)shareImageToQQ:(NSString *)imgLocPath;

- (void)shareImageToWX:(NSString *)imgLocPath;

- (void)shareImageToPYQ:(NSString *)imgLocPath;

- (void)shareImageToWB:(NSString *)imgLocPath;

- (void)shareVideoToQQVideo:(NSString *)_vioname Image:(NSString*)_imgname Score:(int)_score;

- (void)shareVideoToWXVideo:(NSString *)_vioname Image:(NSString*)_imgname Score:(int)_score;

- (void)shareVideoToPYQVideo:(NSString *)_vioname Image:(NSString*)_imgname Score:(int)_score;  //朋友圈

- (void)shareVideoToWBVideo:(NSString *)_vioname Image:(NSString*)_imgname Score:(int)_score;

- (void)shareVideoToJMVideo:(NSString *)_vioname Image:(NSString*)_imgname Score:(int)_score;//上传视频

- (void)connectMagics;

- (void)autoLogon;

- (bool)logonMagicsThird:(NSNumber*)_plat ID:(NSString*)_id Name:(NSString*)_name;

- (SWShareUnit*)hasShareUnit:(NSString*)_videores;

- (void)createALiClient;

//
@property (copy, nonatomic) NSString* m_appToken;
@property (copy, nonatomic) NSString* m_userToken;

@end
