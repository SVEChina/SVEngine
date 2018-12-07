//
//  PLSystemAlbumCell.m
//  Philm
//
//  Created by 陈方方 on 2016/10/21.
//  Copyright © 2016年 yoyo. All rights reserved.
//

#import "PLSystemAlbumCell.h"
#import "PLCreateUITool.h"
#import "QYPhotoService.h"
@interface PLSystemAlbumCell (){
    QYAssetModel *_assetModel;
    UIImageView *_videoCoverView;

    BOOL _animationStart;

    // livePhoto
    UIImageView *_liveImageView;    // livePhoto标识
    UILabel *_durationLabel;        //时间长度
    UIImageView *_hFRateImageView;  //慢动作标识
}
@property(nonatomic, strong) UIImageView *videoCoverView;

@end

@implementation PLSystemAlbumCell
@synthesize animationStart = _animationStart, videoCoverView = _videoCoverView;

- (id)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]){
        self.clipsToBounds = YES;
        //动图
        _videoCoverView = [[UIImageView alloc] initWithImage:nil];
        _videoCoverView.frame = self.bounds;
        _videoCoverView.animationDuration = 0.5;
        _videoCoverView.animationRepeatCount = 0;
        _videoCoverView.contentMode = UIViewContentModeScaleAspectFill;
        _videoCoverView.clipsToBounds = YES;
        [self addSubview:_videoCoverView];

        _liveImageView =
            [PLCreateUITool createImageViewWithPoint:CGPointZero fileName:@"album_live" superView:_videoCoverView];
        _liveImageView.frame =
            CGRectMake(CGRectGetWidth(_videoCoverView.frame) - 25, CGRectGetHeight(_videoCoverView.frame) - 14, 25, 14);
        [_videoCoverView addSubview:_liveImageView];

        UIImage *hFRateImage = [UIImage imageNamed:@"icon_slow_normal"];
        _hFRateImageView = [[UIImageView alloc]
            initWithFrame:CGRectMake(1, CGRectGetHeight(_videoCoverView.frame) - hFRateImage.size.height - 1,
                                     hFRateImage.size.width, hFRateImage.size.height)];
        _hFRateImageView.image = hFRateImage;
        [_videoCoverView addSubview:_hFRateImageView];

        //        _durationLabel = [PLCreateUITool
        //            createLabelInitWithFrame:CGRectMake(4, CGRectGetHeight(frame) - 14, CGRectGetWidth(frame), 14)
        //                     backgroundColor:NX_UIColorFromRGBA(0xffffff, 0.5)
        //                       textAlignment:NXTextAlignmentCenter
        //                                text:@""
        //                           textColor:PL_COLOR_BlACK
        //                                font:[UIFont systemFontOfSize:9]
        //                           superView:_videoCoverView];
    }
    return self;
}
- (void)setAssetModel:(QYAssetModel *)assetModel_
{
    _assetModel = assetModel_;
    [_videoCoverView stopAnimating];

    _videoCoverView.image = nil;
    if (_assetModel)
    {
        //数据
        __weak typeof(self) weakSelf = self;
        [[QYPhotoService shareInstanced] requestImageForAsset:self.assetModel
                                                         size:CGSizeMake(240.0f, 240.f) success:^(UIImage * _Nullable image)
         {
            __strong typeof(weakSelf) strongSelf = weakSelf;
            strongSelf.videoCoverView.image = image;
        }
                                                      failure:nil
                                                progressBlock:nil];
        
        if (_assetModel.medaiType == QYPhotoAssetTypeLiviePhoto)
        {
            _liveImageView.hidden = NO;
            _durationLabel.hidden = YES;
            _hFRateImageView.hidden = YES;
        }
        else
        {
            _liveImageView.hidden = YES;
            _durationLabel.hidden = YES;
            _hFRateImageView.hidden = YES;
        }
    }
}

- (void)setIsShowImageModel:(BOOL)isShowImageModel
{
    _isShowImageModel = isShowImageModel;
    if (_isShowImageModel)
    {
        _liveImageView.hidden = YES;
        _durationLabel.hidden = YES;
    }
}
- (QYAssetModel *)assetModel { return _assetModel; }
- (void)setAnimationStart:(BOOL)animationStart_
{
    _animationStart = animationStart_;
    if (_animationStart)
    {
        [_videoCoverView startAnimating];
    }
    else
    {
        [_videoCoverView stopAnimating];
    }
}
- (BOOL)animationStart { return _animationStart; }
@end
