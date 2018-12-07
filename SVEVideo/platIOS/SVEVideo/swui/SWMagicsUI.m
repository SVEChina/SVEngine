//
//  SWMagicsUI.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWMagicsUI.h"
#import "../swlogic/SWLogicSys.h"
#import "../swbasic/SWBasicSys.h"
#import "SWProgressView.h"
#import "../utility/Utility.h"
#import "SWUISys.h"
#import "../datasrc/SWDataSourceCamera.h"
@interface SWMagicsUI()<UIGestureRecognizerDelegate,SWRecordDelegate>{
    UIButton* pBtnReturn;
    UIButton* pBtnChange;
    UIButton* pBtnTakeImage;
    UIButton* pBtnMagics;
    UIButton* pBtnFilter;
    UIButton* pBtnBeauty;
    SWProgressView *pProgressView;
}
@end

@implementation SWMagicsUI

- (void)dealloc{
    NSLog(@"destory SWMagicsUI");
    [self destoryMagicsEffectUI];
    [self destoryMagicsFilterUI];
    [self destoryMagicsBeautyUI];
}

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        int t_w = frame.size.width;
        int t_h = frame.size.height;
        
        int t_view_px,t_view_py,t_view_w,t_view_h;
        //用户按钮
        t_view_px = TRANS_BY_WIDTH(28,t_w);
        t_view_py = TRANS_BY_HEIGHT(22,t_h);
        t_view_w = TRANS_BY_WIDTH(85,t_w);
        t_view_h = TRANS_BY_HEIGHT(85,t_h);
//        pBtnReturn = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
//        [pBtnReturn setImage:[UIImage imageNamed:@"common_return"] forState:UIControlStateNormal];
//        [pBtnReturn setImage:[UIImage imageNamed:@"common_return_press"] forState:UIControlStateHighlighted];
//        [pBtnReturn addTarget:self action:@selector(eventReturn:) forControlEvents:UIControlEventTouchUpInside];
//        [self addSubview:pBtnReturn];
//        //切换
//        t_view_px = TRANS_BY_WIDTH(634,t_w);
//        t_view_py = TRANS_BY_HEIGHT(22,t_h);
//        t_view_w = TRANS_BY_WIDTH(85,t_w);
//        t_view_h = TRANS_BY_HEIGHT(85,t_h);
//        pBtnChange = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
//        [pBtnChange setImage:[UIImage imageNamed:@"common_swip"] forState:UIControlStateNormal];
//        [pBtnChange setImage:[UIImage imageNamed:@"common_swip_press"] forState:UIControlStateHighlighted];
//        [pBtnChange addTarget:self action:@selector(eventChangeCamera:) forControlEvents:UIControlEventTouchUpInside];
//        [self addSubview:pBtnChange];
//        //魔贴
//        t_view_px = TRANS_BY_WIDTH(634,t_w);
//        t_view_py = TRANS_BY_HEIGHT(133,t_h);
//        t_view_w = TRANS_BY_WIDTH(85,t_w);
//        t_view_h = TRANS_BY_HEIGHT(85,t_h);
//        pBtnMagics = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
//        [pBtnMagics setBackgroundColor:[UIColor clearColor]];
//        [pBtnMagics setImage:[UIImage imageNamed:@"m_function"] forState:UIControlStateNormal];
//        [pBtnMagics setImage:[UIImage imageNamed:@"m_function_1"] forState:UIControlStateSelected];
//        [pBtnMagics addTarget:self action:@selector(eventEffects:) forControlEvents:UIControlEventTouchUpInside];
//        [self addSubview:pBtnMagics];
//        //滤镜
//        t_view_px = TRANS_BY_WIDTH(634,t_w);
//        t_view_py = TRANS_BY_HEIGHT(245,t_h);
//        t_view_w = TRANS_BY_WIDTH(85,t_w);
//        t_view_h = TRANS_BY_HEIGHT(85,t_h);
//        pBtnFilter = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
//        [pBtnFilter setBackgroundColor:[UIColor clearColor]];
//        [pBtnFilter setImage:[UIImage imageNamed:@"m_function"] forState:UIControlStateNormal];
//        [pBtnFilter setImage:[UIImage imageNamed:@"m_function_1"] forState:UIControlStateSelected];
//        [pBtnFilter addTarget:self action:@selector(eventFilter:) forControlEvents:UIControlEventTouchUpInside];
//        [self addSubview:pBtnFilter];
//        //美型
//        t_view_px = TRANS_BY_WIDTH(634,t_w);
//        t_view_py = TRANS_BY_HEIGHT(355,t_h);
//        t_view_w = TRANS_BY_WIDTH(85,t_w);
//        t_view_h = TRANS_BY_HEIGHT(85,t_h);
//        pBtnBeauty = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
//        [pBtnBeauty setBackgroundColor:[UIColor clearColor]];
//        [pBtnBeauty setImage:[UIImage imageNamed:@"m_function"] forState:UIControlStateNormal];
//        [pBtnBeauty setImage:[UIImage imageNamed:@"m_function_1"] forState:UIControlStateSelected];
//        [pBtnBeauty addTarget:self action:@selector(eventBeauty:) forControlEvents:UIControlEventTouchUpInside];
//        [self addSubview:pBtnBeauty];
//        //相机
//        t_view_px = TRANS_BY_WIDTH(297,t_w);
//        t_view_py = TRANS_BY_HEIGHT(1125,t_h);
//        t_view_w = TRANS_BY_WIDTH(153,t_w);
//        t_view_h = TRANS_BY_HEIGHT(153,t_h);
//        pBtnTakeImage = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
//        [pBtnTakeImage setBackgroundColor:[UIColor clearColor]];
//        [pBtnTakeImage setImage:[UIImage imageNamed:@"m_function"] forState:UIControlStateNormal];
//        [pBtnTakeImage setImage:[UIImage imageNamed:@"m_function_1"] forState:UIControlStateSelected];
//        [pBtnTakeImage addTarget:self action:@selector(takePic) forControlEvents:UIControlEventTouchUpInside];
//        [self addSubview:pBtnTakeImage];
//        //创建动效
//        [self createMagicsEffectUI];
//        //创建滤镜选择条
//        [self createMagicsFilterUI];
//        //创建美型调整条
//        [self createMagicsBeautyUI];
      
    }
    return self;
}

