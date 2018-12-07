//
//  QYGroupModel.m
//  QYPhotoAlbum
//
//  Created by yoyo on 2017/9/30.
//  Copyright © 2017年 burning. All rights reserved.
//

#import "QYGroupModel.h"
#import "QYAssetModel.h"

#if __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_8_0
#import <Photos/Photos.h>
#endif

@implementation QYGroupModel

- (id)copyWithZone:(nullable NSZone *)zone
{
    QYGroupModel *groupModel = [[self class] allocWithZone:zone];
    groupModel.collection = self.collection;
    groupModel.count = self.count;
    return groupModel;
}
- (id)mutableCopyWithZone:(nullable NSZone *)zone
{
    QYGroupModel *groupModel = [[[self class] alloc] init];
    groupModel.collection = self.collection;
    groupModel.count = self.count;
    return groupModel;
}
@end
