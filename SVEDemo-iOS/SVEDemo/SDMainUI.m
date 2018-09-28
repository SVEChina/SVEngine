//
//  SDMainUI.m
//  SVEDemo
//
//  Created by 李晓帆 on 2018/9/3.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import "SDMainUI.h"
#import "SVI.h"
#import "SVIEffect.h"
#import "SDLogicSys.h"
@interface SDMainUI(){
    NSArray *effects;
    NSMutableArray *animationsBtn;
    NSArray *animations;
}
@end

@implementation SDMainUI
- (instancetype)init{
    self = [super init];
    if (self) {
        self.backgroundColor = [UIColor clearColor];
        animationsBtn = [[NSMutableArray alloc] init];
    }
    return self;
}

- (instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = [UIColor clearColor];
        animationsBtn = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)initSubViews{
    effects = [[NSArray alloc] initWithObjects:@"hero",@"qiuqian", nil];
    for (int i=0; i<effects.count; i++) {
        UIButton *t_btn = [[UIButton alloc] initWithFrame:CGRectMake(20 + (80 + 20)*i, self.frame.size.height - 50, 80, 40)];
        t_btn.tag = 100 + i;
        t_btn.titleLabel.font = [UIFont systemFontOfSize:14];
        [t_btn setTitle:[NSString stringWithFormat:@"effect%d",i] forState:UIControlStateNormal];
        [t_btn setTitleColor:[UIColor greenColor] forState:UIControlStateNormal];
        [t_btn addTarget:self action:@selector(click:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:t_btn];
    }
}

- (void)click:(UIButton *)btn{
    [[SDLogicSys getInst].pSVI.pEffect removeEffectOP:NULL msg:@""];
    NSString *t_effect_name = effects[btn.tag - 100];
    NSString *t_path = [[NSBundle mainBundle] pathForResource:t_effect_name ofType:@"bundle"];
    [[SDLogicSys getInst].pSVI.pEffect loadEffectPath:t_path OP:NULL msg:@""];
    
    //写死了
    if ([t_effect_name isEqualToString:@"hero"]) {
        animations = [[NSArray alloc] initWithObjects:@"attack", @"crouch", @"fall", @"head-turn", @"idle", @"jump", @"run", @"walk", nil];
    }else if ([t_effect_name isEqualToString:@"qiuqian"]){
        animations = [[NSArray alloc] initWithObjects:@"animation", nil];
    }
    [self refreshViews];
}

- (void)refreshViews{
    for (int i=0; i<animationsBtn.count; i++) {
        UIView *t_subView = animationsBtn[i];
        [t_subView removeFromSuperview];
    }
    [animationsBtn removeAllObjects];
    for (int i = 0; i<animations.count; i++) {
        UIButton *t_btn = [[UIButton alloc] initWithFrame:CGRectMake(self.frame.size.width - 80, 60 + (30 + 30)*i, 60, 30)];
        t_btn.tag = 200 + i;
        t_btn.titleLabel.font = [UIFont systemFontOfSize:10];
        [t_btn setTitle:animations[i] forState:UIControlStateNormal];
        [t_btn setTitleColor:[UIColor redColor] forState:UIControlStateNormal];
        [t_btn addTarget:self action:@selector(aniclick:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:t_btn];
        [animationsBtn addObject:t_btn];
    }
}

- (void)aniclick:(UIButton *)btn {
    NSString *t_ani_name = animations[btn.tag - 200];
    [[SDLogicSys getInst].pSVI.pEffect playAnimation:t_ani_name];
}
/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

@end
