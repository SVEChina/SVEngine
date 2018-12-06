//
//  SWFeatureMagicEffectsUI.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWFeatureMagicEffectsUI.h"
#import "../../swlogic/swmagic/SWMagicUnit.h"
#import "../../swlogic/SWLogicSys.h"
static const NSString *reuseIdentifier = @"FMFeatureMagicEffectCell";
@interface FMFeatureMagicEffectCell : UICollectionViewCell
@property (nonatomic, strong) UIImageView *coverImageV;
@property (nonatomic, strong) UIActivityIndicatorView *activityView;
@property (nonatomic, strong) UIImageView *downLoadImageV;

@property (nonatomic, assign) BOOL isLoading;
- (void)refreshCellWithData:(NSDictionary *)data;
@end
@implementation FMFeatureMagicEffectCell

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
    self.coverImageV.image = [UIImage imageNamed:imageUrl];
}

@end

@interface SWFeatureMagicEffectsUI()<UICollectionViewDelegate, UICollectionViewDataSource>{
    UICollectionView *m_collectionView;
    NSMutableArray *m_effectsArray;
    NSIndexPath    *curIndexPath;
    BOOL            _showed;
}
@end

@implementation SWFeatureMagicEffectsUI
- (instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = [UIColor colorWithRed:32.0/255.0 green:32.0/255.0 blue:32.0/255.0 alpha:0.6];
        m_effectsArray = [[NSMutableArray alloc] init];
        NSString *resourcePath = [[NSBundle mainBundle] pathForResource:@"featuredisplay" ofType:@"plist"];
        NSArray *resourceArray = [[NSArray alloc] initWithContentsOfFile:resourcePath];
        for (NSDictionary *t_dic in resourceArray) {
            NSString *name = t_dic[@"name"];
            NSString *package = t_dic[@"package"];
            NSString *logo = t_dic[@"logo"];
            SWMagicUnit *magicUnit = [[SWMagicUnit alloc] init];
            magicUnit.name = name;
            magicUnit.source = [[NSBundle mainBundle] pathForResource:package ofType:@"bundle"];
            magicUnit.cover = logo;
            magicUnit.isDownload = @"1";
            [m_effectsArray addObject:magicUnit];
        }
        UICollectionViewFlowLayout *layout = [[UICollectionViewFlowLayout alloc] init];
        //        layout.itemSize = CGSizeMake(itemWidth, itemWidth);
        layout.scrollDirection = UICollectionViewScrollDirectionVertical;
        layout.minimumLineSpacing = 11;//行
        layout.minimumInteritemSpacing = 28;//cell间距
        layout.sectionInset = UIEdgeInsetsMake(14, 25, 14, 25);
        m_collectionView = [[UICollectionView alloc]initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height ) collectionViewLayout:layout];
        m_collectionView.backgroundColor = [UIColor clearColor];//[UIColor colorWithRed:
        m_collectionView.delegate = self;
        m_collectionView.dataSource = self;
        [m_collectionView registerClass:[FMFeatureMagicEffectCell class] forCellWithReuseIdentifier:reuseIdentifier];
        m_collectionView.showsHorizontalScrollIndicator = NO;
        m_collectionView.showsVerticalScrollIndicator = NO;
        m_collectionView.bounces = NO;
        [self addSubview:m_collectionView];
        [m_collectionView reloadData];
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

#pragma mark <UICollectionViewDataSource>

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return 1;
}


- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return m_effectsArray.count+1;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    FMFeatureMagicEffectCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifier forIndexPath:indexPath];
    
    if (indexPath.item == 0) {
        UIImage *image = [UIImage imageNamed:@"share_cleareffect"];
        cell.coverImageV.image = image;
        cell.downLoadImageV.hidden = YES;
    }else{
        SWMagicUnit *t_magicUnit = m_effectsArray[indexPath.item - 1];
        [cell refreshCellWithData:t_magicUnit];
    }
    if ((indexPath.item == curIndexPath.item)) {
        [cell cellSelect];
    }else{
        [cell cellUnselect];
    }
    // Configure the cell
    
    return cell;
}

- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath{
}
/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

@end
