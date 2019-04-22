//
// SVDeformImageMove.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVDeformImageMove.h"
#include "../node/SVScene.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"
#include "../event/SVOpEvent.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVStaticData.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMesh.h"
#include "../rendercore/SVRenderTexture.h"
#include "../rendercore/SVRenderCmd.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRendererBase.h"
#include "../core/SVGeoGen.h"
#include "../core/SVPass.h"
#include "../app/SVGlobalMgr.h"
#include "../mtl/SVMtlFaceShape.h"
#include "../mtl/SVMtlShapeVaried.h"
#include "../mtl/SVTexMgr.h"
#include "../base/SVVec2.h"
#include "../node/SVCameraNode.h"
#include "../node/SVNode.h"
#include "../detect/SVDetectMgr.h"
#include "../detect/SVDetectST.h"
#include "../core/SVMathHelper.h"


SVParamDeform::SVParamDeform(){
}

SVParamDeform::~SVParamDeform(){
}

SVDeformImageMove::SVDeformImageMove(SVInst *_app)
:SVGBase(_app){
    m_pMtlBg  = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    m_pIUMP    = MakeSharedPtr<SVImageUsingMove>();
    m_pPointTex = mApp->getTexMgr()->getTextureSync("svres/point.png",true);
    m_param = MakeSharedPtr<SVParamDeform>();
    m_param->reset();
    m_passDeform = nullptr;
    m_passPoint = nullptr;
    m_passBack = nullptr;
    m_pMeshBg = nullptr;
    m_pMeshPoint = mApp->getRenderMgr()->createMeshRObj();
    m_pMeshPoint->createMesh();
    m_rule=1;
    m_tt_w = 0;
    m_tt_h = 0;
    m_dataPoint = nullptr;
    m_fbo = nullptr;
    m_wPointCount = 51;
    m_hPointCont = 64;
    m_inw = 10;
    m_inh = 10;
    m_flip = false;
    is_swith = true;
    m_is_point = false;
    is_detect = false;
    
}

SVDeformImageMove::~SVDeformImageMove(){
    m_passDeform = nullptr;
    m_param=nullptr;
    m_pMeshBg = nullptr;
    m_pMtlBg = nullptr;
    m_pIUMP = nullptr;
    m_dataPoint = nullptr;
    m_fbo = nullptr;
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        t_renderer->destroySVTex(E_TEX_FILTER_DEFORM02);
    }
}

void SVDeformImageMove::init(SVTexturePtr _intex,SVTexturePtr _texout){
    SVRendererBasePtr t_renderer =  mApp->getRenderer();
    if(t_renderer && _intex && _texout){
        m_tt_w = _intex->getwidth();
        m_tt_h = _intex->getheight();
       
        _initPoint();
        t_renderer->createSVTex(E_TEX_FILTER_DEFORM02, m_tt_w, m_tt_h, GL_RGBA);
        SVTexturePtr t_tex = mApp->getRenderer()->getSVTex(E_TEX_FILTER_DEFORM02);
        m_fbo = MakeSharedPtr<SVRenderTexture>(mApp,
                                               t_tex,
                                               false,
                                               false);
        mApp->getRenderMgr()->pushRCmdCreate(m_fbo);
       
        //设置该fbo的矩阵关系
        SVCameraNode t_camera(mApp);
        t_camera.resetCamera(m_tt_w, m_tt_h,120.0f);
        
        m_fbo->setLink(true);
        m_fbo->setProjMat(t_camera.getProjectMatObj());
        m_fbo->setViewMat(t_camera.getViewMatObj());
       
        //
        m_passDeform = MakeSharedPtr<SVPass>();
        m_passDeform->setMtl(m_pMtlBg);
        m_passDeform->setMesh(m_pMeshBg);
        m_passDeform->setInTex(0,_intex);
        m_passDeform->setOutTex(E_TEX_FILTER_DEFORM02);
        
        SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp, "normal2dcolor");
        t_mtl->setBlendEnable(false);
        t_mtl->setModelMatrix(t_camera.getAbsoluteMat());
        t_mtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        t_mtl->setTexcoordFlip(1.0, -1.0);
        t_mtl->setBlendEnable(true);
        t_mtl->setBlendState(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
 
        m_passPoint = MakeSharedPtr<SVPass>();
        m_passPoint->setMtl(t_mtl);
        m_passPoint->setMesh(m_pMeshPoint);
        m_passPoint->setInTex(0,m_pPointTex);
        m_passPoint->setOutTex(E_TEX_FILTER_DEFORM02);
        
        
        t_mtl = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
        t_mtl->setTexcoordFlip(1.0, -1.0);
        m_passBack = MakeSharedPtr<SVPass>();
        m_passBack->setMtl(t_mtl);
        m_passBack->setMesh(mApp->getDataMgr()->m_screenMesh);
        m_passBack->setInTex(0,E_TEX_FILTER_DEFORM02);
        m_passBack->setOutTex(_texout);
    }
}

