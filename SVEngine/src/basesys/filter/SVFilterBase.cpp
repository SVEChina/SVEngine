//
//  SVFilterBase.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/9/25.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVFilterBase.h"
#include "../../node/SVMultPassNode.h"

SVFilterBase::SVFilterBase(SVInst *_app)
: SVGBase(_app){
    m_pPassNode=nullptr;
    m_mtl=nullptr;
    m_name="SVFilterBase";
    m_type=SV_FUNC_NONE;
}

SVFilterBase::~SVFilterBase(){
    destroy();
}

bool SVFilterBase::create(){
    return false;
}

void SVFilterBase::destroy(){
    m_pPassNode=nullptr;
    m_mtl=nullptr;
}

void SVFilterBase::update(f32 dt){
}

SVNodePtr SVFilterBase::getNode(){
    return m_pPassNode;
}

void SVFilterBase::setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type){
}

f32 SVFilterBase::getFilterParam(SVFILTERITEMTYPE _type){
    return 0;
}

void SVFilterBase::setVisible(bool _visible){
    if (m_pPassNode) {
        m_pPassNode->setvisible(_visible);
    }
}
//序列化
void SVFilterBase::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                    RAPIDJSON_NAMESPACE::Value &_objValue){
    
}

void SVFilterBase::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    
}
