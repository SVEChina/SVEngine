//
// SVNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVNode.h"
#include "SVNodeVisit.h"
#include "SVScene.h"
#include "SVCameraNode.h"
#include "../mtl/SVMtlCore.h"
#include "../mtl/SVMtlNocolor.h"
#include "../act/SVActBase.h"
#include "../event/SVEventMgr.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVConfig.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMgr.h"
//
SVNode::SVNode(SVInst *_app)
:SVEventProc(_app) {
    ntype = "SVNode";
    uid = mApp->m_IDPool.applyUID();
    m_name = "";
    m_rsType = RST_DEBUG;
    m_canSelect = false;
    m_beSelect = false;
    m_canProcEvent = false;
    m_visible = true;
    m_dirty = false;
    m_parent = nullptr;             //父节点
    m_iZOrder = 0;
    m_bindIndex = -1;
    m_personID = 1;
    m_enableMipMap = false;
    //基础属性
    m_postion.set(0.0f, 0.0f, 0.0f);
    m_offpos.set(0.0f, 0.0f, 0.0f);
    m_rotation.set(0.0f, 0.0f, 0.0f);
    m_scale.set(1.0f, 1.0f, 1.0f);
    m_bindOffset.set(0.0f, 0.0f, 0.0f);
    m_localMat.setIdentity();
    m_absolutMat.setIdentity();
    m_iabsolutMat.setIdentity();
    //包围盒
    m_drawBox = false;
    m_aabbBox.clear();
    m_aabbBox_sw.clear();
    //
    m_pMtl = nullptr;
}

SVNode::~SVNode() {
    m_parent = nullptr;
    m_pMtl = nullptr;
    mApp->m_IDPool.returnUID(uid);
}

void SVNode::enter(){
   
}

void SVNode::exit(){
}

//做子节点的深度遍历(先子节点 在兄弟节点)
void SVNode::deep_update(f32 dt) {
    //排序
    _sort_child();
    //可见性(属性)判断,优先级最高
    if(m_visible){
        //节点更新
        update(dt);
        //节点剔除(属于渲染优化部分)
        if( !_clip() ) {
            //节点渲染
            render();
            //子节点遍历
            //m_childNodePool.stable_sort_object();
            for (s32 i = 0; i < m_childNodePool.size(); i++) {
                m_childNodePool[i]->deep_update(dt);
            }
        }
    }
}

void SVNode::deep_visit(SVVisitorBasePtr _visit) {
    if( _visit && _visit->visit( THIS_TO_SHAREPTR(SVNode) ) ){
        for (s32 i = 0; i < m_childNodePool.size(); i++) {
            m_childNodePool[i]->deep_visit(_visit);
        }
    }
}

void SVNode::update(f32 dt) {
    SV_LOG_INFO("node type %s \n",ntype.c_str());
    //计算相对矩阵(local)
    if (m_dirty) {
        //更新本地矩阵
        m_localMat.setIdentity();
        FMat4 t_mat_scale = FMat4_identity;
        FMat4 t_mat_rotX = FMat4_identity;
        FMat4 t_mat_rotY = FMat4_identity;
        FMat4 t_mat_rotZ = FMat4_identity;
        FMat4 t_mat_trans = FMat4_identity;
        t_mat_scale.setScale(FVec3(m_scale.x,m_scale.y,m_scale.z));
        t_mat_rotX.setRotateX(m_rotation.x);
        t_mat_rotY.setRotateY(m_rotation.y);
        t_mat_rotZ.setRotateZ(m_rotation.z);
        t_mat_trans.setTranslate( FVec3(m_postion.x + m_offpos.x,m_postion.y + m_offpos.y, m_postion.z + m_offpos.z) );
        m_localMat = t_mat_trans*t_mat_rotZ*t_mat_rotY*t_mat_rotX*t_mat_scale;
        m_dirty = false;
    }
    //计算绝对矩阵(world_matrix)
    if (m_parent) {
        m_absolutMat = m_parent->m_absolutMat*m_localMat;
    } else {
        m_absolutMat = m_localMat;
    }
    //更新包围盒
    m_aabbBox_sw = m_aabbBox;
    m_aabbBox_sw.setTransform(m_absolutMat);
}

bool SVNode::_clip() {
    return false;
}

void SVNode::render() {
    if( m_drawBox && mApp->m_pGlobalParam->m_curScene ){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if( t_rs ){
            SVMtlGeo3dPtr t_mtl = MakeSharedPtr<SVMtlGeo3d>(mApp);
            t_mtl->setColor(0.0f, 1.0f, 0.0f, 1.0f);
            FMat4 m_mat_unit = FMat4_identity;
            t_mtl->setModelMatrix( m_mat_unit.get() );
        SVRenderObjInst::pushAABBCmd(t_rs,RST_DEBUG_INNER,m_aabbBox_sw,t_mtl,"SV3DBOX_aabb");
        }
    }
}

