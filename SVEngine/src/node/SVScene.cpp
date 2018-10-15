//
// SVScene.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVScene.h"
#include "SVCameraNode.h"
#include "SVNode.h"
#include "SVNodeVisit.h"
#include "../app/SVGlobalMgr.h"
#include "../app/SVGlobalParam.h"
#include "../basesys/SVStaticData.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/renderer/SVRendererBase.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderCmd.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"
#include "../event/SVOpEvent.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVCameraMgr.h"
#include "../basesys/SVConfig.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"

SVTree4::SVTree4(SVInst *_app)
:SVGBase(_app){
    m_treeLock = MakeSharedPtr<SVLock>();
    for(s32 i=0;i<4;i++){
        m_pTreeNode[i] = nullptr;
    }
    m_node = nullptr;
}

SVTree4::~SVTree4(){
    m_node = nullptr;
    for(s32 i=0;i<4;i++){
        m_pTreeNode[i] = nullptr;
    }
    m_treeLock = nullptr;
}

//世界大小和深度
void SVTree4::create(SVBoundBox& _box,s32 _depth){
    m_treeBox = _box;
    //构建场景树
    if(_depth>0){
        FVec3 t_max = _box.getMax();
        FVec3 t_min = _box.getMin();
        FVec3 t_center = (t_max + t_min)*0.5f;
        //0 第一象限
        SVBoundBox t_box0(t_center,t_max);
        m_pTreeNode[0] = MakeSharedPtr<SVTree4>(mApp);
        m_pTreeNode[0]->create(t_box0,_depth-1);
        //1 第二象限
        SVBoundBox t_box1( FVec3(t_min.x,t_center.y,0.0f),FVec3(t_center.x,t_max.y,0.0f) );
        m_pTreeNode[1] = MakeSharedPtr<SVTree4>(mApp);
        m_pTreeNode[1]->create(t_box1,_depth-1);
        //2 第三象限
        SVBoundBox t_box2(t_min,t_center);
        m_pTreeNode[2] = MakeSharedPtr<SVTree4>(mApp);
        m_pTreeNode[2]->create(t_box2,_depth-1);
        //3 第四象限
        SVBoundBox t_box3( FVec3(t_center.x,t_min.y,0.0f),FVec3(t_max.x,t_center.y,0.0f) );
        m_pTreeNode[3] = MakeSharedPtr<SVTree4>(mApp);
        m_pTreeNode[3]->create(t_box3,_depth-1);
    }else{
        //叶子节点
        m_node = MakeSharedPtr<SVNode>(mApp);
    }
}

void SVTree4::destroy(){
    m_treeLock->lock();
    for(s32 i=0;i<4;i++){
        if(m_pTreeNode[i]){
            m_pTreeNode[i]->destroy();
        }
    }
    m_treeLock->unlock();
    clearNode();
}

void SVTree4::update(f32 _dt){
    m_treeLock->lock();
    //本身挂载的节点进行更新
    if(m_node) {
        m_node->deep_update(_dt);
    }else{
        for(s32 i=0;i<4;i++){
            if(m_pTreeNode[i]){
                m_pTreeNode[i]->update(_dt);
            }
        }
    }
    m_treeLock->unlock();
}

void SVTree4::visit(SVVisitorBasePtr _visitor){
    m_treeLock->lock();
    if(m_node) {
        m_node->deep_visit(_visitor);
    }else{
        for(s32 i=0;i<4;i++){
            if(m_pTreeNode[i]){
                m_pTreeNode[i]->visit(_visitor);
            }
        }
    }
    m_treeLock->unlock();
}

bool SVTree4::_isIn(SVNodePtr _node) {
    FVec3 t_pos = _node->getPosition();
    //在内部(以1，2，3，4)现象为顺序
    if( m_treeBox.inside(t_pos) ){
        return true;
    }
    return false;
}

void SVTree4::addNode(SVNodePtr _node, s32 iZOrder){
    if (_node){
        _node->setZOrder(iZOrder);
        addNode(_node);
    }
}

//增加节点
void SVTree4::addNode(SVNodePtr _node) {
    if(m_node) {
        m_node->addChild(_node);
    }else{
        for(s32 i=0;i<4;i++){
            if( m_pTreeNode[i]->_isIn(_node) ){
                m_pTreeNode[i]->addNode(_node);
                break;
            }
        }
    }
}

//移除节点
bool SVTree4::removeNode(SVNodePtr _node) {
    bool t_ret = false;
    if(m_node) {
        t_ret = m_node->removeChild(_node);
    }else{
        for(s32 i=0;i<4;i++){
            if( m_pTreeNode[i]->removeNode(_node) ) {
                return true;
            }
        }
    }
    return t_ret;
}

//清理节点
void SVTree4::clearNode() {
    if(m_node) {
        m_node->clearChild();
    }else{
        for(s32 i=0;i<4;i++){
            m_pTreeNode[i]->clearNode();
        }
    }
}

bool SVTree4::hasNode(SVNodePtr _node) {
    if(m_node) {
        return m_node->hasChild(_node);
    }else{
        for(s32 i=0;i<4;i++){
            if( m_pTreeNode[i]->hasNode(_node) ){
                return true;
            }
        }
    }
    return false;
}

