//
//  SWFilterRGBW.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWBaseUI.h"
#import "SWUIBezier.h"

//曲线 RGBW

@interface SWFilterRGBW : SWBaseUI

@property (strong, nonatomic) SWUIBezier* pBezierR;
@property (strong, nonatomic) SWUIBezier* pBezierG;
@property (strong, nonatomic) SWUIBezier* pBezierB;
@property (strong, nonatomic) SWUIBezier* pBezierW;
@property (strong, nonatomic) UIButton* m_pWhite;
@property (strong, nonatomic) UIButton* m_pRed;
@property (strong, nonatomic) UIButton* m_pGreen;
@property (strong, nonatomic) UIButton* m_pBlue;
@property (strong, nonatomic) UIButton* m_pActiveBt;

-(void)combineData;

@end
