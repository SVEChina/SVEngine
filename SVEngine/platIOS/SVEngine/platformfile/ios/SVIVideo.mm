//
//  SVVideo.m
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVIVideo.h"
#import "SVInst.h"

#if TARGET_OS_IPHONE

@interface SVIVideo()
{
    
}
@end


@implementation SVIVideo

- (instancetype)init{
    self = [super init];
    if (self) {
    }
    return self;
}

//设置模板(由加载线程进行处理)
- (void)setTemplate:(NSString *)_path OP:(cb_func_op)_cb msg:(NSString*)_msg{
//    if(m_pApp->m_pTPool->getLogicThread()){
//        SVOpMVSetTemplate* t_op = new SVOpMVSetTemplate(m_pApp,[_path UTF8String]);
//        t_op->setCallBack(_cb, [_msg UTF8String]);
//        m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//        t_op->release();
//    }
}

//清理模板
-(void)clearTemplateOP:(cb_func_op)_cb msg:(NSString*)_msg{
//    if(m_pApp->m_pTPool->getLogicThread()){
//        SVOpMVClearTemplate* t_op = new SVOpMVClearTemplate(m_pApp);;
//        t_op->setCallBack(_cb, [_msg UTF8String]);
//        m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//        t_op->release();
//    }
}

//增加视频流
-(void)addVideo:(NSString *)_path Index:(int)_index OP:(cb_func_op)_cb msg:(NSString*)_msg{
//    SVOpMVAddVideo* t_op = new SVOpMVAddVideo(m_pApp,[_path UTF8String],_index);
//    t_op->setCallBack(_cb, [_msg UTF8String]);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//    t_op->release();
}

//删除视频流
-(void)delVideo:(int)_index OP:(cb_func_op)_cb msg:(NSString*)_msg{
//    SVOpMVDelVideo* t_op = new SVOpMVDelVideo(m_pApp,_index);
//    t_op->setCallBack(_cb, [_msg UTF8String]);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//    t_op->release();
}

//清空视频流
-(void)clearVideoOP:(cb_func_op)_cb msg:(NSString*)_msg{
//    SVOpMVClearVideo* t_op = new SVOpMVClearVideo(m_pApp);
//    t_op->setCallBack(_cb, [_msg UTF8String]);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//    t_op->release();
}

//设置背景音乐
-(void)setBGMusic:(NSString *)_path{
    
//    SVOpCreateBGMusic* t_op = new SVOpCreateBGMusic(m_pApp,[_path UTF8String],true);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//    t_op->release();
}

//清空背景音乐
-(void)clearBGMusic{
    
//    SVOpClearBGMusic* t_op = new SVOpClearBGMusic(m_pApp);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//    t_op->release();
}

//test 测试合并aac
-(void)testMixAAC:(NSString*)_savename Dir:(NSString*)_dir{
//    //需要创建文件夹和文件
//    NSFileManager *fileManager = [NSFileManager defaultManager];
//    BOOL isDir = false;
//    BOOL isDirExist = [fileManager fileExistsAtPath:_dir isDirectory:&isDir];
//    if(!(isDir == YES && isDirExist == YES)){
//        BOOL t_sucess = [fileManager createDirectoryAtPath:_dir withIntermediateDirectories:YES attributes:nil error:nil];
//        if(t_sucess){
//            NSLog(@"create dic sucess!");
//        }
//    }
//    //创建aac文件
//    NSString *fileFullName = [_dir stringByAppendingPathComponent:_savename];
//    NSString *fileAAC = [fileFullName stringByAppendingString:@".aac"];
//    [fileManager createFileAtPath:fileAAC contents:nil attributes:nil];
//    //
//    SVOpMixAcc* t_op = new SVOpMixAcc(m_pApp,[fileAAC UTF8String] );
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//    t_op->release();
}

//录制视频
-(void)recordVideo:(NSString*)_savename Dir:(NSString*)_dir Callback:(cb_func_record)_cb{
    
//    [m_pQuene lock];
//    //需要创建文件夹和文件
//    NSFileManager *fileManager = [NSFileManager defaultManager];
//    BOOL isDir = false;
//    BOOL isDirExist = [fileManager fileExistsAtPath:_dir isDirectory:&isDir];
//    if(!(isDir == YES && isDirExist == YES)){
//        BOOL t_sucess = [fileManager createDirectoryAtPath:_dir withIntermediateDirectories:YES attributes:nil error:nil];
//        if(t_sucess){
//            NSLog(@"create dic sucess!");
//        }
//    }
//    //创建h264和mp4两个文件
//    NSString *fileFullName = [_dir stringByAppendingPathComponent:_savename];
//    NSString *file264 = [fileFullName stringByAppendingPathComponent:@".h264"];
//    NSString *fileMP4 = [fileFullName stringByAppendingPathComponent:@".mp4"];
//    [fileManager createFileAtPath:file264 contents:nil attributes:nil];
//    [fileManager createFileAtPath:fileMP4 contents:nil attributes:nil];
//
//    //封装一个OP出去
//    SVOpMVRecord* t_op = new SVOpMVRecord(m_pApp,[fileFullName UTF8String],_cb);
//    NSString* t_opinfo0 = @"op_saveVideo";
//    t_op->setCallBack(callback_op, [t_opinfo0 UTF8String]);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//    t_op->release();
//    [m_pQuene unlock];
}

