//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <UIKit/UIKit.h>
enum INTRODUCETYPE{
  INTRODUCE_TYPE_IMAGE = 1,
  INTRODUCE_TYPE_GIF,
  INTRODUCE_TYPE_VIDEO
};
@protocol SWIntroduceViewControllerDelegate <NSObject>
@required
- (void)introduceSkip;
@end

@interface SWIntroduceViewController : UIViewController
@property (weak, nonatomic)id<SWIntroduceViewControllerDelegate>delegate;
+ (BOOL)hasShowedIntroduce;
- (void)setIntroduceType:(enum INTRODUCETYPE)type;
@end
