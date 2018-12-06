//
//  PLCreateUITool.m
//  YOYO
//
//  Created by 王成 on 14-8-3.
//  Copyright (c) 2014年 yoyo-corp.com. All rights reserved.
//

#import "PLCreateUITool.h"
//#import "NSString+LabelFrame.h"

@implementation PLCreateUITool
// uilabel
+ (UILabel *)createLabelInitWithFrame:(CGRect)frame
                      backgroundColor:(UIColor *)backColor
                        textAlignment:(NSTextAlignment)alignment
                                 text:(NSString *)text
                            textColor:(UIColor *)textColor
                                 font:(UIFont *)font
                            superView:(UIView *)superView
{
    UILabel *titleLabel;
    if (frame.size.width == 0)
    {
        titleLabel = [UILabel new];
    }
    titleLabel = [[UILabel alloc] initWithFrame:frame];
    titleLabel.backgroundColor = backColor;
    titleLabel.textAlignment = alignment;
    titleLabel.text = text;
    titleLabel.textColor = textColor;
    titleLabel.font = font;
    if (superView)
    {
        [superView addSubview:titleLabel];
    }
    return titleLabel;
}

// uiimage
+ (UIImageView *)createImageViewWithPoint:(CGPoint)point fileName:(NSString *)fileName superView:(UIView *)superView
{
    UIImage *image = [UIImage imageNamed:fileName];

    UIImageView *imageView =
        [[UIImageView alloc] initWithFrame:CGRectMake(point.x, point.y, image.size.width, image.size.height)];
    [imageView setImage:image];
    [superView addSubview:imageView];

    return imageView;
}

