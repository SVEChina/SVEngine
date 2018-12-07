//
//  QYDataService.h
//  QYPhotoAlbum
//
//  Created by yoyo on 2017/9/30.
//  Copyright © 2017年 burning. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "QYPhotoConstant.h"
#import "QYAssetModel.h"
#import "QYGroupModel.h"

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_8_0
#import <Photos/Photos.h>
#endif

@interface QYPhotoService : NSObject

+ (instancetype)shareInstanced;

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_8_0
///**
// 是否具有相册权限
//
// @return YES 已经授权 -- NO 未授权
// */
+ (BOOL)hasAlbumAuthor;

+ (void)requestAuthor:(void (^)(BOOL hasAuthor))block;
/**
 相册权限状态
 
 @return 权限状态枚举
 */
+ (QYAuthorizationStatus)albumPermissonStatues;

/**
 获取 获取用户所有相册列表
 
 @param completion 读取完成之后的回调
 */
- (void)fetchAllGroupsWithcompletion:(fetchAlbumCompletion)completion;

/**
 获取相册用户所有相册列表
 
 @param type 筛选type
 @param completion 完成回调
 */
- (void)fetchAllGroupsWithType:(QYPhotoLibarayAssertType)type completion:(fetchAlbumCompletion)completion;
/**
 获取相机胶卷相册列表对象
 
 @param type  筛选type
 @param completion 读取完成之后的回调
 */
- (void)fetchCameraRollAlbumListWithType:(QYPhotoLibarayAssertType)type completion:(fetchAlbumCompletion)completion;

#pragma mark--图片获取接口
- (PHImageRequestID)requestOriginalImageForAsset:(QYAssetModel *)asset
                                         success:(requestImagSuccessBlock)success
                                         failure:(requestFailBlock)failure
                                   progressBlock:(downloadProgressBlock)progressBlock;

/**
 * @brief 根据传入size获取图片
 */
- (PHImageRequestID)requestImageForAsset:(QYAssetModel *)asset
                                    size:(CGSize)size
                                 success:(requestImagSuccessBlock)success
                                 failure:(requestFailBlock)failure
                           progressBlock:(downloadProgressBlock)progressBlock;

#pragma 导出视频


- (PHImageRequestID)requestVideoWithAsset:(QYAssetModel *)asset
                                  success:(requestVideoSucces)success
                                  failure:(requestFailBlock)failure
                                 progress:(downloadProgressBlock)progressHandler;

- (void)requestVideoWithLivePhoto:(QYAssetModel *)assetModel succes:(requestVideoSucces)success failure:(requestFailBlock) failure;
#pragma mark - 图片和视频的保存

/**
 将本地图片存入系统相册
 
 @param imageUrl 图片路径
 @param cAlbumName 自定义相册
 @param completion 完成回调
 */
- (void)saveImageToAlblm:(NSURL *)imageUrl
         customAlbumName:(NSString *)cAlbumName
              completion:(void (^)(BOOL, QYAssetModel *))completion;

/**
 存入一张图片到系统相册
 
 @param image 图片对象
 @param cAlbumName 自定义相册
 @param completion 完成回调
 */
- (void)saveImageToAblum:(UIImage *)image
         customAlbumName:(NSString *)cAlbumName
              completion:(void (^)(BOOL, QYAssetModel *))completion;

/**
 存入一张图片到相册
 
 @param image 图片对象
 @param completion 完成回调
 */
- (void)saveImageToAblum:(UIImage *)image completion:(void (^)(BOOL, QYAssetModel *))completion;

/**
 保存视频到相册
 
 @param url 视频路径
 @param cAlbumName 自定义相册
 @param completion 完成回调
 */
- (void)saveVideoToAblum:(NSURL *)url
         customAlbumName:(NSString *)cAlbumName
              completion:(void (^)(BOOL, QYAssetModel *))completion;

/**
 保存视频到相册
 
 @param url 视频路径
 @param completion 完成回调
 */
- (void)saveVideoToAblum:(NSURL *)url completion:(void (^)(BOOL, QYAssetModel *))completion;

- (void)deleteMedaiWithAsset:(QYAssetModel *)asset
        withCunstomAlubmName:(NSString *)albumName
                  completion:(deleteAssetCompletionBlock)completion;

- (void)deleteMedaiWithAsset:(QYAssetModel *)asset completion:(deleteAssetCompletionBlock)completion;

#pragma mark -取消掉云端请求
- (void)cancelRequestID:(PHImageRequestID)requestId;

#pragma mark - 注册/移除 PHLibrary Observer
- (void)registerObserver:(id<PHPhotoLibraryChangeObserver>)observer;

- (void)removeRegisterObserver:(id<PHPhotoLibraryChangeObserver>)observer;

#endif
@end
