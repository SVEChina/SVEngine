//
// Created by lidm on 17/9/5.
//

#include "SVQClickUnit.h"
#include "SVGameCommonEvent.h"
#include "SVQClickEvent.h"
#include "SVGameDefine.h"
#include "../app/SVInst.h"
#include "../act/SVActDeform.h"
#include "../node/SVSpineNode.h"
#include "../node/SVBitFontNode.h"
#include "../node/SVScene.h"
#include "../event/SVEvent.h"
#include "../event/SVEventMgr.h"
#include "../core/SVSpine.h"
#include "../logicsys/SVSceneMgr.h"

SVQClickUnit::SVQClickUnit(SVInst*_app)
:SVEventProc(_app) {
    m_state = 0;
    m_initPos.set(0.0f, 0.0f, 0.0f);
    m_initSpeed = 100.0f;
    m_life = 0.0f;
    m_score = 10;
    m_AniSpineNode = nullptr;
}

SVQClickUnit::~SVQClickUnit() {
    if(m_AniSpineNode){
        m_AniSpineNode->removeFromParent();
        m_AniSpineNode->svdestroy();
        SAFE_DELETE(m_AniSpineNode);
    }
}

void SVQClickUnit::init(cptr8 _spinename){
    m_spinename = _spinename;
    m_state = 0;
    //
    stringc skins[] = {"flyheart1","flyheart2","flyheart3","flyheart4","flyheart5"};
    s32 t_skinid = arc4random() % 5;
    //
    SVSpineNodeParam spineparam;
    spineparam.m_spinename = m_spinename;
    spineparam.m_aniname = skins[t_skinid];
    spineparam.m_changeAni = true;
    spineparam.m_loop = false;
    m_aniname = skins[t_skinid];
    m_AniSpineNode = (SVSpineNode*)spineparam.genNode(mApp);
}

void SVQClickUnit::destroy(){
}

void SVQClickUnit::enter(){
    m_state = 0;
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(m_AniSpineNode && t_sc ){
        m_AniSpineNode->setPosition(m_initPos.X, m_initPos.Y, m_initPos.Z);
        m_AniSpineNode->setloop(false);
        m_AniSpineNode->play( m_aniname.c_str() );
        t_sc->addNode(m_AniSpineNode);
    }
}

void SVQClickUnit::exit(){
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(m_AniSpineNode && t_sc ){
        t_sc->removeNode(m_AniSpineNode);
        m_AniSpineNode->clearAction();
    }
}

//
void SVQClickUnit::stop(){
}

void SVQClickUnit::update(f32 _dt,f32 _gametime){
    m_gametime = _gametime;
    if(m_state == 0 || m_state == 1){
        m_life -= _dt;
        if(m_life<0.0f){
            m_state = 2;
        }
    }
}

s32 SVQClickUnit::getState(){
    return m_state;
}

void SVQClickUnit::setInitPos(f32 _x,f32 _y,f32 _z){
    m_initPos.set(_x,_y,_z);
}

void SVQClickUnit::setLife(f32 _life){
    m_life = _life;
}

void SVQClickUnit::setInitSpeed(f32 _sp){
    m_initSpeed = _sp;
}

void SVQClickUnit::setInitScore(s32 _score){
    m_score = _score;
}

void SVQClickUnit::setani(cptr8 _aniname){
    m_aniname = _aniname;
}

bool SVQClickUnit::procEvent(SVEvent *_event){
//    //1抬头 2低头 3张嘴 4张嘴左扭头 5张嘴右扭头 6左眼闭右眼睁 7左眼睁右眼闭 8嘴角上扬 9嘴角向下 10双眼一起闭上 11正面嘟嘴  12左嘟嘴  13右嘟嘴
//    if(!_event)
//        return false;
//    if(m_state == 0){
//        //只有存活状态 才会处理相关消息
//        stringc t_trans_ani = "";
//        if(_event->eventType == EVN_T_RAISE_HEAD){
//            t_trans_ani = "head_up";
//        }else if(_event->eventType == EVN_T_BOW_HEAD){
//            t_trans_ani = "head_down";
//        }else if(_event->eventType == EVN_T_MOUSE_OPEN){
//            t_trans_ani = "mouth_open";
//        }else if(_event->eventType == EVN_T_C_MOUTHOPEN_TURNLEFT){
//            t_trans_ani = "mouth_open_left";
//        }else if(_event->eventType == EVN_T_C_MOUTHOPEN_TURNRIGHT){
//            t_trans_ani = "mouth_open_right";
//        }else if(_event->eventType == EVN_T_C_LEFTEYECLOSE_RIGHTEYEOPEN){
//            t_trans_ani = "leye_close_reye_open";
//        }else if(_event->eventType == EVN_T_C_LEFTEYEOPEN_RIGHTEYECLOSE){
//            t_trans_ani = "leye_open_reye_close";
//        }else if(_event->eventType == EVN_T_MOUSE_RAISE_CORNER){
//            t_trans_ani = "mouth_corner_up";
//        }else if(_event->eventType == EVN_T_MOUSE_BOW_CORNER){
//            t_trans_ani = "mouth_corner_down";
//        }else if(_event->eventType == EVN_T_C_LEFTEYECLOSE_RIGHTEYECLOSE){
//            t_trans_ani = "leye_close_reye_close";
//        }else if(_event->eventType == EVN_T_MOUSE_TOUTH){
//            t_trans_ani = "mouth_touch";
//        }else if(_event->eventType == EVN_T_C_MOUTHTOUTH_TURNLEFT){
//            t_trans_ani = "mouth_touch_left";
//        }else if(_event->eventType == EVN_T_C_MOUTHTOUTH_TURNRIGHT){
//            t_trans_ani = "mouth_touch_right";
//        }
//        if( strcmp(m_aniname.c_str(),t_trans_ani.c_str()) == 0){
//            //表情消亡
//            execDead(false);
//            SV_LOG_INFO("FD unit dead %s \n",m_aniname.c_str());
//            return false;
//        }
//        //
//        SVEventFDUnitAllDead* t_event = dynamic_cast<SVEventFDUnitAllDead*>(_event);
//        if(t_event){
//            //execDead(true);
//            return false;
//        }
//    }
    return true;
}
