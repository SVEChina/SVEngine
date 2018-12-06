//
//  PLCreateUITool.h
//  YOYO
//
//  Created by 王成 on 14-8-3.
//  Copyright (c) 2014年 yoyo-corp.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface PLCreateUITool : NSObject

+ (UILabel *)createLabelInitWithFrame:(CGRect)frame
                      backgroundColor:(UIColor *)backColor
                        textAlignment:(NSTextAlignment)alignment
                                 text:(NSString *)text
                            textColor:(UIColor *)textColor
                                 font:(UIFont *)font
                            superView:(UIView *)superView;

+ (UIImageView *)createImageViewWithPoint:(CGPoint)point fileName:(NSString *)fileName superView:(UIView *)superView;

#pragma mark - button
+ (UIButton *)createButtonWithPoint:(CGPoint)point
                normalImageFileName:(NSString *)normalImageFileName
                  highlightFileName:(NSString *)highlightFileName
                          superView:(UIView *)superView;

+ (UIButton *)createButtonWithRect:(CGRect)rect superView:(UIView *)superView;

/**
 *  创建一个 hightlightImage 为selectedimage unHilighedImage为 normalImage 的button
 */
+ (UIButton *)createBtnWithRect:(CGRect)rect
                      superView:(UIView *)superView
                    normalImage:(UIImage *)normalImage
                  selectedImage:(UIImage *)selectedImage
                   disableImage:(UIImage *)disableImage;
/**
 *
 *  @param normalImage     正常态图片
 *  @param selectedImage   选中态图片
 *  @param hightedImage    高亮态图片
 *  @param unHilighedImage 选中-->正常时的高亮图
 *  @param disableImage    disable态图片
 *
 */
+ (UIButton *)createBtnWithRect:(CGRect)rect
                      superView:(UIView *)superView
                    normalImage:(UIImage *)normalImage
                  selectedImage:(UIImage *)selectedImage
                hightlightImage:(UIImage *)hightedImage
                unHilighedImage:(UIImage *)unHilighedImage
                   disableImage:(UIImage *)disableImage;

+ (UIButton *)createTextButtonWithPoint:(CGPoint)point title:(NSString *)titleStr superView:(UIView *)superView;

+ (UIButton *)createTextAndImageButtonWithRect:(CGRect)rect
                                         image:(NSString *)imageName
                                          text:(NSString *)title
                                     superView:(UIView *)superView;

+ (UIButton *)createTextAndIconButtonWithRect:(CGRect)rect
                                         icon:(NSString *)iconName
                                     leftIcon:(BOOL)isLeft
                                         text:(NSString *)title
                                    textColor:(UIColor *)textColor
                                         font:(int)fontSize
                                    superView:(UIView *)superView;

#pragma mark - sepLine

+ (UIImageView *)createSeparatorLineWithPoint:(CGPoint)point superView:(UIView *)superView;

//垂直分割线
+ (UIImageView *)createVerticalLineWithPoint:(CGPoint)point height:(float)height superView:(UIView *)superView;

+ (UIBarButtonItem *)navigationItemWithNameString:(NSString *)name
                                           Target:(id)target
                                           action:(SEL)action
                                           isleft:(BOOL)isleft;

+ (UIBarButtonItem *)navigationItemBackImage:(UIImage *)naviImage
                                 highlighted:(UIImage *)hightLightedImage
                                      Target:(id)target
                                      action:(SEL)action
                                      isLeft:(BOOL)isleft;

+ (UILabel *)setNavigationItmeTitleView:(NSString *)title;

// 创建黑色分割线
+ (void)createBlackLineWithY:(CGFloat)y SuperView:(UIView *)superView;

/**
 *    @brief 添加图片到指定位置
 *
 *    @param point  位置
 *    @param image  图片
 *    @param target 父视图
 */
+ (void)createTipViewWithPoint:(CGPoint)point image:(UIImage *)image target:(UIView *)target;

@end