void SVNode::setMtl(SVMtlCorePtr _mtl) {
    m_pMtl = _mtl;
}

SVMtlCorePtr SVNode::getMtl(){
    return m_pMtl;
}

void SVNode::addChild(SVNodePtr _node) {
    if (!_node)
        return;
    if (hasChild(_node))
        return;
    m_childNodePool.append(_node);
    _node->m_parent = THIS_TO_SHAREPTR(SVNode);
    m_needsort = true;
}

void SVNode::addChild(SVNodePtr _node, s32 iZOrder){
    if (_node){
        _node->setZOrder(iZOrder);
    }
    addChild(_node);
}

bool SVNode::removeChild(SVNodePtr _node) {
    for (s32 i = 0; i < m_childNodePool.size(); i++) {
        if( m_childNodePool[i]->removeChild(_node) ){
            return true;
        }else{
            if (m_childNodePool[i] == _node) {
                m_childNodePool.removeForce(i);
                _node->m_parent = nullptr;
                _node = nullptr;
                return true;
            }
        }
    }
    return false;
}

bool SVNode::removeFromParent(){
    if(m_parent){
        for (s32 i = 0; i < m_parent->m_childNodePool.size(); i++) {
            if(m_parent->m_childNodePool[i].get() == this ) {
                m_parent->m_childNodePool.removeForce(i);
                m_parent = nullptr;
                return true;
            }
        }
    }
    return false;
}

//递归清理子节点
void SVNode::clearChild() {
    for (s32 i = 0; i < m_childNodePool.size(); i++) {
        m_childNodePool[i]->clearChild();
    }
    m_childNodePool.destroy();
    m_parent = nullptr;
}

bool SVNode::hasChild(SVNodePtr _node) {
    for (s32 i = 0; i < m_childNodePool.size(); i++) {
        if( m_childNodePool[i]->hasChild(_node) ){
            return true;
        }else{
            if (m_childNodePool[i] == _node) {
                return true;
            }
        }
    }
    return false;
}

u32 SVNode::getChildNum() {
    return m_childNodePool.size();
}

SVNodePtr SVNode::getNode(cptr8 _name){
    if(m_name == _name){
        return THIS_TO_SHAREPTR(SVNode);
    }
    for (int i = 0; i < m_childNodePool.size(); i++) {
        SVNodePtr t_node = m_childNodePool[i]->getNode(_name);
        if(t_node){
            return t_node;
        }
    }
    return nullptr;
}

SVNodePtr SVNode::getChild(u32 _index) {
    if(_index>=m_childNodePool.size())
        return nullptr;
    return m_childNodePool[_index];
}

void SVNode::_sort_child() {
    if(!m_needsort)
        return ;
    m_needsort = false;
    s32 t_num = getChildNum();
    s32 t_times = t_num - 1;
    for(s32 i = 0;i<t_times;i++) {
        for(s32 j=0;j<t_times;j++) {
            SVNodePtr t1 = m_childNodePool[j];
            SVNodePtr t2 = m_childNodePool[j+1];
            if(t1->getZOrder()<t2->getZOrder()) {
                SVNodePtr tmp = m_childNodePool[j];
                m_childNodePool[j] = m_childNodePool[j+1];
                m_childNodePool[j+1] = tmp;
            }
        }
        t_times--;
    }
}

//获取本地空间矩阵
FMat4& SVNode::getLocalMat() {
    return m_localMat;
}

FMat4& SVNode::getAbsoluteMat() {
    return m_absolutMat;
}

FMat4& SVNode::getIAbsoluteMat() {
    return m_iabsolutMat;
}

//获取世界矩阵
void SVNode::getWorldMatrix(FMat4& outMat) {
    if (m_parent) {
        m_parent->getWorldMatrix(outMat);
    }
}

void SVNode::setPosition(f32 _x, f32 _y, f32 _z) {
    m_postion.set(_x, _y, _z);
    m_dirty = true;
}

void SVNode::setOffset(f32 _x, f32 _y, f32 _z){
    m_offpos.set(_x, _y, _z);
    m_dirty = true;
}

void SVNode::setRotation(f32 _x, f32 _y, f32 _z) {
    m_rotation.set(_x, _y, _z);
    m_dirty = true;
}

void SVNode::setScale(f32 _x, f32 _y, f32 _z) {
    m_scale.set(_x, _y, _z);
    m_dirty = true;
}

