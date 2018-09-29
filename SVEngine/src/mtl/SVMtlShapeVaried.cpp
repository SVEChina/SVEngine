//
//  SVMtlShapeVaried.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/7/17.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVMtlShapeVaried.h"
#include "../base/SVVec2.h"
#include "../app/SVGlobalMgr.h"
#include "../detect/SVDetectMgr.h"
#include "../detect/SVDetectBase.h"
#include "../detect/SVDetectST.h"
#include "../detect/SVPerson.h"
#include "../rendercore/renderer/SVRendererBase.h"

f32 representParam[24][9]={
    {0,1,-0.688264,-0.666764,0.226535,0.450114,95,0.809862,0.4 },
    {0,1,0.688266,-0.666764,-0.226534,0.450111,91,0.809862,0.4 },
    {0,1,-0.548707,-0.16081,0.675761,-0.114451,3,0.55634,0.2 },
    {0,1,0.548707,-0.160814,-0.67576,-0.11445,29,0.55634,0.2 },
    {0,1,-1.33039,-0.32404,1.83246,0.558574,8,0.55634,0.2 },
    {0,1,1.33039,-0.32404,-1.83246,0.558574,24,0.55634,0.2 },
    {0,1,-0.530478,-0.289089,0.901556,0.0539721,10,0.55634,0.2 },
    {0,1,0.530478,-0.289088,-0.901556,0.0539715,22,0.55634,0.2 },
    {0,1,-1.87309,-0.32635,1.1553,0.137814,3,0.704228,0.2 },
    {0,1,1.87309,-0.326351,-1.1553,0.137814,29,0.704228,0.2 },
    {0,1,-0.17934,-0.361884,0.281838,0.0555995,6,0.422537,0.2 },
    {0,1,0.179339,-0.361883,-0.281837,0.0556004,26,0.422537,0.2 },
    {0,1,-0.227799,-0.0730677,0.902074,0.251155,15,0.422537,0.2 },
    {0,1,0.227798,-0.0730652,-0.902073,0.251154,17,0.422537,0.2 },
    {0,1,-0.412645,-0.0840469,0.423275,0.280581,11,0.422537,0.2 },
    {0,1,0.412646,-0.0840468,-0.423274,0.280582,21,0.422537,0.2 },
    {0,1,-0.0898366,-0.055978,0.478356,-0.19844,55,0.0774651,0.12 },
    {0,1,0.0898376,-0.0559793,-0.478355,-0.198442,58,0.0774651,0.12 },
    {1,1,0,0,0.139937,-0.146538,77,0.24648,0.08 },
    {1,1,0,0,-0.139938,-0.146538,74,0.24648,0.08 },
    {0,1,0.0134553,0.0612143,0.338176,0.0554508,82,0.211268,0.3 },
    {0,1,-0.0134554,0.0612136,-0.338176,0.0554508,83,0.211268,0.3 },
    {0,1,-0.142053,-0.14531,-0.225204,0.0569295,12,0.492959,0.3 },
    {0,1,0.142054,-0.145311,0.225204,0.0569302,20,0.492959,0.3 },
};

f32 percentages[14] ={
    0.5,   //--下巴
    0.5,  // --大眼  (origin脸型初始值为0，其他5个脸型初始值为0.5）
    0.5,  // --瘦鼻
    0.5,  // --瘦脸 （origin脸型初始值为0，其他5个脸型初始值为0.5）
    0.5,  // --额头
    0.5,  // --小脸
    0.5,  // --削脸
    0.5,  // --长鼻
    0.5,  // --嘴型
    0.0,  // --开眼角
    0.0,  // --微笑嘴角
    0.5,  // --移动眼睛 没有滑杆
    0.5,  // --放大眼睛 没有滑杆
    0.5,  //--移动嘴巴 没有滑杆
};

SVMtlFaceShapeVaried::SVMtlFaceShapeVaried(SVInst *_app)
:SVMtlCore(_app,"faceShapeVaried"){
    memcpy( m_g_intensity   ,   percentages , sizeof( f32 ) * 14);
    m_surfaceWidth      =   mApp->m_pGlobalParam->m_inner_width;
    m_surfaceHeight     =   mApp->m_pGlobalParam->m_inner_height;
    m_inv_surfaceWidth  =   1.0/m_surfaceWidth;
    m_inv_surfaceHeight =   1.0/m_surfaceHeight;
}

