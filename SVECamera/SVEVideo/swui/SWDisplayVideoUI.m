//
//  SWDisplayVideoUI.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWDisplayVideoUI.h"
#import "../swlogic/SWLogicSys.h"
#import "../swbasic/SWBasicSys.h"
#import "../utility/Utility.h"
#import "SWUISys.h"

@interface SWDisplayVideoUI(){
    UIButton *pBtnClose;
}
@end
@implementation SWDisplayVideoUI
+ (SWDisplayVideoUI *)createWithFrame:(CGRect)frame{
    return [[SWDisplayVideoUI alloc] initWithFrame:frame];
}

- (void)dealloc{
    NSLog(@"destory SWDisplayVideoUI");
}

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        int t_w = frame.size.width;
        int t_h = frame.size.height;
        int t_view_px,t_view_py,t_view_w,t_view_h;
        
        t_view_px = TRANS_BY_WIDTH(43,t_w);
        t_view_py = TRANS_BY_HEIGHT(45,t_h);
        t_view_w = TRANS_BY_WIDTH(48,t_w);
        t_view_h = TRANS_BY_HEIGHT(48,t_h);
        pBtnClose = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnClose setBackgroundColor:[UIColor clearColor]];
        [pBtnClose setTitle:@"关闭" forState:UIControlStateNormal];
        [pBtnClose setImage:[UIImage imageNamed:@"common_close"] forState:UIControlStateNormal];
        [pBtnClose addTarget:self action:@selector(eventClose:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnClose];
        self.backgroundColor = [UIColor clearColor];
    }
    return self;
}

- (void)loadMovieUrl:(NSURL *)url{
//    [[SWBasicSys getInst].pSWMediaPlayer preparePlay:url Frame:CGRectMake(0, 0, self.bounds.size.width, self.bounds.size.height) View:self];
}

- (void)eventClose:(UIButton *)btn{
    UIViewController *targetVC = [Utility findViewController:self];
    [targetVC dismissViewControllerAnimated:YES completion:^{
//        [[SWBasicSys getInst].pSWMediaPlayer close];
    }];
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

@end