void SVDeformImageMove::setPoint(V2 *_data){
    m_dataPoint = _data;
}

void SVDeformImageMove::updatePointMSL(){
    if(m_dataPoint){
        m_pIUMP->clearContrl();
        pointMove(m_dataPoint);
    }
}

void SVDeformImageMove::_initPoint(){
    if(!m_pMeshBg) {
        m_pMeshBg = mApp->getRenderMgr()->createMeshRObj();
    }
    //索引数据
    u16 m_dataIndex[51*64*2*3];//41*81*2*3
    //
    s32 iWidthPoint = m_wPointCount , iHeightPoint = m_hPointCont;
    s32 iDataCount = iWidthPoint * iHeightPoint;
    s32 iWidthRect  = iWidthPoint-1 ;
    s32 iHeightRect = iHeightPoint-1;
    f32 fWidthDelta =  m_tt_w / iWidthRect;
    f32 fHeightDelta = m_tt_h / iHeightRect;
    s32 iDataIndex = 0;
    memset(m_pointScreen , 0 , sizeof(V2) * iDataCount);
    for (s32 i = 0 ; i < iHeightPoint ; ++i){
        for (s32 j = 0 ; j < iWidthPoint ; ++j ){
            m_pointScreen[iDataIndex].x = fWidthDelta * j;
            m_pointScreen[iDataIndex].y = fHeightDelta * i;
            iDataIndex += 1;
        }
    }
    s32 iRectCount  = iWidthRect * iHeightRect;
    s32 m_iIndexCount = iRectCount * 2 * 3;
    s32 iIndexIndex = 0;
    for (s32 i = 0 ; i < iHeightRect ; ++i){
        for (s32 j = 0; j < iWidthRect ; ++j){
            m_dataIndex[iIndexIndex]     = i * iWidthPoint + j;
            m_dataIndex[iIndexIndex + 1] = i * iWidthPoint + j + 1;
            m_dataIndex[iIndexIndex + 2] = (i + 1) * iWidthPoint + j;
            m_dataIndex[iIndexIndex + 3] = (i + 1) * iWidthPoint + j;
            m_dataIndex[iIndexIndex + 4] = i * iWidthPoint + j + 1;
            m_dataIndex[iIndexIndex + 5] = (i + 1) * iWidthPoint + j + 1;
            iIndexIndex += 6;
        }
    }
    //第一次更新索引数据
    SVDataSwapPtr t_index = MakeSharedPtr<SVDataSwap>();
    t_index->writeData((void  *)m_dataIndex, m_iIndexCount * sizeof(u16));
    m_pMeshBg->setIndexData(t_index, m_iIndexCount);
    //第一次更新顶点数据
    _refreshScreenRectMesh(m_pointScreen,m_pointScreen);
    //创建mesh
    m_pMeshBg->createMesh();
}

