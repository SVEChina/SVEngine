//
// MaTripleDownMgr.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//
#include "SVTripleDownHarpoon.h"
#include "SVTripleDownUnit.h"
#include "SVGameDefine.h"
#include "SVTripleDownEvent.h"
#include "../node/SVScene.h"
#include "../node/SVSpineNode.h"
#include "../event/SVEvent.h"
#include "../event/SVEventMgr.h"
#include "../core/SVSpine.h"
#include "../logicsys/SVSceneMgr.h"
#include "../act/SVActBase.h"
#include "../act/SVActDeform.h"
#include "../act/SVActSequence.h"
#include "../base/SVPreDeclare.h"

#define MAX_POOL_SIZE 12
#define HARPOONDI 30

SVTripleDownHarpoon::SVTripleDownHarpoon(SVInst *_app)
:SVEventProc(_app),m_AniHarpoon(NULL),m_strAction("animation"){
}

SVTripleDownHarpoon::~SVTripleDownHarpoon() {
}

void SVTripleDownHarpoon::init(const char* _scenename,const char* _spinename){
    m_strSceneName = _scenename;
    m_strSpineName = _spinename;
    
    SVSpineNodeParam spineparam;
    spineparam.m_spinename = m_strSpineName;
    spineparam.m_aniname = "animation";
    spineparam.m_changeAni = true;
    spineparam.m_loop = false;
    
    m_AniHarpoon = (SVSpineNode*)spineparam.genNode(mApp);
}

void SVTripleDownHarpoon::destroy(){
    exitex();
    if(m_AniHarpoon){
        m_AniHarpoon->removeFromParent();
        m_AniHarpoon->svdestroy();
        SAFE_DELETE(m_AniHarpoon);
    }

}

void SVTripleDownHarpoon::enter(){
    enterex();
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(m_AniHarpoon && t_sc ){
        t_sc->addNode(m_AniHarpoon , GAMERENDERLAYER::GRL_SCENE);
    }
}

void SVTripleDownHarpoon::exit(){
    exitex();
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(m_AniHarpoon && t_sc){
        t_sc->removeNode(m_AniHarpoon);
        m_AniHarpoon->removeFromParent();
        m_AniHarpoon->clearAction();
    }
}

//
void SVTripleDownHarpoon::stop(){
    if(m_AniHarpoon){
        m_AniHarpoon->clearAction();
    }
}

void SVTripleDownHarpoon::update(float _dt,float _gametime){
    CIRCLEPOOL::Iterator cicleIt = m_circlePool.getLast();
    if (cicleIt == m_circlePool.end()){
        return;
    }
    SVTripleDownUnit* cicleUnit = *cicleIt;
    rectf rectSrc = cicleUnit->getBoundingBox();
    for (CIRCLEPOOL::Iterator it = m_readyPool.begin(); it != m_readyPool.end() ; ){
        SVTripleDownUnit* TripleUnit = *it;
        bool bIsIn = TripleUnit->getBoundingBox().isRectCollided(rectSrc);
        //bool bIsIn = TripleUnit->getBoundingBox().isIn(&rectSrc);
        if (bIsIn == false){
            ++it;
            continue;
        }
        TripleUnit->stop();
        //vector3df* Pos = m_AniHarpoon->getPosition();
        //Pos->Y = m_AniHarpoon->getPosition()->Y + HARPOONDI + m_circlePool.size() * (cicleUnit->getHeight() * 0.5 + 5);

        it = m_readyPool.erase(it);
        m_circlePool.push_back(TripleUnit);
        _checkDown();
    }
    
    if (m_circlePool.size() == MAX_POOL_SIZE){
        _clearCircle();
    }
    
    _dealCircle();
}

void SVTripleDownHarpoon::setPosition(vector3df Pos){
    if(m_AniHarpoon){
        m_AniHarpoon->setPosition(Pos);
    }
}

void SVTripleDownHarpoon::setHeight(float fHeight){
    m_fHeight = fHeight;
}

float SVTripleDownHarpoon::getHeight(){
    return m_fHeight;
}

void SVTripleDownHarpoon::setWidth(float fWidth){
    m_fWidth = fWidth;
}

float SVTripleDownHarpoon::getWidth(){
    return m_fWidth;
}

void SVTripleDownHarpoon::showAni(HARPOONANI eAni){
    if (m_AniHarpoon == NULL){
        return;
    }
    
    if (eAni == HA_COMMON){
        m_AniHarpoon->setloop(true);
        m_AniHarpoon->play("gun");
    }else if (eAni == HA_SELECT){
        m_AniHarpoon->setloop(true);
        m_AniHarpoon->play("zhuangtai");
    }
}

