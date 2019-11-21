//
// SVFrameAniNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFrameAniNode.h"
#include "SVCameraNode.h"
#include "SVScene.h"
#include "../mtl/SVMtlCore.h"
#include "../mtl/SVMtl2D.h"
#include "../mtl/SVTexMgr.h"
#include "../core/SVGeoGen.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRenderMgr.h"

SVFrameAniNode::SVFrameAniNode(SVInst *_app)
:SVNode(_app){
    ntype = "SVFrameAniNode";
    m_rsType = RST_SOLID_3D;
    m_state = tANI_STATE_WAIT;
    m_canSelect = false;
    m_visible = false;
    m_accTime = 0.0f;
    m_totalTime = 90.0f;
    m_frameRate = 15.0f;
    m_framecount = 0;
    m_texprename = "";
    m_pActTex = nullptr;
    m_pMesh = nullptr;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_width = 850;
    m_height = 1000;
    setSize(m_width,m_height);
    m_loop = true;
    m_p_cb_obj = nullptr;
    m_frameani_callback = nullptr;
}

SVFrameAniNode::~SVFrameAniNode() {
    m_pActTex = nullptr;
    m_pMesh = nullptr;
    m_pRenderObj = nullptr;
}

//
void SVFrameAniNode::setSize(f32 _w,f32 _h) {
    m_width = _w;
    m_height = _h;
    m_pMesh = SVGeoGen::genRect(mApp, m_width, m_height, 0, 0, m_width, m_height,m_aabbBox);
}

f32 SVFrameAniNode::getWidth(){
    return m_width;
}

f32 SVFrameAniNode::getHeight(){
    return m_height;
}

void SVFrameAniNode::update(f32 dt) {
    SVNode::update(dt);
    if (!m_visible) {
        return;
    }
    if (m_state == tANI_STATE_STOP) {
        return;
    }
    //时间更新
    if (m_state == tANI_STATE_PLAY) {
        m_accTime += dt;
    }
    if(m_accTime>m_totalTime) {
        _complete();
        if(m_loop) {
            m_accTime = 0.0f;
        }else{
            m_accTime = m_totalTime;
            stop();
        }
    }
    //预先加载
    _preload();
    //根据时间计算激活纹理
    m_pActTex = _selectTex(m_accTime);
    if(!m_pActTex) {
        m_pActTex = mApp->getTexMgr()->getSVETexture();
    }
    //卸载
    _unload();
    //更新材质
    if (m_pRenderObj && m_pMesh ) {
        //创建新的材质
        SVMtl2DPtr t_mtl = MakeSharedPtr<SVMtl2D>(mApp, "normal2d_c");
        t_mtl->setModelMatrix(m_absolutMat.get());
        t_mtl->setTexcoordFlip(1.0f, -1.0f);
        t_mtl->setBlendEnable(true);
        t_mtl->setBlendState(MTL_BLEND_ONE, MTL_BLEND_ONE_MINUS_SRC_ALPHA);
        t_mtl->setAlpha(1.0f);
        t_mtl->setTexture(0,m_pActTex);
        t_mtl->update(dt);
        m_pRenderObj->setMesh(m_pMesh);
        m_pRenderObj->setMtl(t_mtl);
        m_pMtl = t_mtl;
    }
}

void SVFrameAniNode::render() {
    if (m_state == tANI_STATE_STOP) {
        return;
    }
    if (m_visible && m_pRenderObj ){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        m_pRenderObj->pushCmd(t_rs, m_rsType, "SVFrameAniNode");
    }
    SVNode::render();
}

//播放控制
void SVFrameAniNode::play() {
    if (m_state != tANI_STATE_PLAY) {
        m_state = tANI_STATE_PLAY;
        m_visible = true;
        if(m_frameani_callback){
            (*m_frameani_callback)(THIS_TO_SHAREPTR(SVFrameAniNode),m_p_cb_obj,1);
        }
    }
}

void SVFrameAniNode::pause() {
    if (m_state != tANI_STATE_PAUSE) {
        m_state = tANI_STATE_PAUSE;
    }
}

void SVFrameAniNode::stop() {
    if (m_state != tANI_STATE_STOP) {
        m_state = tANI_STATE_STOP;
        m_visible = false;
        if(m_frameani_callback){
            (*m_frameani_callback)(THIS_TO_SHAREPTR(SVFrameAniNode),m_p_cb_obj,3);
        }
    }
}

void SVFrameAniNode::_complete(){
    if(m_frameani_callback){
        (*m_frameani_callback)(THIS_TO_SHAREPTR(SVFrameAniNode),m_p_cb_obj,2);
    }
}