void SVDeformImageMove::_refreshScreenRectMesh(V2 *_data,V2 *_targetData){
    if(m_pMeshBg) {
        //渲染数据
        V2_C_T0 pVer[m_wPointCount*m_hPointCont];
        for (s32 i = 0; i < m_wPointCount*m_hPointCont ; ++i){
            f32 x= _data[i].x/m_tt_w;
            f32 y= _data[i].y/m_tt_h;
            f32 t_x= _targetData[i].x/m_tt_w;
            f32 t_y= _targetData[i].y/m_tt_h;
            pVer[i].x = x*2.0-1.0;
            pVer[i].y = y*2.0-1.0;
            pVer[i].t0x = t_x;
            pVer[i].t0y = t_y;
            pVer[i].r = 255;
            pVer[i].g = 255;
            pVer[i].b = 255;
            pVer[i].a = 255;
        }
        m_pMeshBg->setVertexType(E_VF_V2_C_T0);
        SVDataSwapPtr t_datav = MakeSharedPtr<SVDataSwap>();
        t_datav->writeData(pVer, sizeof(V2_C_T0) * m_wPointCount * m_hPointCont);
        m_pMeshBg->setVertexDataNum( m_wPointCount * m_hPointCont );
        m_pMeshBg->setVertexData(t_datav);
    }
}

void SVDeformImageMove::update(f32 _dt){
    if(!is_swith){
        return;
    }
 
    if(is_detect){
        m_pIUMP->clearContrl();
        for(int i=1;i<=4;i++){
            SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(i);
            if( t_person && t_person->getExist()){
                V2 *t_data = (V2*)t_person->getFaceDataOriginal();
                _updateControl(t_data);
            }
        }
        _updateMesh();
    }
    //
    if(m_passDeform && m_passDeform->m_pMtl){
        m_passDeform->m_pMtl->update(_dt);
    }
    
    if(m_passPoint && m_passPoint->m_pMtl){
        m_passPoint->m_pMtl->update(_dt);
    }
}

void SVDeformImageMove::render(){
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (is_swith && t_rs && false  == t_rs->isSuspend() ) {
        if(m_passDeform && m_passDeform->m_pMtl){
            SVRenderCmdPassCollectionPtr t_cmd = MakeSharedPtr<SVRenderCmdPassCollection>();
            t_cmd->mTag = "SVBackGroundNode";
            t_cmd->setFbo(m_fbo);
            //图片翻转这块需要校正一下 by fyz
            if(m_pMtlBg){
                if(m_flip){
                    m_pMtlBg->setTexcoordFlip(1.0f, -1.0f);
                }else{
                    m_pMtlBg->setTexcoordFlip(1.0f, 1.0f);
                }
            }
            //
            if( m_passDeform->m_outTexType == E_TEX_END ) {
                t_cmd->setTexture(m_passDeform->m_outTex);
            }else{
                SVTexturePtr t_tex = mApp->getRenderer()->getSVTex(m_passDeform->m_outTexType);
                t_cmd->setTexture(t_tex);
            }
            if(m_passDeform->m_pMesh){
                t_cmd->addMtlMesh(m_passDeform->m_pMtl,m_passDeform->m_pMesh);
            }else{
                t_cmd->addMtlMesh(m_passDeform->m_pMtl,mApp->getDataMgr()->m_screenMesh);
            }
            //t_cmd->setMaterial(m_passDeform->m_pMtl);
            if(m_passPoint->m_pMtl&&m_is_point){
                 t_cmd->addMtlMesh(m_passPoint->m_pMtl,m_passPoint->m_pMesh);
            }
            t_rs->pushRenderCmd(RST_FACEMORPH, t_cmd);//m_rsType
        }
        
        if(m_passBack){
            SVRenderCmdPassPtr t_cmd = MakeSharedPtr<SVRenderCmdPass>();
            t_cmd->mTag = "SVFaceDeform";
            t_cmd->setFbo(m_fbo);
            t_cmd->setTexture(m_passBack->m_outTex);
            t_cmd->setMesh(m_passBack->m_pMesh);
            if(m_flip){
                m_passBack->m_pMtl->setTexcoordFlip(1.0f, -1.0f);
            }else{
                m_passBack->m_pMtl->setTexcoordFlip(1.0f, 1.0f);
            }
            t_cmd->setMaterial(m_passBack->m_pMtl);
            t_rs->pushRenderCmd(RST_FACEMORPH, t_cmd);
        }
    }
}

s32 SVDeformImageMove::getWidth(){
    return m_tt_w;
}