void SVNode::setPosition(FVec3& _pos) {
    m_postion = _pos;
    m_dirty = true;
}

void SVNode::setOffset(FVec3& _pos) {
    m_offpos = _pos;
    m_dirty = true;
}

void SVNode::setRotation(FVec3& _rot) {
    m_rotation = _rot;
    m_dirty = true;
}

void SVNode::setQuat(SVQuat& _quat) {
    m_rotation.x = _quat.getAngle(FVec3(1.0f,0.0f,0.0f));
    m_rotation.y = _quat.getAngle(FVec3(0.0f,1.0f,0.0f));
    m_rotation.z = _quat.getAngle(FVec3(0.0f,0.0f,1.0f));
    m_dirty = true;
}

void SVNode::setScale(FVec3& _scale) {
    m_scale = _scale;
    m_dirty = true;
}

FVec3& SVNode::getPosition() {
    return m_postion;
}

FVec3& SVNode::getOffset() {
    return m_offpos;
}

void SVNode::setBindOffset(FVec3& _offset){
    m_bindOffset = _offset;
    m_dirty = true;
}

void SVNode::setBindOffset(f32 _offsetX, f32 _offsetY, f32 _offsetZ){
    m_bindOffset.set(_offsetX, _offsetY, _offsetZ);
    m_dirty = true;
}

FVec3& SVNode::getBindOffset(){
    return m_bindOffset;
}

void SVNode::enableMipMap(){
    m_enableMipMap = true;
}

void SVNode::disableMipMap(){
    m_enableMipMap = false;
}

FVec3& SVNode::getRotation() {
    return m_rotation;
}

SVQuat SVNode::getQuat() {
    SVQuat t_quat(m_rotation.x,m_rotation.y,m_rotation.z);
    return t_quat;
}

FVec3& SVNode::getScale() {
    return m_scale;
}

SVNodePtr SVNode::getParent() {
    return m_parent;
}

void SVNode::setbeSelect(bool _select){
    m_beSelect = _select;
}

void SVNode::setcanSelect(bool _select){
    m_canSelect = _select;
}

void SVNode::setBindIndex(s32 _index){
    m_bindIndex = _index;
}

s32  SVNode::getBindIndex(){
    return m_bindIndex;
}

void SVNode::setRSType(RENDERSTREAMTYPE _rsType) {
    m_rsType = _rsType;
}

RENDERSTREAMTYPE SVNode::getRSType(){
    return m_rsType;
}

void SVNode::setdrawAABB(bool _drawaabb){
    m_drawBox = _drawaabb;
}

bool SVNode::getDrawAABB(){
    return m_drawBox;
}

void SVNode::setAABB(SVBoundBox& _aabb){
    m_aabbBox = _aabb;
}

SVBoundBox& SVNode::getAABB(){
    return m_aabbBox;
}

SVBoundBox& SVNode::getAABBSW(){
    return m_aabbBox_sw;
}

void SVNode::setZOrder(s32 _zorder){
    m_iZOrder = _zorder;
    if(m_parent){
        m_parent->m_needsort = true;
    }
}

void SVNode::setAlpha(f32 _alpha){
}

void SVNode::setLocalMatrix(FMat4 &_mat){
    m_localMat = _mat;
}

//序列化接口
void SVNode::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                    RAPIDJSON_NAMESPACE::Value &_objValue){
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    _toJsonData(_allocator, locationObj);
    _objValue.AddMember("SVNode", locationObj, _allocator);
}

void SVNode::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    _fromJsonData(item);
}

void SVNode::_toJsonData(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                             RAPIDJSON_NAMESPACE::Value &locationObj){
    locationObj.AddMember("name",  RAPIDJSON_NAMESPACE::StringRef(m_name.c_str()), _allocator);
    locationObj.AddMember("posX", m_postion.x, _allocator);
    locationObj.AddMember("posY", m_postion.y, _allocator);
    locationObj.AddMember("posZ", m_postion.z, _allocator);
    locationObj.AddMember("rotX", m_rotation.x, _allocator);
    locationObj.AddMember("rotY", m_rotation.y, _allocator);
    locationObj.AddMember("rotZ", m_rotation.z, _allocator);
    locationObj.AddMember("scaleX", m_scale.x, _allocator);
    locationObj.AddMember("scaleY", m_scale.y, _allocator);
    locationObj.AddMember("scaleZ", m_scale.z, _allocator);
    locationObj.AddMember("offsetX", m_offpos.x, _allocator);
    locationObj.AddMember("offsetY", m_offpos.y, _allocator);
    //
    locationObj.AddMember("zorder", m_iZOrder, _allocator);
    locationObj.AddMember("renderstream", (s32)m_rsType, _allocator);
    locationObj.AddMember("bind", m_bindIndex, _allocator);
    locationObj.AddMember("person", m_personID, _allocator);
    locationObj.AddMember("bindOffsetX", m_bindOffset.x, _allocator);
    locationObj.AddMember("bindOffsetY", m_bindOffset.y, _allocator);
    locationObj.AddMember("bindOffsetZ", m_bindOffset.z, _allocator);
    //
    locationObj.AddMember("canselect", m_canSelect, _allocator);
    locationObj.AddMember("drawaabb", m_drawBox, _allocator);
    locationObj.AddMember("canprocevent", m_canProcEvent, _allocator);
    locationObj.AddMember("visible", m_visible, _allocator);
    locationObj.AddMember("mipmap", m_enableMipMap, _allocator);
}

