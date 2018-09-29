//
//  SWLogicFilter.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWLogicFilter.h"
#import "SWFilter.h"
#import "../swui/SWUISys.h"
#import "../swbasic/SWBasicSys.h"
#import "../swlogic/SWLogicSys.h"
#import "../utility/SWFileUtil.h"
#import "../utility/Utility.h"

#define HOTVIDEOLIST @"HotVideoList"
//热门模块
@interface SWLogicFilter (){
    //滤镜列表
    NSMutableArray* m_FilterList;
    NSMutableArray* m_CurShowHotVideoList;
    NSMutableArray* m_HaveShowedHotVideoList;
    NSTimeInterval  m_cutInterval;
}
@end

@implementation SWLogicFilter

- (void)initModule{
//    NSArray* filter_name_array = [NSArray arrayWithObjects:@"磨皮",
//                                  @"美白",
//                                  @"亮度",
//                                  @"对比度",
//                                  @"饱和度",
//                                  @"颜色",
//                                  @"高光",
//                                  @"阴影",
//                                  @"色温",
//                                  @"曲线",
//                                  @"锐度",
//                                  @"重置",
//                                  @"保存"];
//    NSArray* filter_img_array = [NSArray arrayWithObjects:@"m_mopi",
//                                  @"m_meibai",
//                                  @"m_liangdu",
//                                  @"m_duibidu",
//                                  @"m_baohedu",
//                                  @"m_yanse",
//                                  @"m_gaoguang",
//                                  @"m_yinying",
//                                  @"m_sewen",
//                                  @"m_quxian",
//                                  @"m_ruidu",
//                                  @"m_chongzhi",
//                                  @"m_baocun"];
//    //初始化滤镜列表
//    m_FilterList = [[NSMutableArray alloc] init];
//    if( filter_name_array.count == filter_img_array.count ) {
//        for( int i=0; i<filter_name_array.count; i++ ) {
//            SWFilter* t_filter = [[SWFilter alloc] init];
//            t_filter.m_imageKey = filter_img_array[i];
//            t_filter.m_owner = @"sve";
//            t_filter.m_name = filter_name_array[i];
//            t_filter.m_createdTime = @"0";
//            t_filter.downLoadState = 1;
//            [m_FilterList addObject:t_filter];
//        }
//    }
//    //
}

- (void)destroyModule{
//    if(m_FilterList){
//        [m_FilterList removeAllObjects];
//        m_FilterList = nil;
//    }
}

- (NSMutableArray *)getFilterList {
    return m_FilterList;
}

- (void)addFilter:(SWFilter *)_filter{
    if (_filter) {
//        if (m_FilterList.count > 0) {
//            for (int i=0; i<m_FilterList.count; i++) {
//                SWFilter *t_firstHotVideo = [m_FilterList objectAtIndex:i];
//                if ([t_firstHotVideo.m_createdTime doubleValue] <= [_filter.m_createdTime doubleValue]) {
//                    if (i == 0) {
//                        [m_FilterList insertObject:_filter atIndex:0];
//                    }else{
//                        [m_FilterList insertObject:_filter atIndex:i-1];
//                    }
//                    return;
//                }
//            }
//            [m_FilterList addObject:_filter];
//        }else{
//            [m_FilterList addObject:_filter];
//        }
    }
}

