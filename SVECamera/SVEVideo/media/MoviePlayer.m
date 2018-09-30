//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "MoviePlayer.h"
#import <MediaPlayer/MediaPlayer.h>
#import <AVFoundation/AVFoundation.h>

@interface MoviePlayer(){
    AVPlayer          *mPlayer;
    UIView            *mBackView;
}
@end

@implementation MoviePlayer
-(instancetype)initWithView:(UIView *)targetView{
    self = [super init];
    if (self) {
        [self initializeViews:targetView];
    }
    return self;
}

- (void)dealloc{
    [self destory];
}
- (void)destory{
    [self removeNotification];
    [self stop];
    [mBackView removeFromSuperview];
    mPlayer = nil;
}
- (void)initializeViews:(UIView *)targetView{
    mBackView = [[UIView alloc] initWithFrame:CGRectMake(0, targetView.frame.size.height, targetView.frame.size.width, targetView.frame.size.height)];
    mBackView.backgroundColor = [UIColor whiteColor];
    [targetView addSubview:mBackView];
    
    UIButton *closeBtn = [[UIButton alloc] initWithFrame:CGRectMake(10, 10, 40, 40)];
    [closeBtn setTitle:@"X" forState:UIControlStateNormal];
    [closeBtn setTitleColor:[UIColor redColor] forState:UIControlStateNormal];
    [closeBtn setBackgroundColor:[UIColor greenColor]];
    [closeBtn addTarget:self action:@selector(closeBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    [mBackView addSubview:closeBtn];
}

- (UIView *)getBackView{
    return mBackView;
}

- (void)closeBtnClick:(UIButton *)btn{
    if ([self.delegage respondsToSelector:@selector(closeMoviePlayer)]) {
        [self.delegage closeMoviePlayer];
    }
}

- (void)prepareToPlay:(NSURL *)url{
    if (mPlayer == nil) {
        // 1.获取URL(远程/本地)
//        NSURL *url = [NSURL URLWithString:path];
//         NSURL *url = [[NSBundle mainBundle] URLForResource:@"IMG_0660.mp4" withExtension:nil];
        // 2.创建AVPlayerItem
        AVPlayerItem *item = [AVPlayerItem playerItemWithURL:url];
        
        // 3.创建AVPlayer
        mPlayer = [AVPlayer playerWithPlayerItem:item];
        
        // 4.添加AVPlayerLayer
        AVPlayerLayer *layer = [AVPlayerLayer playerLayerWithPlayer:mPlayer];
        layer.frame = mBackView.bounds;
        [mBackView.layer insertSublayer:layer atIndex:0];
        [self addNotification];
    }
}

- (void)play{
    [mPlayer play];
}
- (void)pause{
    [mPlayer pause];
}
- (void)stop{
    [mPlayer pause];
}
/**
 *  添加播放器通知
 */
-(void)addNotification{
    //给AVPlayerItem添加播放完成通知
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playbackFinished:) name:AVPlayerItemDidPlayToEndTimeNotification object:mPlayer.currentItem];
}

-(void)removeNotification{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}
/**
 *  播放完成通知
 *
 *  @param notification 通知对象
 */
-(void)playbackFinished:(NSNotification *)notification{
    NSLog(@"视频播放完成.");
    // 播放完成后重复播放
    // 跳到最新的时间点开始播放
    [mPlayer seekToTime:CMTimeMake(0, 1)];
    [mPlayer play];
}

// 展开动画
- (void)beginAnimations
{
    [UIView animateWithDuration:0.3 animations:^{
        mBackView.frame = CGRectMake(0, 0, mBackView.frame.size.width, mBackView.frame.size.height);
    } completion:^(BOOL finished) {
        
    }];
    
}

// 收起动画
- (void)endAnimations
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    [UIView beginAnimations:nil context:context];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
    [UIView setAnimationDuration:0.3];//动画时间长度，单位秒，浮点数
    mBackView.frame = CGRectMake(0, mBackView.frame.size.height, mBackView.frame.size.width, mBackView.frame.size.height);
    [UIView commitAnimations];
}

- (void)showViewWithAni:(BOOL)ani{
    if (ani) {
        [self beginAnimations];
    }else{
        mBackView.frame = CGRectMake(0, 0, mBackView.frame.size.width, mBackView.frame.size.height);
    }
}

- (void)hideViewWithAni:(BOOL)ani{
    if (ani) {
        [self endAnimations];
    }else{
        mBackView.frame = CGRectMake(0, mBackView.frame.size.height, mBackView.frame.size.width, mBackView.frame.size.height);
    }
}

@end
