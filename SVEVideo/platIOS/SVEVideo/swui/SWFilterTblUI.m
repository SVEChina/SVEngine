//
//  SWFilterTblUI.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWFilterTblUI.h"
#import "SWUISys.h"
#import "../swlogic/SWLogicSys.h"

//自定义cell(本身)
@interface FilterCVCell : UICollectionViewCell

@property (strong, nonatomic) UIImageView *filterImg;       //图片
@property (strong, nonatomic) UIImageView *filterFrame;     //框
@property (strong, nonatomic) UIView *filterNameBG;         //名称北京
@property (strong, nonatomic) UILabel *filterName;          //名称

@end

@implementation FilterCVCell

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        float t_w = [UIScreen mainScreen].bounds.size.width;
        float t_h = [UIScreen mainScreen].bounds.size.height;
        float t_view_px,t_view_py,t_view_w,t_view_h;
        t_view_px = 0;
        t_view_py = 0;
        t_view_w = frame.size.width;
        t_view_h = frame.size.height;
        //图片
        self.filterImg = [[UIImageView alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [self.filterImg setImage: [UIImage imageNamed:@"m_mopi"] ];
        [self addSubview:self.filterImg ];
        //图片框
        self.filterFrame = [[UIImageView alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [self.filterFrame setImage: [UIImage imageNamed:@"m_mopi"] ];
        [self addSubview:self.filterFrame];
        //
        t_view_px = TRANS_BY_WIDTH(0,t_w);  // 15 - 15
        t_view_py = TRANS_BY_WIDTH(78,t_w);   // 281 - 203
        t_view_w = TRANS_BY_WIDTH(126,t_w);
        t_view_h = TRANS_BY_WIDTH(42,t_w);
        self.filterNameBG = [[UIView alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        //[self.filterNameBG setBackgroundColor:[UIColor redColor]];
        //[self.filterNameBG setAlpha:0.4];
        [self addSubview:self.filterNameBG];
        //名称
        self.filterName = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, t_view_w, t_view_h)];
        [self.filterName setFont:[UIFont systemFontOfSize:10.0]];
        [self.filterName setText:@"暗色调03"];
        [self.filterName setTextAlignment:NSTextAlignmentCenter];
        [self.filterName setTextColor:[UIColor whiteColor]];
        [self.filterNameBG addSubview:self.filterName];
    }
    return self;
}

- (void)setSelected:(BOOL)selected{
//    [super setSelected:selected];
//    if (selected) {
//        self.gameCoverImage.hidden = YES;
//    }else{
//        self.gameCoverImage.hidden = NO;
//    }
}

@end

//滤镜
static NSString *const filter_cellId = @"filter_cellId";

@interface SWFilterTblUI () <UICollectionViewDataSource,UICollectionViewDelegate> {
    UIImageView* pImgBg;
    UICollectionView* pCollectView;
    int cellWidth;
    int cellHeight;
    int cellCount;
    NSIndexPath *currentIndexPath;
}
@end

@implementation SWFilterTblUI

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        int t_w = frame.size.width;
        int t_h = frame.size.height;
        int t_view_px,t_view_py,t_view_w,t_view_h;
        //背景
        t_view_px = TRANS_BY_WIDTH(0,t_w);
        t_view_py = TRANS_BY_WIDTH(0,t_w);
        t_view_w = TRANS_BY_WIDTH(t_w,t_w);
        t_view_h = TRANS_BY_WIDTH(t_h,t_w);
        //colectview
        cellCount = 10;
        //
        cellWidth = TRANS_BY_WIDTH(126,t_w);
        cellHeight = TRANS_BY_WIDTH(125,t_w);
        //
        UICollectionViewFlowLayout *flowLayout = [[UICollectionViewFlowLayout alloc] init];
        flowLayout.scrollDirection = UICollectionViewScrollDirectionHorizontal;
//#warning 设定全局行间距
//        flowLayout.minimumLineSpacing = 20;
//        flowLayout.headerReferenceSize = CGSizeMake(0, 20);
//        flowLayout.sectionInset = UIEdgeInsetsMake(0, 10, 0, 10);
#warning 下面一句是必须要写的
        //
        CGRect t_cvFrame;
        t_cvFrame.origin.x = 0;
        t_cvFrame.origin.y = 0;
        t_cvFrame.size.width = frame.size.width;
        t_cvFrame.size.height = frame.size.height;
        //
        pCollectView = [[UICollectionView alloc] initWithFrame:t_cvFrame collectionViewLayout:flowLayout];
        pCollectView.delegate = self;
        pCollectView.dataSource = self;
        [pCollectView setBackgroundColor:[UIColor blackColor]];
        [pCollectView registerClass:[FilterCVCell class] forCellWithReuseIdentifier:filter_cellId];
        //
        [self addSubview:pCollectView];
        [self refreshData];
    }
    return self;
}