//- (NSArray *)getTurnHotVideoFromWillListWithLimitCount:(int)count{
//    NSMutableArray *resultArray = [[NSMutableArray alloc] initWithCapacity:count];
//    if (m_FilterList.count < count) {
//        [resultArray addObjectsFromArray:m_FilterList];
//    }else{
//        [resultArray addObjectsFromArray:[m_FilterList subarrayWithRange:NSMakeRange(0, count)]];
//    }
//    [m_FilterList removeObjectsInArray:resultArray];
//    [m_HaveShowedHotVideoList addObjectsFromArray:resultArray];
//    return resultArray;
//}
//
////获取视频image key 列表
//- (void)fetchHotVideoWithMaker:(NSString *)maker{
//}
//
//- (bool)hasDownLoadImg:(NSString*)_imgkey{
//    for(int i=0;i<m_FilterList.count;i++){
//        SWFilter* t_hotvideo = [m_FilterList objectAtIndex:i];
//        if([t_hotvideo.m_imageKey isEqualToString:_imgkey] == true){
//            return true;
//        }
//    }
//    for(int i=0;i<m_HaveShowedHotVideoList.count;i++){
//        SWFilter* t_hotvideo = [m_HaveShowedHotVideoList objectAtIndex:i];
//        if([t_hotvideo.m_imageKey isEqualToString:_imgkey] == true){
//            return true;
//        }
//    }
//    return false;
//}
//
//-(SWFilter*)createHotVideo:(NSDictionary*)hotVideoDic{
//    if (!hotVideoDic) {
//        return nil;
//    }
//    SWFilter* t_hotvideo = [[SWFilter alloc] init];
//    [t_hotvideo refreshData:hotVideoDic];
//    return t_hotvideo;
//}
//
//- (void)fetchImageList:(NSNumber*)_page{
//}
//
////获取所有的视频列表
//- (void)fetchAllVideo{
//}
//
////超级接口-上传到热门视频和key
//- (void)superUploadVideo:(NSString*)_videores Image:(NSString*)_imageres GameName:(NSString*)_gamename{
//}
//
////超级接口-清理热门视频
//- (void)superClearVideo:(SWFilter *)hotVideo{
//}
//
//- (void)superClearAllHotVideo{
//}
//
//- (BOOL)superIsExistVideo:(SWFilter *)hotVideo {
//    return NO;
//}
//
////普通接口-下载视频
//- (void)downloadVideo:(SWFilter*)_hotvideo{
//}
//
////下载图片
//-(void)downLoadImg:(SWFilter*)_hotvideo{
//}
//
//- (NSMutableArray *)getAllHotVideoList{
//    NSMutableArray *allHotVideos = [[NSMutableArray alloc] init];
//    [allHotVideos addObjectsFromArray:m_FilterList];
//    [allHotVideos addObjectsFromArray:m_HaveShowedHotVideoList];
//    return allHotVideos;
//}
//
//- (NSMutableArray *)getCurHotVideoList{
//    return m_CurShowHotVideoList;
//}
//
//#pragma mark --- 数据库相关
//- (NSDictionary *)createHotVideoDicWithImageKey:(NSString *)imageKey withDownLoadState:(int)downLoadState{
//    if (imageKey) {
//        NSString *videoKey = [imageKey stringByReplacingOccurrencesOfString:@"fmimg_" withString:@"fmvideo_"];
//        videoKey = [videoKey stringByReplacingOccurrencesOfString:@".png" withString:@".mp4"];
//        NSString *videoPath = [SWFileUtil pathForDocumentsDirectoryWithPath:@"videos"];
//        NSString *imagePath = [SWFileUtil pathForDocumentsDirectoryWithPath:@"images"];
//
//        imagePath = [imagePath stringByAppendingPathComponent:imageKey];
//        videoPath = [videoPath stringByAppendingPathComponent:videoKey];
//        NSDictionary *hotVideoDic = @{
//                                      @"imageKey":imageKey,
//                                      @"videoKey":videoKey,
//                                      @"imagePath":imagePath,
//                                      @"videoPath":videoPath,
//                                      @"downLoadState":[NSNumber numberWithInt:downLoadState]
//                                      };
//        return hotVideoDic;
//    }
//    return nil;
//}
//- (void)db_insertHotVideo:(NSDictionary *)hotVideoDic{
////    if (hotVideoDic) {
////        NSString *imageKey = hotVideoDic[@"imageKey"];
////        [store putObject:hotVideoDic withId:imageKey intoTable:HOTVIDEOLIST];
////    }
//
//}
//
//- (void)db_deleteHotVideo:(NSString *)imageKey{
//
//}
//
//- (NSArray *)db_getAllHotVideoItems{
////    NSArray *items = [store getAllItemsFromTable:HOTVIDEOLIST];
////    return items;
//    return nil;
//}

@end
