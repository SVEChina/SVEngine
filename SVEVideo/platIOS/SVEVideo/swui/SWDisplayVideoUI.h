//
//  SWDisplayVideoUI.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWBaseUI.h"

@interface SWDisplayVideoUI : SWBaseUI
+ (SWDisplayVideoUI *)createWithFrame:(CGRect)frame;
- (void)loadMovieUrl:(NSURL *)url;
@end