void SVNode::_fromJsonData(RAPIDJSON_NAMESPACE::Value &item){
    if (item.HasMember("name") && item["name"].IsString()) {
        m_name = item["name"].GetString();
    }
    if (item.HasMember("posX") && item["posX"].IsFloat()) {
        m_postion.x = item["posX"].GetFloat();
    }
    if (item.HasMember("posY") && item["posY"].IsFloat()) {
        m_postion.y = item["posY"].GetFloat();
    }
    if (item.HasMember("posZ") && item["posZ"].IsFloat()) {
        m_postion.z = item["posZ"].GetFloat();
    }
    if (item.HasMember("rotX") && item["rotX"].IsFloat()) {
        m_rotation.x = item["rotX"].GetFloat();
    }
    if (item.HasMember("rotY") && item["rotY"].IsFloat()) {
        m_rotation.y  = item["rotY"].GetFloat();
    }
    if (item.HasMember("rotZ") && item["rotZ"].IsFloat()) {
        m_rotation.z = item["rotZ"].GetFloat();
    }
    if (item.HasMember("scaleX") && item["scaleX"].IsFloat()) {
        m_scale.x = item["scaleX"].GetFloat();
    }
    if (item.HasMember("scaleY") && item["scaleY"].IsFloat()) {
        m_scale.y = item["scaleY"].GetFloat();
    }
    if (item.HasMember("scaleZ") && item["scaleZ"].IsFloat()) {
        m_scale.z = item["scaleZ"].GetFloat();
    }
    if (item.HasMember("offsetX") && item["offsetX"].IsFloat()) {
        m_offpos.x = item["offsetX"].GetFloat();
    }
    if (item.HasMember("offsetY") && item["offsetY"].IsFloat()) {
        m_offpos.y = item["offsetY"].GetFloat();
    }
    if (item.HasMember("offsetZ") && item["offsetZ"].IsFloat()) {
        m_offpos.z = item["offsetZ"].GetFloat();
    }
    //
    if (item.HasMember("zorder") && item["zorder"].IsInt()) {
        m_iZOrder = item["zorder"].GetInt();
    }
    if (item.HasMember("renderstream") && item["renderstream"].IsInt()) {
        m_rsType = (RENDERSTREAMTYPE)item["renderstream"].GetInt();
    }
    if (item.HasMember("bind") && item["bind"].IsInt()) {
        m_bindIndex  = item["bind"].GetInt();
    }
    if (item.HasMember("person") && item["person"].IsInt()) {
        m_personID  = item["person"].GetInt();
    }
    if (item.HasMember("bindOffsetX") && item["bindOffsetX"].IsFloat()) {
        m_bindOffset.x  = item["bindOffsetX"].GetFloat();
    }
    if (item.HasMember("bindOffsetY") && item["bindOffsetY"].IsFloat()) {
        m_bindOffset.y  = item["bindOffsetY"].GetFloat();
    }
    if (item.HasMember("bindOffsetZ") && item["bindOffsetZ"].IsFloat()) {
        m_bindOffset.z  = item["bindOffsetZ"].GetFloat();
    }
    //
    if (item.HasMember("canselect") && item["canselect"].IsBool()) {
        m_canSelect = item["canselect"].GetBool();
    }
    if (item.HasMember("drawaabb") && item["drawaabb"].IsBool()) {
        m_drawBox = item["drawaabb"].GetBool();
    }
    if (item.HasMember("canprocevent") && item["canprocevent"].IsBool()) {
        m_canProcEvent = item["canprocevent"].GetBool();
    }
    if (item.HasMember("visible") && item["visible"].IsBool()) {
        m_visible = item["visible"].GetBool();
    }
    if (item.HasMember("mipmap") && item["mipmap"].IsBool()) {
        m_enableMipMap = item["mipmap"].GetBool();
    }
    m_dirty = true;
}
