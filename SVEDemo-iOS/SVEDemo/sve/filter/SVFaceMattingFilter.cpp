//
//  SVFaceMattingFilter.cpp
//  SVEngineShow
//
//  Created by 徐子昱 on 2019/10/28.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#include "SVFaceMattingFilter.h"
#include "rendercore/SVRendererBase.h"
#include "rendercore/SVRenderMgr.h"
#include "rendercore/SVRenderMesh.h"
#include "mtl/SVMtlCore.h"
#include "mtl/SVTexMgr.h"
#include "basesys/SVConfig.h"
#include "basesys/SVStaticData.h"
#include "basesys/SVFaceDataMesh.h"
#include "node/SVMultPassNode.h"
#include "node/SV2DFaceMaskSTNode.h"
#include "detect/SVDetectMgr.h"
#include "detect/SVDetectST.h"
#include "core/SVPass.h"
#include "core/SVMathHelper.h"
#include "basesys/SVSceneMgr.h"
#include "node/SVSpriteNode.h"
#include "node/SVScene.h"




SVFaceMattingFilter::SVFaceMattingFilter(SVInst *_app)
:SVFilterBase(_app){
    m_type=SV_FUNC_FACE_MATTING;
    m_name="SVFilterMatting";
    m_spriteNode = MakeSharedPtr<SVSpriteNode>(mApp);
}

SVFaceMattingFilter::~SVFaceMattingFilter(){
    
}

bool SVFaceMattingFilter::create(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer){
        return false;
    }
    s32 t_w =  mApp->m_pGlobalParam->m_inner_width;
    s32 t_h =  mApp->m_pGlobalParam->m_inner_height;
    
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->create(t_w, t_h);
    //
    if(! t_renderer->getSVTex(E_TEX_FILTER_MATTING) ){
        t_renderer->createSVTex(E_TEX_FILTER_MATTING,t_w, t_h, GL_RGBA);
    }
    
    m_mtl = MakeSharedPtr<SVMtlCore>(mApp,"mattingfilter");
    
    
    SVMtlCorePtr t_mtlclear = MakeSharedPtr<SVMtlCore>(mApp,"clear");
    t_mtlclear->setTexcoordFlip(1.0f, 1.0f);
    SVPassPtr t_pass = MakeSharedPtr<SVPass>();
    t_pass->mTag="filter-face-matting-clear";
    t_pass->setMtl(t_mtlclear);
    t_pass->setInTex(0, E_TEX_MAIN);
    t_pass->setOutTex(E_TEX_FILTER_MATTING);
    m_pPassNode->addPass(t_pass);
    
    SVPassPtr t_pass1 = MakeSharedPtr<SVPass>();
    t_pass1->mTag="filter-face-matting";
    t_pass1->setMtl(m_mtl);
    t_pass1->setInTex(0, E_TEX_MAIN);
    t_pass1->setOutTex(E_TEX_FILTER_MATTING);
    m_pPassNode->addPass(t_pass1);
    
    m_spriteNode=MakeSharedPtr<SVSpriteNode>(mApp,t_w,t_h);
    m_spriteNode->setTexture(E_TEX_FILTER_MATTING);
    m_spriteNode->setScale(1.5, -1.5, 1.0);
    SVScenePtr t_sc = mApp->getSceneMgr()->getScene();
    if(t_sc){
        if(m_spriteNode){
            t_sc->addNode(m_spriteNode);
        }
    }
    return true;
}

void SVFaceMattingFilter::destroy(){
    if(m_mtl){
        m_mtl = nullptr;
    }
    if(m_spriteNode){
        m_spriteNode->removeFromParent();
        m_spriteNode = nullptr;
    }
}

void SVFaceMattingFilter::update(f32 dt){
    
    for(int i=1;i<=1;i++){
        SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(i);
        if( t_person && t_person->getExist()){
            
            s32 t_singleface_ptnum = 106;
            memset(m_faceDateExt, 0, sizeof(f32) * 800);
            memset(m_texcoord, 0, sizeof(f32) * 800);
            memcpy(m_faceDateExt, t_person->getFaceData(), 2 * t_singleface_ptnum * sizeof(f32));
            s32 t_camera_w = mApp->getConfig()->getCameraWidth()/2;
            s32 t_camera_h = mApp->getConfig()->getCameraHeight()/2;
            
            f32 t_nosx=(t_camera_w+m_faceDateExt[2 * 44])/mApp->getConfig()->getCameraWidth();
            f32 t_nosy=(t_camera_h+m_faceDateExt[2 * 44+1])/mApp->getConfig()->getCameraHeight();
            
            f32 dxc = m_faceDateExt[2 * 2]/t_camera_w - m_faceDateExt[2 * 30]/t_camera_w;
            f32 dyc = (t_camera_h+m_faceDateExt[2 * 2+1])/mApp->getConfig()->getCameraHeight()-(t_camera_h+m_faceDateExt[2 * 30+1])/mApp->getConfig()->getCameraHeight();
            f32 noseLengthc = sqrtf(dxc * dxc + dyc * dyc);
            dxc= (t_camera_w+m_faceDateExt[2 * 2])/mApp->getConfig()->getCameraWidth()+noseLengthc/4;
            dyc=(t_camera_h+m_faceDateExt[2 * 44+1])/mApp->getConfig()->getCameraHeight()+dyc/2;
            
            m_mtl->setTexSizeIndex(0, dxc, t_nosy);
        
            
            FVec2 eyer = FVec2(m_faceDateExt[2 *77],m_faceDateExt[2 *77+1]);
            FVec2 eyel = FVec2(m_faceDateExt[2 *74],m_faceDateExt[2 *74+1]);
            f32 leng = getDistanceFrom(eyer,eyel);
            f32 t_size = 250.0f/mApp->getConfig()->getCameraWidth();
            f32 _smooth = (leng/240.0);
            leng = t_size*_smooth;
            
            m_mtl->setTexSizeIndex(1,leng, leng*1.3);
            
           
        }else{
           
        }
    }
    
    if (m_mtl) {
        m_mtl->update(dt);
        m_mtl->setBlendEnable(true);
        m_mtl->setBlendState(MTL_BLEND_ONE, MTL_BLEND_ONE_MINUS_SRC_COLOR);
        m_mtl->setTexcoordFlip(1.0, 1.0);
    }
}


