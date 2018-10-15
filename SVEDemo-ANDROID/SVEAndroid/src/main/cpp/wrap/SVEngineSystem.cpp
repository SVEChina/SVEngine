//
// Created by 徐子昱 on 2018/6/28.
//


#include "SVEngineSystem.h"
#include "filter/SVWatermark.h"
#include "operate/SVOpRender.h"
#include "operate/SVOpCreate.h"
#include "operate/SVOpDestroy.h"
#include "work/SVThreadPool.h"
#include "work/SVThreadMain.h"
#include "file/SVFileMgr.h"
#include "operate/SVOpOutCamera.h"
#include "operate/SVOpFilter.h"
#include "basesys/SVBasicSys.h"
#include "basesys/SVStreamIn.h"
#include "basesys/SVStreamOut.h"
#include "event/SVOpEvent.h"
#include "event/SVEventMgr.h"
#include "filter/SVOpFaceBeautyExt.h"



SVEngineSystem::SVEngineSystem(){

}
SVEngineSystem::~SVEngineSystem(){
    delete m_pApp;
}
//初始化引擎
void SVEngineSystem::initEngine(){
    m_pApp=new SVInst();
    SVInst* t_app = (SVInst*)m_pApp;
    if(t_app){
        t_app->init();
    }
}
//配置surface windows
void SVEngineSystem::setWindows(ANativeWindow *_windows) {
    m_pWindows=_windows;
}

//开启引擎
void SVEngineSystem::startEngine(){
    SVInst* t_app = (SVInst*)m_pApp;
    if(t_app){
        t_app->startSVE();
    }
}
//关闭引擎
void SVEngineSystem::stopEngine(){
    SVInst* t_app = (SVInst*)m_pApp;
    if(t_app){
        t_app->stopSVE();
    }
}
//挂起
void SVEngineSystem::suspend(){
    SVInst* t_app = (SVInst*)m_pApp;
    if(t_app){
        t_app->svSuspend();
    }
}
//唤醒
void SVEngineSystem::resume(){
    SVInst* t_app = (SVInst*)m_pApp;
    if(t_app){
        t_app->svResume();
    }
}

void SVEngineSystem::createInStream(SVString _name,int _format,int _type,int _w,int _h,int _angle){
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpCreateYUVInstreamPtr t_op=MakeSharedPtr<SVOpCreateYUVInstream>(t_app,_name.c_str(),_format,_w,_h,_angle);
    t_op->setCallBack(nullptr, "");
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

void SVEngineSystem::createOutTexture(SVString _name,int _format,int _type,int _w,int _h,int _angle){
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpCreateTextureCameraPtr t_op=MakeSharedPtr<SVOpCreateTextureCamera>(t_app,_format,_w,_h,_angle);
    t_op->setCallBack(nullptr, "");
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

void SVEngineSystem::createWatermark(void* rgba,int  width, int height){
    if(m_pApp){
        m_watermark=MakeSharedPtr<SVWatermark>(m_pApp);
        m_watermark->createNode(rgba,width,height);
    }
}

void SVEngineSystem::destoryInStream(SVString _name){
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpDestroyYUVInstreamPtr t_op=MakeSharedPtr<SVOpDestroyYUVInstream>(t_app,_name.c_str());
    t_op->setCallBack(nullptr, "");
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);

}
void SVEngineSystem::destoryOutstream(SVString _name){
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpDestroyTextureOutstreamPtr t_op=MakeSharedPtr<SVOpDestroyTextureOutstream>(t_app,_name.c_str());
    t_op->setCallBack(nullptr, "");
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}
int SVEngineSystem::getTexId(){
    SVInst* t_app = (SVInst*)m_pApp;
    SVStreamOutPtr t_streamOut = t_app->m_pGlobalMgr->m_pBasicSys->getStreamOut();
    if(t_streamOut){
        return  t_streamOut->getTexId();
    }

    return 0;
}

void SVEngineSystem::pushStreamData(SVString _name,char* _srcPtr,int _w,int _h,int _pixelFormat,int _angle){
    SVInst* t_app = (SVInst*)m_pApp;
    SVStreamInPtr t_streamIn = t_app->m_pGlobalMgr->m_pBasicSys->getStreamIn();
    if(t_streamIn){
        t_streamIn->pushStreamData(_name.c_str(),(u8*)_srcPtr, _w, _h,_pixelFormat,_angle);
    }
}

void SVEngineSystem::addResPath(SVString _path) {
    m_path=_path;
    SVInst* t_app = (SVInst*)m_pApp;
    if(t_app){
        t_app->m_pGlobalMgr->m_pFileMgr->addRespath(m_path.c_str());
    }
}

//操作回调
void create_renderer_success(cptr8 _info){

}

//创建渲染器
void SVEngineSystem::createRenderGL(int glVer,EGLContext  _context,int _w,int _h){
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpCreateRenderderPtr t_op = MakeSharedPtr<SVOpCreateRenderder>(t_app);
   // t_op->setCallBack(create_renderer_success,"create_renderergl");
    t_op->setGLParam(glVer,_context,(void*)m_pWindows,_w,_h);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}
//销毁渲染器
void SVEngineSystem::destoryRenderGL(){
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpDestroyRenderderPtr t_op = MakeSharedPtr<SVOpDestroyRenderder>(t_app);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}
//创建渲染环境
void SVEngineSystem::createRenderTarget(int _fboid,int _colorid,int _w,int _h){
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpSetRenderTargetPtr t_op = MakeSharedPtr<SVOpSetRenderTarget>(t_app);
    t_op->setTargetParam(_w,_h,_fboid,_colorid,true);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}
//渲染到纹理
void SVEngineSystem::createRenderTextureTarget(int _texId, int _w, int _h) {
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpSetRenderTextureTargetPtr t_op = MakeSharedPtr<SVOpSetRenderTextureTarget>(t_app);
    t_op->setTargetParam(_w,_h,_texId,true);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}
//开启美颜
void SVEngineSystem::openFaceBeauty(int  _lows) {
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpFaceBeautyExtPtr t_op = MakeSharedPtr< SVOpFaceBeautyExt>(t_app,"","",_lows);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}
void SVEngineSystem::updateFilter(int type, int smooth) {
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpUpdateFilterSmoothPtr t_op = MakeSharedPtr<SVOpUpdateFilterSmooth>(t_app,smooth,type);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

//销毁渲染环境
void SVEngineSystem::destoryRenderTarget(){
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpDestroyRenderTargetPtr t_op = MakeSharedPtr<SVOpDestroyRenderTarget>(t_app);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

//创建场景
void SVEngineSystem::createScene(cb_func_op _cb,char* _msg){
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpCreateScenePtr t_op = MakeSharedPtr<SVOpCreateScene>(t_app,"showScene");
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}
//销毁场景
void SVEngineSystem::destoryScene(){
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpDestroyScenePtr t_op = MakeSharedPtr<SVOpDestroyScene>(t_app,"showScene");
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

void SVEngineSystem::touchPos(float _x, float _y) {
    SVInst* t_app = (SVInst*)m_pApp;
    SVTouchEventPtr t_touch = MakeSharedPtr<SVTouchEvent>();
    t_touch->eventType = EVN_T_TOUCH_BEGIN;
    t_touch->x = _x;
    t_touch->y = _y;
    t_app->getEventMgr()->pushEvent(t_touch);
}

