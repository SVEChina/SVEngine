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
#include "SVGL/SVRendererGL.h"
#include "SVGL/SVRResGL.h"
#include "SVGL/SVRendererGL.h"
#include "SVMetal/SVRendererMetal.h"
#include "SVVulkan/SVRendererVK.h"

s32 SVRenderMesh::getVertexFormateSize(VFTYPE _type) {
    return SVResGLRenderMesh::getVertexFormateSize(_type);
}

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
        //渲染器类型E_RENDERER_METAL,
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

void SVRenderMesh::render() {
    SVResGLRenderMeshPtr t_rendeMeshPtr = std::dynamic_pointer_cast<SVResGLRenderMesh>(m_objVBOPtr);
    if (t_rendeMeshPtr) {
        _updateConf();
        _updateData();
        t_rendeMeshPtr->render();
    }
}

void SVRenderMesh::_updateConf(){
    SVResGLRenderMeshPtr t_rendeMeshPtr = std::dynamic_pointer_cast<SVResGLRenderMesh>(m_objVBOPtr);
    if (t_rendeMeshPtr) {
        if (m_renderMeshConf.dirty) {
            m_renderMeshConf.dirty = false;
            t_rendeMeshPtr->setIndexPoolType(m_renderMeshConf.indexPoolType);
            t_rendeMeshPtr->setVertexPoolType(m_renderMeshConf.vertPoolType);
            t_rendeMeshPtr->setDrawMethod(m_renderMeshConf.drawmethod);
            t_rendeMeshPtr->setVertexType(m_renderMeshConf.vftype);
        }
    }
}

void SVRenderMesh::_updateData(){
    SVResGLRenderMeshPtr t_rendeMeshPtr = std::dynamic_pointer_cast<SVResGLRenderMesh>(m_objVBOPtr);
    if (t_rendeMeshPtr) { 
        if (m_renderMeshData.dirty) {
            m_renderMeshData.dirty = false;
            if (m_renderMeshData.pDataIndex) {
                t_rendeMeshPtr->setIndexData(m_renderMeshData.pDataIndex, m_renderMeshData.indexNum);
            }
            if (m_renderMeshData.pDataVertex) {
                t_rendeMeshPtr->setVertexDataNum( m_renderMeshData.pointNum);
                t_rendeMeshPtr->setVertexData(m_renderMeshData.pDataVertex);
            }
        }
    }
}

SVRenderMeshDvid::SVRenderMeshDvid(SVInst* _app)
:SVRenderMesh(_app){
    _resetMeshConf();
    _resetMeshData();
}

void SVRenderMeshDvid::_resetMeshData(){
    m_renderMeshDvidData.pDataV2 = nullptr;
    m_renderMeshDvidData.pDataV3 = nullptr;
    m_renderMeshDvidData.pDataC0 = nullptr;
    m_renderMeshDvidData.pDataC1 = nullptr;
    m_renderMeshDvidData.pDataT0 = nullptr;
    m_renderMeshDvidData.pDataT1 = nullptr;
    m_renderMeshDvidData.pDataT2 = nullptr;
    m_renderMeshDvidData.pDataNor = nullptr;
    m_renderMeshDvidData.pDataTag = nullptr;
    m_renderMeshDvidData.pDataBTor = nullptr;
    m_renderMeshDvidData.pointNum = 0;
    m_renderMeshDvidData.dirty = false;
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
    SVRendererVKPtr t_rendeVKPtr = std::dynamic_pointer_cast<SVRendererVK>(t_renderBasePtr);
    if (t_rendeVKPtr) {
        //渲染器类型E_RENDERER_VUNKAN,
    }
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
        m_renderMeshDvidData.pDataIndex = _data;
        m_renderMeshDvidData.indexNum = _num;
        m_renderMeshDvidData.dirty = true;
    }
}

