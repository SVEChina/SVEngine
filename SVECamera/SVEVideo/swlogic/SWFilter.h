//
//  SWFilter.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>

//滤镜管理

@interface SWFilter : NSObject

@property (copy, nonatomic) NSString* m_imageKey;       //滤镜图片
@property (copy, nonatomic) NSString* m_owner;          //滤镜属于谁的
@property (copy, nonatomic) NSString* m_name;           //滤镜名称
@property (copy, nonatomic) NSString* m_createdTime;    //滤镜修改时间
@property (assign, nonatomic) int downLoadState; //下载状态   0 未下载 1 已下载  2正在下载

- (void)refreshData:(NSDictionary *)dic;

- (NSDictionary *)convertToDic;

@end
