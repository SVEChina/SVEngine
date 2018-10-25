//
// SVDataNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVDataNode.h"
#include "../core/SVSpine.h"
#include "../act/SVActFollow.h"
#include "../act/SVActionSys.h"
#include "../act/SVActionUnit.h"
#include "../node/SVBitFontNode.h"
#include "../node/SVBMFontNode.h"
#include "../core/SVBMFont.h"
#include "../file/SVBMFontLoader.h"
#include "../node/SVSpriteNode.h"
#include "../node/SVFreeTypeNode.h"
#include "../node/SVNode.h"
#include "../node/SVSpineNode.h"
#include "../node/SVBackGroundNode.h"
#include "../node/SV2DFaceMaskSTNode.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVMtlCore.h"
#include "../mtl/SVMtlFace2D.h"
#include "../node/SV2DFaceMaskNode.h"
#include "../base/SVPreDeclare.h"

SVDataNode::SVDataNode(SVInst *_app)
: SVDataBase(_app){
    m_dataType = MODELDATA_NONE;
    m_posX = 0.0f;
    m_posY = 0.0f;
    m_posZ = 0.0f;
    m_scaleX = 1.0f;
    m_scaleY = 1.0f;
    m_scaleZ = 1.0f;
    m_rotX = 0.0f;
    m_rotY = 0.0f;
    m_rotZ = 0.0f;
    m_personID = 1;
    m_bindIndex = -1;
    m_offsetX = 0.0f;
    m_offsetY = 0.0f;
    //渲染流
    m_renderStreamType = RST_ANIMATE;
    //流中z顺序
    m_zorder = 1;
    //是否可被选择
    m_canSelect = true;
    //是否绘制包围盒
    m_drawAABB = false;
}

SVDataNode::~SVDataNode(){
}

void SVDataNode::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    _toJsonData(_allocator, locationObj);
    _objValue.AddMember("sv", locationObj, _allocator);
}

void SVDataNode::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    _fromJsonData(item);
}

void SVDataNode::_toJsonData(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &locationObj){
    locationObj.AddMember("name",  RAPIDJSON_NAMESPACE::StringRef(m_name.c_str()), _allocator);
    locationObj.AddMember("posX", m_posX, _allocator);
    locationObj.AddMember("posY", m_posY, _allocator);
    locationObj.AddMember("posZ", m_posZ, _allocator);
    locationObj.AddMember("rotX", m_rotX, _allocator);
    locationObj.AddMember("rotY", m_rotY, _allocator);
    locationObj.AddMember("rotZ", m_rotZ, _allocator);
    locationObj.AddMember("scaleX", m_scaleX, _allocator);
    locationObj.AddMember("scaleY", m_scaleY, _allocator);
    locationObj.AddMember("scaleZ", m_scaleZ, _allocator);
    locationObj.AddMember("offsetX", m_offsetX, _allocator);
    locationObj.AddMember("offsetY", m_offsetY, _allocator);
    locationObj.AddMember("bind", m_bindIndex, _allocator);
    locationObj.AddMember("zorder", m_zorder, _allocator);
    locationObj.AddMember("renderstream", (s32)m_renderStreamType, _allocator);
    locationObj.AddMember("canselect", m_canSelect, _allocator);
    locationObj.AddMember("drawaabb", m_drawAABB, _allocator);
}

