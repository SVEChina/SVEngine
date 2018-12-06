//
//  SWMagicsEffectsUI.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWMagicsEffectsUI.h"
#import "../swlogic/SWLogicSys.h"
#import "../swlogic/swmagic/SWMagicCategoryUnit.h"
#import "../swlogic/swmagic/SWMagicUnit.h"
#import "SWUISys.h"

@interface FMMagicEffectCell : UICollectionViewCell
@property (nonatomic, strong) UIImageView *coverImageV;
@property (nonatomic, strong) UIActivityIndicatorView *activityView;
@property (nonatomic, strong) UIImageView *downLoadImageV;

@property (nonatomic, assign) BOOL isLoading;
- (void)refreshCellWithData:(NSDictionary *)data;
@end
@implementation FMMagicEffectCell

-(instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        self.isLoading = NO;
        [self.contentView addSubview:self.coverImageV];
        [self.contentView addSubview:self.downLoadImageV];
        
        self.activityView = [[UIActivityIndicatorView alloc]initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
        self.activityView.center = self.contentView.center;
        [self.activityView setHidesWhenStopped:YES];
        [self.contentView addSubview:self.activityView];
    }
    return self;
}

- (UIImageView *)coverImageV
{
    if (!_coverImageV) {
        _coverImageV = [[UIImageView alloc] initWithFrame:self.bounds];
        _coverImageV.contentMode = UIViewContentModeScaleAspectFit;
        
    }
    return _coverImageV;
}

- (UIImageView *)downLoadImageV
{
    if (!_downLoadImageV) {
        _downLoadImageV = [[UIImageView alloc] initWithFrame:CGRectMake(self.bounds.size.width - 15, self.bounds.size.height - 15, 15, 15)];
        _downLoadImageV.contentMode = UIViewContentModeScaleAspectFit;
        _downLoadImageV.image = [UIImage imageNamed:@"下载图标"];
    }
    return _downLoadImageV;
}

- (void)cellSelect{
    self.contentView.layer.borderColor = [UIColor colorWithRed:0.16 green:0.75 blue:0.72 alpha:1.0].CGColor;
    self.contentView.layer.borderWidth = 1.5f;
}

- (void)cellUnselect{
    self.contentView.layer.borderColor = [UIColor clearColor].CGColor;
    self.contentView.layer.borderWidth = 0.0f;
}

- (void)refreshCellWithData:(SWMagicUnit *)magicUnit{
    NSString *imageUrl = magicUnit.cover;
    // 0未下载 1已下载 2正在下载
    if ([magicUnit.isDownload intValue] == 0) {
        self.downLoadImageV.hidden = NO;
        [self.activityView stopAnimating];
    }else if([magicUnit.isDownload intValue] == 1) {
        self.downLoadImageV.hidden = YES;
        [self.activityView stopAnimating];
    }else if([magicUnit.isDownload intValue] == 2){
        [self.activityView startAnimating];
    }
//    [self.coverImageV sd_setImageWithURL:[NSURL URLWithString:imageUrl] completed:^(UIImage *image, NSError *error, SDImageCacheType cacheType, NSURL *imageURL) {
//    }];
}

@end


@interface SWMagicsEffectsUI()<UICollectionViewDelegate, UICollectionViewDataSource>{
    UIScrollView     *mCollectionContentView;
    UIScrollView     *mTitleContentView;
    UICollectionView *mCollectionView;
    CGFloat          itemWidth;
    CGFloat          itemHeight;
    NSMutableArray   *mCategoryArray;
    NSIndexPath      *curIndexPath;//当前选中的特效所在组和位置
    NSInteger        curSelectSection;//当前选中的组
   
    UIButton         *mLogonBtn;
    
}
@end
static NSString * const reuseIdentifier = @"EffectCell";
@implementation SWMagicsEffectsUI

- (void)dealloc{
    NSLog(@"destory SWMagicsEffectsUI");
}

