//
//  SVICamera.h
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVInterfaceBase.h"

#if TARGET_OS_IPHONE

#include "VideoToolBox/VideoToolbox.h"

@interface SVICamera : SVInterfaceBase

//创建相机(创建一个相机节点）
- (void)createInStream:(NSString*)_name Type:(int)_type width:(int)_w height:(int)_h OP:(cb_func_op)_cb msg:(NSString*)_msg;

//删除相机节点
- (void)destroyInstream:(NSString*)_name OP:(cb_func_op)_cb msg:(NSString*)_msg;

//处理照片
- (void)pushInStream:(NSString*)_name Img:(CMSampleBufferRef)_samplerBuf;

//处理照片并保存
- (void)pushInStream:(NSString*)_name Img:(CMSampleBufferRef)_samplerBuf Save:(bool)_save;

//创建输出流节点
//StreamType为0的时候正常读取输出流数据，为1时只读取美颜的数据
- (void)createOutStream:(NSString*)_name Type:(int)_type StreamType:(int)_streamType OP:(cb_func_op)_cb msg:(NSString*)_msg;

//销毁输出流节点
- (void)destroyOutStream:(NSString*)_name OP:(cb_func_op)_cb msg:(NSString*)_msg;

//打开输出流
- (void)openOutStream:(cb_out_stream)_cb Mode:(int)_mode;

//关闭输出流
- (void)closeOutStream;

@end

#endif
