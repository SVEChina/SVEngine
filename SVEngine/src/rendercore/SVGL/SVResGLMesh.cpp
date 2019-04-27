//
// SVResGLMesh.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVResGLMesh.h"
#include "../../third/rapidjson/document.h"
#include "../../base/SVPreDeclare.h"
#include "../../app/SVInst.h"
#include "../../mtl/SVMtlDef.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../base/SVDataChunk.h"
#include "../../file/SVFileMgr.h"
#include "../../base/SVDataSwap.h"
#include "../../base/SVLock.h"
#include "../../rendercore/SVRenderMgr.h"
#include "../SVContextBase.h"
#include "../SVRendererBase.h"
#include "../SVGL/SVRendererGL.h"
#include "../SVResShader.h"

SVResGLVBO::SVResGLVBO(SVInst* _app)
:SVResVBO(_app){
}

SVResGLVBO::~SVResGLVBO(){
}

void SVResGLVBO::create(SVRendererBasePtr _renderer) {
    SVResVBO::create(_renderer);
}

void SVResGLVBO::destroy(SVRendererBasePtr _renderer) {
    SVResVBO::destroy(_renderer);
}

void SVResGLVBO::render(SVRendererBasePtr _renderer) {
    SVResVBO::render(_renderer);
}

//
SVResGLRenderMesh::SVResGLRenderMesh(SVInst* _app)
:SVResGLVBO(_app){
    _reset();
}

SVResGLRenderMesh::~SVResGLRenderMesh() {
    _reset();
}

void SVResGLRenderMesh::_reset(){
    m_vaoID = 0;
    m_vboID = 0;
    m_indexID = 0;
    m_indexNum = 0;
    m_pointNum = 0;
    m_vftype = E_VF_V3;
    m_drawmethod = E_DM_TRIANGLES;
    m_vertPoolType = GL_STATIC_DRAW;
    m_indexPoolType = GL_STATIC_DRAW;
    instanceOffsetID = 0;
    m_instacneCount = 0;
    m_useIntance = false;
    m_useVAO = false;
}

void SVResGLRenderMesh::create(SVRendererBasePtr _renderer){
    SVResGLVBO::create(_renderer);
}

void SVResGLRenderMesh::destroy(SVRendererBasePtr _renderer) {
    if (m_indexID > 0) {
        glDeleteBuffers(1, &m_indexID);
        m_indexID = 0;
    }
    if (m_vboID > 0) {
        glDeleteBuffers(1, &m_vboID);
        m_vboID = 0;
    }
    if (instanceOffsetID > 0) {
        glDeleteBuffers(1, &instanceOffsetID);
        instanceOffsetID = 0;
    }
    SVResGLVBO::destroy(_renderer);
}

void SVResGLRenderMesh::updateConf(RENDERMESHCONF& _conf) {
    setIndexPoolType(_conf.indexPoolType);
    setVertexPoolType(_conf.vertPoolType);
    setDrawMethod(_conf.drawmethod);
    setVertexType(_conf.vftype);
}

void SVResGLRenderMesh::updateData(RENDERMESHDATA& _data) {
    if (_data.pDataIndex) {
        setIndexData(_data.pDataIndex, _data.indexNum);
    }
    if (_data.pDataVertex) {
        setVertexDataNum(_data.pointNum);
        setVertexData(_data.pDataVertex);
    }
    if (_data.pDataInsOffset) {
        setInstanceOffsetData(_data.pDataInsOffset, _data.instanceCount);
    }
}

//
void SVResGLRenderMesh::setIndexPoolType(u32 itype) {
    if (itype == GL_STREAM_DRAW || itype == GL_STATIC_DRAW || itype == GL_DYNAMIC_DRAW) {
        m_indexPoolType = itype;
    } else {
        SV_LOG_ERROR("error parameter: SVResGLRenderMesh setIndexPoolType \n");
    }
}

