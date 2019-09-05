//
// SV2DFaceMaskSTNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SV2DFaceMaskSTNode.h"

#include "../mtl/SVTexture.h"
#include "../mtl/SVMtlFace2D.h"
#include "../mtl/SVTexMgr.h"
#include "../core/SVMathHelper.h"
#include "../basesys/SVConfig.h"
#include "../basesys/SVStaticData.h"
#include "../basesys/SVFaceDataMesh.h"
#include "../rendercore/SVRenderMesh.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderObject.h"
#include "../detect/SVDetectMgr.h"
#include "SVScene.h"

//ST masknode
SV2DFaceMaskSTNode::SV2DFaceMaskSTNode(SVInst *_app)
: SV2DFaceMaskNode(_app) {
    ntype = "SV2DFaceMaskSTNode";
    //
    memset(m_standervers, 0, sizeof(m_standervers));
    memset(m_standertexcoord, 0, sizeof(m_standertexcoord));
    memset(m_newmesh, 0, sizeof(m_newmesh));
    FaceMesh* facemesh = nullptr;
    StanderFace *standerface = nullptr;
    facemesh = mApp->getDataMgr()->getFaceDataMesh()->getFaceMesh("facemesh_st_simplify");
    standerface = mApp->getDataMgr()->getFaceDataMesh()->getStanderFace("standerface_st_simplify");
    
    if (facemesh == nullptr || standerface == nullptr) {
        return;
    }
    m_texwidth = standerface->design_img_width;
    m_texheight = standerface->design_img_height;
    m_pointsize = standerface->facepoints_count;
    for (s32 i=0; i<m_pointsize*2; i++) {
        m_standervers[i] = standerface->points[i];
//        if (i%2 == 0) {
//            m_standervers[i] = m_standervers[i] - m_texwidth*0.5;
//        }else{
//            m_standervers[i] = m_standervers[i] - m_texheight*0.5;
//        }
    }
    
    for (s32 i=0; i<m_pointsize*2; i++) {
        m_standertexcoord[i] = standerface->points[i];
    }
    
    m_meshsize = facemesh->tricount;
    for (s32 i=0; i<m_meshsize*3; i++) {
        m_newmesh[i] = facemesh->triangles[i];
    }
    m_pIndexs->writeData(m_newmesh, m_meshsize*3*sizeof(u16));
    m_pT0->writeData(m_standertexcoord, m_pointsize*2*sizeof(f32));
    m_pVerts->writeData(m_standervers, m_pointsize*2*sizeof(f32));
    if (m_pFaceMesh) {
        m_pFaceMesh->setVertexType(E_VF_V2_T0);
        //这里在 iphone6 上使用 GL_DYNAMIC_DRAW 会导致切动效的时候崩溃，苹果官方文档中说明 GL_STREAM_DRAW 等同于 GL_DYNAMIC_DRAW ，暂时先这样解决。
#if defined(TARGET_OS_IPHONE)
        m_pFaceMesh->setVertexPoolType(GL_STREAM_DRAW);
#else
        m_pFaceMesh->setVertexPoolType(GL_DYNAMIC_DRAW);
#endif
        m_pFaceMesh->setIndexPoolType(GL_STATIC_DRAW);
        m_pFaceMesh->setIndexData(m_pIndexs, m_meshsize*3);
        m_pFaceMesh->setTexcoord0Data(m_pT0);
        m_pFaceMesh->setDrawMethod(E_DM_TRIANGLES);
        m_pFaceMesh->createMesh();
    }
}

SV2DFaceMaskSTNode::~SV2DFaceMaskSTNode() {    
}

void SV2DFaceMaskSTNode::update(f32 dt) {
    SV2DFaceMaskNode::update(dt);
    if (m_pFaceMesh) {
        _updateVerts();
        m_pFaceMesh->setVertex2Data(m_pVerts);
    }
    if (m_mtlFace2D) {
        m_mtlFace2D->update(dt);
        m_mtlFace2D->setModelMatrix(m_absolutMat.get());
        m_mtlFace2D->setBlendEnable(true);
        m_mtlFace2D->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        m_mtlFace2D->setTexcoordFlip(1.0, 1.0);
        m_mtlFace2D->setTexSizeIndex(0, 1.0f/m_texwidth, 1.0f/m_texheight);
    }
    if (m_pFaceMesh && m_mtlFace2D) {
        m_renderObject->setMtl(m_mtlFace2D);
        m_renderObject->setMesh(m_pFaceMesh);
    }
}

