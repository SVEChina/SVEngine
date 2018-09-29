//
//  SWSetupUI.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWSetupUI.h"
#import "../utility/SWFileUtil.h"
#import "../utility/Utility.h"
#import "../swlogic/SWLogicSys.h"
#import "SWSetUpHeaderCell.h"
#import "SWSetUpCell2.h"
#import "SWSetUpCell3.h"

static const NSString *fmsetupcell = @"fmsetupcell";
static const NSString *fmsetupheadercell = @"fmsetupheadercell";
static const NSString *fmsetupcell2 = @"fmsetupcell2";
static const NSString *fmsetupcell3 = @"fmsetupcell3";
@interface SWSetupUI()<UITableViewDelegate, UITableViewDataSource>{
    UITableView *m_pTableView;
    UIButton    *pBtnClose;
}
@end

@implementation SWSetupUI

+ (SWSetupUI *)createWithFrame:(CGRect)frame{
    return [[SWSetupUI alloc] initWithFrame:frame];
}

- (instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        //关闭
        self.backgroundColor = [UIColor whiteColor];
        [[UINavigationBar appearance] setTintColor:[UIColor blackColor]];
        int t_w = frame.size.width;
        m_pTableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height) style:UITableViewStyleGrouped];
        m_pTableView.delegate = self;
        m_pTableView.dataSource = self;
        [m_pTableView reloadData];
        [self addSubview:m_pTableView];
    }
    return self;
}

- (void)changeToShow{
    UIViewController *targetVC = [Utility findViewController:self];
    targetVC.automaticallyAdjustsScrollViewInsets = false;
    UIButton *t_pBtnClose = [[UIButton alloc]initWithFrame:CGRectMake(0, 0, 40, 40)];
    [t_pBtnClose setImageEdgeInsets:UIEdgeInsetsMake(6, 0, 6, 12)];
    [t_pBtnClose setImage:[UIImage imageNamed:@"common_close"] forState:UIControlStateNormal];
    [t_pBtnClose setImage:[UIImage imageNamed:@"common_close_press"] forState:UIControlStateHighlighted];
    [t_pBtnClose addTarget:self action:@selector(eventCloseVideo:) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *leftItem = [[UIBarButtonItem alloc] initWithCustomView:t_pBtnClose];
    targetVC.navigationItem.leftBarButtonItem = leftItem;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView{
    return 3;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    if (section == 0) {
        return 1;
    }else if (section == 1){
        return 1;
    }else if (section == 2){
        return 4;
    }
    return 0;
}
- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section {
    UIView *headerView = [[UIView alloc] init];
    headerView.backgroundColor = [UIColor clearColor];
    return headerView;
}
//头视图高度
- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section {
    if (section == 0) {
        return 0;
    }
    return 5;
}
- (UIView *)tableView:(UITableView *)tableView viewForFooterInSection:(NSInteger)section {
    UIView *footerView = [[UIView alloc] init];
    footerView.backgroundColor = [UIColor clearColor];
    return footerView;
}  
//脚视图高度
- (CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section {
    return 5;
}


- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    if (indexPath.section == 0 && indexPath.row == 0) {
        return 150;
    }
    return 45;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    if (indexPath.section == 0) {
        SWSetUpHeaderCell *cell = [tableView dequeueReusableCellWithIdentifier:fmsetupheadercell];
        if (cell == nil) {
            cell = [[[NSBundle mainBundle] loadNibNamed:@"SWSetUpHeaderCell" owner:self options:nil] objectAtIndex:0];
        }
        NSString *avatarPath = [SWFileUtil pathForLibraryDirectoryWithPath:@"userinfo"];
        avatarPath = [avatarPath stringByAppendingPathComponent:[SWLogicSys getInst].pLogicUser.m_head];
        UIImage *avatarImage = [UIImage imageWithContentsOfFile:avatarPath];
        if (avatarImage) {
            cell.headImageView.image = avatarImage;
        }else{
            [cell.headImageView setImage:[UIImage imageNamed:@"user_head_nor"]];
        }
        cell.headImageView.layer.cornerRadius = 65;
        cell.headImageView.clipsToBounds = YES;
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
        return cell;
    }else if (indexPath.section == 1){
        SWSetUpCell2 *cell = [tableView dequeueReusableCellWithIdentifier:fmsetupcell2];
        if (cell == nil) {
            cell = [[[NSBundle mainBundle] loadNibNamed:@"SWSetUpCell2" owner:self options:nil] objectAtIndex:0];
        }
        [cell.chooseSwitch removeTarget:self action:@selector(enableMarkLogo:) forControlEvents:UIControlEventValueChanged];
        [cell.chooseSwitch removeTarget:self action:@selector(enableBgMusic:) forControlEvents:UIControlEventValueChanged];
        if (indexPath.row == 0) {
            cell.textLabel.text = @"开启背景音乐";
            [cell.chooseSwitch addTarget:self action:@selector(enableBgMusic:) forControlEvents:UIControlEventValueChanged];
            cell.chooseSwitch.on = [[[NSUserDefaults standardUserDefaults] objectForKey:@"bgmusic"] boolValue];
        }else if (indexPath.row == 1){
            cell.textLabel.text = @"开启水印";
            [cell.chooseSwitch addTarget:self action:@selector(enableMarkLogo:) forControlEvents:UIControlEventValueChanged];
            bool isOn = [[[NSUserDefaults standardUserDefaults] objectForKey:@"marklogo"] boolValue];
            cell.chooseSwitch.on = isOn;
        }
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
        return cell;
    }else if (indexPath.section == 2){
        SWSetUpCell3 *cell = [tableView dequeueReusableCellWithIdentifier:fmsetupcell3];
        if (cell == nil) {
            cell = [[[NSBundle mainBundle] loadNibNamed:@"SWSetUpCell3" owner:self options:nil] objectAtIndex:0];
        }
        if (indexPath.row == 0) {
            cell.textLabel.text = @"意见反馈";
            cell.subTextLabel.hidden = YES;
            cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        }else if (indexPath.row == 1){
            cell.textLabel.text = @"服务条款";
            cell.subTextLabel.hidden = YES;
            cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        }else if (indexPath.row == 2){
            cell.textLabel.text = @"清除缓存";
            cell.subTextLabel.hidden = NO;
            NSString *t_path = [SWFileUtil pathForTemporaryDirectory];
            NSNumber *t_size = [SWFileUtil sizeOfDirectoryAtPath:t_path];
            double t_d_size = [t_size doubleValue];
            cell.subTextLabel.text = [NSString stringWithFormat:@"%0.1lf",t_d_size/1024.0/1024.0];
        }else if (indexPath.row == 3){
            NSString *verson = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
            cell.textLabel.text = @"版本号";
            cell.subTextLabel.text = verson;
            cell.subTextLabel.hidden = NO;
            cell.accessoryType = UITableViewCellAccessoryNone;
        }
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
        return cell;
    }
    
    return nil;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
}

- (void)eventCloseVideo:(UIButton *)btn{
    NSLog(@"share ui eventCloseVideo\n");
    UIViewController *targetVC = [Utility findViewController:self];
    [targetVC dismissViewControllerAnimated:YES completion:^{
        
    }];
}

- (void)enableBgMusic:(UISwitch *)chooseSwitch{
}

- (void)enableMarkLogo:(UISwitch *)chooseSwitch{
}


@end
