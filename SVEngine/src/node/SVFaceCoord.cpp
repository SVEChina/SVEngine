//
// SVFaceCoord.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFaceCoord.h"
#include "SVCameraNode.h"
#include "SVScene.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMesh.h"
#include "../rendercore/SVRenderMgr.h"
#include "../core/SVGeoGen.h"
#include "../mtl/SVMtlNocolor.h"
#include "../base/SVDataSwap.h"
#include "../base/SVDataChunk.h"
#include "../file/SVFileMgr.h"
#include "SVBMFontNode.h"
#include "../core/SVBMFont.h"

SVFaceCoord::SVFaceCoord(SVInst *_app)
: SVNode(_app) {
    ntype = "SVFaceCoord";
    m_meshLock = MakeSharedPtr<SVLock>();
    m_isScreen = false;
    m_activePt = 0;
    m_size = 5.0f;
    m_aabbBox.clear();
    m_pMesh = MakeSharedPtr<SVRenderMesh>(mApp);
    m_pMeshAct = MakeSharedPtr<SVRenderMesh>(mApp);
    m_pRObjNor = MakeSharedPtr<SVRenderObject>();
    m_pRObjAct = MakeSharedPtr<SVRenderObject>();
}

SVFaceCoord::~SVFaceCoord() {
    m_meshLock = nullptr;
    m_pMesh = nullptr;
    m_pMeshAct = nullptr;
    m_pRObjNor = nullptr;
    m_pRObjAct = nullptr;
}

//加载点位文件
bool SVFaceCoord::loadCoord(cptr8 _fname) {
    //填充数据
    SVDataChunk tDataStream;
    bool tflag = mApp->getFileMgr()->loadFileContentStr(&tDataStream, _fname);
    if (!tflag){
        SV_LOG_INFO("loadCoord error\n");
        return false;
    }
    SV_LOG_INFO("loadCoord sucess\n");
    if (!tDataStream.m_data) {
        return false;
    }
    RAPIDJSON_NAMESPACE::Document doc;
    doc.Parse(tDataStream.m_data);
    if (doc.HasParseError()) {
        RAPIDJSON_NAMESPACE::ParseErrorCode code = doc.GetParseError();
        SV_LOG_ERROR("SVLoaderGLTF :rapidjson error code:%d \n", code);
        return false;
    }
    //
    if (!doc.HasMember("name"))
        return false;
    if (!doc.HasMember("param"))
        return false;
    if (!doc["param"].IsObject())
        return false;
    if (!doc.HasMember("data"))
        return false;
    if (!doc["data"].IsArray())
        return false;
    //
    RAPIDJSON_NAMESPACE::Value &item_name = doc["name"];
    SVString t_filename = item_name.GetString();
    //
    RAPIDJSON_NAMESPACE::Value &item_param = doc["param"];
    if( item_param.HasMember("facepoints_count") ) {
        s32 t_count = item_param["facepoints_count"].GetInt();
    }
    if( item_param.HasMember("design_img_width") ) {
        m_width = item_param["design_img_width"].GetInt();
    }
    if( item_param.HasMember("design_img_height") ) {
        m_height = item_param["design_img_height"].GetInt();
    }
    //清空子节点
    clearChild();
    //
    m_aabbBox.clear();
    RAPIDJSON_NAMESPACE::Value &item_data = doc["data"];
    for (s32 i=0; i<item_data.Size()/2; i++) {
        f32 t_x = item_data[i*2].GetFloat();
        f32 t_y = m_height - item_data[i*2+1].GetFloat();
        m_ptList.append( FVec2(t_x,t_y) );
        m_aabbBox.expand(FVec3(t_x,t_y,0.0f));
        //
        SVBMFontPtr m_font = SVBMFont::creatFnt("/svres/bmfont/sveengine.fnt", mApp);
        SVBMFontNodePtr bmNode = MakeSharedPtr<SVBMFontNode>(mApp);
        bmNode->setFont(m_font);
        SVString t_num;
        bmNode->setText(t_num.format("%d",i).c_str());
        bmNode->setSpacing(0.1);
        bmNode->setScale(0.15, 0.15, 0.0);
        bmNode->setPosition(t_x,t_y,0.0);
        addChild(bmNode);
    }
    return true;
}

void SVFaceCoord::setPointSize(f32 _size) {
    m_size = _size;
}

void SVFaceCoord::active(s32 _pt) {
    m_activePt = _pt;
    refresh();
}

void SVFaceCoord::unActive() {
    m_activePt = -1;
    refresh();
}

