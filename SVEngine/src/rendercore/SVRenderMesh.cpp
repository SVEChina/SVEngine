//
// SVRenderMesh.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRenderMesh.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalMgr.h"
#include "../base/SVDataSwap.h"
#include "SVRenderMgr.h"
//
#include "SVGL/SVRendererGL.h"
#include "SVGL/SVRResGL.h"
#include "SVGL/SVResGLMesh.h"
//
#include "SVMetal/SVRendererMetal.h"
#include "SVMetal/SVResMetalMesh.h"
//
#include "SVVulkan/SVRendererVK.h"

SVRenderMesh::SVRenderMesh(SVInst* _app)
:SVRObjBase(_app){
    _resetMeshConf();
    _resetMeshData();
    m_created = false;
}

SVRenderMesh::~SVRenderMesh() {
    _resetMeshConf();
    _resetMeshData();
    m_created = false;
}

void SVRenderMesh::_resetMeshData(){
    m_renderMeshData.indexNum = 0;
    m_renderMeshData.pointNum = 0;
    m_renderMeshData.instanceCount = 0;
    m_renderMeshData.pDataIndex = nullptr;
    m_renderMeshData.pDataVertex = nullptr;
    m_renderMeshData.dirty = false;
}

void SVRenderMesh::_resetMeshConf(){
    m_renderMeshConf.vftype = E_VF_V2;
    m_renderMeshConf.drawmethod = E_DM_TRIANGLES;
    m_renderMeshConf.vertPoolType = GL_STATIC_DRAW;
    m_renderMeshConf.indexPoolType = GL_STATIC_DRAW;
    m_renderMeshConf.bVisible = true;
    m_renderMeshConf.useInstance = false;
    m_renderMeshConf.dirty = false;
}

void SVRenderMesh::create(SVRendererBasePtr _renderer){
    SVRObjBase::create(_renderer);
    SVRendererBasePtr t_renderBasePtr = mApp->getRenderer();
    SVRendererGLPtr t_renderGLPtr = std::dynamic_pointer_cast<SVRendererGL>(t_renderBasePtr);
    if (t_renderGLPtr) {
        //渲染器类型E_RENDERER_GLES,
        m_objVBOPtr = MakeSharedPtr<SVResGLRenderMesh>(mApp);
    }
    SVRendererVKPtr t_rendeVKPtr = std::dynamic_pointer_cast<SVRendererVK>(t_renderBasePtr);
    if (t_rendeVKPtr) {
        //渲染器类型E_RENDERER_VUNKAN,
    }
    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(t_renderBasePtr);
    if (t_rendeMetalPtr) {
        //渲染器类型E_RENDERER_METAL
        m_objVBOPtr = MakeSharedPtr<SVResMetalRenderMesh>(mApp);
    }
    if (m_objVBOPtr) {
        _updateConf();
        _updateData();
        m_objVBOPtr->create(_renderer);
    }
}

void SVRenderMesh::destroy(SVRendererBasePtr _renderer) {
    if (m_objVBOPtr) {
        m_objVBOPtr->destroy(_renderer);
    }
    SVRObjBase::destroy(_renderer);
}

void SVRenderMesh::_updateConf(){
    if (m_objVBOPtr) {
        if (m_renderMeshConf.dirty) {
            m_renderMeshConf.dirty = false;
            m_objVBOPtr->updateConf(m_renderMeshConf);
        }
    }
}

void SVRenderMesh::_updateData(){
    if (m_objVBOPtr) {
        if (m_renderMeshData.dirty) {
            m_renderMeshData.dirty = false;
            m_objVBOPtr->updateData(m_renderMeshData);
        }
    }
}

void SVRenderMesh::setIndexPoolType(u32 itype) {
    if (m_renderMeshConf.indexPoolType != itype) {
        m_renderMeshConf.indexPoolType = itype;
        m_renderMeshConf.dirty = true;
    }
}

void SVRenderMesh::setVertexPoolType(u32 vtype) {
    if (m_renderMeshConf.vertPoolType != vtype) {
        m_renderMeshConf.vertPoolType = vtype;
        m_renderMeshConf.dirty = true;
    }
}

void SVRenderMesh::setDrawMethod(DRAWMETHOD drawtype) {
    if (m_renderMeshConf.drawmethod != drawtype) {
        m_renderMeshConf.drawmethod = drawtype;
        m_renderMeshConf.dirty = true;
    }
}

void SVRenderMesh::setVertexType(VFTYPE type) {
    if (m_renderMeshConf.vftype != type) {
        m_renderMeshConf.vftype = type;
        m_renderMeshConf.dirty = true;
    }
}

void SVRenderMesh::setvisible(bool bVis) {
    if (m_renderMeshConf.bVisible != bVis) {
        m_renderMeshConf.bVisible = bVis;
        m_renderMeshConf.dirty = true;
    }
}

void SVRenderMesh::setIndexData(SVDataSwapPtr _data,s32 _num){
    if (_data) {
        m_renderMeshData.pDataIndex = _data;
        m_renderMeshData.indexNum = _num;
        m_renderMeshData.dirty = true;
    }
}

void SVRenderMesh::setVertexDataNum(s32 _vertexNum){
    m_renderMeshData.pointNum = _vertexNum;
    m_renderMeshData.dirty = true;
}

void SVRenderMesh::setVertexData(SVDataSwapPtr _data){
    if (_data) {  
        m_renderMeshData.pDataVertex = _data;
        m_renderMeshData.dirty = true;
    }
}