void SVResGLRenderMesh::setVertexPoolType(u32 vtype) {
    if (vtype == GL_STREAM_DRAW || vtype == GL_STATIC_DRAW || vtype == GL_DYNAMIC_DRAW) {
        m_vertPoolType = vtype;
    } else {
        SV_LOG_ERROR("error parameter: SVResGLRenderMesh setVertexPoolType \n");
    }
}

void SVResGLRenderMesh::setDrawMethod(DRAWMETHOD drawtype) {
    m_drawmethod = drawtype;
}

void SVResGLRenderMesh::setVertexType(VFTYPE type) {
    m_vftype = type;
}

void SVResGLRenderMesh::setIndexData(SVDataSwapPtr _data,s32 _num){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(_data && t_renderer){
        if(_num>m_indexNum) {
            if (m_indexID > 0) {
                glDeleteBuffers(1, &m_indexID);
                m_indexID = 0;
            }
        }
        if(m_indexID==0) {
            glGenBuffers(1, &m_indexID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, _data->getSize(), _data->getData(),m_indexPoolType);
        }else{
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexID);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _data->getSize(), _data->getData() );
        }
        m_indexNum = _num;
    }
}

void SVResGLRenderMesh::setVertexDataNum(s32 _vertexNum){
    if( m_pointNum > 0 && m_pointNum < _vertexNum ){
        if (m_vboID > 0) {
            glDeleteBuffers(1, &m_vboID);
            m_vboID = 0;
        }
    }
    m_pointNum = _vertexNum;
}

void SVResGLRenderMesh::setInstanceOffsetData(SVDataSwapPtr _pdata, u32 _instanceCount){
    if(_pdata){
        if(_instanceCount>m_instacneCount) {
            if (instanceOffsetID > 0) {
                glDeleteBuffers(1, &instanceOffsetID);
                instanceOffsetID = 0;
            }
        }
        if(instanceOffsetID == 0) {
            glGenBuffers(1, &instanceOffsetID);
            glBindBuffer(GL_ARRAY_BUFFER, instanceOffsetID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, instanceOffsetID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
        m_useIntance = true;
        m_instacneCount = _instanceCount;
    }
    
}

void SVResGLRenderMesh::setVertexData(SVDataSwapPtr _data){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(_data && t_renderer){
        if(m_vboID == 0) {
            glGenBuffers(1, &m_vboID);
            glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
            glBufferData(GL_ARRAY_BUFFER,_data->getSize(), _data->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _data->getSize(), _data->getData() );
        }
    }
}

void SVResGLRenderMesh::render(SVRendererBasePtr _renderer) {
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer) {
        return ;
    }
    if (m_useIntance) {
#ifdef __gl3_h_
        if(m_useVAO) {
            //使用vao
            if(m_indexID>0) {
                //索引模式
            }else{
                //非索引模式
                glDrawArraysInstanced(m_drawmethod, 0, m_pointNum, m_instacneCount);
            }
        }else{
            //使用vbo
            if(m_vboID>0) {
                if(m_indexID>0) {
                    //索引绘制
                    //因为索引这块可能大于65536 所以需要分批渲染 by fyz
                    t_renderer->svBindIndexBuffer(m_indexID);
                    t_renderer->svBindVertexBuffer(m_vboID);
                    _updateVertDsp();
                    _updateInstanceDsp();
                    glDrawElementsInstanced(m_drawmethod, m_indexNum, GL_UNSIGNED_SHORT, 0, m_instacneCount);//NUM_FACE_MESHVER
                    t_renderer->svBindVertexBuffer(0);
                    t_renderer->svBindIndexBuffer(0);
                } else {
                    //非索引绘制
                    t_renderer->svBindVertexBuffer(m_vboID);
                    _updateVertDsp();
                    _updateInstanceDsp();
                    glDrawArraysInstanced(m_drawmethod, 0, m_pointNum, m_instacneCount);
                    t_renderer->svBindVertexBuffer(0);
                }
            }
        }
#endif
    }else{
        if(m_useVAO) {
            //使用vao
            if(m_indexID>0) {
                //索引模式
            }else{
                //非索引模式
                glDrawArrays(m_drawmethod, 0, m_pointNum);
            }
        }else{
            //使用vbo
            if(m_vboID>0) {
                if(m_indexID>0) {
                    //索引绘制
                    //因为索引这块可能大于65536 所以需要分批渲染 by fyz
                    t_renderer->svBindIndexBuffer(m_indexID);
                    t_renderer->svBindVertexBuffer(m_vboID);
                    _updateVertDsp();
                    glDrawElements(m_drawmethod, m_indexNum, GL_UNSIGNED_SHORT, 0);//NUM_FACE_MESHVER
                    t_renderer->svBindVertexBuffer(0);
                    t_renderer->svBindIndexBuffer(0);
                } else {
                    //非索引绘制
                    t_renderer->svBindVertexBuffer(m_vboID);
                    _updateVertDsp();
                    glDrawArrays(m_drawmethod, 0, m_pointNum);
                    t_renderer->svBindVertexBuffer(0);
                }
            }
        }
    }
}

void SVResGLRenderMesh::_updateVertDsp() {
     SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        t_renderer->svUpdateVertexFormate(m_vftype);
    }
}

