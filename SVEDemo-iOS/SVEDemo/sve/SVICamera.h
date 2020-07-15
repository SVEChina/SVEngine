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

/***********************流入的方式*******************************/
//创建相机(创建一个流相机节点）
/**
   _formate 5 BGRA
   _formate 3 NV12
 */
- (void)createInStream:(NSString*)_name formate:(int)_formate width:(int)_w height:(int)_h angle:(float)_angle OP:(cb_func_op)_cb msg:(NSString*)_msg;

//处理流数据
- (void)pushInStream:(NSString*)_name angle:(float)_angle img:(CVPixelBufferRef)_pixelBuf;
/***************************************************************/

/***********************纹理入的方式*******************************/
//创建相机(创建一个纹理相机节点）
/**
 _formate 5 BGRA
 _formate 3 NV12
 */
- (void)createInTextureStream:(NSString*)_name formate:(int)_formate width:(int)_w height:(int)_h angle:(float)_angle context:(EAGLContext *)_context OP:(cb_func_op)_cb msg:(NSString*)_msg;

//处理纹理数据（内部用纹理采集的方式处理）
- (void)pushInTextureStream:(NSString*)_name angle:(float)_angle img:(CVPixelBufferRef)_pixelBuf;
/***************************************************************/

//删除相机节点
- (void)destroyInstream:(NSString*)_name OP:(cb_func_op)_cb msg:(NSString*)_msg;

//创建输出流节点
//_streamType 向外输出到哪个流
- (void)createOutStream:(NSString*)_name streamType:(int)_streamType width:(int)_w height:(int)_h OP:(cb_func_op)_cb msg:(NSString*)_msg;

//销毁输出流节点
- (void)destroyOutStream:(NSString*)_name OP:(cb_func_op)_cb msg:(NSString*)_msg;

//打开输出流
- (void)openOutStream:(cb_out_stream)_cb Mode:(int)_mode;

//关闭输出流
- (void)closeOutStream;
@end

#endif
