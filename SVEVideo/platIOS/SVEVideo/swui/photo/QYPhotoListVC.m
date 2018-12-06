//
//  QYPhotoListVC.m
//  QYPhotoAlbum
//
//  Created by liuming on 2017/11/1.
//  Copyright © 2017年 burning. All rights reserved.
//

#import "QYPhotoListVC.h"
#import "PLSystemAlbumCell.h"
#import "QYImagePreVC.h"
#import "SDAutoLayout.h"
#import "QYImagePreVC.h"
#define leftAndRightMargin 0.0f

@interface QYPhotoListVC ()<UICollectionViewDelegate,UICollectionViewDataSource,UIViewControllerPreviewingDelegate>

@property(nonatomic, strong) UICollectionView *collectionView;

@end

@implementation QYPhotoListVC

- (void)viewDidLoad{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.view.backgroundColor = [UIColor whiteColor];
    UICollectionViewFlowLayout *layout = [[UICollectionViewFlowLayout alloc] init];
    layout.scrollDirection = UICollectionViewScrollDirectionVertical;
    _collectionView = [[UICollectionView alloc] initWithFrame:self.view.bounds collectionViewLayout:layout];
    [_collectionView registerClass:[PLSystemAlbumCell class] forCellWithReuseIdentifier:@"PLSystemAlbumCell"];
    _collectionView.showsVerticalScrollIndicator = NO;
    _collectionView.dataSource = self;
    _collectionView.delegate = self;
    _collectionView.backgroundColor = [UIColor whiteColor];
    [self.view addSubview:_collectionView];
    _collectionView.sd_layout.leftSpaceToView(self.view, 0)
        .topSpaceToView(self.view, 0)
        .rightSpaceToView(self.view, 0)
        .bottomSpaceToView(self.view, 0);
}

- (CGSize)collectionView:(UICollectionView *)collectionView
                  layout:(UICollectionViewLayout *)collectionViewLayout
  sizeForItemAtIndexPath:(NSIndexPath *)indexPath
{
    float AlbumWidth = ((CGRectGetWidth(self.view.bounds) - leftAndRightMargin * 2. - 5.f * 2.) / 3.);
    return CGSizeMake(AlbumWidth, AlbumWidth);
}
- (CGFloat)collectionView:(UICollectionView *)collectionView
                             layout:(UICollectionViewLayout *)collectionViewLayout
    maxLineSpacingForSectionAtIndex:(NSInteger)section
{
    return 5.0f;
}

- (CGFloat)collectionView:(UICollectionView *)collectionView
                                 layout:(UICollectionViewLayout *)collectionViewLayout
    minimumLineSpacingForSectionAtIndex:(NSInteger)section
{
    return 5.0f;
}

- (CGFloat)collectionView:(UICollectionView *)collectionView
                                  layout:(UICollectionViewLayout *)collectionViewLayout
    maxInteritemSpacingForSectionAtIndex:(NSInteger)section
{
    return 0;
}

- (CGFloat)collectionView:(UICollectionView *)collectionView
                                      layout:(UICollectionViewLayout *)collectionViewLayout
    minimumInteritemSpacingForSectionAtIndex:(NSInteger)section
{
    return 0;
}

- (UIEdgeInsets)collectionView:(UICollectionView *)collectionView
                        layout:(UICollectionViewLayout *)collectionViewLayout
        insetForSectionAtIndex:(NSInteger)section
{
    //最左边 和最右边
    return UIEdgeInsetsMake(0, leftAndRightMargin, 0, leftAndRightMargin);
}

- (CGSize)collectionView:(UICollectionView *)collectionView
                             layout:(UICollectionViewLayout *)collectionViewLayout
    referenceSizeForHeaderInSection:(NSInteger)section
{
    return CGSizeZero;
}

#pragma mark datasource
- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView { return 1; }
- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    return self.dataSource.count;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView
                  cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *reuserId = @"PLSystemAlbumCell";
    PLSystemAlbumCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuserId forIndexPath:indexPath];
    [self registerForPreviewingWithDelegate:self sourceView:cell];
    cell.animationStart = NO;
    QYAssetModel *model = [self.dataSource objectAtIndex:indexPath.row];
    cell.assetModel = model;
    cell.isShowImageModel = NO;

    return cell;
}
- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath
{
    QYAssetModel *assetModel = self.dataSource[indexPath.row];
    QYImagePreVC *imagePrc = [[QYImagePreVC alloc] init];
    imagePrc.assetModel = [assetModel copy];
    [self.navigationController pushViewController:imagePrc animated:YES];
}


- (UIViewController *)previewingContext:(id <UIViewControllerPreviewing>)previewingContext viewControllerForLocation:(CGPoint)location{
    //通过[previewingContext sourceView]拿到对应的cell的数据；
    NSIndexPath *indexPath = [_collectionView indexPathForCell:(UICollectionViewCell*)[previewingContext sourceView]];
    QYImagePreVC *preVC = [[QYImagePreVC alloc] init];
    preVC.assetModel = self.dataSource[indexPath.row];
    return preVC;
}
#pragma mark -  pop的代理方法，在此处可对将要进入的vc进行处理
- (void)previewingContext:(id <UIViewControllerPreviewing>)previewingContext commitViewController:(UIViewController *)viewControllerToCommit
{
}
- (void)dealloc { NSLog(@"list vc dealloc"); }
@end