void SVDataNode::_fromJsonData(RAPIDJSON_NAMESPACE::Value &item){
    if (item.HasMember("name") && item["name"].IsString()) {
        m_name = item["name"].GetString();
    }
    if (item.HasMember("bind") && item["bind"].IsInt()) {
        m_bindIndex  = item["bind"].GetInt();
    }
    if (item.HasMember("scaleX") && item["scaleX"].IsFloat()) {
        m_scaleX = item["scaleX"].GetFloat();
    }
    if (item.HasMember("scaleY") && item["scaleY"].IsFloat()) {
        m_scaleY = item["scaleY"].GetFloat();
    }
    if (item.HasMember("scaleZ") && item["scaleZ"].IsFloat()) {
        m_scaleZ = item["scaleZ"].GetFloat();
    }
    if (item.HasMember("offsetX") && item["offsetX"].IsFloat()) {
        m_offsetX = item["offsetX"].GetFloat();
    }
    if (item.HasMember("offsetY") && item["offsetY"].IsFloat()) {
        m_offsetY = item["offsetY"].GetFloat();
    }
    if (item.HasMember("posX") && item["posX"].IsFloat()) {
        m_posX = item["posX"].GetFloat();
    }
    if (item.HasMember("posY") && item["posY"].IsFloat()) {
        m_posY = item["posY"].GetFloat();
    }
    if (item.HasMember("posZ") && item["posZ"].IsFloat()) {
        m_posZ = item["posZ"].GetFloat();
    }
    if (item.HasMember("rotX") && item["rotX"].IsFloat()) {
        m_rotX = item["rotX"].GetFloat();
    }
    if (item.HasMember("rotY") && item["rotY"].IsFloat()) {
        m_rotY = item["rotY"].GetFloat();
    }
    if (item.HasMember("rotZ") && item["rotZ"].IsFloat()) {
        m_rotZ = item["rotZ"].GetFloat();
    }
    if (item.HasMember("zorder") && item["zorder"].IsInt()) {
        m_zorder = item["zorder"].GetInt();
    }
    if (item.HasMember("renderstream") && item["renderstream"].IsInt()) {
        m_renderStreamType = (RENDERSTREAMTYPE)item["renderstream"].GetInt();
    }
    if (item.HasMember("canselect") && item["canselect"].IsBool()) {
        m_canSelect = item["canselect"].GetBool();
    }
    if (item.HasMember("drawaabb") && item["drawaabb"].IsBool()) {
        m_drawAABB = item["drawaabb"].GetBool();
    }
}

void SVDataNode::_toNodeData(SVNodePtr _node){
    _node->setname(m_name.c_str());
    _node->setPosition(m_posX+m_offsetX, m_posY+m_offsetY, m_posZ);
    _node->setScale(m_scaleX, m_scaleY, m_scaleZ);
    _node->setRotation(m_rotX, m_rotY, m_rotZ);
    _node->setZOrder(m_zorder);
    _node->setRSType((RENDERSTREAMTYPE)m_renderStreamType);
    _node->setcanSelect(m_canSelect);
    _node->setdrawAABB(m_drawAABB);
}

SVNodePtr SVDataNode::toNode(){
    //根据动作类型 来确定 是给每人一个动作 还是全局的动作 默认是每人一个动作
    SVNodePtr t_node = MakeSharedPtr<SVNode>(mApp);
    _toNodeData(t_node);
    return t_node;
}

void SVDataNode::refreshIn(SVNodePtr _node){
    if (_node) {
        m_name = _node->getname();
        m_posX = _node->getPosition().x;
        m_posY = _node->getPosition().y;
        m_posZ = _node->getPosition().z;
        m_scaleX = _node->getScale().x;
        m_scaleY = _node->getScale().y;
        m_scaleZ = _node->getScale().z;
        m_rotX = _node->getRotation().x;
        m_rotY = _node->getRotation().y;
        m_rotZ = _node->getRotation().z;
        m_zorder = _node->getZOrder();
        m_renderStreamType = _node->getRSType();
        m_canSelect = _node->getcanSelect();
        m_drawAABB = _node->getDrawAABB();
    }
}

void SVDataNode::refreshOut(SVNodePtr _node){
    if (_node) {
        _node->setname(m_name.c_str());
        _node->setPosition(m_posX+m_offsetX, m_posY+m_offsetY, m_posZ);
        _node->setScale(m_scaleX, m_scaleY, m_scaleZ);
        _node->setRotation(m_rotX, m_rotY, m_rotZ);
        _node->setcanSelect(m_canSelect);
        _node->setZOrder(m_zorder);
        _node->setRSType((RENDERSTREAMTYPE)m_renderStreamType);
        _node->setdrawAABB(m_drawAABB);
    }
}


