//
//  SWShareUnit.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>

//分享单元
@interface SWShareUnit : NSObject

@property (copy, nonatomic) NSString* m_vioKey;
@property (copy, nonatomic) NSString* m_imgKey;
@property (copy, nonatomic) NSString* m_vioRes;
@property (copy, nonatomic) NSString* m_imgRes;
@property (copy, nonatomic) NSString* m_url;
@property (assign, nonatomic) int     m_score;

@end