SVTexturePtr SVFrameAniNode::_selectTex(f32 _time) {
    //二分法查目标
    s32 t_ct =m_framePool.size();
    if(t_ct<0)
        return nullptr;
    m_curFrame = s32(_time*m_frameRate);
    if(m_curFrame>=t_ct)
        return nullptr;
     m_framePool[m_curFrame].m_pTex = mApp->getTexMgr()->getTextureSync( m_framePool[m_curFrame].m_pTexName.c_str(),true,true);
//    for(s32 i=0;i<2;i++) {
//        s32 t_aim_index = (m_curFrame + i)%t_ct;
//        m_framePool[t_aim_index].m_pTex = mApp->getTexMgr()->getTextureSync( m_framePool[t_aim_index].m_pTexName.c_str(),true,true);
//    }
    return m_framePool[m_curFrame].m_pTex;
}

void SVFrameAniNode::_preload() {
    //预加载
//    s32 m_curFrame; //当前帧
//    s32 m_aimFrame; //目标帧
//    s32 m_preFrame; //预先帧
}

void SVFrameAniNode::_unload() {
    //卸载当前帧就可以，用一帧，卸一帧
    if(m_curFrame>=0 && m_curFrame<m_framePool.size()) {
        m_framePool[m_curFrame].m_pTex = nullptr;
    }
}

void SVFrameAniNode::pushFrame(cptr8 _texname) {
    FrameTex t_ftex;
    t_ftex.m_pTexName = _texname;
    t_ftex.m_pTex = nullptr;
    m_framePool.append(t_ftex);
}

void SVFrameAniNode::clearFrame() {
    for(s32 i=0;i<m_framePool.size();i++) {
        m_framePool[i].m_pTex = nullptr;
    }
    m_framePool.destroy();
}

void SVFrameAniNode::setCallback(sv_frameani_callback _cb,void* _obj){
    m_frameani_callback = _cb;
    m_p_cb_obj = _obj;
}

void SVFrameAniNode::setTexPrename(cptr8 _prename){
    m_texprename = _prename;
}

cptr8 SVFrameAniNode::getTexPrename(){
    return m_texprename.c_str();
}

void SVFrameAniNode::setTotalTime(f32 _time){
    m_totalTime = _time;
    if (m_framecount > 0 && m_totalTime > 0) {
        m_frameRate = m_framecount/m_totalTime;
    }
}

f32  SVFrameAniNode::getTotlaTime(){
    return m_totalTime;
}

void SVFrameAniNode::setFrameCount(s32 _count){
    m_framecount = _count;
    if (m_framecount > 0 && m_totalTime > 0) {
        m_frameRate = m_framecount/m_totalTime;
    }
}

s32  SVFrameAniNode::getFrameCount(){
    return m_framecount;
}

void SVFrameAniNode::setLoop(bool _loop){
    m_loop = _loop;
}

bool SVFrameAniNode::getLoop(){
    return m_loop;
}

//序列化
void SVFrameAniNode::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    _toJsonData(_allocator, locationObj);
    locationObj.AddMember("width", m_width, _allocator);
    locationObj.AddMember("height", m_height, _allocator);
    locationObj.AddMember("loop", m_loop, _allocator);
    locationObj.AddMember("time", m_totalTime, _allocator);
    locationObj.AddMember("count", m_framecount, _allocator);
    locationObj.AddMember("prename", RAPIDJSON_NAMESPACE::StringRef(m_texprename.c_str()), _allocator);
    _objValue.AddMember("SVFrameAniNode", locationObj, _allocator);
}

void SVFrameAniNode::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    _fromJsonData(item);
    f32 t_width = 100.0f;
    if (item.HasMember("width") && item["width"].IsFloat()) {
        t_width = item["width"].GetFloat();
    }
    f32 t_height = 100.0f;
    if (item.HasMember("height") && item["height"].IsFloat()) {
        t_height = item["height"].GetFloat();
    }
    setSize(t_width, t_height);
    //
    bool t_loop = false;
    if (item.HasMember("loop") && item["loop"].IsBool()) {
        t_loop = item["loop"].GetBool();
    }
    setLoop(t_loop);
    //
    f32 t_time = 90.0f;
    if (item.HasMember("time") && item["time"].IsFloat()) {
         t_time = item["time"].GetFloat();
    }
    setTotalTime(t_time);
    //
    s32 t_count = 0;
    if (item.HasMember("count") && item["count"].IsInt()) {
        t_count = item["count"].GetInt();
    }
    setFrameCount(t_count);
    //
    SVString t_prename = "";
    if (item.HasMember("prename") && item["prename"].IsString()) {
        t_prename = item["prename"].GetString();
    }
    setTexPrename(t_prename.c_str());
    //
    for (s32 i = 0; i<m_framecount; i++) {
        SVString t_name = SVString::format("%s%d.png",m_texprename.c_str(), i);
        t_name = m_rootPath + t_name;
        pushFrame(t_name.c_str());
    }
    m_dirty = true;
}