void SVResGLRenderMesh::_updateInstanceDsp(){
#ifdef __gl3_h_
    if (m_useIntance) {
        if (instanceOffsetID > 0) {
            glBindBuffer(GL_ARRAY_BUFFER, instanceOffsetID);
            glEnableVertexAttribArray(CHANNEL_INSOFFSET);
            glVertexAttribPointer(CHANNEL_INSOFFSET, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glVertexAttribDivisor(CHANNEL_INSOFFSET, 1);
        }
    }
#endif
}

void SVResGLRenderMesh::_bindVerts(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        if(m_indexID>0){
            t_renderer->svBindIndexBuffer(m_indexID);
        }else{
            t_renderer->svBindIndexBuffer(0);
        }
        //
        if(m_vboID>0){
            t_renderer->svBindVertexBuffer(m_vboID);
        }else{
            t_renderer->svBindVertexBuffer(0);
        }
    }
}

void SVResGLRenderMesh::_unbindVerts(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        for(s32 i=0;i<8;i++){
            glDisableVertexAttribArray(i);
        }
        if(m_indexID>0){
            t_renderer->svBindIndexBuffer(0);
        }
        if(m_vboID>0){
            t_renderer->svBindVertexBuffer(0);
        }
    }
}

//
SVResGLRenderMeshDvid::SVResGLRenderMeshDvid(SVInst* _app)
        :SVResGLRenderMesh(_app){
            _reset();
}

SVResGLRenderMeshDvid::~SVResGLRenderMeshDvid() {
}

void SVResGLRenderMeshDvid::_reset(){
    SVResGLRenderMesh::_reset();
    vertex2ID_0 = 0;
    vertex2ID_1 = 0;
    vertex2ID_2 = 0;
    vertex3ID = 0;
    color0ID = 0;
    color1ID = 0;
    texcoord0ID = 0;
    texcoord1ID = 0;
    texcoord2ID = 0;
    normalID = 0;
    tagentID = 0;
    btagentID = 0;
}

void SVResGLRenderMeshDvid::create(SVRendererBasePtr _renderer){
    SVResGLRenderMesh::create(_renderer);
}

