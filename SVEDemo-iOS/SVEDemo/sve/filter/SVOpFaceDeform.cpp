//
//  SVOpFaceDeform.cpp
//  SVEngineShow
//
//  Created by 徐子昱 on 2018/10/31.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVOpFaceDeform.h"
#include "SVFaceDeform.h"
#include "core/SVDeformImageMove.h"
#include "file/SVParseDeform.h"

SVOpFaceDeform::SVOpFaceDeform(SVInst* _app , SVFaceDeformPtr _deform)
:SVOpBase(_app){
    m_deform=_deform;
}
SVOpFaceDeform::~SVOpFaceDeform(){
    m_deform=nullptr;
}

void SVOpFaceDeform::_process(f32 _dt){
    m_deform->createDeform();
}

SVOpFaceDeformSmooth::SVOpFaceDeformSmooth(SVInst* _app,
                                           SVFaceDeformPtr _deform,
                                           SVMap<u32,V2> _point,SVMap<u32,V2> _area)
:SVOpBase(_app){
    m_point=_point;
    m_area=_area;
    m_deform=_deform;
}
SVOpFaceDeformSmooth::~SVOpFaceDeformSmooth(){
    m_point.clear();
    m_area.clear();
    m_deform=nullptr;
}

void SVOpFaceDeformSmooth::_process(f32 _dt){
    SVMap<u32,V2>::Iterator it = m_point.begin();
    while (it!=m_point.end()) {
        u32 t_postion = it->key;
        V2 t_point = it->data;
        m_deform->getDeform()->setTagPoint(t_postion, t_point);
        it++;
    }
    
    it = m_area.begin();
    while (it!=m_area.end()) {
        u32 t_postion = it->key;
        V2 t_point = it->data;
        m_deform->getDeform()->setTagAreaPoint(t_postion, t_point);
        it++;
    }
}
SVOpDeformSmooth::SVOpDeformSmooth(SVInst* _app,SVFaceDeformPtr _deform,f32 _smooth)
:SVOpBase(_app){
    m_smooth=_smooth;
    m_deform=_deform;
}
SVOpDeformSmooth::~SVOpDeformSmooth(){
    m_deform=nullptr;
}

void SVOpDeformSmooth::_process(f32 _dt){
    m_deform->getDeform()->setSmooth(m_smooth);
}

SVOpFaceDeformEye::SVOpFaceDeformEye(SVInst* _app,SVFaceDeformPtr _deform,f32 _smooth)
:SVOpBase(_app){
    m_smooth=_smooth;
    m_deform=_deform;
}
SVOpFaceDeformEye::~SVOpFaceDeformEye(){
    m_deform=nullptr;
}

void SVOpFaceDeformEye::_process(f32 _dt){
    m_deform->getDeform()->setScaleSmooth(m_smooth*0.2);
}

SVOpFaceDeformParss::SVOpFaceDeformParss(SVInst* _app,SVFaceDeformPtr _deform,cptr8 path)
:SVOpBase(_app){
    m_deform=_deform;
    m_path=path;
}
SVOpFaceDeformParss::~SVOpFaceDeformParss(){
    
}

void SVOpFaceDeformParss::_process(f32 _dt){
    SVParseDeform::parse(mApp, m_path.c_str(), 1, m_deform->getDeform());
}