+ (UIButton *)createButtonWithPoint:(CGPoint)point
                normalImageFileName:(NSString *)normalImageFileName
                  highlightFileName:(NSString *)highlightFileName
                          superView:(UIView *)superView
{
    UIImage *normalImage = [UIImage imageNamed:normalImageFileName];
    UIButton *btn =
        [[UIButton alloc] initWithFrame:CGRectMake(point.x, point.y, normalImage.size.width, normalImage.size.height)];
    btn.exclusiveTouch = YES;
    [btn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    btn.titleLabel.textAlignment = NSTextAlignmentCenter;
    btn.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
    btn.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    [btn setBackgroundImage:normalImage forState:UIControlStateNormal];
    if (highlightFileName && highlightFileName.length > 0)
    {
        [btn setBackgroundImage:[UIImage imageNamed:highlightFileName] forState:UIControlStateHighlighted];
    }
    [superView addSubview:btn];

    return btn;
}
+ (UIButton *)createTextButtonWithPoint:(CGPoint)point title:(NSString *)titleStr superView:(UIView *)superView
{
    UIButton *_backBtn = [[UIButton alloc] initWithFrame:CGRectMake(point.x, point.y, 64, 44)];
    _backBtn.exclusiveTouch = YES;
    _backBtn.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
    _backBtn.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    _backBtn.backgroundColor = [UIColor clearColor];

    _backBtn.titleLabel.textAlignment = NSTextAlignmentCenter;
    [_backBtn setTitle:titleStr forState:UIControlStateNormal];
    _backBtn.titleLabel.font = [UIFont systemFontOfSize:18.0f];
    [_backBtn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [superView addSubview:_backBtn];
    return _backBtn;
}
+ (UIButton *)createButtonWithRect:(CGRect)rect superView:(UIView *)superView
{
    UIButton *btn =
        [[UIButton alloc] initWithFrame:CGRectMake(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height)];
    btn.backgroundColor = [UIColor clearColor];
    btn.exclusiveTouch = YES;
    [btn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    btn.titleLabel.textAlignment = NSTextAlignmentCenter;
    btn.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
    btn.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    [superView addSubview:btn];

    return btn;
}

/**
 *  创建一个 hightlightImage 为selectedimage unHilighedImage为 normalImage 的button
 */
+ (UIButton *)createBtnWithRect:(CGRect)rect
                      superView:(UIView *)superView
                    normalImage:(UIImage *)normalImage
                  selectedImage:(UIImage *)selectedImage
                   disableImage:(UIImage *)disableImage

{
    UIButton *button = [PLCreateUITool createBtnWithRect:rect
                                               superView:superView
                                             normalImage:normalImage
                                           selectedImage:selectedImage
                                         hightlightImage:selectedImage
                                         unHilighedImage:normalImage
                                            disableImage:disableImage];

    return button;
}
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
                   disableImage:(UIImage *)disableImage

{
    UIButton *button = [PLCreateUITool createButtonWithRect:rect superView:superView];

    [button setImage:normalImage forState:UIControlStateNormal];
    [button setImage:selectedImage forState:UIControlStateSelected];

    if (!hightedImage)
    {
        hightedImage = selectedImage;
    }
    if (!unHilighedImage)
    {
        unHilighedImage = normalImage;
    }
    [button setImage:hightedImage forState:UIControlStateHighlighted];
    [button setImage:unHilighedImage forState:UIControlStateHighlighted | UIControlStateSelected];
    if (disableImage)
    {
        [button setImage:disableImage forState:UIControlStateDisabled];
    }

    return button;
}

//+ (UIButton *)createTextAndImageButtonWithRect:(CGRect)rect
//                                         image:(NSString *)imageName
//                                          text:(NSString *)title
//                                     superView:(UIView *)superView
//{
//    UIButton *btn = [[UIButton alloc] initWithFrame:rect];
//    btn.exclusiveTouch = YES;
//    [btn setBackgroundColor:[UIColor clearColor]];
//
//    btn.contentHorizontalAlignment = UIControlContentHorizontalAlignmentLeft;
//    btn.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
//
//    UIImage *image = [UIImage imageNamed:imageName];
//    btn.imageEdgeInsets = UIEdgeInsetsMake(0, 10, 0, 0);
//    [btn setImage:image forState:UIControlStateNormal];
//    [btn setImage:image forState:UIControlStateHighlighted];
//
//    [btn setTitle:title forState:UIControlStateNormal];
//
//    float rightW = (rect.size.width == NX_MAIN_SCREEN_WIDTH) ? 44 : 10;
//    [btn setTitleEdgeInsets:UIEdgeInsetsMake(0, 10 + image.size.width, 0, rightW)];
//
//    btn.titleLabel.textAlignment = NXTextAlignmentLeft;
//    [btn setTitleColor:PL_COLOR_BlACK forState:UIControlStateNormal];
//    btn.titleLabel.font = [UIFont systemFontOfSize:16];
//
//    [superView addSubview:btn];
//
//    return btn;
//}

//+ (UIButton *)createTextAndIconButtonWithRect:(CGRect)rect
//                                         icon:(NSString *)iconName
//                                     leftIcon:(BOOL)isLeft
//                                         text:(NSString *)title
//                                    textColor:(UIColor *)textColor
//                                         font:(int)fontSize
//                                    superView:(UIView *)superView
//{
//    UIButton *btn = [[UIButton alloc] initWithFrame:rect];
//    btn.exclusiveTouch = YES;
//    [btn setBackgroundColor:[UIColor clearColor]];
//
//    UIImage *image = [UIImage imageNamed:iconName];
//    CGSize size = [NSString getStringRect:title
//                                     font:[UIFont systemFontOfSize:fontSize]
//                                labelSize:CGSizeMake(CGRectGetWidth(rect) - image.size.width, CGRectGetHeight(rect))];
//    float x = (CGRectGetWidth(rect) - (size.width + 5 + image.size.width)) * 0.5;
//    if (isLeft)
//    {
//        UIImageView *imageView =
//            [[UIImageView alloc] initWithFrame:CGRectMake(x, (CGRectGetHeight(rect) - image.size.height) / 2,
//                                                          image.size.width, image.size.height)];
//        [imageView setImage:image];
//        [btn addSubview:imageView];
//
//        UILabel *titleLable = [[UILabel alloc]
//            initWithFrame:CGRectMake(CGRectGetMaxX(imageView.frame) + 5, (CGRectGetHeight(rect) - fontSize - 1) * 0.5,
//                                     size.width, fontSize + 1)];
//        [titleLable setFont:[UIFont systemFontOfSize:fontSize]];
//        titleLable.textColor = textColor;
//        titleLable.backgroundColor = [UIColor clearColor];
//        titleLable.textAlignment = NXTextAlignmentCenter;
//        titleLable.text = title;
//        [btn addSubview:titleLable];
//    }
//    else
//    {
//        UILabel *titleLable = [[UILabel alloc]
//            initWithFrame:CGRectMake(x, (CGRectGetHeight(rect) - fontSize - 1) * 0.5, size.width, fontSize + 1)];
//        [titleLable setFont:[UIFont systemFontOfSize:fontSize]];
//        titleLable.textColor = textColor;
//        titleLable.backgroundColor = [UIColor clearColor];
//        titleLable.textAlignment = NXTextAlignmentCenter;
//        titleLable.text = title;
//        [btn addSubview:titleLable];
//
//        UIImageView *imageView =
//            [[UIImageView alloc] initWithFrame:CGRectMake(CGRectGetMaxX(titleLable.frame) + 5,
//                                                          (CGRectGetHeight(rect) - image.size.height) / 2,
//                                                          image.size.width, image.size.height)];
//        [imageView setImage:image];
//        [btn addSubview:imageView];
//    }
//
//    if (superView) [superView addSubview:btn];
//
//    return btn;
//}

//+ (UIImageView *)createSeparatorLineWithPoint:(CGPoint)point superView:(UIView *)superView
//{
//    UIImageView *separatorLineView =
//        [[UIImageView alloc] initWithFrame:CGRectMake(point.x, point.y, NX_MAIN_SCREEN_WIDTH - 2 * point.x, 0.5)];
//
//    //    separatorLineView.backgroundColor = YOYO_COLOR_LINE_GRAY;
//    separatorLineView.backgroundColor = [UIColor colorWithRed:1 green:1 blue:1 alpha:0.1];
//    [superView addSubview:separatorLineView];
//
//    return separatorLineView;
//}
//+ (UIImageView *)createVerticalLineWithPoint:(CGPoint)point height:(float)height superView:(UIView *)superView
//{
//    UIImageView *separatorLineView = [[UIImageView alloc] initWithFrame:CGRectMake(point.x, point.y, 0.5, height)];
//
//    separatorLineView.backgroundColor = YOYO_COLOR_LINE_GRAY;
//    [superView addSubview:separatorLineView];
//
//    return separatorLineView;
//}
//+ (UIBarButtonItem *)navigationItemWithNameString:(NSString *)name
//                                           Target:(id)target
//                                           action:(SEL)action
//                                           isleft:(BOOL)isleft
//{
//    UIButton *naviBtn = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, (name.length > 2) ? 64 : 44, 44)];
//    naviBtn.exclusiveTouch = YES;
//    if (name.length > 4)
//    {
//        naviBtn.frame = CGRectMake(0, 0, 80, 44);
//    }
//    [naviBtn setTitle:name forState:UIControlStateNormal];
//    [naviBtn setTitleColor:PL_COLOR_BlACK forState:UIControlStateNormal];
//    [naviBtn setTitleColor:[UIColor colorWithWhite:0 alpha:0.5] forState:UIControlStateHighlighted];
//    naviBtn.titleLabel.font = [UIFont systemFontOfSize:16];
//    if (target && action)
//    {
//        [naviBtn addTarget:target action:action forControlEvents:UIControlEventTouchUpInside];
//    }
//
//    //设置字的位置
//    if (isleft)
//    {
//        naviBtn.contentHorizontalAlignment = UIControlContentHorizontalAlignmentLeft;
//        naviBtn.titleLabel.textAlignment = NXTextAlignmentLeft;
//        [naviBtn setTitleEdgeInsets:UIEdgeInsetsMake(0, 0, 0, 0)];
//    }
//    else
//    {
//        naviBtn.contentHorizontalAlignment = UIControlContentHorizontalAlignmentRight;
//        naviBtn.titleLabel.textAlignment = NXTextAlignmentRight;
//
//        [naviBtn setTitleEdgeInsets:UIEdgeInsetsMake(0, 0, 0, -10)];
//    }
//
//    UIBarButtonItem *naviBtnItem = [[UIBarButtonItem alloc] initWithCustomView:naviBtn];
//    return naviBtnItem;
//}

+ (UIBarButtonItem *)navigationItemBackImage:(UIImage *)naviImage
                                 highlighted:(UIImage *)hightLightedImage
                                      Target:(id)target
                                      action:(SEL)action
                                      isLeft:(BOOL)isleft
{
    UIButton *naviBtn = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, naviImage.size.width, naviImage.size.height)];
    naviBtn.exclusiveTouch = YES;
    if (naviImage)
    {
        [naviBtn setImage:naviImage forState:UIControlStateNormal];
    }
    if (hightLightedImage)
    {
        [naviBtn setImage:hightLightedImage forState:UIControlStateHighlighted];
    }

    if (isleft)
    {
        naviBtn.contentHorizontalAlignment = UIControlContentHorizontalAlignmentLeft;
        naviBtn.imageEdgeInsets = UIEdgeInsetsMake(0, -15, 0, 0);
    }
    else
    {
        naviBtn.contentHorizontalAlignment = UIControlContentHorizontalAlignmentRight;
        naviBtn.imageEdgeInsets = UIEdgeInsetsMake(0, 0, 0, -15);
    }
    [naviBtn addTarget:target action:action forControlEvents:UIControlEventTouchUpInside];
    if (!naviImage || !hightLightedImage)
    {
        NSLog(@"navigationImage is nil");
    }
    UIBarButtonItem *naviBtnItem = [[UIBarButtonItem alloc] initWithCustomView:naviBtn];
    return naviBtnItem;
}

//+ (UILabel *)setNavigationItmeTitleView:(NSString *)title
//{
//    UILabel *titleLable = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 100, 30)];
//    titleLable.center = CGPointMake(NX_MAIN_SCREEN_WIDTH / 2., 22);
//    [titleLable setFont:[UIFont systemFontOfSize:17.f]];
//    titleLable.textColor = PL_COLOR_BlACK;
//    titleLable.textAlignment = NXTextAlignmentCenter;
//    titleLable.text = title;
//
//    return titleLable;
//}

//+ (void)createBlackLineWithY:(CGFloat)y SuperView:(UIView *)superView
//{
//    UIView *view = [[UIView alloc] initWithFrame:CGRectMake(0, y, NX_MAIN_SCREEN_WIDTH, 4.f)];
//    view.backgroundColor = [UIColor blackColor];
//    [superView addSubview:view];
//}

+ (void)createTipViewWithPoint:(CGPoint)point image:(UIImage *)image target:(UIView *)target
{
    UIImageView *imageView =
        [[UIImageView alloc] initWithFrame:CGRectMake(point.x, point.y, image.size.width, image.size.height)];
    imageView.image = image;
    [target addSubview:imageView];
}

@end