s32 SVDeformImageMove::getHeight(){
    return m_tt_h;
}

void SVDeformImageMove::setDeformSwitch(bool _swith){
    is_swith=_swith;
}

V2 SVDeformImageMove::MSL(V2 point){
    if(m_pIUMP) {
        FVec2 t_xy = m_pIUMP->MLS(FVec2(point.x,point.y));
        V2 xy;
        xy.x = t_xy.x;
        xy.y = t_xy.y;
        return xy;
    }
    V2 xy;
    xy.x = 0;
    xy.y = 0;
    return xy;
}

void SVDeformImageMove::setTagPoint(u32 _postion,V2 _point){
    m_param->m_pointMap.append(_postion, _point);
}

void SVDeformImageMove::setTagAreaPoint(u32 _postion,V2 _point){
    m_param->m_areaPoint.append(_postion, _point);
}

void SVDeformImageMove::pointMove(V2 *t_data){
    V2 t_outlinePoints[106];
    for(s32 i  = 0 ; i < 106 ; i++){
        t_outlinePoints[i].x = t_data[i].x;
        t_outlinePoints[i].y = t_data[i].y;
    }
    FVec2 eyer = FVec2(t_outlinePoints[77].x,t_outlinePoints[77].y);
    FVec2 eyel = FVec2(t_outlinePoints[74].x,t_outlinePoints[74].y);
    
    f32 leng = getDistanceFrom(eyer,eyel);
    f32 _smooth = (leng/240.0);
    FVec2 t_eyel = eyer-eyel;
    f64 angle = atan2(t_eyel.y, t_eyel.x) * 180.0/PI;
    m_pIUMP->setControl(FVec2(m_tt_w*0.5f,m_tt_h*0.5f));
    m_pIUMP->setTargetControl(FVec2(m_tt_w*0.5f,m_tt_h*0.5f));
    m_pIUMP->setControl(FVec2(m_tt_w,m_tt_h));
    m_pIUMP->setTargetControl(FVec2(m_tt_w,m_tt_h));
    m_pIUMP->setControl(FVec2(m_tt_w,0.0));
    m_pIUMP->setTargetControl(FVec2(m_tt_w,0.0));
    
    FVec2 t_rangleV2(t_outlinePoints[46].x,t_outlinePoints[46].y);
    //迭代偏移值
    SVMap<u32,V2>::Iterator it = m_param->m_pointMap.begin();
    while (it!=m_param->m_pointMap.end()) {
        u32 t_postion = it->key;
        V2 t_point = it->data;
        m_pIUMP->setControl(FVec2(t_outlinePoints[t_postion].x+1,t_outlinePoints[t_postion].y+1));
        FVec2 point_v = FVec2(t_outlinePoints[t_postion].x,t_outlinePoints[t_postion].y);
        point_v = rotateBy(-angle,point_v,t_rangleV2);
        point_v = FVec2(point_v.x+t_point.x*_smooth,point_v.y+t_point.y*_smooth);
        point_v = rotateBy(angle,point_v,t_rangleV2);
        m_pIUMP->setTargetControl(point_v);
        it++;
    }

    //
    V2 t_targetData[m_wPointCount*m_hPointCont];
    for(s32 i=0;i< m_wPointCount*m_hPointCont;i++){
        if(m_pointScreen[i].x == 0.0
           ||m_pointScreen[i].x == m_tt_w
           ||m_pointScreen[i].y == 0.0
           ||m_pointScreen[i].y == m_tt_h){
            t_targetData[i].x = m_pointScreen[i].x;
            t_targetData[i].y = m_pointScreen[i].y;
        }else{
            FVec2 t_xy=m_pIUMP->MLS(FVec2(m_pointScreen[i].x,m_pointScreen[i].y));
            t_targetData[i].x=t_xy.x;
            t_targetData[i].y=t_xy.y;
        }
    }
    //
    _refreshScreenRectMesh(t_targetData, m_pointScreen);
}

