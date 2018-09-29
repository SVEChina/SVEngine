//
// SVTripleDownUnit.cpp
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#include "SVFlowerDanceUnit.h"
#include "SVFlowerDanceDef.h"
#include "SVGameDefine.h"
#include "../node/SVSpineNode.h"
#include "../node/SVBitFontNode.h"
#include "../node/SVScene.h"
#include "../core/SVSpine.h"
#include "../logicsys/SVSceneMgr.h"
#include "../event/SVEvent.h"
#include "../event/SVEventMgr.h"
#include "../act/SVActDeform.h"
#include "../node/SVSpriteNode.h"
#include "../act/SVActFollow.h"

SVFlowerDanceUnit::SVFlowerDanceUnit(SVInst *_app)
:SVEventProc(_app)
,m_fDeadingTime(1.0)
,m_fInnerSize(100)
,m_fOutterSize(200)
,m_spCircle(nullptr)
,m_AniTriple(nullptr)
,m_AniPC(nullptr)
,m_bOpened(false){
}

SVFlowerDanceUnit::~SVFlowerDanceUnit() {
}

void SVFlowerDanceUnit::init(cptr8 _scenename,cptr8 _spinename ,cptr8 _circleName,cptr8 _pcname){
    m_strSceneName  = _scenename;
    m_spinename     = _spinename;
    m_strCircleName = _circleName;
    m_spinenamePC = _pcname;
    
    SVSpineNodeParam spineparam;
    spineparam.m_spinename = m_spinename;
    spineparam.m_aniname = "daiji";
    spineparam.m_changeAni = true;
    spineparam.m_loop = false;
    m_AniTriple = (SVSpineNode*)spineparam.genNode(mApp);
    
    SVSpriteNodeParam t_bg_param;
    t_bg_param.m_width = 975;
    t_bg_param.m_height = 975;
    t_bg_param.m_pos.set(0.0f,0.0f,0.0f);
    t_bg_param.m_texname = m_strCircleName;
    t_bg_param.m_zorder = 0;
    m_spCircle = (SVSpriteNode*)t_bg_param.genNode(mApp);
}

void SVFlowerDanceUnit::destroy(){
    exitex();
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(m_AniTriple && t_sc){
        m_AniTriple->removeFromParent();
        t_sc->removeNode(m_AniTriple);
        m_AniTriple->svdestroy();
        SAFE_DELETE(m_AniTriple);
    }
    //瓢虫
    if(m_AniPC){
        m_AniPC->removeFromParent();
        t_sc->removeNode(m_AniPC);
        m_AniPC->svdestroy();
        SAFE_DELETE(m_AniPC);
    }
    if (m_spCircle && t_sc){
        m_spCircle->removeFromParent();
        t_sc->removeNode(m_spCircle);
        SAFE_DELETE(m_spCircle);
    }
}

void SVFlowerDanceUnit::enter(){
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(m_AniTriple && t_sc ){
        m_eState = FD_ALIVE;
        m_AniTriple->setPosition(m_vecPos);
        t_sc->addNode(m_AniTriple , 2);
        m_AniTriple->setloop(true);
        float fScale = m_fInnerSize / 100.0f;
        m_AniTriple->setScale(fScale,fScale, fScale);
        enterex();
    }
    //
    if (m_spCircle && t_sc){
        m_spCircle->setPosition(m_vecPos);
        t_sc->addNode(m_spCircle,1);
        float fWidth  = m_spCircle->getSpriteWidth();
        float fHeight = m_spCircle->getSpriteHeight();
        
        float fScaleX = m_fOutterSize / fWidth * 2 ;
        float fScaleY = m_fOutterSize / fHeight * 2;
        m_spCircle->setScale(fScaleX, fScaleY, 1.0);
    }
    //
    if(!m_AniPC){
        s32 t_vec_y = s32(m_vecPos.Y);
        if( (rand()%t_vec_y) >1800 ){
            //产生瓢虫
            SVSpineNodeParam spineparam_pc;
            spineparam_pc.m_spinename = m_spinenamePC;
            spineparam_pc.m_aniname = "fly";
            spineparam_pc.m_changeAni = true;
            spineparam_pc.m_loop = true;
            m_AniPC = (SVSpineNode*)spineparam_pc.genNode(mApp);
        }
        //瓢虫
        if(m_AniPC && t_sc){
            m_AniPC->setloop(true);
            m_AniPC->setPosition(m_vecPos);
            m_AniPC->play("fly");
            //
            SVActCirclePtr t_circle_act = MakeSharedPtr<SVActCircle>();
            t_circle_act->setCircleParam(m_vecPos, m_fOutterSize+30.0f);
            s32 t_angle_sp = rand()%50 + 50;
            t_circle_act->setAngleParam(t_angle_sp, 0.0f);
            m_AniPC->addAction(t_circle_act);
            //
            t_sc->addNode(m_AniPC , 3);
        }
    }
}

void SVFlowerDanceUnit::exit(){
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(m_AniTriple && t_sc){
        exitex();
        t_sc->removeNode(m_AniTriple);
        m_AniTriple->removeFromParent();
        m_AniTriple->clearAction();
    }
    
    if(m_AniPC && t_sc){
        m_AniPC->clearAction();
        t_sc->removeNode(m_AniPC);
        m_AniPC->removeFromParent();
        m_AniPC->clearAction();
    }
}

void SVFlowerDanceUnit::setOpened(bool bOpened){
    m_bOpened = bOpened;
}

bool SVFlowerDanceUnit::getOpened(){
    return m_bOpened;
}

void SVFlowerDanceUnit::playAni(const char* pName, bool bLoop){
    if (m_AniTriple && strcmp(m_strLastAni.c_str() , pName) != 0){
        m_AniTriple->setloop(bLoop);
        m_AniTriple->play(pName);
        m_strLastAni = pName;
        printf("playAniName %s\n" , pName);
    }
}
//
void SVFlowerDanceUnit::stop(){
    if(m_AniTriple){
        m_AniTriple->clearAction();
    }
}

void SVFlowerDanceUnit::update(float _dt){
    if (m_eState == FD_DEADING){
        m_fDeadingTime -= _dt;
        if (m_fDeadingTime < 0){
            m_eState = FD_DEADED;
        }
    }
}

SVSpineNode* SVFlowerDanceUnit::getPC(){
    return m_AniPC;
}

FLOWERDANCESTATE SVFlowerDanceUnit::getFlowerDanceState(){
    return m_eState;
}

void SVFlowerDanceUnit::setPosition(vector3df vecPos){
    m_vecPos = vecPos;
}

float SVFlowerDanceUnit::getOutterSize(){
    return m_fOutterSize;
}

float SVFlowerDanceUnit::getInnerSize(){
    return m_fInnerSize;
}

vector3df SVFlowerDanceUnit::getPosition(){
    return m_vecPos;
}

void  SVFlowerDanceUnit::setOutterSize(float fSize){
    m_fOutterSize = fSize;
}

void  SVFlowerDanceUnit::setInnerSize(float fSize){
    m_fInnerSize = fSize;
}

void SVFlowerDanceUnit::setType(int iType){
    m_iType = iType;
}
int  SVFlowerDanceUnit::getType(){
    return m_iType;
}