- (void)createMagicsEffectUI{
    if (!self.magicsEffectUI) {
        self.magicsEffectUI = [[SWMagicsEffectsUI alloc] initWithFrame:CGRectMake(0, self.bounds.size.height, self.bounds.size.width, 250)];
        self.magicsEffectUI.hidden = YES;
        [self addSubview:self.magicsEffectUI];
    }
    [self.magicsEffectUI refreshAllData];
}

- (void)destoryMagicsEffectUI{
    [self.magicsEffectUI removeFromSuperview];
    self.magicsEffectUI = nil;
}

- (void)createMagicsFilterUI{
    if (!self.magicsFilterUI) {
        int t_w = self.bounds.size.width;
        int t_h = self.bounds.size.height;
        float t_view_px = TRANS_BY_WIDTH(0,t_w);
        float t_view_py = TRANS_BY_HEIGHT(942,t_h);
        float t_view_w = TRANS_BY_WIDTH(750,t_w);
        float t_view_h = TRANS_BY_HEIGHT(164,t_h);
        self.magicsFilterUI = [[SWMagicsFiltersUI alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        self.magicsFilterUI.hidden = YES;
        [self addSubview:self.magicsFilterUI];
    }
}

- (void)destoryMagicsFilterUI{
    [self.magicsFilterUI removeFromSuperview];
    self.magicsFilterUI = nil;
}

- (void)createMagicsBeautyUI{
    if (!self.magicsBeautyUI) {
        int t_w = self.bounds.size.width;
        int t_h = self.bounds.size.height;
        float t_view_px = TRANS_BY_WIDTH(0,t_w);
        float t_view_py = TRANS_BY_HEIGHT(830,t_h);
        float t_view_w = TRANS_BY_WIDTH(750,t_w);
        float t_view_h = TRANS_BY_HEIGHT(275,t_h);
        self.magicsBeautyUI = [[SWFilterSlider alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        self.magicsBeautyUI.hidden = YES;
        [self addSubview:self.magicsBeautyUI];
    }
}

- (void)destoryMagicsBeautyUI{
    [self.magicsBeautyUI  removeFromSuperview];
    self.magicsBeautyUI = nil;
}

- (void)changeToShow{
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
}

- (void)eventFilter:(UIButton *)btn{
}

- (void)eventBeauty:(UIButton *)btn{
}

- (void)showEffectUIAnimate:(BOOL)ani{
}

- (void)hideEffectUIAnimate:(BOOL)ani{
}

- (void)eventBtnLong:(UILongPressGestureRecognizer *)gestureRecognizer{
}

- (void)takePic{
//    [[SWBasicSys getInst].pSWRecord saveImageToFileCompress:1.0f];
}

- (void)startRecordVideo{
    NSString *curTime = [Utility createTimeStamp];
    NSString *fileName = [NSString stringWithFormat:@"fm2Movie_%@.mp4",curTime];
    NSURL *videoURL = [[NSURL alloc] initFileURLWithPath:[NSString pathWithComponents:@[NSTemporaryDirectory(), fileName]]];
//    [[SWBasicSys getInst].pSWRecord.pRecordVideo setReocrdURL:videoURL];
//    [[SWBasicSys getInst].pSWRecord startRecording];
}

- (void)stopRecordVideo{
//    [[SWBasicSys getInst].pSWRecord stopRecording];
}

- (void)cancelleRecordVideo{
//    [[SWBasicSys getInst].pSWRecord sstopRecording];
}

#pragma mark -- fmrecord delegate
- (void)saveImageOver{
}

- (void)videoRecordError:(NSError *)error{
    
}
- (void)videoRecordDidStart{
    
}
- (void)videoRecordWillStop{
    
}
- (void)videoRecordDidStop{
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
}

@end
