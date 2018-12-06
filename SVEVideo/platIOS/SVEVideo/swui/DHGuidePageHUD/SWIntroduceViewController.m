//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWIntroduceViewController.h"
#import "DHGuidePageHUD.h"
#define SHOWINTRODUCE @"showintroduce"
@interface SWIntroduceViewController ()<DHGuidePageHUDDelegate>

@end

@implementation SWIntroduceViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

+ (BOOL)hasShowedIntroduce{
    return [[NSUserDefaults standardUserDefaults] boolForKey:SHOWINTRODUCE];
}

- (void)setIntroduceType:(enum INTRODUCETYPE)type{
    if (type == INTRODUCE_TYPE_IMAGE) {
        [self setStaticGuidePage];
    }else if (type == INTRODUCE_TYPE_GIF){
        [self setDynamicGuidePage];
    }else if (type == INTRODUCE_TYPE_VIDEO){
        [self setVideoGuidePage];
    }
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:SHOWINTRODUCE];
}

#pragma mark - 设置APP静态图片引导页
- (void)setStaticGuidePage {
    NSArray *imageNameArray = @[@"guideImage1.jpg",@"guideImage2.jpg",@"guideImage3.jpg",@"guideImage4.jpg"];
    DHGuidePageHUD *guidePage = [[DHGuidePageHUD alloc] dh_initWithFrame:self.view.frame imageNameArray:imageNameArray buttonIsHidden:NO];
    guidePage.delegate = self;
    guidePage.slideInto = YES;
    [self.view addSubview:guidePage];
}

#pragma mark - 设置APP动态图片引导页
- (void)setDynamicGuidePage {
    NSArray *imageNameArray = @[@"guideImage6.gif",@"guideImage7.gif",@"guideImage8.gif"];
    DHGuidePageHUD *guidePage = [[DHGuidePageHUD alloc] dh_initWithFrame:self.view.frame imageNameArray:imageNameArray buttonIsHidden:YES];
    guidePage.delegate = self;
    guidePage.slideInto = YES;
    [self.view addSubview:guidePage];
}

#pragma mark - 设置APP视频引导页
- (void)setVideoGuidePage {
    NSURL *videoURL = [NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:@"guideMovie1" ofType:@"mov"]];
    DHGuidePageHUD *guidePage = [[DHGuidePageHUD alloc] dh_initWithFrame:self.view.bounds videoURL:videoURL];
    guidePage.delegate = self;
    [self.view addSubview:guidePage];
}

#pragma mark --- DHGuidePageHUDDelegate
- (void)skip{
    if ([self.delegate respondsToSelector:@selector(introduceSkip)]) {
        [self.delegate introduceSkip];
    }
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