SVDataNodeDeform::SVDataNodeDeform(SVInst *_app)
: SVDataNode(_app){
}

SVDataNodeDeform::~SVDataNodeDeform(){
}

void SVDataNodeDeform::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
//    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
//    RAPIDJSON_NAMESPACE::Value locationArray(RAPIDJSON_NAMESPACE::kArrayType);
//    SVMap<u32,V2>::Iterator it= m_pointMap.begin();
//    while (it!=m_pointMap.end()) {
//        u32 t_postion=it->key;
//        V2 t_point=it->data;
//        RAPIDJSON_NAMESPACE::Value pointObj(RAPIDJSON_NAMESPACE::kObjectType);
//        pointObj.AddMember("index", t_postion , _allocator);
//        pointObj.AddMember("x", t_point.x , _allocator);
//        pointObj.AddMember("y", t_point.y , _allocator);
//        locationArray.PushBack(pointObj,_allocator );
//    }
//    locationObj.AddMember("pointMove", locationArray, _allocator);
//    _objValue.AddMember("sv_deform", locationObj, _allocator);
}

void SVDataNodeDeform::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
//    m_pointMap.clear();
//    if (item.HasMember("pointMove") && item["pointMove"].IsArray()) {
//        RAPIDJSON_NAMESPACE::Value locationArray=item["pointMove"].GetArray();
//        for(int i=0;i<locationArray.Size();i++){
//            RAPIDJSON_NAMESPACE::Value obj=locationArray[i].GetObject();
//            u32 t_postion=0;
//            V2  t_point;
//            if (obj.HasMember("index") && obj["index"].IsInt()) {
//                 t_postion = obj["index"].GetInt();
//            }
//            if (obj.HasMember("x") && obj["x"].IsFloat()) {
//                t_point.x = obj["x"].GetFloat();
//            }
//            if (obj.HasMember("y") && obj["y"].IsFloat()) {
//                t_point.y = obj["y"].GetFloat();
//            }
//            m_pointMap.append(t_postion,t_point);
//        }
//    }
}

SVNodePtr SVDataNodeDeform::toNode(){
    return nullptr;
}

//
SVDataNodeFilter::SVDataNodeFilter(SVInst *_app)
: SVDataNode(_app){
    m_dataFilter=MakeSharedPtr<SVDataFilter>(mApp);
}

SVDataNodeFilter::~SVDataNodeFilter(){
    m_dataFilter=nullptr;
}

void SVDataNodeFilter::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
    m_dataFilter->toJSON(_allocator,_objValue);
}

void SVDataNodeFilter::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    m_dataFilter->fromJSON(item);
}

SVNodePtr SVDataNodeFilter::toNode(){
    return m_dataFilter->toNode();
}

//
SVDataNodeBmpFont::SVDataNodeBmpFont(SVInst *_app)
: SVDataNode(_app){
    m_renderStreamType = RST_ANIMATE;
    m_dataType = MODELDATA_BITFONT;
    m_fontW = 50;
    m_fontH = 50;
    m_innerX = 0;
    m_innerY = 0;
    m_content = "100";
}

SVDataNodeBmpFont::~SVDataNodeBmpFont(){
    
}

void SVDataNodeBmpFont::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    _toJsonData(_allocator, locationObj);
    locationObj.AddMember("file", RAPIDJSON_NAMESPACE::StringRef(m_file_name.c_str()), _allocator);
    locationObj.AddMember("fontW", m_fontW, _allocator);
    locationObj.AddMember("fontH", m_fontH, _allocator);
    locationObj.AddMember("innerX", m_innerX, _allocator);
    locationObj.AddMember("innerY", m_innerY, _allocator);
    _objValue.AddMember("sv_bitfont", locationObj, _allocator);
}

