//
//  SWFilterColorBalanceSlider.m
//  SVEVideo
//
//  Created by xiaofan on 2018/6/11.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import "SWFilterColorBalanceSlider.h"
#import "../swlogic/SWLogicSys.h"
#import "../utility/Utility.h"
#import "../utility/UIImage+Helper.h"
#import "SWUISys.h"
#import "SWMainUI.h"
#import "SWState.h"


@interface SWFilterColorBalanceSlider(){
    UIButton *m_pShadowBtn;
    UIButton *m_pMiddleToneBtn;
    UIButton *m_pHighlightBtn;
    UISlider *m_pSlider1;
    UISlider *m_pSlider2;
    UISlider *m_pSlider3;
}
@end

@implementation SWFilterColorBalanceSlider
- (instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if(self){
        self.backgroundColor = [UIColor clearColor];
        int t_w = frame.size.width;
        int t_h = frame.size.height;
        int t_view_px,t_view_py,t_view_w,t_view_h;
        //
        t_view_px = TRANS_BY_WIDTH(200,t_w);
        t_view_py = TRANS_BY_WIDTH(300,t_w);
        t_view_w = TRANS_BY_WIDTH(60,t_w);
        t_view_h = TRANS_BY_WIDTH(60,t_w);
        m_pShadowBtn = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [m_pShadowBtn setBackgroundColor:[UIColor clearColor]];
        [m_pShadowBtn setImage:[UIImage imageNamed:@"colorbalance_shadow"] forState:UIControlStateNormal];
        [m_pShadowBtn setImage:[UIImage imageNamed:@"colorbalance_shadow"] forState:UIControlStateSelected];
        [m_pShadowBtn addTarget:self action:@selector(eventBtn:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:m_pShadowBtn];
        //
        t_view_px = TRANS_BY_WIDTH(345,t_w);
        t_view_py = TRANS_BY_WIDTH(300,t_w);
        t_view_w = TRANS_BY_WIDTH(60,t_w);
        t_view_h = TRANS_BY_WIDTH(60,t_w);
        m_pMiddleToneBtn = [[UIButton alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [m_pMiddleToneBtn setBackgroundColor:[UIColor clearColor]];
        [m_pMiddleToneBtn setImage:[UIImage imageNamed:@"colorbalance_middletone"] forState:UIControlStateNormal];
        [m_pMiddleToneBtn setImage:[UIImage imageNamed:@"colorbalance_middletone"] forState:UIControlStateSelected];
        [m_pMiddleToneBtn addTarget:self action:@selector(eventBtn:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:m_pMiddleToneBtn];
        //
        t_view_px = TRANS_BY_WIDTH(506,t_w);
        t_view_py = TRANS_BY_WIDTH(300,t_w);//
        t_view_w = TRANS_BY_WIDTH(60,t_w);
        t_view_h = TRANS_BY_WIDTH(60,t_w);
        m_pHighlightBtn = [[UIButton alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [m_pHighlightBtn setBackgroundColor:[UIColor clearColor]];
        [m_pHighlightBtn setImage:[UIImage imageNamed:@"colorbalance_highlight"] forState:UIControlStateNormal];
        [m_pHighlightBtn setImage:[UIImage imageNamed:@"colorbalance_highlight"] forState:UIControlStateSelected];
        [m_pHighlightBtn addTarget:self action:@selector(eventBtn:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:m_pHighlightBtn];
        //
        [self eventBtn:m_pMiddleToneBtn];
        //
        t_view_px = TRANS_BY_WIDTH(120,t_w);
        t_view_py = TRANS_BY_WIDTH(48,t_w);
        t_view_w = TRANS_BY_WIDTH(500,t_w);
        t_view_h = TRANS_BY_WIDTH(5,t_w);
        UIColor *startColor = [UIColor colorWithRed:0.0/255.0 green:255.0/255.0 blue:255.0/255.0 alpha:1.0];
        UIColor *endColor = [UIColor colorWithRed:255.0/255.0 green:0.0/255.0 blue:0.0/255.0 alpha:1.0];
        NSArray *colors = @[startColor,endColor];
        UIImage *slider1TrackImg = [Utility getGradientImageWithColors:colors imgSize:CGSizeMake(t_view_w, t_view_h)];
        UIImageView *imageView1 = [[UIImageView alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        imageView1.image = slider1TrackImg;
        [self addSubview:imageView1];
        
        t_view_px = TRANS_BY_WIDTH(120,t_w);
        t_view_py = TRANS_BY_WIDTH(26,t_w);//
        t_view_w = TRANS_BY_WIDTH(500,t_w);
        t_view_h = TRANS_BY_WIDTH(50,t_w);
        m_pSlider1 = [[UISlider alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        m_pSlider1.minimumValue = -1.0;
        m_pSlider1.maximumValue = 1.0;
        m_pSlider1.value = 0.0;
        m_pSlider1.continuous = YES;
        [m_pSlider1 setMinimumTrackTintColor:[UIColor clearColor]];
        [m_pSlider1 setMaximumTrackTintColor:[UIColor clearColor]];
        UIImage *sliderThumbImage = [Utility buttonImageFromColor:[UIColor whiteColor] size:CGSizeMake(20, 20)];
        sliderThumbImage = [sliderThumbImage roundedCornerImageWithCornerRadius:5];
        [m_pSlider1 setThumbImage:sliderThumbImage forState:UIControlStateNormal];
        [m_pSlider1 addTarget:self action:@selector(eventSlider1:) forControlEvents:UIControlEventValueChanged];
        [self addSubview:m_pSlider1];
        
        //
        t_view_px = TRANS_BY_WIDTH(120,t_w);
        t_view_py = TRANS_BY_WIDTH(153,t_w);
        t_view_w = TRANS_BY_WIDTH(500,t_w);
        t_view_h = TRANS_BY_WIDTH(5,t_w);
        startColor = [UIColor colorWithRed:255.0/255.0 green:0.0/255.0 blue:255.0/255.0 alpha:1.0];
        endColor = [UIColor colorWithRed:0.0/255.0 green:255.0/255.0 blue:0.0/0.0 alpha:1.0];
        colors = @[startColor,endColor];
        UIImage *slider2TrackImg = [Utility getGradientImageWithColors:colors imgSize:CGSizeMake(t_view_w, t_view_h)];
        UIImageView *imageView2 = [[UIImageView alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        imageView2.image = slider2TrackImg;
        [self addSubview:imageView2];
        t_view_px = TRANS_BY_WIDTH(120,t_w);
        t_view_py = TRANS_BY_WIDTH(128,t_w);//
        t_view_w = TRANS_BY_WIDTH(500,t_w);
        t_view_h = TRANS_BY_WIDTH(50,t_w);
        m_pSlider2 = [[UISlider alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        m_pSlider2.minimumValue = -1.0;
        m_pSlider2.maximumValue = 1.0;
        m_pSlider2.value = 0.0;
        m_pSlider2.continuous = YES;
        m_pSlider2.minimumTrackTintColor = [UIColor clearColor];
        m_pSlider2.maximumTrackTintColor = [UIColor clearColor];
        [m_pSlider2 setThumbImage:sliderThumbImage forState:UIControlStateNormal];
        [m_pSlider2 addTarget:self action:@selector(eventSlider2:) forControlEvents:UIControlEventValueChanged];
        [self addSubview:m_pSlider2];
        
        //
        t_view_px = TRANS_BY_WIDTH(120,t_w);
        t_view_py = TRANS_BY_WIDTH(255,t_w);
        t_view_w = TRANS_BY_WIDTH(500,t_w);
        t_view_h = TRANS_BY_WIDTH(5,t_w);
        startColor = [UIColor colorWithRed:255.0/255.0 green:255.0/255.0 blue:0.0/255.0 alpha:1.0];
        endColor = [UIColor colorWithRed:0.0/255.0 green:0.0/255.0 blue:255.0/0.0 alpha:1.0];
        colors = @[startColor,endColor];
        UIImage *slider3TrackImg = [Utility getGradientImageWithColors:colors imgSize:CGSizeMake(t_view_w, t_view_h)];
        UIImageView *imageView3 = [[UIImageView alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        imageView3.image = slider3TrackImg;
        [self addSubview:imageView3];
        t_view_px = TRANS_BY_WIDTH(120,t_w);
        t_view_py = TRANS_BY_WIDTH(230,t_w);//
        t_view_w = TRANS_BY_WIDTH(500,t_w);
        t_view_h = TRANS_BY_WIDTH(50,t_w);
        m_pSlider3 = [[UISlider alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        m_pSlider3.minimumValue = -1.0;
        m_pSlider3.maximumValue = 1.0;
        m_pSlider3.value = 0.0;
        m_pSlider3.continuous = YES;
        m_pSlider3.minimumTrackTintColor = [UIColor clearColor];
        m_pSlider3.maximumTrackTintColor = [UIColor clearColor];
        [m_pSlider3 setThumbImage:sliderThumbImage forState:UIControlStateNormal];
        [m_pSlider3 addTarget:self action:@selector(eventSlider3:) forControlEvents:UIControlEventValueChanged];
        [self addSubview:m_pSlider3];
        
    }
    return  self;
}

- (void)eventBtn:(UIButton *)btn{
    if(m_pShadowBtn == btn){
        NSLog(@"阴影");
        if(btn.isSelected)return;
        m_pMiddleToneBtn.selected = false;
        m_pHighlightBtn.selected = false;
        m_pShadowBtn.selected = true;
        m_pShadowBtn.backgroundColor = [UIColor redColor];
        m_pMiddleToneBtn.backgroundColor = [UIColor clearColor];
        m_pHighlightBtn.backgroundColor = [UIColor clearColor];
        m_pSlider1.value=[SWLogicSys getInst].pSWState.svSDRedShift;
        m_pSlider2.value=[SWLogicSys getInst].pSWState.svSDGreenShift;
        m_pSlider3.value=[SWLogicSys getInst].pSWState.svSDBlueShift;
    }else if (m_pMiddleToneBtn == btn){
        NSLog(@"中间调");
        if(btn.isSelected)return;
        m_pShadowBtn.selected = false;
        m_pHighlightBtn.selected = false;
        m_pMiddleToneBtn.selected = true;
        m_pMiddleToneBtn.backgroundColor = [UIColor redColor];
        m_pShadowBtn.backgroundColor = [UIColor clearColor];
        m_pHighlightBtn.backgroundColor = [UIColor clearColor];
        m_pSlider1.value=[SWLogicSys getInst].pSWState.svRedShift;
        m_pSlider2.value=[SWLogicSys getInst].pSWState.svGreenShift;
        m_pSlider3.value=[SWLogicSys getInst].pSWState.svBlueShift;
    }else if (m_pHighlightBtn == btn){
        NSLog(@"高光");
        if(btn.isSelected)return;
        m_pShadowBtn.selected = false;
        m_pMiddleToneBtn.selected = false;
        m_pHighlightBtn.selected = true;
        m_pHighlightBtn.backgroundColor = [UIColor redColor];
        m_pShadowBtn.backgroundColor = [UIColor clearColor];
        m_pMiddleToneBtn.backgroundColor = [UIColor clearColor];
        m_pSlider1.value=[SWLogicSys getInst].pSWState.svHHRedShift;
        m_pSlider2.value=[SWLogicSys getInst].pSWState.svHHGreenShift;
        m_pSlider3.value=[SWLogicSys getInst].pSWState.svHHBlueShift;
    }
    
}

- (void)eventSlider1:(UISlider *)slider{
//    if(m_pShadowBtn.selected){
//          [SWLogicSys getInst].pSWState.svSDRedShift=slider.value;
//          [[SWLogicSys getInst].pSVI.pEffect updateFilterOP:nil msg:nil smooth:slider.value filtertype:SVI_SDREDSHIFT_FILTER];
//    }else if(m_pMiddleToneBtn.selected){
//          [SWLogicSys getInst].pSWState.svRedShift=slider.value;
//          [[SWLogicSys getInst].pSVI.pEffect updateFilterOP:nil msg:nil smooth:slider.value filtertype:SVI_REDSHIFT_FILTER];
//    }else if(m_pHighlightBtn.selected){
//          [SWLogicSys getInst].pSWState.svHHRedShift=slider.value;
//          [[SWLogicSys getInst].pSVI.pEffect updateFilterOP:nil msg:nil smooth:slider.value filtertype:SVI_HHREDSHIFT_FILTER];
//    }
}
    
- (void)eventSlider2:(UISlider *)slider{
//    if(m_pShadowBtn.selected){
//         [SWLogicSys getInst].pSWState.svSDGreenShift=slider.value;
//         [[SWLogicSys getInst].pSVI.pEffect updateFilterOP:nil msg:nil smooth:slider.value filtertype:SVI_SDGREENSHIFT_FILTER];
//    }else if(m_pMiddleToneBtn.selected){
//        [SWLogicSys getInst].pSWState.svGreenShift=slider.value;
//         [[SWLogicSys getInst].pSVI.pEffect updateFilterOP:nil msg:nil smooth:slider.value filtertype:SVI_GREENSHIFT_FILTER];
//    }else if(m_pHighlightBtn.selected){
//        [SWLogicSys getInst].pSWState.svHHGreenShift=slider.value;
//         [[SWLogicSys getInst].pSVI.pEffect updateFilterOP:nil msg:nil smooth:slider.value filtertype:SVI_HHGREENSHIFT_FILTER];
//    }
}
    
- (void)eventSlider3:(UISlider *)slider{
//    if(m_pShadowBtn.selected){
//        [SWLogicSys getInst].pSWState.svSDBlueShift=slider.value;
//        [[SWLogicSys getInst].pSVI.pEffect updateFilterOP:nil msg:nil smooth:slider.value filtertype:SVI_SDBLUESHIFT_FILTER];
//    }else if(m_pMiddleToneBtn.selected){
//        [SWLogicSys getInst].pSWState.svGreenShift=slider.value;
//        [[SWLogicSys getInst].pSVI.pEffect updateFilterOP:nil msg:nil smooth:slider.value filtertype:SVI_BLUESHIFT_FILTER];
//    }else if(m_pHighlightBtn.selected){
//        [SWLogicSys getInst].pSWState.svHHBlueShift=slider.value;
//        [[SWLogicSys getInst].pSVI.pEffect updateFilterOP:nil msg:nil smooth:slider.value filtertype:SVI_HHBLUESHIFT_FILTER];
//    }
}
@end