void SV2DFaceMaskSTNode::_updateVerts(){
    SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(m_personID);
    if( t_person && t_person->getExist() ){
        s32 t_singleface_ptnum = 106;
        memset(m_faceDateExt, 0, sizeof(f32) * MAX_FACEPOINTS_NUM);
        memcpy(m_faceDateExt, t_person->getFaceData(), 2 * t_singleface_ptnum * sizeof(f32));
        s32 t_camera_w = mApp->getConfig()->getCameraWidth();
        s32 t_camera_h = mApp->getConfig()->getCameraHeight();
        //补点
        SVSTPointExt::st_foreHeadPointExtWithFaceLandMark(m_faceDateExt, t_singleface_ptnum);
//        SVSTPointExt::st_lipsPointExtWithFaceLandMark(m_faceDateExt, t_singleface_ptnum);
//        SVSTPointExt::st_faceOutlinePointExtWithFaceLandMark(m_faceDateExt, t_singleface_ptnum);
//        m_faceDateExt[t_singleface_ptnum*2] = -t_camera_w*0.5;
//        m_faceDateExt[t_singleface_ptnum*2+1] = -t_camera_h*0.5;
//        t_singleface_ptnum++;
//        m_faceDateExt[t_singleface_ptnum*2] = t_camera_w*0.5;
//        m_faceDateExt[t_singleface_ptnum*2+1] = -t_camera_h*0.5;
//        t_singleface_ptnum++;
//        m_faceDateExt[t_singleface_ptnum*2] = t_camera_w*0.5;
//        m_faceDateExt[t_singleface_ptnum*2+1] = t_camera_h*0.5;
//        t_singleface_ptnum++;
//        m_faceDateExt[t_singleface_ptnum*2] = -t_camera_w*0.5;
//        m_faceDateExt[t_singleface_ptnum*2+1] = t_camera_h*0.5;
//        t_singleface_ptnum++;
        m_pVerts->writeData(m_faceDateExt, t_singleface_ptnum * 2 * sizeof(f32));
        m_mtlFace2D->setTexSizeIndex(1, 1.0f/t_camera_w, 1.0f/t_camera_h);
        m_pFaceMesh->setvisible(true);
    }else{
        m_pFaceMesh->setvisible(true);
    }
}

void SV2DFaceMaskSTNode::render(){
    if (!mApp->m_pGlobalParam->m_curScene)
        return;
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (m_renderObject ) {
        m_renderObject->pushCmd(t_rs, RST_MASK2D, "SV2DFaceMaskSTNode");
    }
}

//序列化接口
void SV2DFaceMaskSTNode::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue) {
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    _toJsonData(_allocator, locationObj);
    if (m_mtlFace2D && m_mtlFace2D->getMaskTexture()) {
        SVTexturePtr maskTexture = m_mtlFace2D->getMaskTexture();
        SVString texName = maskTexture->getname();
        s32 pos = texName.rfind('/');
        m_texName = SVString::substr(texName.c_str(), pos+1);
        locationObj.AddMember("texture", RAPIDJSON_NAMESPACE::StringRef(m_texName.c_str()), _allocator);
    }
    _objValue.AddMember("SV2DFaceMaskSTNode", locationObj, _allocator);
}

void SV2DFaceMaskSTNode::fromJSON(RAPIDJSON_NAMESPACE::Value &item) {
    _fromJsonData(item);
    if (item.HasMember("texture") && item["texture"].IsString()) {
        SVString t_textureName = item["texture"].GetString();
        SVString t_texturePath = m_rootPath + t_textureName;
        SVMtlFace2DPtr t_material_face2d = MakeSharedPtr<SVMtlFace2D>(mApp);
        SVTexturePtr tex = mApp->getTexMgr()->getTextureSync(t_texturePath.c_str(), true);
        t_material_face2d->setMaskTexture(tex);
        setMaterial(t_material_face2d);
    }
}

