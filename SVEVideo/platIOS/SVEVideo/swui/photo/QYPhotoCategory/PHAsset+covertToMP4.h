//
//  PHAsset+LivePhotoCovertToMP4.h
//  NXFramework
//
//  Created by liuming on 2017/11/2.
//

#import <Photos/Photos.h>

@interface PHAsset (LivePhotoCovertToMP4)

/**
 *  获取livePhoto资源的名称
 */
- (NSString *)getOriginalVedioName;
/**
 *  获取livePhoto转成MP4的文件路径
 */
- (NSString *)getSaveLivePhotoMP4Path;
/**
 *  将livePhoto转换成 MP4
 *
 *  @param block 转换回调
 */
- (void)getLivePhotoOfMP4Data:(void (^)(NSData *data, NSString *filePath, UIImage *coverImage,NSError * error))block;

/**
 *  获取livePhoto数据
 */
- (void)getLivePhotoData:(void (^)(PHLivePhoto *data))block;
/**
 *  验证 当前 PHAssert能否导出视频
 *
 *  @return 验证结果
 */
- (BOOL)canOutputVideo;

/**
 *  检测是否存在视频资源文件
 */
- (BOOL)checkHasVideoSource;

/**
 *  这个方法只能导出 video 不能导出 livePhoto
 *
 */
- (void)exportVideo:(void (^)(NSString *path))result progress:(void (^)(float progress))progress;

@end