SVMtlFaceShapeVaried::SVMtlFaceShapeVaried(SVMtlFaceShapeVaried *_mtl)
:SVMtlCore(_mtl){
    m_eyesDistance              =   _mtl->m_eyesDistance;
    m_inversedEyesDistance      =   _mtl->m_inversedEyesDistance;
    m_surfaceWidth              =   _mtl->m_surfaceWidth;
    m_surfaceHeight             =   _mtl->m_surfaceHeight;
    m_inv_surfaceWidth          =   _mtl->m_inv_surfaceWidth;
    m_inv_surfaceHeight         =   _mtl->m_inv_surfaceHeight;
    
    memcpy(m_startPoint    ,    _mtl->m_startPoint      , sizeof(f32)*48);
    memcpy(m_endPoint      ,    _mtl->m_endPoint        , sizeof(f32)*48);
    memcpy(m_actionType    ,    _mtl->m_actionType      , sizeof(f32)*24);
    memcpy(m_intensity     ,    _mtl->m_intensity       , sizeof(f32)*24);
    memcpy(m_radius        ,    _mtl->m_radius          , sizeof(f32)*24);
    memcpy(m_RealStep      ,    _mtl->m_RealStep        , sizeof(f32)*1);
    memcpy(m_outlinePoints ,    _mtl->m_outlinePoints   , sizeof(f32)*212);
    memcpy(m_g_intensity   ,    _mtl->m_g_intensity     , sizeof(f32)*14);
}

SVMtlFaceShapeVaried::~SVMtlFaceShapeVaried(){
    
}

SVMtlCorePtr SVMtlFaceShapeVaried::clone(){
    return PointerSharedPtr<SVMtlFaceShapeVaried>(new SVMtlFaceShapeVaried(this));
}

void SVMtlFaceShapeVaried::update(f32 dt){
    
    SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(1);
    if( t_person && t_person->getExist() ){
        V2 *t_data = (V2*)t_person->getFaceDataOriginal();
        for(s32 i  = 0 ; i < 106 ; i++){
            m_outlinePoints[i*2]   = t_data[i].x;
            m_outlinePoints[i*2+1] = mApp->m_pGlobalParam->m_inner_height-t_data[i].y;
        }
        t_data = (V2*)m_outlinePoints;
        FVec2 t_77 = FVec2(t_data[77].x,t_data[77].y);
        FVec2 t_74 = FVec2(t_data[74].x,t_data[74].y);
        FVec2 t_e1 = (t_77-t_74)*0.25;
        FVec2 t_e2 = FVec2(-t_e1.y,t_e1.x);
        f32 t_len = getDistanceFrom(t_77, t_74);
        f32 t_inversedStandardLength = 1.0 / t_len;
        //
        for(s32 i = 0 ; i < 24 ; i++){
            f32 *t_param        = representParam[i];
            f32 t_action_type   = t_param[0];
            f32 t_symmetri_type = t_param[1];
            f32 t_coeff1a       = t_param[2];
            f32 t_coeff1b       = t_param[3];
            f32 t_coeff2a       = t_param[4];
            f32 t_coeff2b       = t_param[5];
            s32 t_id            = t_param[6];
            f32 t_radius_coeff  = t_param[7];
            f32 t_intensity     = t_param[8];
            FVec2 t_point         = FVec2(t_data[t_id].x,t_data[t_id].y);
            FVec2 t_startPt       = t_point   + t_e1*t_coeff1a + t_e2*t_coeff1b;
            FVec2 t_endPt         = t_startPt + t_e1*t_coeff2a + t_e2*t_coeff2b;
            f32 t_radius = t_radius_coeff*t_len;
            m_startPoint[i*2]         = t_startPt.x;
            m_startPoint[i*2+1]       = t_startPt.y;
            m_endPoint[i*2]           = t_endPt.x;
            m_endPoint[i*2+1]         = t_endPt.y;
            m_actionType[i]           = t_action_type;
            m_intensity[i]            = t_intensity;
            m_radius[i]               = t_radius;
        }
        m_RealStep[0]=24;
        //memcpy(m_outlinePoints,t_person->getFaceDataOriginal(), sizeof(f32)*106*2);
        m_eyesDistance                = t_len;
        m_inversedEyesDistance        = t_inversedStandardLength;
    }
}

void SVMtlFaceShapeVaried::_submitMtl(SVRendererBasePtr _render){
    _render->submitUniformf2v("startPoint", m_startPoint,24);
    _render->submitUniformf2v("endPoint", m_endPoint,24);
    _render->submitUniformf1v("actionType", m_actionType,24);
    _render->submitUniformf1v("intensity", m_intensity,24);
    _render->submitUniformf1v("radius", m_radius,24);
    _render->submitUniformi1v("RealStep", m_RealStep,1);
    _render->submitUniformf2v("outlinePoints", m_outlinePoints,106);
    _render->submitUniformf1v("eyesDistance", percentages,14);
    //
    _render->submitUniformf("inversedEyesDistance", m_eyesDistance);
    _render->submitUniformf("g_intensity", m_inversedEyesDistance);
    _render->submitUniformf("surfaceWidth", m_surfaceWidth);
    _render->submitUniformf("surfaceHeight", m_surfaceHeight);
    _render->submitUniformf("inv_surfaceWidth", m_inv_surfaceWidth);
    _render->submitUniformf("inv_surfaceHeight", m_inv_surfaceHeight);
}