void SVDataNodeBmpFont::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    _fromJsonData(item);
    if (item.HasMember("file") && item["file"].IsString()) {
        m_file_name = item["file"].GetString();
    }
    if (item.HasMember("fontW") && item["fontW"].IsInt()) {
        m_fontW = item["fontW"].GetInt();
    }
    if (item.HasMember("fontH") && item["fontH"].IsInt()) {
        m_fontH = item["fontH"].GetInt();
    }
    if (item.HasMember("innerX") && item["innerX"].IsInt()) {
        m_innerX = item["innerX"].GetInt();
    }
    if (item.HasMember("innerY") && item["innerY"].IsInt()) {
        m_innerY = item["innerY"].GetInt();
    }
}

SVNodePtr SVDataNodeBmpFont::toNode(){
    SVBitFontNodePtr t_font_node = MakeSharedPtr<SVBitFontNode>(mApp);
    _toNodeData(t_font_node);
    t_font_node->setFontSize(m_fontW, m_fontH);
    SVString t_resPath = m_root_path +  m_file_name;
    t_font_node->setTexture(t_resPath.c_str());
    t_font_node->setText(m_content.c_str());
    t_font_node->setInnerOff(m_innerX, m_innerY);
    t_font_node->setAtcPt(ATCH_MC);
    return t_font_node;
}

void SVDataNodeBmpFont::refreshIn(SVBitFontNodePtr _bitfontNode){
    SVDataNode::refreshIn(_bitfontNode);
    if (_bitfontNode) {
        SVString t_pngPath = _bitfontNode->getTexturePath();
        s32 t_pos = t_pngPath.rfind('/');
        s32 t_len = t_pngPath.size();
        m_file_name = SVString::substr(t_pngPath, t_pos+1, t_len - t_pos-1);
        m_root_path = SVString::substr(t_pngPath, 0, t_pos+1);
        m_content = _bitfontNode->getText();
        m_fontW = _bitfontNode->getFontW();
        m_fontH = _bitfontNode->getFontH();
        m_innerX = _bitfontNode->getInnerOffXScale();
        m_innerY = _bitfontNode->getInnerOffYScale();
        m_type = _bitfontNode->getAtcPt();
    }
}

void SVDataNodeBmpFont::refreshOut(SVBitFontNodePtr _bitfontNode){
    SVDataNode::refreshOut(_bitfontNode);
    if (_bitfontNode) {
        SVString t_resPath = m_root_path +  m_file_name;
        _bitfontNode->setTexture(t_resPath.c_str());
        _bitfontNode->setText(m_content.c_str());
        _bitfontNode->setFontSize(m_fontW, m_fontH);
        _bitfontNode->setInnerOff(m_innerX, m_innerY);
        _bitfontNode->setAtcPt(m_type);
    }
}

//
SVDataNodeBackGround::SVDataNodeBackGround(SVInst *_app)
: SVDataNode(_app){
    m_renderStreamType = RST_ANIMATE;
    m_dataType = MODELDATA_SPRITE;
    m_spriteW = 720;
    m_spriteH = 1280;
    m_root_path = "";
    m_file_name = "/svres/di9_16.png";
    m_spritePtr=nullptr;
}

SVDataNodeBackGround::~SVDataNodeBackGround(){
    
}

void SVDataNodeBackGround::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    _toJsonData(_allocator, locationObj);
    locationObj.AddMember("file", RAPIDJSON_NAMESPACE::StringRef(m_file_name.c_str()), _allocator);
    locationObj.AddMember("spriteW", m_spriteW, _allocator);
    locationObj.AddMember("spriteH", m_spriteH, _allocator);
    RAPIDJSON_NAMESPACE::Value locationArray(RAPIDJSON_NAMESPACE::kArrayType);
    SVMap<u32,V2>::Iterator it= m_pointMap.begin();
    while (it!=m_pointMap.end()) {
        u32 t_postion=it->key;
        V2 t_point=it->data;
        RAPIDJSON_NAMESPACE::Value pointObj(RAPIDJSON_NAMESPACE::kObjectType);
        pointObj.AddMember("index", t_postion , _allocator);
        pointObj.AddMember("x", t_point.x , _allocator);
        pointObj.AddMember("y", t_point.y , _allocator);
        locationArray.PushBack(pointObj,_allocator );
    }
    locationObj.AddMember("pointMove", locationArray, _allocator);
    _objValue.AddMember("sv_deform", locationObj, _allocator);
    _objValue.AddMember("sv_sprite", locationObj, _allocator);
}