void SVRenderMesh::createMesh(){
    if (!m_created) {
        m_created = true;
        mApp->getRenderMgr()->pushRCmdCreate(THIS_TO_SHAREPTR(SVRenderMesh));
    }
}

void SVRenderMesh::render(SVRendererBasePtr _renderer) {
    if (m_objVBOPtr) {
        _updateConf();
        _updateData();
        m_objVBOPtr->render(_renderer);
    }
}

/*
    SVRenderMeshDvid
 */

SVRenderMeshDvid::SVRenderMeshDvid(SVInst* _app)
:SVRenderMesh(_app){
    _resetMeshConf();
    _resetMeshData();
}

void SVRenderMeshDvid::_resetMeshData(){
    SVRenderMesh::_resetMeshData();
    m_renderMeshData.pDataV2 = nullptr;
    m_renderMeshData.pDataV3 = nullptr;
    m_renderMeshData.pDataC0 = nullptr;
    m_renderMeshData.pDataC1 = nullptr;
    m_renderMeshData.pDataT0 = nullptr;
    m_renderMeshData.pDataT1 = nullptr;
    m_renderMeshData.pDataT2 = nullptr;
    m_renderMeshData.pDataNor = nullptr;
    m_renderMeshData.pDataTag = nullptr;
    m_renderMeshData.pDataBTor = nullptr;
    m_renderMeshData.pDataInsOffset = nullptr;
    m_renderMeshData.instanceCount = 0;
    m_renderMeshData.pointNum = 0;
    m_renderMeshData.dirty = false;
}

SVRenderMeshDvid::~SVRenderMeshDvid() {
    _resetMeshConf();
    _resetMeshData();
}

void SVRenderMeshDvid::create(SVRendererBasePtr _renderer){
    SVRObjBase::create(_renderer);
    SVRendererBasePtr t_renderBasePtr = mApp->getRenderer();
    SVRendererGLPtr t_renderGLPtr = std::dynamic_pointer_cast<SVRendererGL>(t_renderBasePtr);
    if (t_renderGLPtr) {
        //渲染器类型E_RENDERER_GLES,
        m_objVBOPtr = MakeSharedPtr<SVResGLRenderMeshDvid>(mApp);
    }
//    SVRendererVKPtr t_rendeVKPtr = std::dynamic_pointer_cast<SVRendererVK>(t_renderBasePtr);
//    if (t_rendeVKPtr) {
//        //渲染器类型E_RENDERER_VUNKAN,
//    }
    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(t_renderBasePtr);
    if (t_rendeMetalPtr) {
        //渲染器类型E_RENDERER_METAL,
    }
    if (m_objVBOPtr) {
        _updateConf();
        _updateData();
        m_objVBOPtr->create(_renderer);
    }
}

void SVRenderMeshDvid::destroy(SVRendererBasePtr _renderer) {
    if (m_objVBOPtr) {
        m_objVBOPtr->destroy(_renderer);
    }
    SVRObjBase::destroy(_renderer);
}

void SVRenderMeshDvid::setIndexData(SVDataSwapPtr _data,s32 _num){
    if (_data) {
        m_renderMeshData.pDataIndex = _data;
        m_renderMeshData.indexNum = _num;
        m_renderMeshData.dirty = true;
    }
}

void SVRenderMeshDvid::setVertexDataNum(s32 _vertexNum){
    m_renderMeshData.pointNum = _vertexNum;
    m_renderMeshData.dirty = true;
}

void SVRenderMeshDvid::setVertex2Data(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshData.pDataV2 = _pdata;
        m_renderMeshData.dirty = true;
    }
}

void SVRenderMeshDvid::setVertex3Data(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshData.pDataV3 = _pdata;
        m_renderMeshData.dirty = true;
    }
}

void SVRenderMeshDvid::setColor0Data(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshData.pDataC0 = _pdata;
        m_renderMeshData.dirty = true;
    }
}

void SVRenderMeshDvid::setColor1Data(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshData.pDataC1 = _pdata;
        m_renderMeshData.dirty = true;
    }
}

void SVRenderMeshDvid::setTexcoord0Data(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshData.pDataT0 = _pdata;
        m_renderMeshData.dirty = true;
    }
}

void SVRenderMeshDvid::setTexcoord1Data(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshData.pDataT1 = _pdata;
        m_renderMeshData.dirty = true;
    }
}

void SVRenderMeshDvid::setTexcoord2Data(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshData.pDataT2 = _pdata;
        m_renderMeshData.dirty = true;
    }
}

void SVRenderMeshDvid::setNormalData(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshData.pDataNor = _pdata;
        m_renderMeshData.dirty = true;
    }
}

void SVRenderMeshDvid::setTagentData(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshData.pDataTag = _pdata;
        m_renderMeshData.dirty = true;
    }
}

void SVRenderMeshDvid::setBTagentData(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshData.pDataBTor = _pdata;
        m_renderMeshData.dirty = true;
    }
}
#ifdef __gl3_h_
void SVRenderMeshDvid::setInstanceOffsetData(SVDataSwapPtr _pdata, u32 _instanceCount){
    if (_pdata) {
        m_renderMeshData.pDataInsOffset = _pdata;
        m_renderMeshData.instanceCount = _instanceCount;
        m_renderMeshData.dirty = true;
    }
}

void SVRenderMeshDvid::setInstanceEnable(bool _enable){
    m_renderMeshConf.useInstance = _enable;
    m_renderMeshConf.dirty = true;
}
#endif