- (instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        mCategoryArray = [[NSMutableArray alloc] init];
        itemWidth = (frame.size.width - 25*2 - 20 * 4)/5;
        itemHeight = itemWidth;
    
        //承载魔贴View
        mCollectionContentView = [[UIScrollView alloc]initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height)];
        mCollectionContentView.showsHorizontalScrollIndicator = NO;
        mCollectionContentView.showsVerticalScrollIndicator = NO;
        mCollectionContentView.scrollEnabled = NO;
        mCollectionContentView.bounces = NO;
        mCollectionContentView.backgroundColor = [UIColor colorWithRed:32.0/255.0 green:32.0/255.0 blue:32.0/255.0 alpha:0.6];
        mCollectionContentView.pagingEnabled = YES;
        [self addSubview:mCollectionContentView];
        //
        mTitleContentView = [[UIScrollView alloc]initWithFrame:CGRectMake(0,  mCollectionContentView.frame.origin.y + mCollectionContentView.frame.size.height, mCollectionContentView.bounds.size.width, 50)];
        mTitleContentView.showsHorizontalScrollIndicator = NO;
        mTitleContentView.showsVerticalScrollIndicator = NO;
        mTitleContentView.bounces = NO;
        mTitleContentView.backgroundColor = [UIColor colorWithRed:32.0/255.0 green:32.0/255.0 blue:32.0/255.0 alpha:0.8];
        mTitleContentView.delegate = self;
//        [self addSubview:mTitleContentView];
        [self refreshAllData];
        curSelectSection = 0;
        curIndexPath = [NSIndexPath indexPathForItem:0 inSection:0];
    }
    return self;
}

- (void)showWithAni:(BOOL)ani{
    if (_showed) {
        return;
    }
    self.hidden = NO;
    CGFloat screenHeight = [UIScreen mainScreen].bounds.size.height;
    if (ani) {
        [UIView animateWithDuration:0.2f animations:^{
            self.frame = CGRectMake(0, screenHeight - self.bounds.size.height, self.bounds.size.width, self.bounds.size.height);
        } completion:^(BOOL finished) {
            if (finished) {
                _showed = true;
            }
        }];
    }else{
        self.frame = CGRectMake(0, screenHeight - self.bounds.size.height, self.bounds.size.width, self.bounds.size.height);
        _showed = true;
        
    }
}

- (void)hideWithAni:(BOOL)ani{
    if (!_showed) {
        return;
    }
    CGFloat screenHeight = [UIScreen mainScreen].bounds.size.height;
    if (ani) {
        [UIView animateWithDuration:0.2f animations:^{
            self.frame = CGRectMake(0, screenHeight, self.bounds.size.width, self.bounds.size.height);
        } completion:^(BOOL finished) {
            if (finished) {
                _showed = NO;
                self.hidden = YES;
            }
        }];
    }else{
        self.frame = CGRectMake(0, screenHeight, self.bounds.size.width, self.bounds.size.height);
        _showed = NO;
        self.hidden = YES;
    }
}

- (void)sortArrayUseID:(NSMutableArray *)sortArray{
    [sortArray sortUsingComparator:^NSComparisonResult(id  _Nonnull obj1, id  _Nonnull obj2) {
        NSDictionary *dic1 = obj1;
        NSDictionary *dic2 = obj2;
        NSString *id1 = dic1[@"id"];
        NSString *id2 = dic2[@"id"];
        NSComparisonResult result = [id1 compare:id2];
        return result;
    }];
}

- (void)refreshAllData{
//    [self refreshViews];
}

