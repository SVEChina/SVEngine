//
//  SWBasicSys.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWBasicSys.h"
#import "../utility/SWFileUtil.h"
#import "../utility/Utility.h"

static SWBasicSys *mInst;

@interface SWBasicSys (){
}
@end

@implementation SWBasicSys

+(instancetype) getInst{
    if(mInst == nil){
        mInst = [SWBasicSys new];
    }
    return mInst;
}

- (void)initSys{
    //创建存储视频的文件夹路径
    NSString *videosDirectory = [SWFileUtil pathForDocumentsDirectoryWithPath:@"videos"];
    if (![SWFileUtil existsItemAtPath:videosDirectory]) {
        BOOL result = [SWFileUtil createDirectoriesForFileAtPath:videosDirectory];
        if (!result) {
            NSLog(@"create ../videos failed");
        }
    }
    //创建图片存储文件夹
    NSString *imagesDirectory = [SWFileUtil pathForDocumentsDirectoryWithPath:@"images"];
    if (![SWFileUtil existsItemAtPath:imagesDirectory]) {
        BOOL result = [SWFileUtil createDirectoriesForFileAtPath:imagesDirectory];
        if (!result) {
            NSLog(@"create ../images failed");
        }
    }
    //创建保存用户信息的文件夹
    NSString *userInfoDirectory = [SWFileUtil pathForLibraryDirectoryWithPath:@"userinfo"];
    if (![SWFileUtil existsItemAtPath:userInfoDirectory]) {
        BOOL result = [SWFileUtil createDirectoriesForPath:userInfoDirectory];
        if (!result) {
            NSLog(@"create ../userinfo failed");
        }
    }
    //LOGO
    if (![[NSUserDefaults standardUserDefaults] objectForKey:@"marklogo"]) {
        [[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithBool:YES] forKey:@"marklogo"];
        [[NSUserDefaults standardUserDefaults] synchronize];
    }
    //数据来源
    self.m_pDataSrc = [SWDataSource createDataSrcType:DATASRCCAMERA];
    [self.m_pDataSrc initModule];
    
    self.m_inStream = [SWInStream createInStream:SWSTREAMSAMPLE];
    [self.m_inStream initModule];
    
    self.m_outStream = [SWOutStream createOutStream:SWSTREAMSAMPLE];
    [self.m_outStream initModule];
    
    self.m_detection = [SWDetection createDetectionEngine:SWDETECTSENSETIME];
    [self.m_detection initModule];
    
    self.m_pFMMotion = [[SWMotionManager alloc] init];
    [self.m_pFMMotion initModule];
    [self.m_pFMMotion startSensor];

}

- (void)destroySys{
    [self.m_pFMMotion destroyModule];
    self.m_pFMMotion = nil;
}


@end