void SVDataNodeBackGround::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    _fromJsonData(item);
    if (item.HasMember("file") && item["file"].IsString()) {
        m_file_name = item["file"].GetString();
    }
    if (item.HasMember("spriteW") && item["spriteW"].IsInt()) {
        m_spriteW = item["spriteW"].GetInt();
    }
    if (item.HasMember("spriteH") && item["spriteH"].IsInt()) {
        m_spriteH = item["spriteH"].GetInt();
    }
    m_pointMap.clear();
    if (item.HasMember("pointMove") && item["pointMove"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value locationArray=item["pointMove"].GetArray();
        for(int i=0;i<locationArray.Size();i++){
            RAPIDJSON_NAMESPACE::Value obj=locationArray[i].GetObject();
            u32 t_postion=0;
            V2  t_point;
            if (obj.HasMember("index") && obj["index"].IsInt()) {
                t_postion = obj["index"].GetInt();
            }
            if (obj.HasMember("x") && obj["x"].IsFloat()) {
                t_point.x = obj["x"].GetFloat();
            }
            if (obj.HasMember("y") && obj["y"].IsFloat()) {
                t_point.y = obj["y"].GetFloat();
            }
            m_pointMap.append(t_postion,t_point);
        }
    }
}

SVNodePtr SVDataNodeBackGround::toNode(){
    if(!m_spritePtr){
        m_spritePtr = MakeSharedPtr<SVBackGroundNode>(mApp);
        _toNodeData(m_spritePtr);
        SVString t_resPath = m_root_path +  m_file_name;
        m_spritePtr->setTexture(t_resPath.c_str());
    }
    return m_spritePtr;
}

void SVDataNodeBackGround::refreshOut(SVSpriteNodePtr _spriteNode){
    SVDataNode::refreshOut(_spriteNode);
    if (_spriteNode) {
        SVString t_resPath = m_root_path +  m_file_name;
        _spriteNode->setTexture(t_resPath.c_str());
        _spriteNode->setSpriteSize(m_spriteW, m_spriteH);
    }
}

void SVDataNodeBackGround::refreshIn(SVSpriteNodePtr _spriteNode){
    SVDataNode::refreshIn(_spriteNode);
    if (_spriteNode) {
        SVString t_pngPath = SVString(_spriteNode->getTexturePath());
        if (t_pngPath.size() > 0) {
            s32 t_pos = t_pngPath.rfind('/');
            s32 t_len = t_pngPath.size();
            m_file_name = SVString::substr(t_pngPath, t_pos+1, t_len - t_pos-1);
            m_root_path = SVString::substr(t_pngPath, 0, t_pos+1);
        }
        m_spriteW = _spriteNode->getWidth();
        m_spriteH = _spriteNode->getHeight();
    }
}

//
SVDataNodeMask::SVDataNodeMask(SVInst *_app)
: SVDataNode(_app){
    m_dataType = MODELDATA_MASK;
    m_renderStreamType = RST_ANIMATE;
}

SVDataNodeMask::~SVDataNodeMask(){
    
}

void SVDataNodeMask::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
    //生成一个object
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    locationObj.AddMember("name", RAPIDJSON_NAMESPACE::StringRef(m_name.c_str()), _allocator);
    locationObj.AddMember("file", RAPIDJSON_NAMESPACE::StringRef(m_file_name.c_str()), _allocator);
    _objValue.AddMember("sv_mask", locationObj, _allocator);  //添加object到Document中
}

void SVDataNodeMask::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    if (item.HasMember("file") && item["file"].IsString()) {
        m_file_name = item["file"].GetString();
    }
    if (item.HasMember("name") && item["name"].IsString()) {
        m_name = item["name"].GetString();
    }
}