//
- (void)refreshData{
//    cellCount = [[SWLogicSys getInst].pLogicGames getGameNum];
    [pCollectView reloadData];
}

- (void)changeToShow{
}

#pragma mark ---- UICollectionViewDataSource

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView{
    return 1;
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section{
    return cellCount;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath{
    //每一个cell属性
    UICollectionViewCell *cell = [pCollectView dequeueReusableCellWithReuseIdentifier:filter_cellId forIndexPath:indexPath];
    FilterCVCell* t_filter_cell = (FilterCVCell*)cell;
    if(t_filter_cell){
    }
    return cell;
}

//UICollectionView也可设置段头段尾
- (UICollectionReusableView *)collectionView:(UICollectionView *)collectionView viewForSupplementaryElementOfKind:(NSString *)kind atIndexPath:(NSIndexPath *)indexPath{
    return nil;
}

- (BOOL)collectionView:(UICollectionView *)collectionView canMoveItemAtIndexPath:(NSIndexPath *)indexPath{
    return YES;
}


- (void)collectionView:(UICollectionView *)collectionView moveItemAtIndexPath:(NSIndexPath *)sourceIndexPath toIndexPath:(NSIndexPath*)destinationIndexPath{
}

#pragma mark ---- UICollectionViewDelegateFlowLayout

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath{
    return (CGSize){cellWidth,cellHeight};
}


- (UIEdgeInsets)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout insetForSectionAtIndex:(NSInteger)section{
    return UIEdgeInsetsMake(2, 2, 2, 2);
}

- (CGFloat)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout minimumLineSpacingForSectionAtIndex:(NSInteger)section{
    return 2.0f;
}

- (CGFloat)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout minimumInteritemSpacingForSectionAtIndex:(NSInteger)section{
    return 2.f;
}

//- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout referenceSizeForHeaderInSection:(NSInteger)section{
//    return (CGSize){headWidth,headHeight};
//}

//- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout referenceSizeForFooterInSection:(NSInteger)section{
//    return (CGSize){tailWidth,tailHeight};
//}

#pragma mark ---- UICollectionViewDelegate

- (BOOL)collectionView:(UICollectionView *)collectionView shouldHighlightItemAtIndexPath:(NSIndexPath *)indexPath{
    return YES;
}

// 选中某item
- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath{
}

- (void)collectionView:(UICollectionView *)collectionView didDeselectItemAtIndexPath:(NSIndexPath *)indexPath{
}

// 长按某item，弹出copy和paste的菜单
- (BOOL)collectionView:(UICollectionView *)collectionView shouldShowMenuForItemAtIndexPath:(NSIndexPath *)indexPath{
    return YES;
}

// 使copy和paste有效
- (BOOL)collectionView:(UICollectionView *)collectionView canPerformAction:(SEL)action forItemAtIndexPath:(NSIndexPath *)indexPath withSender:(nullable id)sender{
//    if ([NSStringFromSelector(action) isEqualToString:@"copy:"] || [NSStringFromSelector(action) isEqualToString:@"paste:"]){
//        return YES;
//    }
    return NO;
}

//
- (void)collectionView:(UICollectionView *)collectionView performAction:(SEL)action forItemAtIndexPath:(NSIndexPath *)indexPath withSender:(nullable id)sender{
    if([NSStringFromSelector(action) isEqualToString:@"copy:"]){
        //        NSLog(@"-------------执行拷贝-------------");
        [pCollectView performBatchUpdates:^{
            //[_section0Array removeObjectAtIndex:indexPath.row];
            [pCollectView deleteItemsAtIndexPaths:@[indexPath]];
        } completion:nil];
    }
    else if([NSStringFromSelector(action) isEqualToString:@"paste:"]){
        NSLog(@"-------------执行粘贴-------------");
    }
}


@end
