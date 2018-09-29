//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
@protocol MoviePlayerDelegate<NSObject>
@required
- (void)closeMoviePlayer;
@end
@interface MoviePlayer : NSObject
@property(weak, nonatomic)id<MoviePlayerDelegate>delegage;
-(instancetype)initWithView:(UIView *)targetView;
- (UIView *)getBackView;
- (void)showViewWithAni:(BOOL)ani;
- (void)hideViewWithAni:(BOOL)ani;
- (void)destory;
- (void)prepareToPlay:(NSURL *)url;
- (void)play;
- (void)pause;
- (void)stop;
@end
