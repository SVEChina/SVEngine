//
//  SWShareUI.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "SWBaseUI.h"
//分享UI
enum SHARETYPE{
    SHARENONE = 0,
    SHAREVIDEO = 1,
    SHAREIMAGE
};
@interface SWShareUI : SWBaseUI

+ (SWShareUI *)createWithFrame:(CGRect)frame;

- (void)refreshData;

- (void)changeToShow;

- (void)setShareVideo:(NSString*)_vio Img:(NSString*)_img Score:(int)_score;

- (void)setShareImage:(NSString*)_local;

@property (copy, nonatomic) NSString* m_whereFrom;
@end