void SVRenderMeshDvid::setVertexDataNum(s32 _vertexNum){
    m_renderMeshDvidData.pointNum = _vertexNum;
    m_renderMeshDvidData.dirty = true;
}

void SVRenderMeshDvid::setVertex2Data(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshDvidData.pDataV2 = _pdata;
        m_renderMeshDvidData.dirty = true;
    }
}

void SVRenderMeshDvid::setVertex3Data(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshDvidData.pDataV3 = _pdata;
        m_renderMeshDvidData.dirty = true;
    }
}

void SVRenderMeshDvid::setColor0Data(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshDvidData.pDataC0 = _pdata;
        m_renderMeshDvidData.dirty = true;
    }
}

void SVRenderMeshDvid::setColor1Data(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshDvidData.pDataC1 = _pdata;
        m_renderMeshDvidData.dirty = true;
    }
}

void SVRenderMeshDvid::setTexcoord0Data(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshDvidData.pDataT0 = _pdata;
        m_renderMeshDvidData.dirty = true;
    }
}

void SVRenderMeshDvid::setTexcoord1Data(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshDvidData.pDataT1 = _pdata;
        m_renderMeshDvidData.dirty = true;
    }
}

void SVRenderMeshDvid::setTexcoord2Data(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshDvidData.pDataT2 = _pdata;
        m_renderMeshDvidData.dirty = true;
    }
}

void SVRenderMeshDvid::setNormalData(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshDvidData.pDataNor = _pdata;
        m_renderMeshDvidData.dirty = true;
    }
}

void SVRenderMeshDvid::setTagentData(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshDvidData.pDataTag = _pdata;
        m_renderMeshDvidData.dirty = true;
    }
}

void SVRenderMeshDvid::setBTagentData(SVDataSwapPtr _pdata){
    if (_pdata) {
        m_renderMeshDvidData.pDataBTor = _pdata;
        m_renderMeshDvidData.dirty = true;
    }
}

void SVRenderMeshDvid::_updateData(){
    SVResGLRenderMeshDvidPtr t_rendeMeshPtr = std::dynamic_pointer_cast<SVResGLRenderMeshDvid>(m_objVBOPtr);
    if (t_rendeMeshPtr) {
        if (m_renderMeshDvidData.dirty) {
            m_renderMeshDvidData.dirty = false;
            t_rendeMeshPtr->setVertexDataNum(m_renderMeshDvidData.pointNum);
            t_rendeMeshPtr->setVertex2Data(m_renderMeshDvidData.pDataV2);
            t_rendeMeshPtr->setVertex3Data(m_renderMeshDvidData.pDataV3);
            t_rendeMeshPtr->setColor0Data(m_renderMeshDvidData.pDataC0);
            t_rendeMeshPtr->setColor1Data(m_renderMeshDvidData.pDataC1);
            t_rendeMeshPtr->setTexcoord0Data(m_renderMeshDvidData.pDataT0);
            t_rendeMeshPtr->setTexcoord1Data(m_renderMeshDvidData.pDataT1);
            t_rendeMeshPtr->setTexcoord2Data(m_renderMeshDvidData.pDataT2);
            t_rendeMeshPtr->setNormalData(m_renderMeshDvidData.pDataNor);
            t_rendeMeshPtr->setTagentData(m_renderMeshDvidData.pDataTag);
            t_rendeMeshPtr->setBTagentData(m_renderMeshDvidData.pDataBTor);
            t_rendeMeshPtr->setIndexData(m_renderMeshDvidData.pDataIndex, m_renderMeshDvidData.indexNum);
        }
    }
}

void SVRenderMeshDvid::render() {
    SVResGLRenderMeshDvidPtr t_rendeMeshPtr = std::dynamic_pointer_cast<SVResGLRenderMeshDvid>(m_objVBOPtr);
    if (t_rendeMeshPtr) {
        _updateConf();
        _updateData();
        t_rendeMeshPtr->render();
    }
}
