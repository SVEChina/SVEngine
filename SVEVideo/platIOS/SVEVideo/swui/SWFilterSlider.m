//
//  SWFilterSlider.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWFilterSlider.h"
#import "../swlogic/SWLogicSys.h"
#import "../utility/Utility.h"
#import "../utility/UIImage+Helper.h"
#import "SWUISys.h"
#import "SWMainUI.h"

@interface SWFilterSlider(){
    UISlider *m_pSlider;
    UIButton* m_pLeft;
    UIButton* m_pRight;
    UILabel * m_pLabel;
    int m_sliderValue;
}
@end

@implementation SWFilterSlider

- (instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = [UIColor clearColor];
        int t_w = frame.size.width;
        int t_h = frame.size.height;
        int t_view_px,t_view_py,t_view_w,t_view_h;
        //
        t_view_px = TRANS_BY_WIDTH(330,t_w);
        t_view_py = TRANS_BY_WIDTH(5,t_w);//1242 - 1230
        t_view_w = TRANS_BY_WIDTH(120,t_w);
        t_view_h = TRANS_BY_WIDTH(25,t_w);
        m_pLabel = [[UILabel alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        m_pLabel.backgroundColor = [UIColor clearColor];
        m_pLabel.textAlignment = NSTextAlignmentCenter;
        m_pLabel.font = [UIFont systemFontOfSize:12];
        [self addSubview:m_pLabel];
        m_pLabel.hidden = YES;
        //
        t_view_px = TRANS_BY_WIDTH(23,t_w);
        t_view_py = TRANS_BY_WIDTH(22,t_w);//1242 - 1230
        t_view_w = TRANS_BY_WIDTH(50,t_w);
        t_view_h = TRANS_BY_WIDTH(50,t_w);
        m_pLeft = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [m_pLeft setBackgroundColor:[UIColor clearColor]];
        [m_pLeft setImage:[UIImage imageNamed:@"m_left"] forState:UIControlStateNormal];
        [m_pLeft setImage:[UIImage imageNamed:@"m_left"] forState:UIControlStateSelected];
        [m_pLeft addTarget:self action:@selector(eventLeft:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:m_pLeft];
        //
        t_view_px = TRANS_BY_WIDTH(675,t_w);
        t_view_py = TRANS_BY_WIDTH(22,t_w);//1242 - 1230
        t_view_w = TRANS_BY_WIDTH(50,t_w);
        t_view_h = TRANS_BY_WIDTH(50,t_w);
        m_pRight = [[UIButton alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [m_pRight setBackgroundColor:[UIColor clearColor]];
        [m_pRight setImage:[UIImage imageNamed:@"m_right"] forState:UIControlStateNormal];
        [m_pRight setImage:[UIImage imageNamed:@"m_right"] forState:UIControlStateSelected];
        [m_pRight addTarget:self action:@selector(eventRight:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:m_pRight];

        //
        t_view_px = TRANS_BY_WIDTH(116,t_w);
        t_view_py = TRANS_BY_WIDTH(21,t_w);//1242 - 1239
        t_view_w = TRANS_BY_WIDTH(517,t_w);
        t_view_h = TRANS_BY_WIDTH(54,t_w);
        m_pSlider = [[UISlider alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        m_pSlider.minimumValue = 0.0;
        m_pSlider.maximumValue = 1.0;
        m_pSlider.value = 0.5;
        m_pSlider.continuous = YES;
        m_pSlider.minimumTrackTintColor = [UIColor whiteColor];
        m_pSlider.maximumTrackTintColor = [UIColor lightGrayColor];
        UIImage *sliderThumbImage = [Utility buttonImageFromColor:[UIColor whiteColor] size:CGSizeMake(20, 20)];
        sliderThumbImage = [sliderThumbImage roundedCornerImageWithCornerRadius:sliderThumbImage.size.width*0.5];
        [m_pSlider setThumbImage:sliderThumbImage forState:UIControlStateNormal];
        [m_pSlider addTarget:self action:@selector(eventSlider:) forControlEvents:UIControlEventValueChanged];
        [m_pSlider addTarget:self action:@selector(eventSliderOver:) forControlEvents:UIControlEventTouchCancel];
        [m_pSlider addTarget:self action:@selector(eventSliderOver:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:m_pSlider];
    }
    return self;
}

- (float)getSliderValue {
    if( self.Mode == 0) {
        m_sliderValue = m_pSlider.value*100;
    }else if( self.Mode == 1) {
        m_sliderValue = 200*m_pSlider.value - 100;
    }
    return m_sliderValue;
}

- (void)setSliderMode:(int)Mode {
    if( self.Mode != Mode ) {
        self.Mode = Mode;
        if( self.Mode == 0) {
            //value (0 , 100)
        }else if( self.Mode == 1) {
            //value (-100 , 100)
        }
    }
}

- (void)eventSlider:(UISlider *)slider{
    NSLog(@"eventSlider\n");
    if( self.Mode == 0) {
        m_sliderValue = m_pSlider.value*100;
    }else if( self.Mode == 1) {
        m_sliderValue = 200*m_pSlider.value - 100;
    }
    [[SWUISys getInst].pMainVC.pMainView setSliderValue:m_sliderValue];
    m_pLabel.hidden = NO;
    m_pLabel.text = [NSString stringWithFormat:@"%d",m_sliderValue];
}

- (void)eventSliderOver:(UISlider *)slider{
    m_pLabel.hidden = YES;
}

- (void)setSliderValue:(float)value{
    if( self.Mode == 0) {
        m_pSlider.value = value/100.0;
    }else if( self.Mode == 1) {
        m_pSlider.value = (value + 100.0)/200.0;
    }
   
}

-  (float)transEyeBigValue:(float)value{
    float t_newValue = 0.05 + 0.15*value;
    return t_newValue;
}

- (void)eventLeft:(UIButton *)btn{
    NSLog(@"eventLeft\n");
}

- (void)eventRight:(UIButton *)btn{
    NSLog(@"eventRight\n");
}

-  (float)transFaceBeautyValue:(float)value{
    float t_newValue = 0.5+value;
    return t_newValue;
}


/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

@end