void SVDeformImageMove::_updateControl(V2 *t_data){
    V2 t_outlinePoints[106];
    for(s32 i  = 0 ; i < 106 ; i++){
        t_outlinePoints[i].x = t_data[i].x;
        t_outlinePoints[i].y = t_data[i].y;
    }
    FVec2 eyer = FVec2(t_outlinePoints[77].x,t_outlinePoints[77].y);
    FVec2 eyel = FVec2(t_outlinePoints[74].x,t_outlinePoints[74].y);
    
    f32 leng = getDistanceFrom(eyer,eyel);
    f32 _smooth = (leng/240.0);
    FVec2 t_eyel = eyer-eyel;
    f64 angle = atan2(t_eyel.y, t_eyel.x) * 180.0/PI;
    m_pIUMP->setControl(FVec2(m_tt_w*0.5f,m_tt_h*0.5f));
    m_pIUMP->setTargetControl(FVec2(m_tt_w*0.5f,m_tt_h*0.5f));
    m_pIUMP->setControl(FVec2(m_tt_w,m_tt_h));
    m_pIUMP->setTargetControl(FVec2(m_tt_w,m_tt_h));
    m_pIUMP->setControl(FVec2(m_tt_w,0.0));
    m_pIUMP->setTargetControl(FVec2(m_tt_w,0.0));
    FVec2 t_rangleV2(t_outlinePoints[46].x,t_outlinePoints[46].y);
    //迭代偏移值
    SVMap<u32,V2>::Iterator it = m_param->m_pointMap.begin();
    while (it!=m_param->m_pointMap.end()) {
        u32 t_postion = it->key;
        V2 t_point = it->data;
        m_pIUMP->setControl(FVec2(t_outlinePoints[t_postion].x,t_outlinePoints[t_postion].y));
      //  if(t_point.x!=0.0&&t_point.y!=0.0){
            FVec2 point_v = FVec2(t_outlinePoints[t_postion].x,t_outlinePoints[t_postion].y);
            point_v = rotateBy(-angle,point_v,t_rangleV2);
            point_v = FVec2(point_v.x+t_point.x*_smooth,point_v.y+t_point.y*_smooth);
            point_v = rotateBy(angle,point_v,t_rangleV2);
            m_pIUMP->setTargetControl(point_v);
        //}
        it++;
    }
}

void SVDeformImageMove::_updateMesh(){
    
    V2 t_targetData[m_wPointCount*m_hPointCont];
    for(s32 i=0;i< m_wPointCount*m_hPointCont;i++){
        if(m_pointScreen[i].x == 0.0
           ||m_pointScreen[i].x == m_tt_w
           ||m_pointScreen[i].y == 0.0
           ||m_pointScreen[i].y == m_tt_h){
            t_targetData[i].x = m_pointScreen[i].x;
            t_targetData[i].y = m_pointScreen[i].y;
        }else{
            FVec2 t_xy=m_pIUMP->MLS(FVec2(m_pointScreen[i].x,m_pointScreen[i].y));
            t_targetData[i].x=t_xy.x;
            t_targetData[i].y=t_xy.y;
        }
    }
    //
    _refreshScreenRectMesh(t_targetData,m_pointScreen );
}

//serial 序列化接口
void SVDeformImageMove::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_aloc,
                               RAPIDJSON_NAMESPACE::Value &_objValue){
    RAPIDJSON_NAMESPACE::Value t_obj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    RAPIDJSON_NAMESPACE::Value t_array(RAPIDJSON_NAMESPACE::kArrayType);
    SVMap<u32,V2>::Iterator it= m_param->m_pointMap.begin();
    while (it!=m_param->m_pointMap.end()) {
        u32 t_postion=it->key;
        V2 t_point=it->data;
        RAPIDJSON_NAMESPACE::Value t_cell(RAPIDJSON_NAMESPACE::kObjectType);
        t_cell.AddMember("index", t_postion , _aloc);
        t_cell.AddMember("x", t_point.x , _aloc);
        t_cell.AddMember("y", t_point.y , _aloc);
        t_array.PushBack(t_cell,_aloc);
        it++;
    }
    t_obj.AddMember("ptoff", t_array, _aloc);
    _objValue.AddMember("sv_deform", t_obj, _aloc);
}