SVNodePtr SVDataNodeMask::toNode(){
    SV2DFaceMaskSTNodePtr t_maskNodePtr = MakeSharedPtr<SV2DFaceMaskSTNode>(mApp);
    t_maskNodePtr->setPosition(m_posX, m_posY, m_posZ);
    t_maskNodePtr->setPeronID(m_personID);
    SVMtlFace2DPtr t_material_face2d = MakeSharedPtr<SVMtlFace2D>(mApp);
    SVString t_resPath = m_root_path + m_file_name;
    SVTexturePtr tex = mApp->getTexMgr()->getTextureSync(t_resPath, true);
    t_material_face2d->setMaskTexture(tex);
    t_maskNodePtr->setname(m_name.c_str());
    t_maskNodePtr->setMaterial(t_material_face2d);
    return t_maskNodePtr;
}

void SVDataNodeMask::refreshIn(SV2DFaceMaskNodePtr _maskNode){
    SVDataNode::refreshIn(_maskNode);
    if (_maskNode) {
        SVString t_pngPath = SVString(_maskNode->getMaterial()->getMaskTexture()->getname());
        if (t_pngPath.size() > 0) {
            s32 t_pos = t_pngPath.rfind('/');
            s32 t_len = t_pngPath.size();
            m_file_name = SVString::substr(t_pngPath, t_pos+1, t_len - t_pos-1);
            m_root_path = SVString::substr(t_pngPath, 0, t_pos+1);
        }
    }
}

void SVDataNodeMask::refreshOut(SV2DFaceMaskNodePtr _maskNode){
    SVDataNode::refreshOut(_maskNode);
    if (_maskNode) {
        SVString t_resPath = m_root_path +  m_file_name;
        SVMtlFace2DPtr t_material_face2d = MakeSharedPtr<SVMtlFace2D>(mApp);
        SVTexturePtr tex = mApp->getTexMgr()->getTextureSync(t_resPath.c_str(), true);
        t_material_face2d->setMaskTexture(tex);
        _maskNode->setMaterial(t_material_face2d);
    }
}

//
SVDataNodeBMFont::SVDataNodeBMFont(SVInst *_app)
: SVDataNode(_app){
    m_renderStreamType = RST_ANIMATE;
    m_dataType = MODELDATA_BMFONT;
    m_fontW = 50;
    m_fontH = 50;
    m_spacing = 0.0f;
    m_content = "bmfont";
}

SVDataNodeBMFont::~SVDataNodeBMFont(){
    
}

void SVDataNodeBMFont::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    _toJsonData(_allocator, locationObj);
    locationObj.AddMember("file", RAPIDJSON_NAMESPACE::StringRef(m_file_name.c_str()), _allocator);
    locationObj.AddMember("fontW", m_fontW, _allocator);
    locationObj.AddMember("fontH", m_fontH, _allocator);
    locationObj.AddMember("spacing", m_spacing, _allocator);
    locationObj.AddMember("text", RAPIDJSON_NAMESPACE::StringRef(m_content.c_str()), _allocator);
    _objValue.AddMember("sv_bmfont", locationObj, _allocator);
}

void SVDataNodeBMFont::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    _fromJsonData(item);
    if (item.HasMember("file") && item["file"].IsString()) {
        m_file_name = item["file"].GetString();
    }
    if (item.HasMember("fontW") && item["fontW"].IsInt()) {
        m_fontW = item["fontW"].GetInt();
    }
    if (item.HasMember("fontH") && item["fontH"].IsInt()) {
        m_fontH = item["fontH"].GetInt();
    }
    if (item.HasMember("spacing") && item["spacing"].IsFloat()) {
        m_spacing = item["spacing"].GetFloat();
    }
    if (item.HasMember("text") && item["text"].IsString()) {
        m_content = item["text"].GetString();
    }
}

