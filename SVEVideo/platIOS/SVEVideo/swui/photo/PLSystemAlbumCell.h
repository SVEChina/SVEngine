//
//  PLSystemAlbumCell.h
//  Philm
//
//  Created by 陈方方 on 2016/10/21.
//  Copyright © 2016年 yoyo. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "QYAssetModel.h"

@interface PLSystemAlbumCell : UICollectionViewCell

@property(nonatomic, strong) QYAssetModel *assetModel;

@property(nonatomic, assign) BOOL isShowImageModel;

@property(nonatomic, assign) BOOL animationStart;

@end