void SVDeformImageMove::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    m_param->reset();
    if (item.HasMember("ptoff") && item["ptoff"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value locationArray=item["ptoff"].GetArray();
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
            m_param->m_pointMap.append(t_postion,t_point);
        }
    }
}

void SVDeformImageMove::reset(){
    m_param->reset();
    updatePointMSL();
    updatePointMesh(m_dataPoint);
}

void SVDeformImageMove::updatePointMesh( V2* _facepoint){
    V2_C_T0 verts[636];
    for(s32 i=0;i<106;i++){
        f32 _inx=_facepoint[i].x;
        f32 _iny=_facepoint[i].y;
        f32 _x=_inx-m_tt_w/2;
        f32 _y=_iny-m_tt_h/2;
        u8 t_r=0;
        u8 t_g=255;
        u8 t_b=0;
        u8 t_a=255;
        SVMap<u32,V2>::Iterator it= m_param->m_pointMap.begin();
        while (it!=m_param->m_pointMap.end()) {
            u32 t_postion=it->key;
            V2 t_point=it->data;
            if(i==t_postion&&(t_point.x!=0.0||t_point.y!=0.0)){
                t_r=255;
                t_g=0;
                t_b=0;
                t_a=255;
            }
            it++;
        }
        //   V2_T0 verts[4];
        verts[i*6].x = -0.5f * m_inw+_x;
        verts[i*6].y = -0.5f * m_inh+_y;
        verts[i*6].t0x = 0.0;
        verts[i*6].t0y = 0.0;
        verts[i*6].r = t_r;
        verts[i*6].g = t_g;
        verts[i*6].b = t_b;
        verts[i*6].a = t_a;
        
        verts[i*6+1].x = 0.5f * m_inw+_x;
        verts[i*6+1].y = -0.5f * m_inh+_y;
        verts[i*6+1].t0x = 1.0;
        verts[i*6+1].t0y = 0.0;
        verts[i*6+1].r = t_r;
        verts[i*6+1].g = t_g;
        verts[i*6+1].b = t_b;
        verts[i*6+1].a = t_a;
        
        verts[i*6+2].x = -0.5f * m_inw+_x;
        verts[i*6+2].y = 0.5f * m_inh+_y;
        verts[i*6+2].t0x = 0.0;
        verts[i*6+2].t0y = 1.0;
        verts[i*6+2].r = t_r;
        verts[i*6+2].g = t_g;
        verts[i*6+2].b = t_b;
        verts[i*6+2].a = t_a;
        
        verts[i*6+3].x = -0.5f * m_inw+_x;
        verts[i*6+3].y = 0.5f * m_inh+_y;
        verts[i*6+3].t0x = 0.0;
        verts[i*6+3].t0y = 1.0;
        verts[i*6+3].r = t_r;
        verts[i*6+3].g = t_g;
        verts[i*6+3].b = t_b;
        verts[i*6+3].a = t_a;
        
        verts[i*6+4].x = 0.5f * m_inw+_x;
        verts[i*6+4].y = 0.5f * m_inh+_y;
        verts[i*6+4].t0x = 1.0;
        verts[i*6+4].t0y = 1.0;
        verts[i*6+4].r = t_r;
        verts[i*6+4].g = t_g;
        verts[i*6+4].b = t_b;
        verts[i*6+4].a = t_a;
        
        verts[i*6+5].x = 0.5f * m_inw+_x;
        verts[i*6+5].y = -0.5f * m_inh+_y;
        verts[i*6+5].t0x = 1.0;
        verts[i*6+5].t0y = 0.0;
        verts[i*6+5].r = t_r;
        verts[i*6+5].g = t_g;
        verts[i*6+5].b = t_b;
        verts[i*6+5].a = t_a;
        //
    }
    SVDataSwapPtr t_data = MakeSharedPtr<SVDataSwap>();
    t_data->writeData(&verts[0], sizeof(V2_C_T0) * 636);
    m_pMeshPoint->setVertexDataNum(636);
    m_pMeshPoint->setVertexData(t_data);
    m_pMeshPoint->setVertexType(E_VF_V2_C_T0);
    m_pMeshPoint->setDrawMethod(E_DM_TRIANGLES);
}