//逻辑场景
SVScene::SVScene(SVInst *_app,cptr8 _name)
:SVGBase(_app) {
    m_name = _name;
    m_color.setColorARGB(0x00000000);
    m_worldW = 0;
    m_worldH = 0;
    m_worldD = 0;
    //场景树
    m_pSceneTree = MakeSharedPtr<SVTree4>(_app);
    //渲染场景
    m_pRenderScene = MakeSharedPtr<SVRenderScene>(_app);
}

SVScene::~SVScene() {
    if(m_pSceneTree){
        m_pSceneTree->destroy();
        m_pSceneTree = nullptr;
    }
    m_pRenderScene = nullptr;
}

void SVScene::create(f32 _worldw ,f32 _worldh,s32 _depth){
    m_worldW = _worldw;
    m_worldH = _worldh;
    m_worldD = _depth;
    if(m_pSceneTree){
        SVBoundBox _box;
        FVec3 t_min,t_max;
        t_min.set(-0.5f*_worldw,-0.5f*_worldh, 0.0f);
        t_max.set(0.5f*_worldw,0.5f*_worldh, 0.0f);
        _box.set(t_min, t_max);
        m_pSceneTree->create(_box,_depth);
    }
    s32 m_sw = mApp->m_pGlobalParam->m_inner_width;
    s32 m_sh = mApp->m_pGlobalParam->m_inner_height;
    SVCameraNodePtr mainCamera = mApp->m_pGlobalMgr->m_pCameraMgr->getMainCamera();
    mainCamera->resetCamera(m_sw,m_sh);
}

void SVScene::destroy(){
}

void SVScene::addNode(SVNodePtr _node){
    if(_node && m_pSceneTree){
        m_pSceneTree->addNode(_node);
    }
}

void SVScene::addNode(SVNodePtr _node,s32 _zorder){
    if(_node && m_pSceneTree){
        _node->setZOrder(_zorder);
        addNode(_node);
    }
}

void SVScene::removeNode(SVNodePtr _node){
    if(_node && m_pSceneTree){
        m_pSceneTree->removeNode(_node);
    }
}

void SVScene::active() {
}

void SVScene::unactive() {
}

void SVScene::setSceneColor(f32 _r,f32 _g,f32 _b,f32 _a) {
    m_color.setColor(_r, _g, _b, _a);
}

void SVScene::update(f32 dt) {
    //遍历场景树
    if(m_pSceneTree){
        m_pSceneTree->update(dt);
    }
    //
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if( t_renderer && t_renderer->hasSVTex(E_TEX_MAIN) ){
        if (m_pRenderScene && false == m_pRenderScene->isSuspend() ) {
            SVRenderCmdFboBindPtr t_fbo_bind = MakeSharedPtr<SVRenderCmdFboBind>(t_renderer->getRenderTexture());
            t_fbo_bind->mTag = "main_frame_bind";
            m_pRenderScene->pushRenderCmd(RST_SCENE_BEGIN, t_fbo_bind);
            //
            SVRenderCmdClearPtr t_clear = MakeSharedPtr<SVRenderCmdClear>();
            t_clear->mTag = "main_frame_clear";
            t_clear->setClearColor(m_color.r, m_color.g, m_color.b, m_color.a);
            m_pRenderScene->pushRenderCmd(RST_SCENE_BEGIN, t_clear);
            //
            SVRenderCmdFboUnbindPtr t_fbo_unbind = MakeSharedPtr<SVRenderCmdFboUnbind>(t_renderer->getRenderTexture());
            t_fbo_unbind->mTag = "main_frame_unbind";
            m_pRenderScene->pushRenderCmd(RST_SCENE_END, t_fbo_unbind);
        }
    }
}

void SVScene::visit(SVVisitorBasePtr _visitor){
    if( m_pSceneTree ){
        m_pSceneTree->visit(_visitor);
    }
}

SVRenderScenePtr SVScene::getRenderRS(){
    return m_pRenderScene;
}

bool SVScene::procEvent(SVEventPtr _event) {
    return true;
}

//序列化场景
void SVScene::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                  RAPIDJSON_NAMESPACE::Value &_objValue) {
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    locationObj.AddMember("name",  RAPIDJSON_NAMESPACE::StringRef(m_name.c_str()), _allocator);
    u32 t_color = m_color.getColorARGB();
    locationObj.AddMember("color", t_color, _allocator);
    locationObj.AddMember("worldw", m_worldW, _allocator);
    locationObj.AddMember("worldh", m_worldH, _allocator);
    locationObj.AddMember("worldd", m_worldD, _allocator);
    //序列化树 ? 要做这么复杂吗
    if(m_pSceneTree){
        
    }
    //
    _objValue.AddMember("SVScene", locationObj, _allocator);
}

void SVScene::fromJSON(RAPIDJSON_NAMESPACE::Value &item) {
    if (item.HasMember("name") && item["name"].IsString()) {
        m_name = item["name"].GetString();
    }
    if (item.HasMember("color") && item["color"].IsUint()) {
        u32 t_color = item["color"].GetUint();
        m_color.setColorARGB(t_color);
    }
    if (item.HasMember("worldw") && item["worldw"].IsFloat()) {
        m_worldW = item["worldw"].GetFloat();
    }
    if (item.HasMember("worldh") && item["worldh"].IsFloat()) {
        m_worldH = item["worldh"].GetFloat();
    }
    if (item.HasMember("worldd") && item["worldd"].IsInt()) {
        m_worldD = item["worldd"].GetInt();
    }
    //
    if(!m_pSceneTree){
        create(m_worldW,m_worldH,m_worldD);
    }
}