//播放
-(void)playOP:(cb_func_op)_cb msg:(NSString*)_msg{
//
//    SVOpMVPlay* t_op = new SVOpMVPlay(m_pApp);
//    t_op->setCallBack(_cb, [_msg UTF8String]);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//    t_op->release();
}

//停止
-(void)stopOP:(cb_func_op)_cb msg:(NSString*)_msg{
    
//    SVOpMVStop* t_op = new SVOpMVStop(m_pApp);
//    t_op->setCallBack(_cb, [_msg UTF8String]);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//    t_op->release();
}
//移动视频
-(void)moveMVOP:(float)_percent Callback:(cb_func_op)_cb msg:(NSString*)_msg{
//    SVOpMVMove* t_op = new SVOpMVMove(m_pApp,_percent);
//    t_op->setCallBack(_cb, [_msg UTF8String]);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//    t_op->release();
}

-(void)pauseOP:(cb_func_op)_cb msg:(NSString*)_msg{
//    SVOpMVPause* t_op = new SVOpMVPause(m_pApp);
//    t_op->setCallBack(_cb, [_msg UTF8String]);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//    t_op->release();
}
//设置合成输出文件
-(void)mixVideoSetOutFile:(NSString*)_savename Dir:(NSString*)_dir OP:(cb_func_op)_cb msg:(NSString*)_msg{
//
//    [m_pQuene lock];
//    //需要创建文件夹和文件
//    NSFileManager *fileManager = [NSFileManager defaultManager];
//    BOOL isDir = false;
//    BOOL isDirExist = [fileManager fileExistsAtPath:_dir isDirectory:&isDir];
//    if(!(isDir == YES && isDirExist == YES)){
//        BOOL t_sucess = [fileManager createDirectoryAtPath:_dir withIntermediateDirectories:YES attributes:nil error:nil];
//        if(t_sucess){
//            NSLog(@"create dic sucess!");
//        }
//    }
//    //创建h264和mp4两个文件
//    NSString *fileFullName = [_dir stringByAppendingPathComponent:_savename];
//    NSString *file264 = [fileFullName stringByAppendingString:@".h264"];
//    NSString *fileMP4 = [fileFullName stringByAppendingString:@".mp4"];
//    NSString *fileAAC = [fileFullName stringByAppendingString:@".aac"];
//    [fileManager createFileAtPath:file264 contents:nil attributes:nil];
//    [fileManager createFileAtPath:fileMP4 contents:nil attributes:nil];
//    [fileManager createFileAtPath:fileAAC contents:nil attributes:nil];
//    //
//    //设置视频合成文件
//    SVOpSetVideoMixFile* t_op1 = new SVOpSetVideoMixFile(m_pApp,[fileFullName UTF8String]);
//    NSString* t_opinfo1 = @"op_vm_file";
//    t_op1->setCallBack(callback_op, [t_opinfo1 UTF8String]);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op1);
//    t_op1->release();
//
//    [m_pQuene unlock];
}
//设置合成回调
-(void)mixVideoSetCallback:(cb_func_percent)_percentcb OP:(cb_func_op)_cb msg:(NSString*)_msg{
//
//    [m_pQuene lock];
//    //设置视频合成回调
//    SVOpSetVideoMixCallback* t_op3 = new SVOpSetVideoMixCallback(m_pApp,_percentcb);
//    NSString* t_opinfo3 = @"op_vm_output";
//    t_op3->setCallBack(callback_op, [t_opinfo3 UTF8String]);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op3);
//    t_op3->release();
//    [m_pQuene unlock];
    
}
//设置合成分辨率
-(void)mixVideoSetOutSizeW:(int)_w H:(int)_h OP:(cb_func_op)_cb msg:(NSString*)_msg{
//    [m_pQuene lock];
//    //设置视频合成输出
//    SVOpSetVideoMixOutput* t_op2 = new SVOpSetVideoMixOutput(m_pApp,720,1280);
//    NSString* t_opinfo2 = @"op_vm_output";
//    t_op2->setCallBack(callback_op, [t_opinfo2 UTF8String]);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op2);
//    t_op2->release();
//    [m_pQuene unlock];
}
//设置开始合成
-(void)mixVideoStartOP:(cb_func_op)_cb msg:(NSString*)_msg{
//    if(m_pApp->getSVMode() != SV_ST_MIX){
//        //请先设置合成模式
//        return;
//    }
//    [m_pQuene lock];
//
//    SVOpStartVideoMix* t_op2 = new SVOpStartVideoMix(m_pApp);
//    NSString* t_opinfo2 = @"op_vm_startmix";
//    t_op2->setCallBack(callback_op, [t_opinfo2 UTF8String]);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op2);
//    t_op2->release();
//
//    [m_pQuene unlock];
}

//清理资源
-(void)mixVideoClearOP:(cb_func_op)_cb msg:(NSString*)_msg{
//    [m_pQuene lock];
//    SVOpClearVideoMix* t_op2 = new SVOpClearVideoMix(m_pApp);
//    NSString* t_opinfo2 = @"op_vm_clearmix";
//    t_op2->setCallBack(callback_op, [t_opinfo2 UTF8String]);
//    m_pApp->m_pTPool->getMainThread()->pushThreadOp(t_op2);
//    t_op2->release();
//
//    [m_pQuene unlock];
}

//保存编辑配置
-(void)saveEditConfig:(NSString*)_name{
}


@end

#endif