//=====================
void SVSTPointExt::st_foreHeadPointExtWithFaceLandMark(f32 *faceLandMark, s32 &markIndex) {
    f32 t_nose_top_x = faceLandMark[2 * 43];
    f32 t_nose_top_y = faceLandMark[2 * 43 + 1];
    
    f32 t_nose_bottom_x = faceLandMark[2 * 49];
    f32 t_nose_bottom_y = faceLandMark[2 * 49 + 1];
    
    V2 t_v1;
    t_v1.x = t_nose_top_x;
    t_v1.y = t_nose_top_y;
    
    V2 t_v2;
    t_v2.x = t_nose_bottom_x;
    t_v2.y = t_nose_bottom_y;
    f32 t_roll_mask = SVMathHelper::atan2fV2(t_v1, t_v2);
    f32 dx = faceLandMark[2 * 49] - faceLandMark[2 * 43];
    f32 dy = faceLandMark[2 * 49 + 1] - faceLandMark[2 * 43 + 1];
    f32 noseLength = sqrtf(dx * dx + dy * dy);
    
    //补额头上的点
    for (s32 i = 34; i <= 37; i++) {
        f32 x_2 = t_nose_top_x;
        f32 y_2 = t_nose_top_y;
        f32 dx = faceLandMark[2 * i] - faceLandMark[2 * 43];
        f32 dy = faceLandMark[2 * i + 1] - faceLandMark[2 * 43 + 1];
        f32 len = sqrtf(dx * dx + dy * dy);
        V2 r_point{0, 0};
        r_point = SVMathHelper::translateToLeft((s32) x_2, (s32) y_2, t_roll_mask, len);
        r_point = SVMathHelper::translateToUp((s32) r_point.x, (s32) r_point.y, t_roll_mask,
                                              1.5f * noseLength);
        s32 index = markIndex * 2;
        faceLandMark[index] = r_point.x;
        faceLandMark[index + 1] = r_point.y;
        markIndex++;
    }
    for (s32 i = 38; i <= 41; i++) {
        f32 x_1 = faceLandMark[2 * i];
        f32 y_1 = faceLandMark[2 * i + 1];
        f32 x_2 = t_nose_top_x;
        f32 y_2 = t_nose_top_y;
        f32 len = SVMathHelper::vec2Length(x_1, y_1, x_2, y_2);
        f32 dx = faceLandMark[2 * i] - faceLandMark[2 * 43];
        f32 dy = faceLandMark[2 * i + 1] - faceLandMark[2 * 43 + 1];
        len = sqrtf(dx * dx + dy * dy);
        V2 r_point;
        r_point = SVMathHelper::translateToRight((s32) x_2, (s32) y_2, t_roll_mask, len);
        r_point = SVMathHelper::translateToUp((s32) r_point.x, (s32) r_point.y, t_roll_mask,
                                              1.5f * noseLength);
        s32 index = markIndex * 2;
        faceLandMark[index] = r_point.x;
        faceLandMark[index + 1] = r_point.y;
        
        markIndex++;
    }
}

void SVSTPointExt::st_lipsPointExtWithFaceLandMark(f32 *faceLandMark, s32 &markIndex) {
    V2 t_toptip_bottom_originPoint[8];
    s32 j = 0;
    for (s32 i = 96; i <= 103; i++) {
        s32 x = (s32) faceLandMark[2 * i];
        s32 y = (s32) faceLandMark[2 * i + 1];
        V2 point;
        point.x = x;
        point.y = y;
        t_toptip_bottom_originPoint[j] = point;
        j++;
    }
    SVArray<V2> t_toptip_bottom_curvePoint;
    SVMathHelper::createCurve(t_toptip_bottom_originPoint, 8, 3,t_toptip_bottom_curvePoint);
    //
    s32 t_toptip_bottom_curvePointNum = (s32) t_toptip_bottom_curvePoint.size();
    for (s32 i = 0; i < t_toptip_bottom_curvePointNum; i++) {
        V2 point = t_toptip_bottom_curvePoint[i];
        s32 index = markIndex * 2;
        faceLandMark[index] = point.x;
        faceLandMark[index + 1] = point.y;
        markIndex++;
    }
    //
    //
    V2 t_toptip_top_originPoint[12];
    j = 0;
    for (s32 i = 84; i <= 95; i++) {
        s32 x = (s32) faceLandMark[2 * i];
        s32 y = (s32) faceLandMark[2 * i + 1];
        V2 point;
        point.x = x;
        point.y = y;
        t_toptip_top_originPoint[j] = point;
        j++;
        
    }
    SVArray<V2> t_toptip_top_curvePoint;
    SVMathHelper::createCurve(t_toptip_top_originPoint, 12, 3, t_toptip_top_curvePoint);
    s32 t_toptip_top_curvePointNum = (s32) t_toptip_top_curvePoint.size();
    for (s32 i = 0; i < t_toptip_top_curvePointNum; i++) {
        V2 point = t_toptip_top_curvePoint[i];
        s32 index = markIndex * 2;
        faceLandMark[index] = point.x;
        faceLandMark[index + 1] = point.y;
        markIndex++;
    }
    
}


void SVSTPointExt::st_faceOutlinePointExtWithFaceLandMark(f32 *faceLandMark, s32 &markIndex) {
    
    f32 t_nose_tip_x = faceLandMark[2 * 46];
    f32 t_nose_tip_y = faceLandMark[2 * 46 + 1];
    for (s32 i = 0; i < 33; i++) {
        f32 t_x = faceLandMark[2 * i];
        f32 t_y = faceLandMark[2 * i + 1];
        f32 t_p_x = t_x - t_nose_tip_x;
        f32 t_p_y = t_y - t_nose_tip_y;
        f32 p = SVMathHelper::vec2Length(t_p_x, t_p_y, 0, 0);
        V2 t_v1;
        t_v1.x = t_p_x;
        t_v1.y = t_p_y;
        
        V2 t_v2;
        t_v2.x = 0;
        t_v2.y = 0;
        f32 r = SVMathHelper::atan2fV2(t_v1, t_v2);
        //
        f32 x = 1.3 * p * cosf(r);
        f32 y = 1.3 * p * sinf(r);
        s32 index = markIndex * 2;
        faceLandMark[index] = x + t_nose_tip_x;
        faceLandMark[index + 1] = y + t_nose_tip_y;
        markIndex++;
    }
}
