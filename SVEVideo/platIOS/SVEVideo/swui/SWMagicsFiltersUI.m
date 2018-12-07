//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWMagicsFiltersUI.h"
#import "../swlogic/SWLogicSys.h"
#import "../swlogic/swmagic/SWMagicFilterUnit.h"

static const NSString *reuseIdentifier = @"FMMagicFilterCell";
@interface FMMagicFilterCell : UICollectionViewCell
@property (nonatomic, strong) UIImageView *coverImageV;
@property (nonatomic, strong) UILabel     *filterName;
@end
@implementation FMMagicFilterCell
-(instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        self.contentView.backgroundColor = [UIColor clearColor];
        if (!_coverImageV) {
            _coverImageV = [[UIImageView alloc] initWithFrame:CGRectMake(2, 2, self.bounds.size.width - 4, self.bounds.size.height - 4)];
            _coverImageV.layer.cornerRadius = (self.bounds.size.width - 4)*0.5;
            _coverImageV.clipsToBounds = YES;
            _coverImageV.contentMode = UIViewContentModeScaleAspectFit;
            [self.contentView addSubview:self.coverImageV];
        }
        
        
        if (!_filterName) {
            _filterName = [[UILabel alloc] initWithFrame:CGRectMake(0, _coverImageV.bounds.size.height - 18, _coverImageV.bounds.size.width, 18)];
            _filterName.font = [UIFont systemFontOfSize:8];
            _filterName.backgroundColor = [UIColor colorWithRed:0.67 green:0.67 blue:0.67 alpha:0.7];
            _filterName.textAlignment = NSTextAlignmentCenter;
            _filterName.textColor = [UIColor whiteColor];
            _filterName.text = @"滤镜";
            [_coverImageV addSubview:_filterName];
        }
    }
    return self;
}

- (void)selectCell{
    _coverImageV.layer.borderColor = [UIColor whiteColor].CGColor;
    _coverImageV.layer.borderWidth = 2.5f;
}

- (void)unselectCell{
    _coverImageV.layer.borderWidth = 0.0f;
    _coverImageV.layer.borderColor = [UIColor clearColor].CGColor;
}

@end

@interface SWMagicsFiltersUI()<UICollectionViewDelegate, UICollectionViewDataSource>{
    NSMutableArray     *m_pFilterList;
    UICollectionView   *m_pCollectionView;
    NSIndexPath        *m_curIndexPath;
}
@end


@implementation SWMagicsFiltersUI
- (instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        m_pFilterList = [[NSMutableArray alloc] init];
        NSString *t_path = [[NSBundle mainBundle] pathForResource:@"filterlist.plist" ofType:nil];
        NSArray *filters = [[NSMutableArray alloc] initWithContentsOfFile:t_path];
        for (NSDictionary *filter in filters) {
            SWMagicFilterUnit *filterUnit = [[SWMagicFilterUnit alloc] init];
            [filterUnit refreshUnit:filter];
            [m_pFilterList addObject:filterUnit];
        }
        //
        
        UICollectionViewFlowLayout *layout = [[UICollectionViewFlowLayout alloc] init];
        NSInteger cellNumOneLine = 5;
        CGFloat padding = 10;
        CGFloat itemWidth = (frame.size.width - (cellNumOneLine+1)*padding)/cellNumOneLine;
        CGFloat itemHeight = itemWidth;
        layout.itemSize = CGSizeMake(itemWidth, itemHeight);
        layout.scrollDirection = UICollectionViewScrollDirectionHorizontal;
        layout.minimumLineSpacing = 10;//行
        layout.minimumInteritemSpacing = padding;//cell间距
        layout.sectionInset = UIEdgeInsetsMake(0, padding, 0, padding);
        m_pCollectionView = [[UICollectionView alloc]initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height - 2*padding ) collectionViewLayout:layout];
        m_pCollectionView.backgroundColor = [UIColor clearColor];//[UIColor colorWithRed:
        m_pCollectionView.delegate = self;
        m_pCollectionView.dataSource = self;
        [m_pCollectionView registerClass:[FMMagicFilterCell class] forCellWithReuseIdentifier:reuseIdentifier];
        m_pCollectionView.showsHorizontalScrollIndicator = NO;
        m_pCollectionView.showsVerticalScrollIndicator = NO;
        m_pCollectionView.bounces = NO;
        [self addSubview:m_pCollectionView];
        
        m_curIndexPath = [NSIndexPath indexPathForItem:0 inSection:0];
        [m_pCollectionView selectItemAtIndexPath:m_curIndexPath animated:NO scrollPosition:(UICollectionViewScrollPositionNone)];
    }
    return self;
}

- (void)dealloc{
    [m_pFilterList removeAllObjects];
}

- (void)show{
    self.hidden = NO;
    _showed = YES;
}

- (void)hide{
    self.hidden = YES;
    _showed = NO;
}

#pragma mark <UICollectionViewDataSource>

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return 1;
}


- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return m_pFilterList.count;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    FMMagicFilterCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifier forIndexPath:indexPath];
    SWMagicFilterUnit *filterUnit = [m_pFilterList objectAtIndex:indexPath.item];
    NSString *t_filterLogoName = filterUnit.filterLogo;
    NSString *t_filterName = filterUnit.filterName;
    UIImage *t_filterLogoImage = [UIImage imageNamed:t_filterLogoName];
    if (t_filterName) {
        cell.filterName.text = t_filterName;
    }
    if (t_filterLogoName) {
        cell.coverImageV.image = t_filterLogoImage;
    }
    if (indexPath == m_curIndexPath) {
        [cell selectCell];
    }else{
        [cell unselectCell];
    }
    return cell;
}



- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath{
//    FMMagicFilterCell *cell = (FMMagicFilterCell *)[collectionView cellForItemAtIndexPath:indexPath];
//    [cell selectCell];
//    SWMagicFilterUnit *filterUnit = [m_pFilterList objectAtIndex:indexPath.item];
//    if ([filterUnit.filterName isEqualToString:@"无"]) {
//        [[SWLogicSys getInst].pLogicAni clearFilterEffect];
//    }else {
//        NSString *filterPath = [[NSBundle mainBundle] pathForResource:filterUnit.filterPackage ofType:@"bundle"];
//        [[SWLogicSys getInst].pLogicAni setFilterEffect:filterPath];
//    }
//    m_curIndexPath = indexPath;
}

- (void)collectionView:(UICollectionView *)collectionView didDeselectItemAtIndexPath:(NSIndexPath *)indexPath{
//    [[SWLogicSys getInst].pLogicAni clearFilterEffect];
//    FMMagicFilterCell *cell = (FMMagicFilterCell *)[collectionView cellForItemAtIndexPath:indexPath];
//    [cell unselectCell];
}
/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

@end