- (void)refreshViews{
//    NSArray *titleSubViews = mTitleContentView.subviews;
//    for (int i=0; i<titleSubViews.count; i++) {
//        UIView *subView = titleSubViews[i];
//        [subView removeFromSuperview];
//    }
//    NSArray *collectionViews = mCollectionContentView.subviews;
//    for (int i=0; i<collectionViews.count; i++) {
//        UIView *subView = collectionViews[i];
//        [subView removeFromSuperview];
//    }
//    CGFloat width = mCollectionContentView.bounds.size.width;
//    mTitleContentView.contentSize = CGSizeMake(mCategoryArray.count *width/5, mTitleContentView.bounds.size.height);
//    mCollectionContentView.contentSize = CGSizeMake( mCategoryArray.count * width , mCollectionContentView.bounds.size.height);
//    if (mCategoryArray.count > 0) {
//        for (int i = 0; i < mCategoryArray.count ; i++) {
//            SWMagicCategoryUnit *categoryUnit = mCategoryArray[i];
//            /*
//             NSString *categoryName = categoryUnit.name;
//             UIButton *btn = [[UIButton alloc]initWithFrame:CGRectMake(width/5 *i, 0, width/5, mTitleContentView.frame.size.height)];
//
//             [btn setTitle:categoryName forState:UIControlStateNormal];
//             [btn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
//             [btn setTitleColor:[UIColor colorWithRed:0.16 green:0.75 blue:0.72 alpha:1.0] forState:UIControlStateSelected];
//             btn.tag = 100 + i;
//             [btn addTarget:self action:@selector(changeSection:) forControlEvents:UIControlEventTouchUpInside];
//             if (i == curSelectSection) {
//             btn.selected = YES;
//             }
//             [mTitleContentView addSubview:btn];
//             */
//            UICollectionViewFlowLayout *layout = [[UICollectionViewFlowLayout alloc] init];
//            //        layout.itemSize = CGSizeMake(itemWidth, itemWidth);
//            layout.scrollDirection = UICollectionViewScrollDirectionVertical;
//            layout.minimumLineSpacing = 11;//行
//            layout.minimumInteritemSpacing = 28;//cell间距
//            layout.sectionInset = UIEdgeInsetsMake(14, 25, 14, 25);
//            UICollectionView *collectionView = [[UICollectionView alloc]initWithFrame:CGRectMake(i * width, 0, width, mCollectionContentView.frame.size.height ) collectionViewLayout:layout];
//            collectionView.tag = 200 + i;
//            collectionView.backgroundColor = [UIColor clearColor];//[UIColor colorWithRed:
//            collectionView.delegate = self;
//            collectionView.dataSource = self;
//            [collectionView registerClass:[FMMagicEffectCell class] forCellWithReuseIdentifier:reuseIdentifier];
//            collectionView.showsHorizontalScrollIndicator = NO;
//            collectionView.showsVerticalScrollIndicator = NO;
//            collectionView.bounces = NO;
//            [mCollectionContentView addSubview:collectionView];
//            [collectionView reloadData];
//        }
//    }else{
//        NSDictionary *userinfo = [[NSUserDefaults standardUserDefaults] objectForKey:@"userinfo"];
//        if ([userinfo[@"userid"] isEqualToString:@"default"]) {
//            mLogonBtn = [[UIButton alloc] initWithFrame:CGRectMake((mCollectionContentView.bounds.size.width - 200)*0.5, (mCollectionContentView.bounds.size.height - 80)*0.5, 200, 80)];
//            [mLogonBtn setTitle:@"点击登录" forState:UIControlStateNormal];
//            [mLogonBtn addTarget:self action:@selector(eventLogoBtn:) forControlEvents:UIControlEventTouchUpInside];
//            [mCollectionContentView addSubview:mLogonBtn];
//        }
//    }
  
}

- (void)eventLogoBtn:(UIButton *)btn{
}

- (void)changeSection:(UIButton *)btn {
//    NSInteger tag = btn.tag - 100;
//    if (curSelectSection == tag) {
//        return;
//    }
//    UIButton *lastBtn = [mTitleContentView viewWithTag:curSelectSection + 100];
//    lastBtn.selected = NO;
//    btn.selected = YES;
//    curSelectSection = tag;
//    [mCollectionContentView setContentOffset:CGPointMake(tag * self.bounds.size.width, 0)  animated:YES];
////    UICollectionView *collectionView = [mCollectionContentView viewWithTag:200 + curSelectSection];
////    [collectionView reloadData];
//    SWMagicCategoryUnit *categoryUnit = mCategoryArray[tag];
//    [[SWLogicSys getInst].pLogicAni downLoadMagicEffectCategoryByCategoryDic:[categoryUnit convertUnitToDictionary]];
}

- (void)refreshSectionWithCategory:(SWMagicCategoryUnit *)categoryUnit{
    if (categoryUnit) {
        for (int i=0; i<mCategoryArray.count; i++) {
            SWMagicCategoryUnit *t_categoryUnit = mCategoryArray[i];
            NSString *categoryID = categoryUnit.categoryID;
            NSString *t_categoryID = t_categoryUnit.categoryID;
            if ([categoryID isEqualToString:t_categoryID]) {
                [mCategoryArray replaceObjectAtIndex:i withObject:categoryUnit];
                UICollectionView *collectionView = [mCollectionContentView viewWithTag:200 + i];
                [collectionView reloadData];
                break;
            }
        }
    }else{
        
    }
   
}

