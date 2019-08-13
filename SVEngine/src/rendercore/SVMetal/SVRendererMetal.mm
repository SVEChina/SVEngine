//
// SVRendererMetal.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRendererMetal.h"
#include "SVResMetalFbo.h"
#include "SVContextMetal.h"
#include "../../app/SVInst.h"
#include "../../base/SVDataSwap.h"

SVRendererMetal::SVRendererMetal(SVInst* _app)
:SVRendererBase(_app){
    m_pDevice = nullptr;
    m_pCmdQueue = nullptr;
    m_pLibrary = nullptr;
    m_pCurEncoder = nullptr;
}

SVRendererMetal::~SVRendererMetal(){
}

//初始化
void SVRendererMetal::init(void* _device,s32 _w,s32 _h){
    m_pDevice = (__bridge id)_device;//MTLCreateSystemDefaultDevice();
    if (m_pDevice == nil) {
        SV_LOG_INFO("don't support metal !");
    }
    m_pCmdQueue = m_pDevice.newCommandQueue;
    m_pLibrary = [m_pDevice newDefaultLibrary];
    //创建主fbo
    m_inWidth = _w;
    m_inHeight = _h;
    //
    m_pRenderContext = MakeSharedPtr<SVContextMetal>(mApp);
    //创建主纹理
    mApp->m_pGlobalParam->m_inner_width = _w;
    mApp->m_pGlobalParam->m_inner_height = _h;
//    //
//    SVTexturePtr t_tex = createSVTex(E_TEX_MAIN,_w,_h,GL_RGBA);
//    //主FBO
//    m_pRenderTex = MakeSharedPtr<SVResMetalRenderTexture>(mApp,
//                                                  t_tex,
//                                                  true,
//                                                  true);
//    mApp->getRenderMgr()->pushRCmdCreate(m_pRenderTex);
}

//销毁
void SVRendererMetal::destroy(){
}

//重置大小
void SVRendererMetal::resize(s32 _w,s32 _h) {
}

//提交纹理
 void SVRendererMetal::submitTex(u32 _channel,TexUnit& _unit){
     
 }
//提交unifrom matrix
 void SVRendererMetal::submitUniformMatrix(cptr8 _name,f32* _data){
     
 }

//提交unifrom matrix array
void SVRendererMetal::submitUniformMatrixArray(cptr8 _name,f32* _data,s32 _size){
    
}

//提交unifrom i1
 void SVRendererMetal::submitUniformi(cptr8 _name,s32 _data){
     
 }
//提交unifrom i2
 void SVRendererMetal::submitUniformi2(cptr8 _name,s32 _data1,s32 _data2){
     
 }
//提交unifrom i3
 void SVRendererMetal::submitUniformi3(cptr8 _name,s32 _data1,s32 _data2,s32 _data3){
     
 }
//提交unifrom i4
 void SVRendererMetal::submitUniformi4(cptr8 _name,s32 _data1,s32 _data2,s32 _data3,s32 _data4){
     
 }
//提交unifrom f1
 void SVRendererMetal::submitUniformf(cptr8 _name,f32 _data){
     
 }
//提交unifrom f2
 void SVRendererMetal::submitUniformf2(cptr8 _name,f32 _data1,f32 _data2){
     
 }
//提交unifrom f3
 void SVRendererMetal::submitUniformf3(cptr8 _name,f32 _data1,f32 _data2,f32 _data3){
     
 }
//提交unifrom f4
 void SVRendererMetal::submitUniformf4(cptr8 _name,f32 _data1,f32 _data2,f32 _data3,f32 _data4){
     
 }
//提交unifrom s32 v1
 void SVRendererMetal::submitUniformi1v(cptr8 _name,s32* _data,s32 _size){
     
 }
//提交unifrom s32 v2
 void SVRendererMetal::submitUniformi2v(cptr8 _name,s32* _data,s32 _size){
     
 }
//提交unifrom s32 v3
 void SVRendererMetal::submitUniformi3v(cptr8 _name,s32* _data,s32 _size){
     
 }
//提交unifrom s32 v4
 void SVRendererMetal::submitUniformi4v(cptr8 _name,s32* _data,s32 _size){
     
 }
//提交unifrom f32 v1
 void SVRendererMetal::submitUniformf1v(cptr8 _name,f32* _data,s32 _size){
     
 }
//提交unifrom f32 v2
 void SVRendererMetal::submitUniformf2v(cptr8 _name,f32* _data,s32 _size){
     
 }
//提交unifrom f32 v3
 void SVRendererMetal::submitUniformf3v(cptr8 _name,f32* _data,s32 _size){
     
 }
//提交unifrom f32 v4
 void SVRendererMetal::submitUniformf4v(cptr8 _name,f32* _data,s32 _size){
     
 }
//提交融合参数
 void SVRendererMetal::submitBlend(SVBlendParam& _param){
     
 }
//提交模板参数
 void SVRendererMetal::submitStencil(SVStencilParam& _param){
     
 }
//提交深度参数
 void SVRendererMetal::submitDepth(SVDepthParam& _param){
     
 }
//提交隐藏面参数
 void SVRendererMetal::submitCull(SVCullFaceParam& _param){
     
 }
//提交zfighting
 void SVRendererMetal::submitZOff(SVZOffParam& _param){
     
 }
//提交线宽
 void SVRendererMetal::submitLineWidth(f32 _width){
     
 }
//提交点大小
 void SVRendererMetal::submitPointSize(f32 _size){
     
 }
//绑定FBO
 void SVRendererMetal::svBindFrameBuffer(u32 _id){
     
 }
//清理颜色
 void SVRendererMetal::svBindClearColor(u32 _id){
     
 }
//颜色缓存
 void SVRendererMetal::svBindColorBuffer(u32 _id){
     
 }
//顶点缓存
 void SVRendererMetal::svBindVertexBuffer(u32 _id){
     
 }
//索引缓存
 void SVRendererMetal::svBindIndexBuffer(u32 _id){
     
 }
//顶点格式更新
 void SVRendererMetal::svUpdateVertexFormate(VFTYPE _vf,s32 _count,s32 _mode){
     
 }
//视口
void SVRendererMetal::svPushViewPort(u32 _x,u32 _y,u32 _w,u32 _h){
    
}
//退出视口
void SVRendererMetal::svPopViewPort() {
    
}
