//
//  SWFilter.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWFilter.h"
#import "../swui/SWUISys.h"
#import "../swbasic/SWBasicSys.h"
#import "../swlogic/SWLogicSys.h"
#import "../utility/SWFileUtil.h"
//热门视频
@interface SWFilter(){
}

@end

@implementation SWFilter
- (void)refreshData:(NSDictionary *)dic{
    if (dic) {
//        _m_imageKey = dic[@"imageKey"];
//        _m_videoKey = dic[@"videoKey"];
//        NSString *imagePath = [SWFileUtil pathForDocumentsDirectoryWithPath:@"images"];
//        imagePath = [imagePath stringByAppendingPathComponent:_m_imageKey];
//        _m_imagePath = imagePath;
//        NSString *videoPath = [SWFileUtil pathForDocumentsDirectoryWithPath:@"videos"];
//        videoPath = [videoPath stringByAppendingPathComponent:_m_videoKey];
//        _m_videoPath = videoPath;
//        NSArray *components = [_m_imageKey componentsSeparatedByString:@"_"];
//        if (components.count > 2) {
//            _m_createdTime= components[1];
//        }
//        _downLoadState = [dic[@"downLoadState"] intValue];
    }
}
- (NSDictionary *)convertToDic{
//    NSDictionary *mutableDic = @{
//                                 @"imageKey":_m_imageKey,
//                                 @"videoKey":_m_videoKey,
//                                 @"imagePath":_m_imagePath,
//                                 @"videoPath":_m_videoPath,
//                                 @"downLoadState":[NSNumber numberWithInt:_downLoadState]
//                                 };
//    return mutableDic;
    return nil;
}
@end


