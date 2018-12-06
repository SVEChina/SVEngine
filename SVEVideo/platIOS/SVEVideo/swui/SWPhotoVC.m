//
//  SWPhotoVC.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWPhotoVC.h"
#import "SWUISys.h"
#import "../swlogic/SWLogicSys.h"
#import "../swbasic/SWBasicSys.h"
#import "../utility/Utility.h"

@interface SWPhotoVC()<UITableViewDelegate,UITableViewDataSource>{
    
}
@end

@implementation SWPhotoVC

- (void)dealloc{
    NSLog(@"destory SWMagicsUI");
}

- (NSMutableArray *)dataSource {
    if (_dataSource == nil) {
        _dataSource = [[NSMutableArray alloc] init];
    }
    return _dataSource;
}

- (void)viewDidLoad{
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    self.title = @"首页";
    self.listTable = [[UITableView alloc] initWithFrame:self.view.bounds style:UITableViewStylePlain];
    self.listTable.backgroundColor = [UIColor whiteColor];
    self.listTable.delegate = self;
    self.listTable.dataSource = self;
    self.listTable.rowHeight = 76;
    self.listTable.separatorStyle = UITableViewCellSeparatorStyleNone;
    self.listTable.showsVerticalScrollIndicator = NO;
    self.listTable.showsHorizontalScrollIndicator = NO;
    self.listTable.tableFooterView = [[UIView alloc] init];
    [self.view addSubview:self.listTable];
    
    self.indicatorView =
    [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleGray];
    self.indicatorView.frame = CGRectMake((CGRectGetWidth(self.view.bounds) - 100) / 2.0,
                                          (CGRectGetHeight(self.view.bounds) - 100) / 2.0, 100, 100);
    [self.view addSubview:self.indicatorView];
    [self.indicatorView startAnimating];
    [QYPhotoService requestAuthor:^(BOOL hasAuthor) {
        if (hasAuthor){
            [self loadAlblums];
        }
    }];
}

- (void)loadAlblums{
    [[QYPhotoService shareInstanced] fetchAllGroupsWithcompletion:^(NSArray *_Nullable array) {
        if (array){
            [self.dataSource addObjectsFromArray:array];
            [self.listTable reloadData];
        }
        [self.indicatorView stopAnimating];
        [self.indicatorView removeFromSuperview];
        self.indicatorView = nil;
    }];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.dataSource.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *identifer = @"AlbumContentsCellId";
    PLAlbumContensCell *cell = [tableView dequeueReusableCellWithIdentifier:identifer];
    if (!cell){
        cell = [[PLAlbumContensCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identifer];
    }
    cell.indexPath = indexPath;
    QYGroupModel *model = [self.dataSource objectAtIndex:indexPath.row];
    [cell setContentModel:model];
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    QYGroupModel *groupModel = self.dataSource[indexPath.row];
    QYPhotoListVC *listVC = [[QYPhotoListVC alloc] init];
    listVC.title = groupModel.collection.localizedTitle;
    listVC.dataSource = [groupModel.asstArray mutableCopy];
    [self.navigationController pushViewController:listVC animated:YES];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

- (void)changeToShow{
}

#pragma mark -- fmrecord delegate

@end
