//
// Created by lidm on 17/9/5.
//

#include "SVFaceDanceUnit.h"
#include "SVFaceDanceEvent.h"
#include "SVGameDefine.h"
#include "../node/SVSpineNode.h"
#include "../node/SVBitFontNode.h"
#include "../node/SVScene.h"
#include "../core/SVSpine.h"
#include "../logicsys/SVSceneMgr.h"
#include "../event/SVEvent.h"
#include "../event/SVEventMgr.h"
#include "../act/SVActDeform.h"
#include "../base/SVUtil.h"

SVFaceDanceUnit::SVFaceDanceUnit(SVInst*_app)
:SVEventProc(_app) {
    m_state = 0;
    m_initPos.set(0.0f, 0.0f, 0.0f);
    m_initSpeed = 100.0f;
    m_life = 0.0f;
    m_score = 10;
    m_facetype = -1;
    m_AniSpineNode = nullptr;
    m_critPercent = 20.0f; //20%
    m_fontwidth = 50.0f;
    m_enery = 0.0f;
}

SVFaceDanceUnit::~SVFaceDanceUnit() {
}

void SVFaceDanceUnit::init(const char* _scenename,const char* _spinename,const char* _scorepng){
    m_scenename = _scenename;
    m_spinename = _spinename;
    m_scorePng = _scorepng;
    m_state = 0;
    
    SVSpineNodeParam spineparam;
    spineparam.m_spinename = m_spinename;
    spineparam.m_aniname = "animation";
    spineparam.m_changeAni = true;
    spineparam.m_loop = false;
    m_AniSpineNode = (SVSpineNode*)spineparam.genNode(mApp);
    
    //
    if(m_scorePng.size()>0){
        m_scoreNode = new SVBitFontNode(mApp);
        m_scoreNode->svinit();
        m_scoreNode->init(m_scorePng.c_str(),m_fontwidth);
        m_scoreNode->setText("42");
    }
}

void SVFaceDanceUnit::destroy(){
    mApp->m_pGlobalMgr->m_pEventMgr->unregistProcer(this);
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(m_AniSpineNode && t_sc){
        m_AniSpineNode->removeFromParent();
        t_sc->removeNode(m_AniSpineNode);
        m_AniSpineNode->svdestroy();
        SAFE_DELETE(m_AniSpineNode);
    }
}

void SVFaceDanceUnit::enter(){
    m_state = 0;
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(m_AniSpineNode && t_sc ){
        m_AniSpineNode->setPosition(m_initPos.X, m_initPos.Y, m_initPos.Z);
        m_AniSpineNode->setloop(true);
        m_AniSpineNode->play( m_aniname.c_str() );
        //

        SVActParamMoveBy t_param_move;
        t_param_move.m_dir      = vector3df(0.0f, -1.0f, 0.0f);
        t_param_move.m_Speed    = m_initSpeed;
        t_param_move.m_time     = 10.0f;
        SVActPtr t_act          = t_param_move.genAct();
        m_AniSpineNode->addAction(t_act);
        //
        t_sc->addNode(m_AniSpineNode);
        //
        m_life = (m_initPos.Y * 2 + 10)/m_initSpeed;
        mApp->m_pGlobalMgr->m_pEventMgr->registProcer(this);
    }
}

void SVFaceDanceUnit::exit(){
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(t_sc){
        if(m_AniSpineNode){
            mApp->m_pGlobalMgr->m_pEventMgr->unregistProcer(this);
            t_sc->removeNode(m_AniSpineNode);
            m_AniSpineNode->clearAction();
        }
        if(m_scoreNode){
            t_sc->removeNode(m_scoreNode);
        }
    }
}

//
void SVFaceDanceUnit::stop(){
    if(m_AniSpineNode){
        m_AniSpineNode->clearAction();
    }
}

void SVFaceDanceUnit::update(float _dt,float _gametime){
    m_gametime = _gametime;
    if(m_state == 0 || m_state == 1){
        m_life -= _dt;
        if(m_life<0.0f){
            //死亡状态(正常死亡)
            m_state = 2;
        }
    }
}

s32 SVFaceDanceUnit::getFaceState(){
    return m_state;
}

void SVFaceDanceUnit::setInitPos(f32 _x,f32 _y,f32 _z){
    m_initPos.set(_x,_y,_z);
}

void SVFaceDanceUnit::setInitSpeed(f32 _sp){
    m_initSpeed = _sp;
}

void SVFaceDanceUnit::setInitScore(s32 _score){
    m_score = _score;
}

void SVFaceDanceUnit::setInitEnery(f32 _enery){
    m_enery = _enery;
    m_enery = 20;
}

void SVFaceDanceUnit::setFaceType(s32 _type,const char* _aniname,const char* _deadani){
    m_facetype = _type;
    m_aniname = _aniname;
    m_anidead = _deadani;
}

