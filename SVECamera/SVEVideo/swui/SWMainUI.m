//
//  SWMainUI.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWMainUI.h"
#import "SWUISys.h"
#import "SWFrameUI.h"
#import "SWSVView.h"
#import "SWState.h"
#import "../datasrc/SWInStreamSampleBuffer.h"
#import "../swlogic/SWLogicSys.h"
#import "../swlogic/SWLogicAni.h"
#import "../swbasic/SWBasicSys.h"
#import "../datasrc/SWDataSourceCamera.h"
#import <SVEngine/SVIEffect.h>
#import <SVEngine/SVIGameRedPacket.h>
#import <SVEngine/SVIDivision.h>
#import "SWStateFilterFile.h"
//

@interface SWMainUI()<UIGestureRecognizerDelegate>{
    SWFrameUI* pTopPanel;
    SWFrameUI* pBottomPanel;
    //填充UI
    UIView* pTopFit;
    UIView* pBottomFit;
    //上功能条
    UIView* pTopToolbar;
    //下功能条
    UIView* pBottomToolbar;
    
    UIButton *m_pTakPhoto;
    
    UIButton *m_pTakeVideo;
    
    
    //
    CGPoint m_point;
    UIImageView *m_imageView;
    
    //
    int mDivisionType;
}

@end

@implementation SWMainUI

