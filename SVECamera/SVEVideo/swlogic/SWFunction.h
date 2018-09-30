//
//  SWFunction.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>

//功能管理

@interface SWFunction : NSObject

@property (copy, nonatomic) NSString* m_image;          //功能图片
@property (copy, nonatomic) NSString* m_name;           //功能名称
@property (assign, nonatomic) bool m_enable;            //是否开启
@property (assign, nonatomic) NSString* m_owner;        //拥有权限

- (void)refreshData:(NSDictionary *)dic;

- (NSDictionary *)convertToDic;

@end