void SVResGLRenderMeshDvid::destroy(SVRendererBasePtr _renderer) {
    SVResGLRenderMesh::destroy(_renderer);
    if (vertex2ID_0 != 0){
        glDeleteBuffers(1, &vertex2ID_0);
        vertex2ID_0 = 0;
    }
    if (vertex3ID != 0){
        glDeleteBuffers(1, &vertex3ID);
        vertex3ID = 0;
    }
    if (color0ID != 0){
        glDeleteBuffers(1, &color0ID);
        color0ID = 0;
    }
    if (color1ID != 0){
        glDeleteBuffers(1, &color1ID);
        color1ID = 0;
    }
    if (texcoord0ID != 0){
        glDeleteBuffers(1, &texcoord0ID);
        texcoord0ID = 0;
    }
    if (texcoord1ID != 0){
        glDeleteBuffers(1, &texcoord1ID);
        texcoord1ID = 0;
    }
    if (texcoord2ID != 0){
        glDeleteBuffers(1, &texcoord2ID);
        texcoord2ID = 0;
    }
    if (normalID != 0){
        glDeleteBuffers(1, &normalID);
        normalID = 0;
    }
    if (tagentID != 0){
        glDeleteBuffers(1, &tagentID);
        tagentID = 0;
    }
    if (btagentID != 0){
        glDeleteBuffers(1, &btagentID);
        btagentID = 0;
    }
}

