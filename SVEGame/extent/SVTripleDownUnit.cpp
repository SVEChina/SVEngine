//
// SVTripleDownUnit.cpp
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#include "SVTripleDownUnit.h"
#include "SVGameDefine.h"
#include "../node/SVSpineNode.h"
#include "../node/SVBitFontNode.h"
#include "../node/SVScene.h"
#include "../core/SVSpine.h"
#include "../logicsys/SVSceneMgr.h"
#include "../event/SVEvent.h"
#include "../event/SVEventMgr.h"
#include "../act/SVActDeform.h"

SVTripleDownUnit::SVTripleDownUnit(SVInst *_app)
:SVEventProc(_app),m_fDeadingTime(0.5),m_fScale(1.0f){
}

SVTripleDownUnit::~SVTripleDownUnit() {
}

void SVTripleDownUnit::init(const char* _scenename,const char* _spinename){
    m_strSceneName = _scenename;
    m_spinename = _spinename;
    
    SVSpineNodeParam spineparam;
    spineparam.m_spinename = m_spinename;
    spineparam.m_aniname = "animation";
    spineparam.m_changeAni = true;
    spineparam.m_loop = false;
    m_AniTriple = (SVSpineNode*)spineparam.genNode(mApp);
    
}

void SVTripleDownUnit::destroy(){
    exitex();
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(m_AniTriple && t_sc){
        m_AniTriple->removeFromParent();
        t_sc->removeNode(m_AniTriple);
        m_AniTriple->svdestroy();
        SAFE_DELETE(m_AniTriple);
    }

}

void SVTripleDownUnit::enter(){
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(m_AniTriple && t_sc ){
        m_eState = TUS_ALIVE;
        t_sc->addNode(m_AniTriple , GAMERENDERLAYER::GRL_SCENE);
        enterex();
    }
}

void SVTripleDownUnit::exit(){
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(m_AniTriple && t_sc){
        exitex();
        t_sc->removeNode(m_AniTriple);
        m_AniTriple->removeFromParent();
        m_AniTriple->clearAction();
    }
}

//
void SVTripleDownUnit::stop(){
    if(m_AniTriple){
        m_AniTriple->clearAction();
    }
}

void SVTripleDownUnit::update(float _dt,float _gametime){
    if (m_eState == TUS_DEADING){
        m_fDeadingTime -= _dt;
        if (m_fDeadingTime < 0){
            m_eState = TUS_DEADED;
        }
    }
}

void SVTripleDownUnit::setTripleType(TRIPLEUNITTYPE eType){
    m_eUnitType = eType;
}

TRIPLEUNITTYPE SVTripleDownUnit::getTripleType(){
    return m_eUnitType;
}

void SVTripleDownUnit::setTripleAniName(const char* cName){
    m_strAction = cName;
}

const char* SVTripleDownUnit::getTripleAniName(){
    return m_strAction.c_str();
}

void SVTripleDownUnit::setTripleState(TRIPLEUNITSTATE eState){
    m_eState = eState;
}

TRIPLEUNITSTATE SVTripleDownUnit::getTripleState(){
    return m_eState;
}

void SVTripleDownUnit::ShowAni(CIRCLEANI eAniState){
    if (eAniState == CA_START){
        m_AniTriple->setloop(true);
        m_AniTriple->play("chushi");
    }else if (eAniState == CA_LOOP){
        m_AniTriple->setloop(false);
        m_AniTriple->play("guocheng");
        m_AniTriple->addAni("jieshu");
    }else if (eAniState == CA_END){
        m_AniTriple->setloop(true);
        m_AniTriple->play("jieshu");
    }else if (eAniState == CA_BAOZHA){
        m_AniTriple->setloop(false);
        m_AniTriple->play("baozha");
    }
}

void SVTripleDownUnit::setPosition(vector3df Pos){
    if(m_AniTriple){
        m_AniTriple->setPosition(Pos);
    }
}

vector3df SVTripleDownUnit::getPosition(){
    if (m_AniTriple){
        return *(m_AniTriple->getPosition());
    }
    return vector3df(0.0f , 0.0f , 0.0f );
}

void SVTripleDownUnit::setWidth(float fWidth){
    m_fWidth = fWidth;
}

float SVTripleDownUnit::getWidth(){
    return m_fWidth;
}

void SVTripleDownUnit::setHeight(float fHeight){
    m_fHeight = fHeight;
}

float SVTripleDownUnit::getHeight(){
    return m_fHeight;
}

void SVTripleDownUnit::addAction(SVActPtr pAction){
    if (m_AniTriple && pAction){
        m_AniTriple->addAction(pAction);
    }
}

rectf& SVTripleDownUnit::getBoundingBox(){
    vector3df* position = m_AniTriple->getPosition();
    m_rectBoundingBox.m_leftBottomCorner.X = position->X + 0.5 * m_fWidth * -1;
    m_rectBoundingBox.m_leftBottomCorner.Y = position->Y;
    m_rectBoundingBox.m_rightTopCorner.X   = position->X + 0.5 * m_fWidth;
    m_rectBoundingBox.m_rightTopCorner.Y   = position->Y + m_fHeight;
    return m_rectBoundingBox;
}

void SVTripleDownUnit::setScale(float fScale){
    m_fScale = fScale;
    if (m_AniTriple){
        m_AniTriple->setScale(m_fScale , m_fScale ,m_fScale);
    }
}
float SVTripleDownUnit::getScale(){
    return m_fScale;
}

SVActionTripleDownChangeAni::SVActionTripleDownChangeAni(SVTripleDownUnit* pUniy)
:m_pUnit(pUniy),m_bEnd(false){
}

SVActionTripleDownChangeAni::~SVActionTripleDownChangeAni(){
}

void SVActionTripleDownChangeAni::enter(SVNode *_node){
    
}
    
void SVActionTripleDownChangeAni::run(SVNode *_node, float dt){
    if (m_pUnit){
        m_pUnit->ShowAni(CA_END);
    }
    m_bEnd = true;
}
    
bool SVActionTripleDownChangeAni::isEnd(){
    return m_bEnd;
}