- (void)refreshCellWithMagicEffect:(SWMagicUnit *)magicUnit{
    if (magicUnit) {
        for (int i=0; i<mCategoryArray.count; i++) {
            SWMagicCategoryUnit *categoryUnit = mCategoryArray[i];
            NSString *t_categoryID = categoryUnit.categoryID;
            if ([magicUnit.categoryID isEqualToString:t_categoryID]) {
                NSMutableArray *spines = categoryUnit.spines;
                for (int j=0; j<spines.count; j++) {
                    SWMagicUnit *spine  = spines[j];
                    if ([spine.magicEffectID isEqualToString:magicUnit.magicEffectID]) {
                        [categoryUnit.spines replaceObjectAtIndex:j withObject:magicUnit];
                        UICollectionView *curCollectionView = [mCollectionContentView viewWithTag:i+200];
                        [curCollectionView reloadItemsAtIndexPaths:[NSArray arrayWithObject:[NSIndexPath indexPathForItem:j+1 inSection:0]]];
                        /*
                        NSString *isDownLoad = magicUnit.isDownload;
                        //如果下载成功，消费金币
                        if ([isDownLoad isEqualToString:@"1"]) {
                            int userGold = [SWLogicSys getInst].pLogicUser.m_gold;
                            int price = [magicUnit.price intValue];
                            if (userGold >= price) {
                                userGold = userGold - price;
                                [SWLogicSys getInst].pLogicUser.m_gold = userGold;
                                [[SWLogicSys getInst].pLogicUser saveLocal];
                                //花费金币
                                NSString *message = [NSString stringWithFormat:@"花费%d金币",price];
                                
                                [[SWUISys getInst].pMagicsView makeToast:message
                                                                duration:1.5f
                                                                position:CSToastPositionCenter];
                            }
                        }
                         */
                        return;
                    }
                }
 
            }
        }
    }
}

#pragma mark <UICollectionViewDataSource>

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return 1;
}


- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    NSInteger itemNumber = 0;
    SWMagicCategoryUnit *categoryUnit = mCategoryArray[collectionView.tag - 200];
    NSArray *spineArray = categoryUnit.spines;
    if (spineArray.count > 0) {
        itemNumber = spineArray.count + 1;
    }
    return itemNumber;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    FMMagicEffectCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifier forIndexPath:indexPath];
    SWMagicCategoryUnit *categoryUnit = mCategoryArray[collectionView.tag - 200];
    NSArray *spineArray = categoryUnit.spines;
    if (indexPath.item == 0) {
        UIImage *image = [UIImage imageNamed:@"share_cleareffect"];
        cell.coverImageV.image = image;
        cell.downLoadImageV.hidden = YES;
    }else{
        [cell refreshCellWithData:spineArray[indexPath.item-1]];
    }
    if (((collectionView.tag - 200) == curIndexPath.section) && (indexPath.item == curIndexPath.item)) {
        [cell cellSelect];
    }else{
        [cell cellUnselect];
    }
    // Configure the cell
    
    return cell;
}



- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath{   
}



#pragma mark <UICollectionViewDelegate>

/*
 // Uncomment this method to specify if the specified item should be highlighted during tracking
 - (BOOL)collectionView:(UICollectionView *)collectionView shouldHighlightItemAtIndexPath:(NSIndexPath *)indexPath {
 return YES;
 }
 */

/*
 // Uncomment this method to specify if the specified item should be selected
 - (BOOL)collectionView:(UICollectionView *)collectionView shouldSelectItemAtIndexPath:(NSIndexPath *)indexPath {
 return YES;
 }
 */

/*
 // Uncomment these methods to specify if an action menu should be displayed for the specified item, and react to actions performed on the item
 - (BOOL)collectionView:(UICollectionView *)collectionView shouldShowMenuForItemAtIndexPath:(NSIndexPath *)indexPath {
 return NO;
 }
 
 - (BOOL)collectionView:(UICollectionView *)collectionView canPerformAction:(SEL)action forItemAtIndexPath:(NSIndexPath *)indexPath withSender:(id)sender {
 return NO;
 }
 
 - (void)collectionView:(UICollectionView *)collectionView performAction:(SEL)action forItemAtIndexPath:(NSIndexPath *)indexPath withSender:(id)sender {
 
 }
 */
/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

@end