void SVTripleDownHarpoon::addCircle(SVTripleDownUnit* pUnit){
    if (pUnit == NULL){
        return;
    }
    pUnit->ShowAni(CA_LOOP);
    vector3df psrcPos  = pUnit->getPosition();
    vector3df TarPos1  = (*m_AniHarpoon->getPosition()) + vector3df(0 , m_fHeight , 0);
    vector3df TarPos2  = (*m_AniHarpoon->getPosition()) + vector3df(0 , HARPOONDI , 0);
    float     fDis1    = psrcPos.getDistanceFrom(TarPos1);
    float     fDis2    = TarPos1.getDistanceFrom(TarPos2);

    SVActParamMoveTo _actParam;
    _actParam.m_target = TarPos1;
    _actParam.m_time   = fDis1 / 300.0f;
    SVActPtr pAct1 = _actParam.genAct();
    
    _actParam.m_target = TarPos2;
    _actParam.m_time   = fDis2 / 200.0f;
    SVActPtr pAct2 = _actParam.genAct();
    
    SVActSequencePtr pActSeq = MakeSharedPtr<SVActSequence>();
    SVActPtr pAniChange = MakeSharedPtr<SVActionTripleDownChangeAni>(pUnit);
    pActSeq->addAct(pAct1);
    pActSeq->addAct(pAniChange);
    pActSeq->addAct(pAct2);
    pUnit->addAction(pActSeq);
    
    int iSize = m_circlePool.size();
    if (iSize == 0){
        m_circlePool.push_back(pUnit);
    }else{
        m_readyPool.push_back(pUnit);
    }
    
}

void SVTripleDownHarpoon::setType(HARPOONTYPE eType){
    m_eType = eType;
}

HARPOONTYPE SVTripleDownHarpoon::getType(){
    return m_eType;
}

void SVTripleDownHarpoon::_checkDown(){
    int iSize = m_circlePool.size();
    if (iSize < 3){
        return;
    }
    CIRCLEPOOL::Iterator it = m_circlePool.getLast();
    SVTripleDownUnit* TripleUnit = *it;
    TRIPLEUNITTYPE eStartType = TripleUnit->getTripleType();
    bool bDown = true;
    for (int i = 0 ; i < 2 ; ++i){
        --it;
        SVTripleDownUnit* UnitInner = *it;
        if (eStartType != UnitInner->getTripleType()){
            bDown = false;
        }
    }
    
    if (bDown == true){
        TRIPLEUNITTYPE eTripType = TUT_TYPE_MAX;
        vector2df vecPos(0,0);
        for (int i = 0 ; i < 3 ; ++i){
            CIRCLEPOOL::Iterator itLast = m_circlePool.getLast();
            SVTripleDownUnit* UnitInner = *itLast;
            eTripType = UnitInner->getTripleType();
            UnitInner->setTripleState(TUS_DEADING);
            UnitInner->ShowAni(CA_BAOZHA);
            m_circlePool.erase(itLast);
        }
        vecPos.X = m_AniHarpoon->getPosition()->X - 0.5 * m_fWidth ;
        vecPos.Y = m_AniHarpoon->getPosition()->Y + m_fHeight + 100;
        SVEventTripleDown* pTripleEvent = new SVEventTripleDown();
        pTripleEvent->eMissType    = eTripType;
        pTripleEvent->eHarpoonType = m_eType;
        pTripleEvent->vecPos.X     = vecPos.X;
        pTripleEvent->vecPos.Y     = vecPos.Y;
        mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(pTripleEvent);
    }
}

void SVTripleDownHarpoon::_clearCircle(){
    TRIPLEUNITTYPE eTripType = TUT_TYPE_MAX;
    int iSize = m_circlePool.size();
    CIRCLEPOOL::Iterator itBegin = m_circlePool.begin();
    for (int i = 0 ; i < iSize ; ++i){
        SVTripleDownUnit* UnitInner = *itBegin;
        eTripType = UnitInner->getTripleType();
        UnitInner->setTripleState(TUS_DEADING);
        UnitInner->ShowAni(CA_BAOZHA);
        itBegin++;
    }
    m_circlePool.clear();
}

void SVTripleDownHarpoon::_dealCircle(){
    if (m_readyPool.size() == 0 || m_circlePool.size() != 0){
        return;
    }
    
    CIRCLEPOOL::Iterator iterBegin = m_readyPool.begin();
    m_circlePool.push_back(*iterBegin);
    m_readyPool.erase(iterBegin);
}
