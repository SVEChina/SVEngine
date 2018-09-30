//
//  UIImage+Helper.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIImage (Helper)
- (UIImage *)drawOnSelf:(UIImage *)image rect:(CGRect)frame;
-(UIImage *)compressOriginalImagetoSize:(CGSize)size;
- (UIImage *)scaleImagetoScale:(float)scaleSize;
- (UIImage *)roundedCornerImageWithCornerRadius:(CGFloat)cornerRadius;
@end
