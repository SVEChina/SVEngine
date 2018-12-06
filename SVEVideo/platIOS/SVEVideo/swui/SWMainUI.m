//
//  SWMainUI.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWMainUI.h"
#import "SWUISys.h"
#import "SWFrameUI.h"
#import "SWSVView.h"
#import "SWState.h"
#import "../datasrc/SWInStreamSampleBuffer.h"
#import "../swlogic/SWLogicSys.h"
#import "../swlogic/SWLogicAni.h"
#import "../swbasic/SWBasicSys.h"
#import "../datasrc/SWDataSourceCamera.h"
#import "../swui/cells/MainCollectionViewCell.h"
const NSString *cellID = @"MainCollectionCell";
@interface SWMainUI()<UICollectionViewDelegate, UICollectionViewDataSource>{
    UICollectionView *m_pCollectView;
    NSInteger    m_cellCount;
    CGFloat      m_cellWidth;
    CGFloat      m_cellHeight;
    UIButton     *m_button1;
    UIButton     *m_button2;
    UIButton     *m_button3;
}

@end

@implementation SWMainUI


- (void)dealloc{
    
}

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self initTableView:frame];
        int t_w = frame.size.width;
        int t_h = frame.size.height;
        int t_view_px,t_view_py,t_view_w,t_view_h;
        t_view_px = TRANS_BY_WIDTH(36.6,t_w);
        t_view_py = TRANS_BY_HEIGHT(1226,t_h);
        t_view_w = TRANS_BY_WIDTH(172.8,t_w);
        t_view_h = TRANS_BY_HEIGHT(52.4,t_h);
        m_button1 = [[UIButton alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        m_button1.tag = 100 + 1;
        m_button1.titleLabel.font = [UIFont systemFontOfSize:12];
        [m_button1 setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
        [m_button1 setTitle:@"任务过滤器" forState:UIControlStateNormal];
        [m_button1 addTarget:self action:@selector(btnAction:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:m_button1];
        
        t_view_px = TRANS_BY_WIDTH(282.7,t_w);
        m_button2 = [[UIButton alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        m_button2.tag = 100 + 2;
        m_button2.titleLabel.font = [UIFont systemFontOfSize:12];
        [m_button2 setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
        [m_button2 setTitle:@"我也拍一个" forState:UIControlStateNormal];
        [m_button2 addTarget:self action:@selector(btnAction:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:m_button2];
        
        t_view_px = TRANS_BY_WIDTH(539,t_w);
        m_button3 = [[UIButton alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        m_button3.tag = 100 + 3;
        m_button3.titleLabel.font = [UIFont systemFontOfSize:12];
        [m_button3 setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
        [m_button3 setTitle:@"我的任务" forState:UIControlStateNormal];
        [m_button3 addTarget:self action:@selector(btnAction:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:m_button3];
    }
    return self;
}

- (void)initTableView:(CGRect)frame{
    int t_w = frame.size.width;
    int t_h = frame.size.height;
    int t_view_px,t_view_py,t_view_w,t_view_h;
    //colectview
    m_cellCount = 5;
    //
    m_cellWidth = TRANS_BY_WIDTH(320,t_w);    // 
    m_cellHeight = TRANS_BY_HEIGHT(320,t_h);   //
    //
    UICollectionViewFlowLayout *flowLayout = [[UICollectionViewFlowLayout alloc] init];
    flowLayout.sectionInset = UIEdgeInsetsMake(5, 5, 5, 5);
    //
    CGRect t_cvFrame;
    t_view_px = TRANS_BY_WIDTH(0,t_w);
    t_view_py = TRANS_BY_HEIGHT(0,t_h);
    t_view_w = TRANS_BY_WIDTH(750,t_w);
    t_view_h = TRANS_BY_HEIGHT(1171,t_h);
    t_cvFrame.origin.x = t_view_px;
    t_cvFrame.origin.y = t_view_py;
    t_cvFrame.size.width = t_view_w;
    t_cvFrame.size.height = t_view_h;
    //
    m_pCollectView = [[UICollectionView alloc] initWithFrame:t_cvFrame collectionViewLayout:flowLayout];
    //
    m_pCollectView.delegate = self;
    m_pCollectView.dataSource = self;
    //
    [m_pCollectView setBackgroundColor:[UIColor colorWithRed:242.0f/255.0f green:242.0f/255.0f blue:242.0f/255.0f alpha:1.0f]];
    [m_pCollectView registerClass:[MainCollectionViewCell class] forCellWithReuseIdentifier:cellID];
    //
    [self addSubview:m_pCollectView];
}

- (void)changeToShow{
    UIView* t_targetView = [SWUISys getInst].pMainVC.view;
    for(UIView *t_view in [t_targetView subviews]){
        [t_view removeFromSuperview];
    }
    [self removeFromSuperview];
    [t_targetView addSubview:self];
}

#pragma mark ---- UICollectionViewDataSource

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView{
    return 1;
}


- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section{
    return m_cellCount;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath{
    //每一个cell属性
    UICollectionViewCell *cell = [m_pCollectView dequeueReusableCellWithReuseIdentifier:cellID forIndexPath:indexPath];
    MainCollectionViewCell* t_cell = (MainCollectionViewCell*)cell;
    if(t_cell){
        t_cell.backgroundColor = [UIColor redColor];
    }
    return cell;
}

- (BOOL)collectionView:(UICollectionView *)collectionView canMoveItemAtIndexPath:(NSIndexPath *)indexPath{
    return YES;
}


- (void)collectionView:(UICollectionView *)collectionView moveItemAtIndexPath:(NSIndexPath *)sourceIndexPath toIndexPath:(NSIndexPath*)destinationIndexPath{
}

#pragma mark ---- UICollectionViewDelegateFlowLayout

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath{
    return (CGSize){m_cellWidth,m_cellHeight};
}


- (UIEdgeInsets)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout insetForSectionAtIndex:(NSInteger)section{
    return UIEdgeInsetsMake(5, 10, 5, 10);    //section间距
}

- (CGFloat)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout minimumLineSpacingForSectionAtIndex:(NSInteger)section{
    return 10.0f;    //上下cell
}

- (CGFloat)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout minimumInteritemSpacingForSectionAtIndex:(NSInteger)section{
    return 10.0f;     //左右cell
}
#pragma mark ---- UICollectionViewDelegate

- (BOOL)collectionView:(UICollectionView *)collectionView shouldHighlightItemAtIndexPath:(NSIndexPath *)indexPath{
    return YES;
}

// 选中某item
- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath{
    
}

- (void)collectionView:(UICollectionView *)collectionView didDeselectItemAtIndexPath:(NSIndexPath *)indexPath{
    //    RankeCVCell *lastCell = (RankeCVCell *)[collectionView cellForItemAtIndexPath:indexPath];
    //    [lastCell setSelected:NO];
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
   
}
- (UIView*)hitTest:(CGPoint)point withEvent:(UIEvent *)event{
    UIView *hitView = [super hitTest:point withEvent:event];
    if(hitView == self){
        return nil;
    }
    return hitView;
}

- (void)btnAction:(UIButton *)btn{
    switch (btn.tag) {
        case 101:{
            break;
        }
        case 102:{
            [self showCameraView];
            break;
        }
        case 103:{
            break;
        }
        default:
            break;
    }
}

- (void)showCameraView{
    [[SWUISys getInst].pMainVC.pSVView changeToShow];
}
@end
