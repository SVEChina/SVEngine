//
//  QYAssetModel.h
//  QYPhotoAlbum
//
//  Created by yoyo on 2017/9/30.
//  Copyright © 2017年 burning. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "QYPhotoConstant.h"
@class PHAsset;
@interface QYAssetModel : NSObject<NSCopying, NSMutableCopying>

- (instancetype)initWithAsset:(PHAsset *)asset;

@property(nonatomic, strong) PHAsset *asset;
@property(nonatomic, assign) QYPhotoAssetType medaiType;

@end
