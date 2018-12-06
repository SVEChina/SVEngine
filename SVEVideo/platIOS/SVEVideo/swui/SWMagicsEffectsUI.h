//
//  SWMagicsEffectsUI.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWBaseUI.h"
#import "../swlogic/swmagic/SWMagicCategoryUnit.h"
#import "../swlogic/swmagic/SWMagicUnit.h"
@interface SWMagicsEffectsUI : SWBaseUI
@property (assign, nonatomic)BOOL showed;
- (void)refreshAllData;
- (void)refreshSectionWithCategory:(SWMagicCategoryUnit *)categoryUnit;
- (void)refreshCellWithMagicEffect:(SWMagicUnit *)magicUnit;
- (void)showWithAni:(BOOL)ani;
- (void)hideWithAni:(BOOL)ani;
@end