- (void)dealloc{
    
}

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        //
        self.userInteractionEnabled = YES;
        self.m_uihide = false;
        //
        int t_w = frame.size.width;
        int t_h = frame.size.height;
        int t_view_px,t_view_py,t_view_w,t_view_h;
        CGPoint t_center;
        
        //顶层面板
        t_view_px = TRANS_BY_WIDTH(0,t_w);
        t_view_py = TRANS_BY_HEIGHT(0,t_h);
        t_view_w = TRANS_BY_WIDTH(750,t_w);
        t_view_h = TRANS_BY_HEIGHT(185,t_h);
        pTopPanel = [[SWFrameUI alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pTopPanel setBackgroundColor:[UIColor clearColor]];
        [self addSubview:pTopPanel];
        
        //上填充
        t_view_px = TRANS_BY_WIDTH(0,t_w);
        t_view_py = TRANS_BY_HEIGHT(0,t_h);
        t_view_w = TRANS_BY_WIDTH(750,t_w);
        t_view_h = TRANS_BY_HEIGHT(87,t_h);
        pTopFit = [[UIView alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pTopFit setBackgroundColor: [UIColor blackColor]];
        [pTopPanel addSubview:pTopFit];
        
        //上工具条
        t_view_px = TRANS_BY_WIDTH(0,t_w);
        t_view_py = TRANS_BY_HEIGHT(87,t_h);
        t_view_w = TRANS_BY_WIDTH(750,t_w);
        t_view_h = TRANS_BY_HEIGHT(98,t_h);
        pTopToolbar = [[UIView alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pTopToolbar setBackgroundColor: [UIColor blackColor]];
        [pTopPanel addSubview:pTopToolbar];
        
        //底层面板
        t_view_px = TRANS_BY_WIDTH(0,t_w);
        t_view_py = TRANS_BY_HEIGHT(1460,t_h);
        t_view_w = TRANS_BY_WIDTH(750,t_w);
        t_view_h = TRANS_BY_HEIGHT(165,t_h);
        pBottomPanel = [[SWFrameUI alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBottomPanel setBackgroundColor:[UIColor blackColor]];
        [self addSubview:pBottomPanel];
        
        //下填充
        t_view_px = TRANS_BY_WIDTH(0,t_w);
        t_view_py = TRANS_BY_HEIGHT(94,t_h);//1554-1460
        t_view_w = TRANS_BY_WIDTH(750,t_w);
        t_view_h = TRANS_BY_HEIGHT(71,t_h);
        pBottomFit = [[UIView alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBottomFit setBackgroundColor: [UIColor blackColor]];
        [pBottomPanel addSubview:pBottomFit];
        
        //下工具条
        t_view_px = TRANS_BY_WIDTH(0,t_w);
        t_view_py = TRANS_BY_HEIGHT(0,t_h);//1460-1299
        t_view_w = TRANS_BY_WIDTH(750,t_w);
        t_view_h = TRANS_BY_HEIGHT(98,t_h);
        pBottomToolbar = [[UIView alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBottomToolbar setBackgroundColor: [UIColor blackColor]];
        [pBottomPanel addSubview:pBottomToolbar];
        
        t_view_px = TRANS_BY_WIDTH(250,t_w);
        t_view_py = TRANS_BY_HEIGHT(1100,t_h);
        t_view_w = TRANS_BY_WIDTH(100,t_w);
        t_view_h = TRANS_BY_HEIGHT(100,t_h);
        m_pTakPhoto = [[UIButton alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        m_pTakPhoto.backgroundColor = [UIColor greenColor];
        [m_pTakPhoto addTarget:self action:@selector(eventTakePhoto:) forControlEvents:UIControlEventTouchUpInside];
//        [self addSubview:m_pTakPhoto];
        
        t_view_px = TRANS_BY_WIDTH(280,t_w);
        t_view_py = TRANS_BY_HEIGHT(1100,t_h);
        t_view_w = TRANS_BY_WIDTH(200,t_w);
        t_view_h = TRANS_BY_HEIGHT(200,t_h);
        m_pTakeVideo = [[UIButton alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        m_pTakeVideo.backgroundColor = [UIColor redColor];
        m_pTakPhoto.layer.cornerRadius = 20.0f;
        m_pTakPhoto.clipsToBounds = YES;
        [m_pTakeVideo addTarget:self action:@selector(eventTakeVideo:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:m_pTakeVideo];
        
        t_view_px = TRANS_BY_WIDTH(580,t_w);
        t_view_py = TRANS_BY_HEIGHT(1100,t_h);
        t_view_w = TRANS_BY_WIDTH(100,t_w);
        t_view_h = TRANS_BY_HEIGHT(100,t_h);
        UIButton *t_divisionBtn = [[UIButton alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        t_divisionBtn.backgroundColor = [UIColor greenColor];
        t_divisionBtn.layer.cornerRadius = 20.0f;
        t_divisionBtn.clipsToBounds = YES;
        [t_divisionBtn addTarget:self action:@selector(eventDvivision:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:t_divisionBtn];
        mDivisionType = 0;
        //
        [self initTopToolBar:frame];
        [self initBottomToolBar:frame];

    }
    return self;
}

-(void)initTopToolBar:(CGRect)frame{
    
}

-(void)initBottomToolBar:(CGRect)frame{
    
}

-(void)initSlider:(CGRect)frame{
   
}

- (void)changeToShow{
}


- (UIView*)hitTest:(CGPoint)point withEvent:(UIEvent *)event{
    UIView *hitView = [super hitTest:point withEvent:event];
    if(hitView == self){
        return nil;
    }
    return hitView;
}

//隐藏框架
- (void)hideFrame {
    //上面隐藏
    [pTopPanel disappear:2 AtOnce:false];
    //下面隐藏
    [pBottomPanel disappear:1 AtOnce:false];
    //
    self.m_uihide = true;
}

//显示框架
- (void)showFrame {
    [pTopPanel appear:2];
    [pBottomPanel appear:1];
    self.m_uihide = false;
}

//空消息
- (void)eventNull:(UIButton *)btn{
    NSLog(@"eventNull\n");
}

//拍照
- (void)eventTakePhoto:(UIButton *)btn{
    NSLog(@"eventTakePhoto\n");
    [[SWBasicSys getInst].m_outStream.m_pRecordImg savedImageToFileCompression:1.0];
}

- (void)eventTakeVideo:(UIButton *)btn{
    btn.selected = !btn.isSelected;
    if (btn.isSelected) {
        [[SWBasicSys getInst].m_outStream.m_pRecordVideo startRecord];
    }else{
        [[SWBasicSys getInst].m_outStream.m_pRecordVideo finishRecord];
    }
}

//所有
- (void)eventAll:(UIButton *)btn{
    NSLog(@"eventAll\n");
    [self hideFrame];
}

//导出
- (void)eventExport:(UIButton *)btn{
    NSLog(@"eventExport\n");
}

//用户
- (void)eventUser:(UIButton *)btn{
    NSLog(@"mainui user btn\n");
}

//切换相机
- (void)eventChangeCamera:(UIButton *)btn{
    NSLog(@"eventChangeCamera\n");
    //[[SWBasicSys getInst].pSWCamera swipCamera];
}

- (void)eventDvivision:(UIButton *)btn{
    mDivisionType++;
    if (mDivisionType>3) {
        mDivisionType = 0;
    }
    [[SWLogicSys getInst].pSVI.pDivison switchDivision:mDivisionType];
}

- (void)refreshUI{
  
}

-(void) touchesBegan:(NSSet *)touches withEvents:(UIEvent *)event {
    NSLog(@"touchesBegan\n");
}

-(void) touchesMoved:(NSSet *)touches withEvents:(UIEvent *)event {
    NSLog(@"touchesMoved\n");
}

-(void) touchesEnded:(NSSet *)touches withEvents:(UIEvent *)event {
    NSLog(@"touchesEnded\n");
}

-(void) touchesCancelled:(NSSet *)touches withEvents:(UIEvent *)event{
    NSLog(@"touchesCancelled\n");
}

@end
