//
//  SDMainVC.m
//  SVEDemo
//
//  Created by 李晓帆 on 2018/9/3.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import "SDMainVC.h"
#import "SDSVView.h"
#import "SDMainUI.h"
#import "SDLogicSys.h"
@interface SDMainVC ()

@end

@implementation SDMainVC

- (void)viewDidLoad {
    [super viewDidLoad];
    //创建相机窗口
    self.pSVView = [[SDSVView alloc] initWithFrame: CGRectMake(0, 0, self.view.frame.size.width ,self.view.frame.size.height )];
    [self.pSVView createGLView: [[SDLogicSys getInst] getGLContext]];
    self.view.backgroundColor = [UIColor clearColor];
    [self.view addSubview:self.pSVView];
    
    self.pMainUI = [[SDMainUI alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width ,self.view.frame.size.height )];
    [self.pMainUI initSubViews];
    [self.view addSubview:self.pMainUI];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
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
