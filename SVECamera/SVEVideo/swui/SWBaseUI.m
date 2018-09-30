//
//  SWBaseUI.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWBaseUI.h"

@interface SWBaseUI(){
}
@end

@implementation SWBaseUI

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.m_uihide = false;
    }
    return self;
}

- (void)destroy{
}

- (void)changeToShow{
}

- (void)refreshData{
}

//1 上 2 下 3 左 4 右
- (void)appear:(int)Dir {
    [UIView animateWithDuration: 0.2 delay: 0.1 options: UIViewAnimationOptionCurveEaseInOut animations: ^{
        CGPoint t_aimpt = self.center;
        if(Dir == 1 ){
            t_aimpt.y -= self.frame.size.height;
        }else if( Dir == 2) {
            t_aimpt.y += self.frame.size.height;
        }else if( Dir == 3) {
            t_aimpt.x -= self.frame.size.width;
        }else if( Dir == 4) {
            t_aimpt.x += self.frame.size.width;
        }
        [self setCenter:t_aimpt];
        [self setAlpha:0.4f];
    } completion: ^(BOOL finished) {
        [UIView animateWithDuration: 0.2 animations: ^{
            [self setAlpha:1.0f];
        }];
    }];
}

//1 上 2 下 3 左 4 右
- (void)disappear:(int)Dir AtOnce:(bool)Flag {
    if(Flag) {
        //立刻重置
        CGPoint t_aimpt = self.center;
        if(Dir == 1 ){
            t_aimpt.y += self.frame.size.height;
        }else if( Dir == 2) {
            t_aimpt.y -= self.frame.size.height;
        }else if( Dir == 3) {
            t_aimpt.x += self.frame.size.width;
        }else if( Dir == 4) {
            t_aimpt.x -= self.frame.size.width;
        }
        [self setCenter:t_aimpt];
        [self setAlpha:0.0f];
    }else {
        //动画
        [UIView animateWithDuration: 0.2 delay: 0.1 options: UIViewAnimationOptionCurveEaseInOut animations: ^{
            CGPoint t_aimpt = self.center;
            if(Dir == 1 ){
                t_aimpt.y += self.frame.size.height;
            }else if( Dir == 2) {
                t_aimpt.y -= self.frame.size.height;
            }else if( Dir == 3) {
                t_aimpt.x += self.frame.size.width;
            }else if( Dir == 4) {
                t_aimpt.x -= self.frame.size.width;
            }
            [self setCenter:t_aimpt];
            [self setAlpha:0.0f];
        } completion: ^(BOOL finished) {
            [UIView animateWithDuration: 0.2 animations: ^{
                [self setAlpha:0.0f];
            }];
        }];
    }
}

@end
