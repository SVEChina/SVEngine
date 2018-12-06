//
//  SWPhotoVC.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SWBaseUI.h"
#import "photo/QYPhotoService.h"
#import "photo/PLAlbumContensCell.h"
#import "photo/Models/QYGroupModel.h"
#import "photo/QYPhotoListVC.h"


@interface SWPhotoVC : UIViewController

@property(nonatomic, strong) QYPhotoService *svervice;
@property(nonatomic, strong) NSMutableArray *dataSource;
@property(nonatomic, strong) UITableView *listTable;
@property(nonatomic, strong) UIActivityIndicatorView *indicatorView;

- (void)changeToShow;

- (void)eventEffects:(UIButton *)btn;

@end
