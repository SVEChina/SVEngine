//
// MaTripleDownMgr.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#include "SVFlowerDanceMgr.h"
#include "SVFlowerDanceUnit.h"
#include "SVFlowerDanceEvent.h"
#include "SVGameCommonEvent.h"
#include "../event/SVEventMgr.h"
#include "../app/SVInst.h"
#include "../node/SVSpineNode.h"

SVFlowerDanceMgr::SVFlowerDanceMgr(SVInst *_app)
:SVEventProc(_app)
,m_maxNum(50)
,m_vecLastPos(0.0 , -800.0 , 0.0)
,m_iGameBgmId(0){
}

SVFlowerDanceMgr::~SVFlowerDanceMgr() {
}

void SVFlowerDanceMgr::init(){
    enterex();
    srand((unsigned int)time(0));
    s32 iSize = m_arrAnimate.size();
    for(s32 i=0;i<m_maxNum;i++){
        SVFlowerDanceUnit* t_unit = new SVFlowerDanceUnit(mApp);
        int iRand = rand() % 10;
        if (iRand <= 7){
            t_unit->init(m_scenename.c_str() ,
                         m_arrAnimate[0].c_str(),
                         m_strCircle.c_str(),
                         m_strSpineNamePC.c_str());
            t_unit->setType(1);
            float fInner  = 60  + rand() % 40;
            float fOutter = 220 + rand() % 40;
            t_unit->setInnerSize(fInner);
            t_unit->setOutterSize(fOutter);
        }else{
            t_unit->init(m_scenename.c_str() ,
                         m_arrAnimate[1].c_str(),
                         m_strCircle.c_str(),
                         m_strSpineNamePC.c_str());
            t_unit->setType(2);
            float fInner  = 40  + rand() % 60;
            float fOutter = 180 + rand() % 40;
            t_unit->setInnerSize(fInner);
            t_unit->setOutterSize(fOutter);
        }
        
        m_obFreePool.push_back(t_unit);
    }
    SVFlowerDanceUnit* t_unit = _generateFlower();
    if (t_unit){
        t_unit->playAni("open");
    }
    FLOWERPOOL::Iterator it = m_obUsePool.begin();
    if (it != m_obUsePool.end()){
        SVEventInitFlower* pEventPos = new SVEventInitFlower();
        pEventPos->m_fFlower_inner = (*it)->getInnerSize();
        pEventPos->m_fFlower_outter= (*it)->getOutterSize();
        pEventPos->m_vecPos        = (*it)->getPosition();
        mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(pEventPos);
    }
}

void SVFlowerDanceMgr::destroy(){
    exitex();
    //使用池
    FLOWERPOOL::Iterator it_use = m_obUsePool.begin();
    while( it_use!=m_obUsePool.end() ){
        SVFlowerDanceUnit* t_unit = (*it_use);
        t_unit->destroy();
        it_use++;
    }
    m_obUsePool.clear();
    //缓存池
    FLOWERPOOL::Iterator it_free = m_obFreePool.begin();
    while( it_free!=m_obFreePool.end() ){
        SVFlowerDanceUnit* t_unit = (*it_free);
        t_unit->destroy();
        it_free++;
    }
    m_obFreePool.clear();
    
    if (m_iGameBgmId >= 0){
    }
}

void SVFlowerDanceMgr::stop(){
    FLOWERPOOL::Iterator it = m_obUsePool.begin();
    while( it!=m_obUsePool.end() ){
        SVFlowerDanceUnit* t_unit = (*it);
        t_unit->stop();
        it++;
    }
}

void SVFlowerDanceMgr::update(float _dt){
    if(m_obFreePool.size() != 0){
        SVFlowerDanceUnit* t_unit = _generateFlower();
        if (t_unit){
            t_unit->playAni("daiji" , true);
        }
    }
    
    FLOWERPOOL::Iterator it = m_obUsePool.begin();
    while( it!=m_obUsePool.end() ){
        SVFlowerDanceUnit* t_unit = (*it);
        if(t_unit->getFlowerDanceState() < FD_DEADED ){
            //生成或者暂停状态
            t_unit->update(_dt);
            it++;
        }else if(t_unit->getFlowerDanceState() == FD_DEADED){
            //死亡状态
            t_unit->exit();
            m_obFreePool.push_back(t_unit);
            it = m_obUsePool.erase(it);
        }else{
            it++;
        }
    }
}

bool SVFlowerDanceMgr::procEvent(SVEvent *_event){
    if(!_event)
        return true;
    
    SVEeventFDPlayerPos* pPos = dynamic_cast<SVEeventFDPlayerPos*>(_event);
    if (pPos){
        FLOWERPOOL::Iterator it = m_obUsePool.begin();
        while( it!=m_obUsePool.end() ){
            SVFlowerDanceUnit* t_unit = (*it);
            //是否跟瓢虫碰撞
            SVSpineNode* t_pc = t_unit->getPC();
            if( t_pc ){
                f32 t_dis = t_pc->getPosition()->getDistanceFrom( pPos->m_vecPos );
                if(t_dis<30.0f && t_dis>0.0f){
                    //
                    t_pc->clearAction();
                    SVGameOver* t_gameover = new SVGameOver(nullptr);
                    mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(t_gameover);
                }
            }
            //被另外一朵花捕获
            if (t_unit->getPosition().getDistanceFrom(pPos->m_vecPos) < t_unit->getOutterSize()){
                t_unit->playAni("open" );
                SVEventEnterFlower* pEventEnter = new SVEventEnterFlower();
                pEventEnter->m_fFlower_outter = t_unit->getOutterSize();
                pEventEnter->m_fFlower_inner  = t_unit->getInnerSize();
                pEventEnter->m_vecPos         = t_unit->getPosition();
                pEventEnter->m_iType          = t_unit->getType();
                pEventEnter->m_bIsOpened      = t_unit->getOpened();
                mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(pEventEnter);
                t_unit->setOpened(true);
                return true;
            }
            it++;
        }
    }
    
    return true;
}

//生成花的位置
SVFlowerDanceUnit* SVFlowerDanceMgr::_generateFlower(){
    SVFlowerDanceUnit* t_unit = nullptr;
    FLOWERPOOL::Iterator it_free = m_obFreePool.begin();
    if( it_free != m_obFreePool.end() ){
        t_unit = (*it_free);
        m_obFreePool.erase(it_free);
    }
    if(t_unit){
        vector3df _tPos;
        _tPos.X = -140 +  rand() % 320;
        _tPos.Y = m_vecLastPos.Y + 400 + rand() % 250;
        t_unit->setPosition(_tPos);
        t_unit->enter();
        m_obUsePool.push_back(t_unit);
        m_vecLastPos = _tPos;
    }
    return t_unit;
}


