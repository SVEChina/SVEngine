//
//  QYGroupModel.h
//  QYPhotoAlbum
//
//  Created by yoyo on 2017/9/30.
//  Copyright © 2017年 burning. All rights reserved.
//

#import <Foundation/Foundation.h>
@class PHAssetCollection;
@class QYAssetModel;

@interface QYGroupModel : NSObject<NSCopying, NSMutableCopying>
@property(nonatomic, strong) PHAssetCollection* collection;
@property(nonatomic, strong) NSMutableArray<QYAssetModel *>* asstArray;  ///<* 筛选出的
@property(nonatomic, assign) NSInteger count;

@end