//流格式
void SVResGLRenderMeshDvid::_updateVertDsp() {
    if (m_vftype & D_VF_V2) {
        glBindBuffer(GL_ARRAY_BUFFER, vertex2ID_0);
        glEnableVertexAttribArray(CHANNEL_POSITION);
        glVertexAttribPointer(CHANNEL_POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
    if (m_vftype & D_VF_V3) {
        glBindBuffer(GL_ARRAY_BUFFER, vertex3ID);
        glEnableVertexAttribArray(CHANNEL_POSITION);
        glVertexAttribPointer(CHANNEL_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    if (m_vftype & D_VF_NOR) {
        glBindBuffer(GL_ARRAY_BUFFER, normalID);
        glEnableVertexAttribArray(CHANNEL_NORMAL);
        glVertexAttribPointer(CHANNEL_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    if (m_vftype & D_VF_C0) {
        glBindBuffer(GL_ARRAY_BUFFER, color0ID);
        glEnableVertexAttribArray(CHANNEL_COLOR0);
        glVertexAttribPointer(CHANNEL_COLOR0, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
    }
    if (m_vftype & D_VF_C1) {
        glBindBuffer(GL_ARRAY_BUFFER, color1ID);
        glEnableVertexAttribArray(CHANNEL_COLOR1);
        glVertexAttribPointer(CHANNEL_COLOR1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
    }
    if (m_vftype & D_VF_T0) {
        glBindBuffer(GL_ARRAY_BUFFER, texcoord0ID);
        glEnableVertexAttribArray(CHANNEL_TEXCOORD0);
        glVertexAttribPointer(CHANNEL_TEXCOORD0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
    if (m_vftype & D_VF_T1) {
        glBindBuffer(GL_ARRAY_BUFFER, texcoord1ID);
        glEnableVertexAttribArray(CHANNEL_TEXCOORD1);
        glVertexAttribPointer(CHANNEL_TEXCOORD1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
    if (m_vftype & D_VF_T2) {
        glBindBuffer(GL_ARRAY_BUFFER, texcoord2ID);
        glEnableVertexAttribArray(CHANNEL_TEXCOORD2);
        glVertexAttribPointer(CHANNEL_TEXCOORD2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
}

void SVResGLRenderMeshDvid::setVertex2Data(SVDataSwapPtr _pdata){
    if(_pdata){
        if(vertex2ID_0 == 0) {
            glGenBuffers(1, &vertex2ID_0);
            glBindBuffer(GL_ARRAY_BUFFER, vertex2ID_0);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, vertex2ID_0);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setVertex3Data(SVDataSwapPtr _pdata){
    if(_pdata){
        if(vertex3ID == 0) {
            glGenBuffers(1, &vertex3ID);
            glBindBuffer(GL_ARRAY_BUFFER, vertex3ID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, vertex3ID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setColor0Data(SVDataSwapPtr _pdata){
    if(_pdata){
        if(color0ID == 0) {
            glGenBuffers(1, &color0ID);
            glBindBuffer(GL_ARRAY_BUFFER, color0ID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, color0ID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setColor1Data(SVDataSwapPtr _pdata){
    if(_pdata){
        if(color1ID == 0) {
            glGenBuffers(1, &color1ID);
            glBindBuffer(GL_ARRAY_BUFFER, color1ID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, color1ID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setTexcoord0Data(SVDataSwapPtr _pdata){
    if(_pdata){
        if(texcoord0ID == 0) {
            glGenBuffers(1, &texcoord0ID);
            glBindBuffer(GL_ARRAY_BUFFER, texcoord0ID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, texcoord0ID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setTexcoord1Data(SVDataSwapPtr _pdata){
    if(_pdata){
        if(texcoord1ID == 0) {
            glGenBuffers(1, &texcoord1ID);
            glBindBuffer(GL_ARRAY_BUFFER, texcoord1ID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, texcoord1ID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setTexcoord2Data(SVDataSwapPtr _pdata){
    if(_pdata){
        if(texcoord2ID == 0) {
            glGenBuffers(1, &texcoord2ID);
            glBindBuffer(GL_ARRAY_BUFFER, texcoord2ID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, texcoord2ID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setNormalData(SVDataSwapPtr _pdata){
    if(_pdata){
        if(normalID == 0) {
            glGenBuffers(1, &normalID);
            glBindBuffer(GL_ARRAY_BUFFER, normalID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, normalID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setTagentData(SVDataSwapPtr _pdata){
    if(_pdata){
        if(tagentID == 0) {
            glGenBuffers(1, &tagentID);
            glBindBuffer(GL_ARRAY_BUFFER, tagentID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, tagentID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setBTagentData(SVDataSwapPtr _pdata){
    if(_pdata){
        if(btagentID == 0) {
            glGenBuffers(1, &btagentID);
            glBindBuffer(GL_ARRAY_BUFFER, btagentID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, btagentID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::updateConf(RENDERMESHCONF& _conf){
    SVResGLRenderMesh::updateConf(_conf);
}

void SVResGLRenderMeshDvid::updateData(RENDERMESHDATA& _data) {
    setVertexDataNum(_data.pointNum);
    setVertex2Data(_data.pDataV2);
    setVertex3Data(_data.pDataV3);
    setColor0Data(_data.pDataC0);
    setColor1Data(_data.pDataC1);
    setTexcoord0Data(_data.pDataT0);
    setTexcoord1Data(_data.pDataT1);
    setTexcoord2Data(_data.pDataT2);
    setNormalData(_data.pDataNor);
    setTagentData(_data.pDataTag);
    setBTagentData(_data.pDataBTor);
    setIndexData(_data.pDataIndex, _data.indexNum);
    setInstanceOffsetData(_data.pDataInsOffset, _data.instanceCount);
}

void SVResGLRenderMeshDvid::render(SVRendererBasePtr _renderer){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    SVRendererGLPtr t_rendererGL = std::dynamic_pointer_cast<SVRendererGL>(t_renderer);
    if(t_rendererGL) {
        
        if (m_useIntance) {
#ifdef __gl3_h_
            _updateVertDsp();
            _updateInstanceDsp();
            _bindVerts();
            if ( m_indexID>0 ) {
                glDrawElementsInstanced(m_drawmethod, m_indexNum, GL_UNSIGNED_SHORT, 0, m_instacneCount);//NUM_FACE_MESHVER
            } else {
                glDrawArraysInstanced(m_drawmethod, 0, m_pointNum, m_instacneCount);
            }
            _unbindVerts();
#endif
        }else{
            _updateVertDsp();
            _bindVerts();
            if ( m_indexID>0 ) {
                glDrawElements(m_drawmethod, m_indexNum, GL_UNSIGNED_SHORT, 0);//NUM_FACE_MESHVER
            } else {
                glDrawArrays(m_drawmethod, 0, m_pointNum);
            }
            _unbindVerts();
        }
    }
    
}
