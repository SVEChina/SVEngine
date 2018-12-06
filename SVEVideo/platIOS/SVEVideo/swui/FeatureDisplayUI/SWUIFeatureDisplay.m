//
//  SWUIFeatureDisplay.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWUIFeatureDisplay.h"
#import "SWFeatureMagicEffectsUI.h"
#import "../../swlogic/SWLogicSys.h"
#import "../../utility/Utility.h"
#import "../../swui/SWUISys.h"
#import "../../swbasic/SWBasicSys.h"
#import "../../datasrc/SWDataSourceCamera.h"
#import "../../datasrc/SWDataSourcePic.h"
#import "../../datasrc/SWDataSourceVideo.h"
#import "../../datasrc/SWDataSourceNet.h"
@interface SWUIFeatureDisplay(){
    UIButton* pBtnReturn;
    UIButton* pBtnChange;
    UIButton* pBtnTakeImage;
    UIButton* pBtnMagics;
    SWFeatureMagicEffectsUI *featuremagicEffectUI;
}
@end

@implementation SWUIFeatureDisplay
+ (SWUIFeatureDisplay *)createWithFrame:(CGRect)frame{
    return [[SWUIFeatureDisplay alloc] initWithFrame:frame];
}

- (instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = [UIColor clearColor];
        int t_w = frame.size.width;
        int t_h = frame.size.height;
        int t_view_px,t_view_py,t_view_w,t_view_h;
        //用户按钮
        t_view_px = TRANS_BY_WIDTH(28,t_w);
        t_view_py = TRANS_BY_HEIGHT(22,t_h);
        t_view_w = TRANS_BY_WIDTH(85,t_w);
        t_view_h = TRANS_BY_HEIGHT(85,t_h);
        pBtnReturn = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnReturn setImage:[UIImage imageNamed:@"common_return"] forState:UIControlStateNormal];
        [pBtnReturn setImage:[UIImage imageNamed:@"common_return_press"] forState:UIControlStateHighlighted];
        [pBtnReturn addTarget:self action:@selector(eventReturn:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnReturn];
        //切换
        t_view_px = TRANS_BY_WIDTH(634,t_w);
        t_view_py = TRANS_BY_HEIGHT(22,t_h);
        t_view_w = TRANS_BY_WIDTH(85,t_w);
        t_view_h = TRANS_BY_HEIGHT(85,t_h);
        pBtnChange = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnChange setImage:[UIImage imageNamed:@"common_swip"] forState:UIControlStateNormal];
        [pBtnChange setImage:[UIImage imageNamed:@"common_swip_press"] forState:UIControlStateHighlighted];
        [pBtnChange addTarget:self action:@selector(eventChangeCamera:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnChange];
        //魔贴
        t_view_px = TRANS_BY_WIDTH(634,t_w);
        t_view_py = TRANS_BY_HEIGHT(133,t_h);
        t_view_w = TRANS_BY_WIDTH(85,t_w);
        t_view_h = TRANS_BY_HEIGHT(85,t_h);
        pBtnMagics = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnMagics setBackgroundColor:[UIColor clearColor]];
        [pBtnMagics setImage:[UIImage imageNamed:@"face_motie"] forState:UIControlStateNormal];
        [pBtnMagics setImage:[UIImage imageNamed:@"face_motie_press"] forState:UIControlStateSelected];
        [pBtnMagics addTarget:self action:@selector(eventEffects:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnMagics];
        
        //相机
        t_view_px = TRANS_BY_WIDTH(297,t_w);
        t_view_py = TRANS_BY_HEIGHT(1125,t_h);
        t_view_w = TRANS_BY_WIDTH(153,t_w);
        t_view_h = TRANS_BY_HEIGHT(153,t_h);
        pBtnTakeImage = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnTakeImage setBackgroundColor:[UIColor clearColor]];
        [pBtnTakeImage setImage:[UIImage imageNamed:@"magic_camera"] forState:UIControlStateNormal];
        [pBtnTakeImage setImage:[UIImage imageNamed:@"magic_camera"] forState:UIControlStateSelected];
        [pBtnTakeImage addTarget:self action:@selector(takePic) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnTakeImage];
        //创建动效
        [self createMagicsEffectUI];
    }
    return self;
}


- (void)changeToShow{
}

- (void)createMagicsEffectUI{
    featuremagicEffectUI = [[SWFeatureMagicEffectsUI alloc] initWithFrame:CGRectMake(0, self.bounds.size.height, self.bounds.size.width, 250)];
    featuremagicEffectUI.hidden = YES;
    [self addSubview:featuremagicEffectUI];
}

//用户
- (void)eventReturn:(UIButton *)btn{
    NSLog(@"mainui return btn\n");
    
}


//切换相机
- (void)eventChangeCamera:(UIButton *)btn{
    NSLog(@"eventChangeCamera\n");
    if ([[SWBasicSys getInst].m_pDataSrc getDataSrcType] == DATASRCCAMERA) {
        SWDataSourceCamera *camera = (SWDataSourceCamera *)[SWBasicSys getInst].m_pDataSrc;
        [camera swipCamera];
    }
}

//膜贴列表
- (void)eventEffects:(UIButton *)btn{
    if (pBtnMagics.isSelected) {
        pBtnMagics.selected = NO;
        [featuremagicEffectUI hideWithAni:YES];
    }else{
        pBtnMagics.selected = YES;
        [featuremagicEffectUI showWithAni:YES];
    }
    
}

- (void)takePic{
//    [[SWBasicSys getInst].pSWRecord saveImageToFileCompress:1.0f];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
    pBtnMagics.selected = NO;
    [featuremagicEffectUI hideWithAni:YES];
}
/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

@end