SVNodePtr SVDataNodeBMFont::toNode(){
    SVBMFontNodePtr t_bmfont_node = MakeSharedPtr<SVBMFontNode>(mApp);
    _toNodeData(t_bmfont_node);
    t_bmfont_node->setFontSize(m_fontW, m_fontH);
    SVString t_resPath = m_root_path +  m_file_name;
    SVBMFontPtr font = MakeSharedPtr<SVBMFont>(mApp);
    SVBMFontLoader t_loder(mApp);
    t_loder.loadData(t_resPath.c_str(), font);
    t_bmfont_node->setFont(font);
    t_bmfont_node->setText(m_content.c_str());
    t_bmfont_node->setSpacing(m_spacing);
    t_bmfont_node->setAtcPt(ATCH_MC);
    return t_bmfont_node;
}

void SVDataNodeBMFont::refreshIn(SVBMFontNodePtr _bmfontNode){
    SVDataNode::refreshIn(_bmfontNode);
    if (_bmfontNode) {
        m_content = _bmfontNode->getText();
        m_fontW = _bmfontNode->getFontW();
        m_fontH = _bmfontNode->getFontH();
        m_spacing = _bmfontNode->getSpacing();
        m_type = _bmfontNode->getAtcPt();
    }
}

void SVDataNodeBMFont::refreshOut(SVBMFontNodePtr _bmfontNode){
    SVDataNode::refreshOut(_bmfontNode);
    if (_bmfontNode) {
        SVString t_resPath = m_root_path +  m_file_name;
        _bmfontNode->setText(m_content.c_str());
        _bmfontNode->setFontSize(m_fontW, m_fontH);
        _bmfontNode->setSpacing(m_spacing);
        _bmfontNode->setAtcPt(m_type);
    }
}

#ifdef CONFIG_IS_LOAD_FREETYPE
//
SVTTFNodeModelData::SVTTFNodeModelData(SVInst *_app)
: SVDataNode(_app){
    m_dataType = MODELDATA_MASK;
}

SVTTFNodeModelData::~SVTTFNodeModelData(){
    
}

void SVTTFNodeModelData::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_jsonDoc){
//    //生成一个object
//    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
//    locationObj.AddMember("name", RAPIDJSON_NAMESPACE::StringRef(m_name.c_str()), _allocator);
//    locationObj.AddMember("file", RAPIDJSON_NAMESPACE::StringRef(m_file_name.c_str()), _allocator);
//    _jsonDoc.AddMember("sv_mask", locationObj, _allocator);  //添加object到Document中
}

void SVTTFNodeModelData::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
//    if (item.HasMember("file") && item["file"].IsString()) {
//        m_file_name = item["file"].GetString();
//    }
//    if (item.HasMember("name") && item["name"].IsString()) {
//        m_name = item["name"].GetString();
//    }
}

SVNodePtr SVTTFNodeModelData::toNode(){
    SVFreeTypeNodePtr t_ttfNodePtr = MakeSharedPtr<SVFreeTypeNode>(mApp);
    t_ttfNodePtr->setPosition(m_posX, m_posY, m_posZ);
    t_ttfNodePtr->setPeronID(m_personID);
//    SVMtlFace2dPtr t_material_face2d = MakeSharedPtr<SVMtlFace2d>(mApp);
//    SVString t_resPath = m_root_path + m_file_name;
//    SVTexturePtr tex = mApp->getTexMgr()->getTextureSync(t_resPath, true);
//    t_material_face2d->setMaskTexture(tex);
//    t_maskNodePtr->setname(m_name.c_str());
//    t_maskNodePtr->setMaterial(t_material_face2d);
    return t_ttfNodePtr;
}

void SVTTFNodeModelData::refreshIn(SVFreeTypeNodePtr _ttfNode){
    SVDataNode::refreshIn(_ttfNode);
    if (_ttfNode) {
        
    }
}

void SVTTFNodeModelData::refreshOut(SVFreeTypeNodePtr _ttfNode){
    SVDataNode::refreshOut(_ttfNode);
    if (_ttfNode) {
        
    }
}

#endif