void SVFaceDanceUnit::execDead(bool _all){
    if(m_state == 0){
        m_state = 1;    //暂停状态  (死亡了 但是对象生命周期还在)
        //消除能量值
        s32 t_enery = m_enery;
        //暴击概率
        bool t_iscrit = false;
        int t_crit_value = rand()%100;
        if(t_crit_value<m_critPercent){
            t_iscrit = true;
        }
        //暴击加成
        float t_crit_mult = 1.0;
        if(t_iscrit){
            t_crit_mult = 1.5f;//暴击
        }
        if(_all){
            t_crit_mult = 1.0f; //全部消除事件 不走暴击 不带能量值
            t_iscrit = false;
            t_enery = 0;
            if(m_life>1.0f){
               m_life = 1.0f;   //马上缩短生命周期
            }
        }
        //
        //计算分数(乘上暴击倍数)
        int t_score = int(m_score*t_crit_mult);
        //产生分数动画
        SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
        if(t_sc && m_scoreNode && m_AniSpineNode){
            vector3df* t_pos = m_AniSpineNode->getPosition();
            m_scoreNode->clearAction();
            m_scoreNode->setPosition(t_pos->X,t_pos->Y,t_pos->Z);
            m_scoreNode->setvisible(true);
            m_scoreNode->setAtcPt( ATCH_MC );
            stringc t_score_str = stringc("+") + stringc(t_score);
            m_scoreNode->setText( t_score_str.c_str() );
//            //
//            MaActionMoveByTime* t_act = new MaActionMoveByTime();
//            t_act->setDir(0.0f, 1.0f, 0.0f);
//            t_act->setSpeed(100.0f);
//            t_act->setTime(0.5f);
//            m_scoreNode->addAction(t_act);
//            t_act->release();
//            t_sc->addChild(m_scoreNode , GAMERENDERLAYER::GRL_UI);
        }
        //
        if(m_AniSpineNode){
            m_AniSpineNode->setloop(false);
            m_AniSpineNode->play( m_anidead.c_str() );
        }
        //发送死亡消息出去
        SVEventFDUnitDead* t_unitdead = new SVEventFDUnitDead(m_scenename.c_str());
        t_unitdead->m_aniname = m_aniname;
        t_unitdead->m_gameTime = m_gametime;    //
        t_unitdead->m_score = m_score;          //
        t_unitdead->m_energy = t_enery;
        t_unitdead->m_iscrit = t_iscrit;
        mApp->m_pGlobalMgr->m_pEventMgr->pushEventToSecondPool(t_unitdead);
    }
}

bool SVFaceDanceUnit::procEvent(SVEvent *_event){
    //1抬头 2低头 3张嘴 4张嘴左扭头 5张嘴右扭头 6左眼闭右眼睁 7左眼睁右眼闭 8嘴角上扬 9嘴角向下 10双眼一起闭上 11正面嘟嘴  12左嘟嘴  13右嘟嘴
    if(!_event)
        return false;
    if(m_state == 0){
        //只有存活状态 才会处理相关消息
        stringc t_trans_ani = "";
        if(_event->eventType == EVN_T_RAISE_HEAD){
            t_trans_ani = "head_up";
        }else if(_event->eventType == EVN_T_BOW_HEAD){
            t_trans_ani = "head_down";
        }else if(_event->eventType == EVN_T_MOUSE_OPEN){
            t_trans_ani = "mouth_open";
        }else if(_event->eventType == EVN_T_C_MOUTHOPEN_TURNLEFT){
            t_trans_ani = "mouth_open_left";
        }else if(_event->eventType == EVN_T_C_MOUTHOPEN_TURNRIGHT){
            t_trans_ani = "mouth_open_right";
        }else if(_event->eventType == EVN_T_C_LEFTEYECLOSE_RIGHTEYEOPEN){
            t_trans_ani = "leye_close_reye_open";
        }else if(_event->eventType == EVN_T_C_LEFTEYEOPEN_RIGHTEYECLOSE){
            t_trans_ani = "leye_open_reye_close";
        }else if(_event->eventType == EVN_T_MOUSE_RAISE_CORNER){
            t_trans_ani = "mouth_corner_up";
        }else if(_event->eventType == EVN_T_MOUSE_BOW_CORNER){
            t_trans_ani = "mouth_corner_down";
        }else if(_event->eventType == EVN_T_C_LEFTEYECLOSE_RIGHTEYECLOSE){
            t_trans_ani = "leye_close_reye_close";
        }else if(_event->eventType == EVN_T_MOUSE_TOUTH){
            t_trans_ani = "mouth_touch";
        }else if(_event->eventType == EVN_T_C_MOUTHTOUTH_TURNLEFT){
            t_trans_ani = "mouth_touch_left";
        }else if(_event->eventType == EVN_T_C_MOUTHTOUTH_TURNRIGHT){
            t_trans_ani = "mouth_touch_right";
        }
        if( strcmp(m_aniname.c_str(),t_trans_ani.c_str()) == 0){
            //表情消亡
            execDead(false);
            //SV_LOG_INFO("FD unit dead %s \n",m_aniname.c_str());
            return false;
        }
        //
        SVEventFDUnitAllDead* t_event = dynamic_cast<SVEventFDUnitAllDead*>(_event);
        if(t_event){
            //execDead(true);
            return false;
        }
    }
    return true;
}
