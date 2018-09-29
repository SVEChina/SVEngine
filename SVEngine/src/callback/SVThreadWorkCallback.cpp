//
// SVThreadWorkCallback.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVThreadWorkCallback.h"
#include "../app/SVInst.h"
#include "../base/SVThread.h"
#include "../base/SVSign.h"
#include "../work/SVThreadPool.h"
//空跑模式
void process_wt_null(SVInst* _app,SVThread* _thread,SV_THREAD_STAGE _stage){
}

//停止模式
void process_wt_stop(SVInst* _app,SVThread* _thread,SV_THREAD_STAGE _stage){
}

//预览模式
void process_wt_preview(SVInst* _app,SVThread* _thread,SV_THREAD_STAGE _stage){
}

//合成模式
void process_wt_mix(SVInst* _app,SVThread* _thread,SV_THREAD_STAGE _stage){
}

//相机模式
void process_wt_camera(SVInst* _app,SVThread* _thread,SV_THREAD_STAGE _stage){
}

//录制模式
void process_wt_record(SVInst* _app,SVThread* _thread,SV_THREAD_STAGE _stage){
}

//工作线程回调
void workThreadCallback(SVInst* _app,SVThread* _thread,SV_THREAD_STAGE _stage){
    SV_LOG_INFO("thread back name %s stage %d \n",_thread->getName(),int(_stage) );
}

//线程销毁回调
void workThreadDestroyCallback(SVInst* _app,SVThread* _thread,SV_THREAD_STAGE _stage){
    SV_LOG_INFO("thread destroy: name %s \n",_thread->getName() );
}
