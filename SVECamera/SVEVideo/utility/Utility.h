//
//  Utility.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface Utility : NSObject
+ (UIImage *)convertViewToImage:(UIView *)view;
+ (UIViewController *)findViewController:(UIView *)sourceView;
+ (NSString *)createTimeStamp;
+ (NSString *)createTimeStampSinceDay:(int)day;//获取几天前的时间戳
+ (UIImage *)buttonImageFromColor:(UIColor *)color size:(CGSize)size;
+ (UIImage *)getGradientImageWithColors:(NSArray*)colors imgSize:(CGSize)imgSize;
@end
