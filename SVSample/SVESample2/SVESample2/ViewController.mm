//
//  ViewController.m
//  SVESample1
//
//  Created by 付一洲 on 2018/9/29.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import "ViewController.h"
#import "SDSVView.h"
#import "SDLogicSys.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    self.pSVView = [[SDSVView alloc] initWithFrame: CGRectMake(0,
                                                               0,
                                                               self.view.frame.size.width ,
                                                               self.view.frame.size.height )];
    [self.pSVView createGLView: [[SDLogicSys getInst] getGLContext]];
    self.view.backgroundColor = [UIColor clearColor];
    [self.view addSubview:self.pSVView];
}


@end
