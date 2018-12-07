//
//  SWFilterRGBW.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWFilterRGBW.h"
#import "../swlogic/SWLogicSys.h"
#import "../utility/Utility.h"
#import "../utility/UIImage+Helper.h"

@interface SWFilterRGBW(){
    SWUIBezier* m_pActiveBezier;
    unsigned char *pToneCurveByteArray;
}
@end

@implementation SWFilterRGBW

- (instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        //
        pToneCurveByteArray = (unsigned char*)malloc(256*4);
        //
        self.backgroundColor = [UIColor clearColor];
        int t_w = frame.size.width;
        int t_h = frame.size.height;
        int t_view_px,t_view_py,t_view_w,t_view_h;
        //
        t_view_px = TRANS_BY_WIDTH(198,t_w);
        t_view_py = TRANS_BY_WIDTH(678,t_w);//1242 - 554
        t_view_w = TRANS_BY_WIDTH(50,t_w);
        t_view_h = TRANS_BY_WIDTH(50,t_w);
        self.m_pWhite = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        self.m_pWhite.layer.cornerRadius = t_view_w*0.5f;
        [self.m_pWhite setBackgroundColor:[UIColor whiteColor]];
        [self.m_pWhite addTarget:self action:@selector(eventWhite:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:self.m_pWhite];
//        button.layer.cornerRadius = 2.0;//2.0是圆角的弧度，根据需求自己更改
//        button.layer.borderColor = [UIColor blackColor];//设置边框颜色
//        button.layer.borderWidth = 1.0f;//设置边框颜色
        t_view_px = TRANS_BY_WIDTH(302,t_w);
        t_view_py = TRANS_BY_WIDTH(678,t_w);//1232 - 554
        t_view_w = TRANS_BY_WIDTH(50,t_w);
        t_view_h = TRANS_BY_WIDTH(50,t_w);
        self.m_pRed = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        self.m_pRed.layer.cornerRadius = t_view_w*0.5f;
        [self.m_pRed setBackgroundColor:[UIColor redColor]];
        [self.m_pRed addTarget:self action:@selector(eventRed:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:self.m_pRed];
        //
        t_view_px = TRANS_BY_WIDTH(406,t_w);
        t_view_py = TRANS_BY_WIDTH(678,t_w);//1232 - 554
        t_view_w = TRANS_BY_WIDTH(50,t_w);
        t_view_h = TRANS_BY_WIDTH(50,t_w);
        self.m_pGreen = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        self.m_pGreen.layer.cornerRadius = t_view_w*0.5f;
        [self.m_pGreen setBackgroundColor:[UIColor greenColor]];
        [self.m_pGreen addTarget:self action:@selector(eventGreen:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:self.m_pGreen];
        //
        t_view_px = TRANS_BY_WIDTH(510,t_w);
        t_view_py = TRANS_BY_WIDTH(678,t_w);//1232 - 554
        t_view_w = TRANS_BY_WIDTH(50,t_w);
        t_view_h = TRANS_BY_WIDTH(50,t_w);
        self.m_pBlue = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        self.m_pBlue.layer.cornerRadius = t_view_w*0.5f;
        [self.m_pBlue setBackgroundColor:[UIColor blueColor]];
        [self.m_pBlue addTarget:self action:@selector(eventBlue:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:self.m_pBlue];
        //
        t_view_px = TRANS_BY_WIDTH(55,t_w);
        t_view_py = TRANS_BY_WIDTH(30,t_w);//584 - 554
        t_view_w = TRANS_BY_WIDTH(640,t_w);
        t_view_h = TRANS_BY_WIDTH(640,t_w);
        self.pBezierR = [[SWUIBezier alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [self.pBezierR linkRGBW:self];
        [self.pBezierR setBackgroundColor:[UIColor clearColor]];
        [self.pBezierR setAlpha:0.2];
        self.pBezierR.lineColor = [UIColor redColor];
        [self addSubview:self.pBezierR];
        //
        self.pBezierG = [[SWUIBezier alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [self.pBezierG linkRGBW:self];
        [self.pBezierG setBackgroundColor:[UIColor clearColor]];
        [self.pBezierG setAlpha:0.2];
        self.pBezierG.lineColor = [UIColor greenColor];
        [self addSubview:self.pBezierG];
        //
        self.pBezierB = [[SWUIBezier alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [self.pBezierB linkRGBW:self];
        [self.pBezierB setBackgroundColor:[UIColor clearColor]];
        [self.pBezierB setAlpha:0.2];
        self.pBezierB.lineColor = [UIColor blueColor];
        [self addSubview:self.pBezierB];
        //
        self.pBezierW = [[SWUIBezier alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [self.pBezierW linkRGBW:self];
        [self.pBezierW setBackgroundColor:[UIColor clearColor]];
        [self.pBezierW setAlpha:1.0];
        self.pBezierW.lineColor = [UIColor whiteColor];
        [self addSubview:self.pBezierW];
        //
        m_pActiveBezier = self.pBezierW;
        [self bringSubviewToFront:m_pActiveBezier];
        [self bringSubviewToFront:self.m_pWhite];
        [self bringSubviewToFront:self.m_pRed];
        [self bringSubviewToFront:self.m_pGreen];
        [self bringSubviewToFront:self.m_pBlue];
    }
    return self;
}

- (void)eventWhite:(UIButton *)btn{
    NSLog(@"eventWhite\n");
    if(m_pActiveBezier){
        [m_pActiveBezier unactive];
    }
    [self.pBezierW active];
    m_pActiveBezier = self.pBezierW;
    [self bringSubviewToFront:m_pActiveBezier];
    //
    //动画
    [UIView animateWithDuration: 0.2 delay: 0.1 options: UIViewAnimationOptionCurveEaseInOut animations: ^{
        self.m_pWhite.transform = CGAffineTransformMakeScale(1.2,1.2);
        if(self.m_pActiveBt){
            self.m_pActiveBt.transform = CGAffineTransformMakeScale(1.0,1.0);
        }
    } completion: ^(BOOL finished) {
        [UIView animateWithDuration: 0.2 animations: ^{
            self.m_pActiveBt = self.m_pWhite;
        }];
    }];
}

- (void)eventRed:(UIButton *)btn{
    NSLog(@"eventRed\n");
    if(m_pActiveBezier){
        [m_pActiveBezier unactive];
    }
    [self.pBezierR active];
    m_pActiveBezier = self.pBezierR;
    [self bringSubviewToFront:m_pActiveBezier];
    //动画
    [UIView animateWithDuration: 0.2 delay: 0.1 options: UIViewAnimationOptionCurveEaseInOut animations: ^{
        self.m_pRed.transform = CGAffineTransformMakeScale(1.2,1.2);
        if(self.m_pActiveBt){
            self.m_pActiveBt.transform = CGAffineTransformMakeScale(1.0,1.0);
        }
    } completion: ^(BOOL finished) {
        [UIView animateWithDuration: 0.2 animations: ^{
            self.m_pActiveBt = self.m_pRed;
        }];
    }];
}

- (void)eventGreen:(UIButton *)btn{
    NSLog(@"eventGreen\n");
    if(m_pActiveBezier){
        [m_pActiveBezier unactive];
    }
    [self.pBezierG active];
    m_pActiveBezier = self.pBezierG;
    [self bringSubviewToFront:m_pActiveBezier];
    //动画
    [UIView animateWithDuration: 0.2 delay: 0.1 options: UIViewAnimationOptionCurveEaseInOut animations: ^{
        self.m_pGreen.transform = CGAffineTransformMakeScale(1.2,1.2);
        if(self.m_pActiveBt){
            self.m_pActiveBt.transform = CGAffineTransformMakeScale(1.0,1.0);
        }
    } completion: ^(BOOL finished) {
        [UIView animateWithDuration: 0.2 animations: ^{
            self.m_pActiveBt = self.m_pGreen;
        }];
    }];
}

- (void)eventBlue:(UIButton *)btn{
    NSLog(@"eventBlue\n");
    if(m_pActiveBezier){
        [m_pActiveBezier unactive];
    }
    [self.pBezierB active];
    m_pActiveBezier = self.pBezierB;
    [self bringSubviewToFront:m_pActiveBezier];
    //动画
    [UIView animateWithDuration: 0.2 delay: 0.1 options: UIViewAnimationOptionCurveEaseInOut animations: ^{
        self.m_pBlue.transform = CGAffineTransformMakeScale(1.2,1.2);
        if(self.m_pActiveBt){
            self.m_pActiveBt.transform = CGAffineTransformMakeScale(1.0,1.0);
        }
    } completion: ^(BOOL finished) {
        [UIView animateWithDuration: 0.2 animations: ^{
            self.m_pActiveBt = self.m_pBlue;
        }];
    }];
}

//融合数据
-(void)combineData {
    float* t_w_data  = [self.pBezierW getBezierData];
    float* t_r_data  = [self.pBezierR getBezierData];
    float* t_g_data  = [self.pBezierG getBezierData];
    float* t_b_data  = [self.pBezierB getBezierData];
    for (unsigned int currentCurveIndex = 0; currentCurveIndex < 256; currentCurveIndex++){
        // BGRA for upload to texture
        unsigned char r = fmin(fmax(currentCurveIndex + t_r_data[currentCurveIndex], 0), 255);
        pToneCurveByteArray[currentCurveIndex * 4 ] = fmin(fmax(r +t_w_data[r], 0), 255);
        unsigned char g = fmin(fmax(currentCurveIndex + t_g_data[currentCurveIndex], 0), 255);
        pToneCurveByteArray[currentCurveIndex * 4 + 1] = fmin(fmax(g +t_w_data[g], 0), 255);
        unsigned char b = fmin(fmax(currentCurveIndex +t_b_data[currentCurveIndex], 0), 255);
        pToneCurveByteArray[currentCurveIndex * 4 + 2] = fmin(fmax(b + t_w_data[b], 0), 255);
        pToneCurveByteArray[currentCurveIndex * 4 + 3] = 255;
    }
    [SWLogicSys getInst].pSWState.svRGBA=[NSString stringWithUTF8String:(char*)pToneCurveByteArray];
//    [[SWLogicSys getInst].pSVI.pEffect updateFilterBSplineOP:nil msg:nil dataTex:pToneCurveByteArray];
    //将数据送给SV引擎
}


@end
