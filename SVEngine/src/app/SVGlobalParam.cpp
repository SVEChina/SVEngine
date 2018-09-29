//
// SVGlobalParam.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVGlobalParam.h"
#include "../rendercore/renderer/SVRenderState.h"
#include "../rendercore/SVGL/SVRenderStateGL.h"

SVGlobalParam::SVGlobalParam(SVInst *_app)
:SVGBase(_app) {
    m_curScene = nullptr;
    m_inner_width = 720;
    m_inner_height = 1280;
    m_fontFileName = "svres/simsun.ttf";
    setSeed(0);
}

SVGlobalParam::~SVGlobalParam() {
    clear();
}

void SVGlobalParam::clear() {
    m_curScene = nullptr;
    m_inner_width = 720;
    m_inner_height = 1280;
    
    for (s32 i = 0; i<m_stack_proj.size(); i++) {
        FMat4 *t_mat4 = m_stack_proj[i];
        delete t_mat4;
    }
    m_stack_proj.clear();
    for (s32 i = 0; i<m_stack_view.size(); i++) {
        FMat4 *t_mat4 = m_stack_view[i];
        delete t_mat4;
    }
    m_stack_view.clear();
    for (s32 i = 0; i<m_stack_vp.size(); i++) {
        FMat4 *t_mat4 = m_stack_vp[i];
        delete t_mat4;
    }
    m_stack_vp.clear();
}

void SVGlobalParam::setSeed(u32 seed) {
    //AtomicLock atomic(&lock);
    m_random.setSeed(seed);
}

u32 SVGlobalParam::getSeed() const {
    return m_random.getSeed();
}

u32 SVGlobalParam::getRandom() {
    //AtomicLock atomic(&lock);
    return m_random.get();
}

s32 SVGlobalParam::getRandomInt(s32 from,s32 to) {
    //AtomicLock atomic(&lock);
    return m_random.getInt(from,to);
}

f32 SVGlobalParam::getRandomFloat(f32 from,f32 to) {
    //AtomicLock atomic(&lock);
    return m_random.getFloat(from,to);
}

double SVGlobalParam::getRandomDouble(double from,double to) {
    //AtomicLock atomic(&lock);
    return m_random.getDouble(from,to);
}

//
void SVGlobalParam::updateMainMat(FMat4 _projMat, FMat4 _viewMat, FMat4 _vpMat){
    FMat4 *mat4Proj = new FMat4(_projMat);
    if (m_stack_proj.size() > 0) {
        FMat4 *t_mat = m_stack_proj[0];
        delete t_mat;
        m_stack_proj.set(0, mat4Proj);
    }else{
        m_stack_proj.append(mat4Proj);
    }
    //
    FMat4 *mat4View = new FMat4(_viewMat);
    if (m_stack_view.size() > 0) {
        FMat4 *t_mat = m_stack_view[0];
        delete t_mat;
        m_stack_view.set(0, mat4View);
    }else{
        m_stack_view.append(mat4View);
    }
    //
    FMat4 *mat4VP = new FMat4(_vpMat);
    if (m_stack_vp.size() > 0) {
        FMat4 *t_mat = m_stack_vp[0];
        delete t_mat;
        m_stack_vp.set(0, mat4VP);
    }else{
        m_stack_vp.append(mat4VP);
    }
}

void SVGlobalParam::addToProjStack(FMat4 _mat){
    FMat4 *mat4 = new FMat4(_mat);
    m_stack_proj.append(mat4);
}
FMat4 *SVGlobalParam::getProjMat(){
    if (m_stack_proj.size() > 0) {
        return m_stack_proj[m_stack_proj.size() - 1];
    }
    return nullptr;
}
bool SVGlobalParam::removeProjMat(){
    bool ret = false;
    if (m_stack_proj.size() > 0) {
        FMat4 *mat4 = m_stack_proj[m_stack_proj.size() - 1];
        delete mat4;
        m_stack_proj.remove(m_stack_proj.size() - 1);
        ret = true;
    }
    return ret;
}
//
void SVGlobalParam::addToViewStack(FMat4 _mat){
    FMat4 *mat4 = new FMat4(_mat);
    m_stack_view.append(mat4);
}
FMat4 *SVGlobalParam::getViewMat(){
    if (m_stack_view.size() > 0) {
        return m_stack_view[m_stack_view.size() - 1];
    }
    return nullptr;
}
bool SVGlobalParam::removeViewMat(){
    bool ret = false;
    if (m_stack_view.size() > 0) {
        FMat4 *mat4 = m_stack_view[m_stack_view.size() - 1];
        delete mat4;
        m_stack_view.remove(m_stack_view.size() - 1);
        ret = true;
    }
    return ret;
}
//
void SVGlobalParam::addToVPStack(FMat4 _mat){
    FMat4 *mat4 = new FMat4(_mat);
    m_stack_vp.append(mat4);
}
FMat4 *SVGlobalParam::getVPMat(){
    if (m_stack_vp.size() > 0) {
        return m_stack_vp[m_stack_vp.size() - 1];
    }
    return nullptr;
}
bool SVGlobalParam::removeVPMat(){
    bool ret = false;
    if (m_stack_vp.size() > 0) {
        FMat4 *mat4 = m_stack_vp[m_stack_vp.size() - 1];
        delete mat4;
        m_stack_vp.remove(m_stack_vp.size() - 1);
        ret = true;
    }
    return ret;
}
