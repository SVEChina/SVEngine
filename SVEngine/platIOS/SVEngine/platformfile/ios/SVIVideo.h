//
//  SVVideo.h
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVInterfaceBase.h"

#if TARGET_OS_IPHONE

#include "VideoToolBox/VideoToolbox.h"

@interface SVIVideo : SVInterfaceBase

//设置模板
- (void)setTemplate:(NSString *)_path OP:(cb_func_op)_cb msg:(NSString*)_msg;
//清理模板
-(void)clearTemplateOP:(cb_func_op)_cb msg:(NSString*)_msg;
//增加视频流
-(void)addVideo:(NSString *)_path Index:(int)_index OP:(cb_func_op)_cb msg:(NSString*)_msg;
//删除视频流
-(void)delVideo:(int)_index OP:(cb_func_op)_cb msg:(NSString*)_msg;
//清空视频流
-(void)clearVideoOP:(cb_func_op)_cb msg:(NSString*)_msg;
//设置背景音乐
-(void)setBGMusic:(NSString *)_path;
//清空背景音乐
-(void)clearBGMusic;
//test 测试合并aac
-(void)testMixAAC:(NSString*)_savename Dir:(NSString*)_dir;
//播放
-(void)playOP:(cb_func_op)_cb msg:(NSString*)_msg;
//停止
-(void)stopOP:(cb_func_op)_cb msg:(NSString*)_msg;
//暂停
-(void)pauseOP:(cb_func_op)_cb msg:(NSString*)_msg;
//移动
-(void)moveMVOP:(float)_percent Callback:(cb_func_op)_cb msg:(NSString*)_msg;
//合成视频SDK
//设置合成输出文件
-(void)mixVideoSetOutFile:(NSString*)_savename Dir:(NSString*)_dir OP:(cb_func_op)_cb msg:(NSString*)_msg;
//设置合成回调
-(void)mixVideoSetCallback:(cb_func_percent)_percentcb OP:(cb_func_op)_cb msg:(NSString*)_msg;
//设置合成分辨率
-(void)mixVideoSetOutSizeW:(int)_w H:(int)_h OP:(cb_func_op)_cb msg:(NSString*)_msg;
//设置开始合成
-(void)mixVideoStartOP:(cb_func_op)_cb msg:(NSString*)_msg;
//清理资源
-(void)mixVideoClearOP:(cb_func_op)_cb msg:(NSString*)_msg;
//保存编辑配置
-(void)saveEditConfig:(NSString*)_name;
//录制视频
-(void)recordVideo:(NSString*)_savename Dir:(NSString*)_dir Callback:(cb_func_record)_cb;

@end

#endif
