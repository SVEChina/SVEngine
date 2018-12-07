
//
//  SWFunctionTblUI.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWFunctionTblUI.h"
#import "SWUISys.h"
#import "SWStartupUI.h"
#import "SWMainVC.h"
#import "SWMainUI.h"
#import "../swlogic/SWLogicSys.h"
#import "../swlogic/SWLogicFilter.h"
#import "../swbasic/SWBasicSys.h"
#import "../utility/SWFileUtil.h"

//自定义cell(本身)
@interface FunctionCell : UICollectionViewCell
{
    CGPoint m_originalImgCenter;
    CGPoint m_originalNameCenter;
}
@property (strong, nonatomic) UIView *pBG;
@property (strong, nonatomic) UIImageView *pImg;
@property (strong, nonatomic) UILabel *pName;

@end

@implementation FunctionCell

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
        self.pBG = [[UIView alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        self.pBG.layer.cornerRadius = 6.0f; //设置圆角
        [ self.pBG setBackgroundColor:[UIColor clearColor] ];
        [ self addSubview:self.pBG ];
        //图片
        t_view_px = TRANS_BY_WIDTH(13,t_w);  // 28 - 15
        t_view_py = TRANS_BY_WIDTH(8,t_w);   // 1324 - 1316
        t_view_w = TRANS_BY_WIDTH(66,t_w);
        t_view_h = TRANS_BY_WIDTH(66,t_w);
        self.pImg = [[UIImageView alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [self.pImg setImage: [UIImage imageNamed:@"m_mopi"] ];
        [self.pBG addSubview:self.pImg];
        m_originalImgCenter = self.pImg.center;
        //
        t_view_px = TRANS_BY_WIDTH(5,t_w);  // 28 - 15
        t_view_py = TRANS_BY_WIDTH(77,t_w);  // 1393 - 1316
        t_view_w = TRANS_BY_WIDTH(82,t_w);
        t_view_h = TRANS_BY_WIDTH(30,t_w);
        self.pName = [[UILabel alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [self.pName setFont:[UIFont systemFontOfSize:10.0]];
        [self.pName setText:@"磨皮"];
        [self.pName setTextAlignment:NSTextAlignmentCenter];
        [self.pName setTextColor:[UIColor whiteColor]];
        self.pName.backgroundColor = [UIColor clearColor];
        [self.pBG addSubview:self.pName];
        m_originalNameCenter = self.pName.center;

    }
    return self;
}

-(void)refreshCell:(SWFunction*)func {
    self.pImg.center = m_originalImgCenter;
    self.pName.center = m_originalNameCenter;
    [self.pImg setImage: [UIImage imageNamed:func.m_image] ];
    [self.pName setText:func.m_name];
}

-(void)moveOffset:(CGPoint)off {
    CGPoint t_src = self.pImg.center;
    t_src.x += off.x;
    t_src.y += off.y;
    [self.pImg setCenter:t_src];
    //
    t_src = self.pName.center;
    t_src.x += off.x;
    t_src.y += off.y;
    [self.pName setCenter:t_src];
}

@end

static NSString *const tFunctionCell = @"FunctionCell";

@interface SWFunctionTblUI () <UICollectionViewDataSource,UICollectionViewDelegate> {
    UICollectionView *pCollectView;
    FunctionCell* pActiveCell;
    int cellWidth;
    int cellHeight;
    int cellCount;
}
@end

@implementation SWFunctionTblUI

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        //
        pActiveCell = nil;
        //
        int t_w = frame.size.width;
        int t_h = frame.size.height;
        int t_view_px,t_view_py,t_view_w,t_view_h;
        //colectview
        cellCount = 13;
        cellWidth = TRANS_BY_WIDTH(91,t_w);
        cellHeight = TRANS_BY_WIDTH(126,t_w);
        //
        UICollectionViewFlowLayout *flowLayout = [[UICollectionViewFlowLayout alloc] init];
        flowLayout.scrollDirection = UICollectionViewScrollDirectionHorizontal;
#warning 设定全局行间距
        flowLayout.minimumLineSpacing = 0;
        flowLayout.headerReferenceSize = CGSizeMake(0, 0);
        flowLayout.sectionInset = UIEdgeInsetsMake(0, 0, 0, 0);
#warning 下面一句是必须要写的
        //
        CGRect t_cvFrame;
        t_cvFrame.origin.x = 0;
        t_cvFrame.origin.y = 0;
        t_cvFrame.size.width = t_w;
        t_cvFrame.size.height = t_h;
        //
        pCollectView = [[UICollectionView alloc] initWithFrame:t_cvFrame collectionViewLayout:flowLayout];
        [pCollectView setBackgroundColor:[UIColor blackColor]];
        pCollectView.delegate = self;
        pCollectView.dataSource = self;
        [pCollectView registerClass:[FunctionCell class] forCellWithReuseIdentifier:tFunctionCell];
        //
        [self addSubview:pCollectView];
    }
    return self;
}

//
- (void)refreshData{
    if( [SWLogicSys getInst].pLogicFunction ) {
        NSMutableArray* t_functions = [[SWLogicSys getInst].pLogicFunction getFunctionLibSel];
        if(t_functions) {
            cellCount = (int)t_functions.count;
        }
        //UI刷新
        [pCollectView reloadData];
    }
}

- (void)changeToShow{
    [self refreshData];
}

- (void)disappear:(int)Dir AtOnce:(bool)Flag {
    if( pActiveCell ) {
        [pActiveCell setSelected:FALSE];
        [pActiveCell.pBG setBackgroundColor:[UIColor clearColor]];
    }
    [super disappear:Dir AtOnce:Flag];
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
    UICollectionViewCell *cell = [pCollectView dequeueReusableCellWithReuseIdentifier:tFunctionCell forIndexPath:indexPath];
    FunctionCell* t_function_cell = (FunctionCell*)cell;
    if( t_function_cell ) {
        [t_function_cell setBackgroundColor: [UIColor clearColor]];
        NSInteger t_index = indexPath.item;
        NSMutableArray* t_functions = [[SWLogicSys getInst].pLogicFunction getFunctionLibSel];
        if(t_functions) {
            SWFunction *t_function = [t_functions objectAtIndex:t_index];
            [t_function_cell refreshCell:t_function];
            //最后两个元素做位置校正
            NSInteger t_count = t_functions.count;
            NSInteger t_index = indexPath.row + indexPath.section;
            if( t_count - t_index <= 2 ) {
                CGPoint t_off;
                t_off.x = (cellHeight - cellWidth)*0.5f;
                t_off.y = 0.0f;
                [t_function_cell moveOffset:t_off];
            }
        }
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

-(BOOL)collectionView:(UICollectionView *)collectionView shouldSelectItemAtIndexPath:(NSIndexPath *)indexPath{
    return YES;
}

- (void)collectionView:(UICollectionView *)collectionView moveItemAtIndexPath:(NSIndexPath *)sourceIndexPath toIndexPath:(NSIndexPath*)destinationIndexPath{
}

#pragma mark ---- UICollectionViewDelegateFlowLayout

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath{
    NSMutableArray* t_functions = [[SWLogicSys getInst].pLogicFunction getFunctionLibSel];
    if(t_functions) {
        NSInteger t_count = t_functions.count;
        NSInteger t_index = indexPath.row + indexPath.section;
        if( t_count - t_index <= 2 ) {   //最后两个元素的size不一样
            return (CGSize){cellHeight,cellHeight};
        }
    }
    return (CGSize){cellWidth,cellHeight};
}

- (UIEdgeInsets)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout insetForSectionAtIndex:(NSInteger)section{
    return UIEdgeInsetsMake(2, 2, 2, 2);    //section间距
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
//
//- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout referenceSizeForFooterInSection:(NSInteger)section{
//    return (CGSize){tailWidth,tailHeight};
//}

#pragma mark ---- UICollectionViewDelegate

- (BOOL)collectionView:(UICollectionView *)collectionView shouldHighlightItemAtIndexPath:(NSIndexPath *)indexPath{
    return YES;
}

// 点击高亮
- (void)collectionView:(UICollectionView *)collectionView didHighlightItemAtIndexPath:(NSIndexPath *)indexPath{
}

// 选中某item
- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath{
    //选中的目标状态亮起来
    FunctionCell *t_cell_sel = (FunctionCell *)[collectionView cellForItemAtIndexPath:indexPath];
    if(pActiveCell!=t_cell_sel) {
        //切换
        if( pActiveCell ) {
            [pActiveCell setSelected:FALSE];
            [pActiveCell.pBG setBackgroundColor:[UIColor clearColor]];
        }
        if( t_cell_sel ){
            [t_cell_sel setSelected:TRUE];
            [t_cell_sel.pBG setBackgroundColor:[UIColor darkGrayColor]];
        }
        pActiveCell = t_cell_sel;
        //开启目标功能
        NSMutableArray* t_functions = [[SWLogicSys getInst].pLogicFunction getFunctionLibSel];
        if(t_functions) {
            SWFunction *t_function_sel = [t_functions objectAtIndex:indexPath.item];
            if( t_function_sel ){
                [[SWUISys getInst].pMainVC.pMainView openFunction: t_function_sel.m_name];
            }
        }
    } else {
        if( pActiveCell ) {
            [pActiveCell setSelected:FALSE];
            [pActiveCell.pBG setBackgroundColor:[UIColor clearColor]];
            pActiveCell = nil;
        }
        //
        NSMutableArray* t_functions = [[SWLogicSys getInst].pLogicFunction getFunctionLibSel];
        if(t_functions) {
            SWFunction *t_function_sel = [t_functions objectAtIndex:indexPath.item];
            if( t_function_sel ){
                //关闭目标功能
                [[SWUISys getInst].pMainVC.pMainView closeFunction: t_function_sel.m_name AtOnce:false];
            }
        }
    }
}

- (void)collectionView:(UICollectionView *)collectionView didDeselectItemAtIndexPath:(NSIndexPath *)indexPath{
//    FunctionCell *t_cell_sel = (FunctionCell *)[collectionView cellForItemAtIndexPath:indexPath];
//    if( t_cell_sel ){
//        [t_cell_sel setSelected:FALSE];
//        [t_cell_sel.pBG setBackgroundColor:[UIColor clearColor]];
//    }
}

- (void)scrollViewDidScroll:( UIScrollView *)scrollView {
    int a = 0;
}

//// 长按某item，弹出copy和paste的菜单
//- (BOOL)collectionView:(UICollectionView *)collectionView shouldShowMenuForItemAtIndexPath:(NSIndexPath *)indexPath{
//    return YES;
//}
//
//// 使copy和paste有效
//- (BOOL)collectionView:(UICollectionView *)collectionView canPerformAction:(SEL)action forItemAtIndexPath:(NSIndexPath *)indexPath withSender:(nullable id)sender{
//    //    if ([NSStringFromSelector(action) isEqualToString:@"copy:"] || [NSStringFromSelector(action) isEqualToString:@"paste:"]){
//    //        return YES;
//    //    }
//    return NO;
//}

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