//重新构建数据
void SVFaceCoord::refresh() {
    f32 t_sc_w = 1.0f;
    f32 t_sc_h = 1.0f;
    if(m_isScreen) {
//        t_sc_w = ;
//        t_sc_h = ;
    }
    s32 t_count = 0;
    s32 t_count_act = 0;
    //
    SVDataSwapPtr t_dataswap = MakeSharedPtr<SVDataSwap>();
    SVDataSwapPtr t_dataswap_act = MakeSharedPtr<SVDataSwap>();
    for(s32 i=0;i<m_ptList.size();i++) {
        FVec2 t_pt = m_ptList[i];
        //构建mesh
        V3_T0 t_pt_0,t_pt_1,t_pt_2,t_pt_3;
        t_pt_0.x = t_pt.x - 0.5f*m_size*t_sc_w;
        t_pt_0.y = t_pt.y - 0.5f*m_size*t_sc_h;
        t_pt_0.z = 0.0f;
        t_pt_1.x = t_pt.x + 0.5f*m_size*t_sc_w;
        t_pt_1.y = t_pt.y - 0.5f*m_size*t_sc_h;
        t_pt_1.z = 0.0f;
        t_pt_2.x = t_pt.x - 0.5f*m_size*t_sc_w;
        t_pt_2.y = t_pt.y + 0.5f*m_size*t_sc_h;
        t_pt_2.z = 0.0f;
        t_pt_3.x = t_pt.x + 0.5f*m_size*t_sc_w;
        t_pt_3.y = t_pt.y + 0.5f*m_size*t_sc_h;
        t_pt_3.z = 0.0f;
        //
        t_pt_0.t0x = 0.0f;
        t_pt_0.t0y = 0.0f;
        t_pt_1.t0x = 1.0f;
        t_pt_1.t0y = 0.0f;
        t_pt_2.t0x = 0.0f;
        t_pt_2.t0y = 1.0f;
        t_pt_3.t0x = 1.0f;
        t_pt_3.t0y = 1.0f;
        //
        if(i == m_activePt) {
            t_dataswap_act->appendData(&t_pt_0, sizeof(V3_T0));
            t_dataswap_act->appendData(&t_pt_1, sizeof(V3_T0));
            t_dataswap_act->appendData(&t_pt_2, sizeof(V3_T0));
            t_dataswap_act->appendData(&t_pt_2, sizeof(V3_T0));
            t_dataswap_act->appendData(&t_pt_1, sizeof(V3_T0));
            t_dataswap_act->appendData(&t_pt_3, sizeof(V3_T0));
            t_count_act++;
        }else{
            t_dataswap->appendData(&t_pt_0, sizeof(V3_T0));
            t_dataswap->appendData(&t_pt_1, sizeof(V3_T0));
            t_dataswap->appendData(&t_pt_2, sizeof(V3_T0));
            t_dataswap->appendData(&t_pt_2, sizeof(V3_T0));
            t_dataswap->appendData(&t_pt_1, sizeof(V3_T0));
            t_dataswap->appendData(&t_pt_3, sizeof(V3_T0));
            t_count++;
        }
    }
    //普通
    m_pMesh->setVertexType(E_VF_V3_T0);
    m_pMesh->setVertexDataNum(t_count*6);
    m_pMesh->setVertexData(t_dataswap);
    m_pMesh->setDrawMethod(E_DM_TRIANGLES);
    m_pMesh->createMesh();
    //act
    m_pMeshAct->setVertexType(E_VF_V3_T0);
    m_pMeshAct->setVertexDataNum(t_count_act*6);
    m_pMeshAct->setVertexData(t_dataswap_act);
    m_pMeshAct->setDrawMethod(E_DM_TRIANGLES);
    m_pMeshAct->createMesh();
}

void SVFaceCoord::update(f32 dt) {
    SVNode::update(dt);
    //渲染普通点(绿色)
    if (m_pRObjNor && m_pMesh) {
        SVMtlGeo3dPtr t_mtl_box = MakeSharedPtr<SVMtlGeo3d>(mApp);
        t_mtl_box->setColor(0.1f, 0.80f, 0.05f, 1.0f);
        t_mtl_box->update(dt);
        t_mtl_box->setModelMatrix(m_absolutMat.get());
        t_mtl_box->setTexcoordFlip(1.0, -1.0f);
        t_mtl_box->setDepthEnable(false);
        t_mtl_box->setBlendEnable(true);
        t_mtl_box->setBlendState(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        m_pRObjNor->setMesh(m_pMesh);
        m_pRObjNor->setMtl(t_mtl_box);
    }
    //渲染激活点(红色)
    if (m_pRObjAct && m_pMesh && m_activePt>=0 ) {
        SVMtlGeo3dPtr t_mtl_box = MakeSharedPtr<SVMtlGeo3d>(mApp);
        t_mtl_box->setColor( 0.80f, 0.02f, 0.05f, 1.0f);
        t_mtl_box->update(dt);
        t_mtl_box->setModelMatrix(m_absolutMat.get());
        t_mtl_box->setTexcoordFlip(1.0, -1.0f);
        t_mtl_box->setDepthEnable(false);
        t_mtl_box->setBlendEnable(true);
        t_mtl_box->setBlendState(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        m_pRObjAct->setMesh(m_pMeshAct);
        m_pRObjAct->setMtl(t_mtl_box);
    }
}

void SVFaceCoord::render() {
    if (mApp->m_pGlobalParam->m_curScene && m_visible ){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (m_pRObjNor) {
            m_pRObjNor->pushCmd(t_rs, RST_SKY, "SVFaceCoord-nor");
        }
        //
        if (m_pRObjAct && m_activePt>=0 ) {
            m_pRObjAct->pushCmd(t_rs, RST_SKY, "SVFaceCoord-act");
        }
    }
    SVNode::render();
}










