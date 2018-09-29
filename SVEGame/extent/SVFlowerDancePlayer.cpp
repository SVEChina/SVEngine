//
// SVTripleDownPlaySys.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//
#include "SVFlowerDancePlayer.h"
#include "SVFlowerDanceEvent.h"
#include "SVGameCommonEvent.h"
#include "../app/SVInst.h"
#include "../event/SVOpEvent.h"
#include "../event/SVEventMgr.h"
#include "../node/SVSpineNode.h"
#include "../node/SVScene.h"
#include "../logicsys/SVSceneMgr.h"
#include "../node/SVSpriteNode.h"
#include "../act/SVActFollow.h"
#include "../node/SVCameraNode.h"
#include "../basesys/SVCameraMgr.h"
#include "../node/SVParticleNode.h"

#define TOANGLE 180 / 3.1415
#define TORADIAN 3.1415 / 180
SVFlowerDancePlayer::SVFlowerDancePlayer(SVInst *_app)
:SVEventProc(_app)
,m_ePlayerState(FD_STAYONFLOWER)
,m_bPressed(false)
,m_AniBee(NULL){
    m_exist_time = 2.0f;
    m_waste_time = 0.0f;
    m_cameraFollow = nullptr;
    m_pTrailNode = nullptr;
}

SVFlowerDancePlayer::~SVFlowerDancePlayer(){
    SAFE_DELETE(m_pTrailNode);
}
    
void SVFlowerDancePlayer::init(){
    SVSpineNodeParam spineparam;
    spineparam.m_spinename = m_strSpineName;
    spineparam.m_aniname = "fly";
    spineparam.m_changeAni = true;
    spineparam.m_loop = false;
    m_AniBee = (SVSpineNode*)spineparam.genNode(mApp);
    
    SVSpriteNodeParam t_bg_param;
    t_bg_param.m_width = 1536;
    t_bg_param.m_height = 2048;
    t_bg_param.m_pos.set(0.0f,0.0f,0.0f);
    t_bg_param.m_texname = m_strBgName;
    t_bg_param.m_zorder = 0;
    m_SpriteBg = (SVSpriteNode*)t_bg_param.genNode(mApp);
    m_SpriteBg->setUI(true);
    
    //绑定相机跟踪
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(t_sc){
        s32 t_camid = t_sc->getCameraID();
        SVCameraNode* t_camnode = mApp->m_pGlobalMgr->m_cameraMgr->getCamera(t_camid);
        if(t_camnode && m_AniBee){
            m_cameraFollow = MakeSharedPtr<SVActFollow>();
            m_cameraFollow->setTargetNode(m_AniBee);
            t_camnode->addAction(m_cameraFollow);
        }
    }
    enterex();
}
    
void SVFlowerDancePlayer::destroy(){
    exitex();
    //
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(t_sc){
        s32 t_camid = t_sc->getCameraID();
        SVCameraNode* t_camnode = mApp->m_pGlobalMgr->m_cameraMgr->getCamera(t_camid);
        if(t_camnode){
            if(m_cameraFollow){
                t_camnode->delAction(m_cameraFollow);
                m_cameraFollow = nullptr;
            }
            t_camnode->resetDefaultCamera();
        }
        m_cameraFollow = nullptr;
    }
    
    //
    if(m_AniBee && t_sc){
        m_AniBee->removeFromParent();
        t_sc->removeNode(m_AniBee);
        m_AniBee->svdestroy();
        SAFE_DELETE(m_AniBee);
    }
    
    //
    if (m_SpriteBg && t_sc){
        t_sc->removeNode(m_SpriteBg);
    }
    
    //尾巴
    if(m_pTrailNode && t_sc){
        m_pTrailNode->setBindNode(nullptr);
        t_sc->removeNode(m_pTrailNode);
        setTailNode(nullptr);
    }
}
    
void SVFlowerDancePlayer::update(float _dt){
    //停在花上就不动
    if (m_ePlayerState == FD_STAYONFLOWER){
        m_waste_time = 0.0f;
        _computeBeeHeadDirection();
        return;
    }
    //朝向花的引力飞行
    if (m_ePlayerState == FD_FLYING_CIRCLE){
        m_waste_time = 0.0f;
        _beeMoveCircle(_dt , m_fSpeed_verticlein);
        if (m_curFlowerPos.getDistanceFrom(m_playerPos) <= m_curFlower_inner){
            m_ePlayerState = FD_STAYONFLOWER;
            m_AniBee->play("stop");
        }
        //
        _computeBeeHeadDirection();
    }
    //背着花的引力飞行
    if (m_ePlayerState == FD_FLYING_CIRCLE_ACEE){
        m_waste_time = 0.0f;
        _beeMoveCircle(_dt , m_fSpeed_verticleout);
        if (m_curFlowerPos.getDistanceFrom(m_playerPos) >= m_curFlower_outter){
            m_ePlayerState = FD_FLYING_OUT_FLOWER;
            m_moveLen = 0;
            m_lastPlayerPos = m_playerPos;
        }
        //
        _computeBeeHeadDirection();
    }
    //离开花 开始计算死亡
    if (m_ePlayerState == FD_FLYING_OUT_FLOWER){
        m_waste_time += _dt;
        _beeMoveLine(_dt);
        //
        SVEeventFDPlayerPos* PosEvent = new SVEeventFDPlayerPos();
        PosEvent->m_vecPos = m_playerPos;
        mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(PosEvent);
        //
        if( m_waste_time>=m_exist_time){
            //发送死亡消息
            SVGameOver* t_gameover = new SVGameOver(nullptr);
            mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(t_gameover);
        }
    }
}

bool SVFlowerDancePlayer::procEvent(SVEvent *_event){
    //用手控制
    SVTouchEvent* pTouch = dynamic_cast<SVTouchEvent*>(_event);
    if (pTouch){
        if (pTouch->eventType == EVN_T_TOUCH_BEGIN){
            m_bPressed = true;
            printf("TOUCH BEGIN\n");
        }else if (pTouch->eventType == EVN_T_TOUCH_END){
            m_bPressed = false;
            printf("TOUCH END\n");
        }
        
        if (m_ePlayerState != FD_FLYING_OUT_FLOWER){
            if (m_bPressed){
                m_ePlayerState = FD_FLYING_CIRCLE_ACEE;
                m_AniBee->setloop(true);
                m_AniBee->play("fly");
                printf("FD_FLYING_CIRCLE_ACEE\n");
            }else{
                m_ePlayerState = FD_FLYING_CIRCLE;
                m_AniBee->setloop(true);
                m_AniBee->play("fly2");
                printf("FD_FLYING_CIRCLE\n");
            }
        }
    }
    //用人脸控制
    SVPersonEvent* pFace = dynamic_cast<SVPersonEvent*>(_event);
    if (pFace && (pFace->eventType == EVN_T_MOUSE_OPEN || pFace->eventType == EVN_T_MOUSE_CLOSE_ACTION)){
        if (pFace->eventType == EVN_T_MOUSE_OPEN){
            m_bPressed = true;
            printf("TOUCH BEGIN\n");
        }else if (pFace->eventType == EVN_T_MOUSE_CLOSE_ACTION){
            m_bPressed = false;
            printf("TOUCH END\n");
        }
        //
        if (m_ePlayerState != FD_FLYING_OUT_FLOWER){
            if (m_bPressed){
                m_ePlayerState = FD_FLYING_CIRCLE_ACEE;
                m_AniBee->setloop(true);
                m_AniBee->play("fly");
                printf("FD_FLYING_CIRCLE_ACEE\n");
            }else{
                m_ePlayerState = FD_FLYING_CIRCLE;
                m_AniBee->setloop(true);
                m_AniBee->play("fly2");
                printf("FD_FLYING_CIRCLE\n");
            }
        }
    }
    
    
    
    
    SVEventEnterFlower* pEnterEvent = dynamic_cast<SVEventEnterFlower*>(_event);
    if (pEnterEvent){
        if (m_bPressed){
            m_ePlayerState = FD_FLYING_CIRCLE_ACEE;
        }else{
            m_ePlayerState = FD_FLYING_CIRCLE;
        }
        
        m_lastFlowerPos   = m_curFlowerPos;
        m_curFlower_inner = pEnterEvent->m_fFlower_inner;
        m_curFlower_outter= pEnterEvent->m_fFlower_outter;
        m_curFlowerPos    = pEnterEvent->m_vecPos;
        _computeBeeAngleSpeedDirection();
    }
    
    SVEventInitFlower* pInitEvent = dynamic_cast<SVEventInitFlower*>(_event);
    if (pInitEvent){
        m_curFlower_inner = pInitEvent->m_fFlower_inner;
        m_curFlower_outter= pInitEvent->m_fFlower_outter;
        m_curFlowerPos    = pInitEvent->m_vecPos;
        
        //玩家的最开始的位置
        m_playerPos.X = m_curFlowerPos.X;
        m_playerPos.Y = m_curFlowerPos.Y + m_curFlower_inner;
        m_AniBee->setPosition(m_playerPos);
    }
    return true;
}
    
void SVFlowerDancePlayer::enter(){
    //起始角速度和垂直方向的速度
    m_fSpeed_circle_angle = 120.0f;
    m_fSpeed_verticleout  = -250.0f;
    m_fSpeed_verticlein   = 30.0f;
    
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(m_AniBee && t_sc ){
        t_sc->addNode(m_AniBee ,3);
        m_AniBee->setloop(true);
        m_AniBee->play("stop");
    }
    
    if (m_SpriteBg && t_sc){
        t_sc->addNode(m_SpriteBg , 0);
    }
    
    if(m_pTrailNode && m_AniBee && t_sc){
        t_sc->addNode(m_pTrailNode);
        //设置粒子发射器绑定的节点
        m_pTrailNode->setBindNode(m_AniBee);
    }
}

void SVFlowerDancePlayer::exit(){
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(m_pTrailNode && t_sc){
        m_pTrailNode->setBindNode(nullptr);
        t_sc->removeNode(m_pTrailNode);
    }
}

void SVFlowerDancePlayer::setTailNode(SVParticleNode* _tailnode){
    if(m_pTrailNode!=_tailnode){
        if(_tailnode)
            _tailnode->retain();
        if(m_pTrailNode)
            m_pTrailNode->release();
        m_pTrailNode = _tailnode;
    }
}

void SVFlowerDancePlayer::_beeMoveCircle(float _dt, float fVerticleSpeed){
    //先转化成以所在花的中心为坐标系原点
    float t_new_player_x = m_playerPos.X - m_curFlowerPos.X;
    float t_new_player_y = m_playerPos.Y - m_curFlowerPos.Y;
    //
    float t_r = sqrt(t_new_player_x*t_new_player_x + t_new_player_y*t_new_player_y);//半径
    //考虑垂直花心方向的运动
    float t_delta_r = fVerticleSpeed * _dt;
    t_r = t_r - t_delta_r;
    //考虑切线方向的运动
    float t_angle_player = atan2(t_new_player_y, t_new_player_x) * TOANGLE;
    float t_angle_player_delta = _dt * m_fSpeed_circle_angle;
    float t_new_angle_player = t_angle_player - t_angle_player_delta;
    float t_player_x = t_r * cosf(t_new_angle_player * TORADIAN);
    float t_player_y = t_r * sinf(t_new_angle_player * TORADIAN);
    //最后转回转回原坐标系
    m_playerPos.X = t_player_x + m_curFlowerPos.X;
    m_playerPos.Y = t_player_y + m_curFlowerPos.Y;
    m_AniBee->setPosition(m_playerPos);
    m_lastplayerR = t_r;
    m_lastPlayerAngle = t_new_angle_player;
}

void    SVFlowerDancePlayer::_beeMoveCircle1(float _dt, float fVerticleSpeed){
    vector3df vecNewPlayer = m_playerPos - m_curFlowerPos;
    
    float t_angle_player_delta = _dt * m_fSpeed_circle_angle;
    vecNewPlayer.rotateXYBy(t_angle_player_delta);
    vector3df vecNew = vecNewPlayer;
    
    float f_r = vecNewPlayer.getLength();
    f_r += fVerticleSpeed * _dt;
    vector3df vecNormal = vecNewPlayer.normalize();
    vecNormal.setLength(f_r);
    
    m_playerPos.X = vecNewPlayer.X + m_curFlowerPos.X;
    m_playerPos.Y = vecNewPlayer.Y + m_curFlowerPos.Y;
    m_AniBee->setPosition(m_playerPos);
}

void SVFlowerDancePlayer::_beeMoveLine(float _dt){
    float t_new_player_x = m_playerPos.X - m_curFlowerPos.X;
    float t_new_player_y = m_playerPos.Y - m_curFlowerPos.Y;
    float t_r = m_lastplayerR;//半径
    float t_v = 2*m_fSpeed_circle_angle * TORADIAN * t_r;
    float t_move_len = t_v * _dt;
    m_moveLen += t_move_len;
    float t_angle_a = m_lastPlayerAngle;
    float t_angle_b = atan2(m_moveLen, t_r) * TOANGLE;
    float t_angle = t_angle_a - t_angle_b;
    float t_len = sqrtf(m_moveLen * m_moveLen + t_r * t_r);
    float t_player_x = t_len * cosf(t_angle*TORADIAN);
    float t_player_y = t_len * sinf(t_angle*TORADIAN);
    //最后转回转回原坐标系
    m_playerPos.X = t_player_x + m_curFlowerPos.X;
    m_playerPos.Y = t_player_y + m_curFlowerPos.Y;
    m_AniBee->setPosition(m_playerPos);
    //计算瓢虫碰撞
    //_collison();
    //计算飞出去的直线角度
    _computeBeeHeadDirection_dir(t_angle); //角度
    
}

////碰撞瓢虫
//void SVFlowerDancePlayer::_collison(){
//    
//}

void SVFlowerDancePlayer::_computeBeeHeadDirection(){
    //计算角度
    vector3df tmp_dir = m_playerPos - m_curFlowerPos;
    tmp_dir = tmp_dir.normalize();
    // 0-180 可以计算真实的角度
    f32 t_bee_angle = radToDeg( acos(tmp_dir.X) );
    //如果三四象限角度需要修正
    if(tmp_dir.Y<0){
        t_bee_angle = 360.0f - t_bee_angle;
    }
    if( m_fSpeed_circle_angle >0){
        m_AniBee->setRotation(0.0f, 0.0f, t_bee_angle + 180);
    }else{
        m_AniBee->setRotation(0.0f, 0.0f, t_bee_angle);
    }

    SV_LOG_INFO("BEE ANGLE %f \n",t_bee_angle);
}

//
void SVFlowerDancePlayer::_computeBeeHeadDirection_dir(f32 t_angle){
    if( m_AniBee ){
        m_AniBee->setRotation(0.0f, 0.0f, t_angle - 90.0f);
    }
}

void SVFlowerDancePlayer::_computeBeeAngleSpeedDirection(){
    //计算小蜜蜂切入后速度方向
    
    //判断两条线段是否相交
    vector3df t_vecA = m_lastFlowerPos;
    vector3df t_vecB = m_curFlowerPos;
    vector3df t_vecC = m_lastPlayerPos;
    vector3df t_vecD = m_playerPos;
    vector3df t_vec_AB = t_vecB - t_vecA;
    vector3df t_vec_AC = t_vecC - t_vecA;
    vector3df t_vec_AD = t_vecD - t_vecA;
    vector3df t_vec_CD = t_vecD - t_vecC;
    vector3df t_vec_CA = t_vecA - t_vecC;
    vector3df t_vec_CB = t_vecB - t_vecC;
    float t_cross_vec_AB_AC = t_vec_AB.X*t_vec_AC.Y - t_vec_AC.X*t_vec_AB.Y;
    float t_cross_vec_AB_AD = t_vec_AB.X*t_vec_AD.Y - t_vec_AD.X*t_vec_AB.Y;
    float t_cross_vec_CD_CA = t_vec_CD.X*t_vec_CA.Y - t_vec_CA.X*t_vec_CD.Y;
    float t_cross_vec_CD_CB = t_vec_CD.X*t_vec_CB.Y - t_vec_CB.X*t_vec_CD.Y;
    if ((t_cross_vec_AB_AC*t_cross_vec_AB_AD <= 0) && (t_cross_vec_CD_CA*t_cross_vec_CD_CB) <=0 ) {
        //相交，角速度要取反
        m_fSpeed_circle_angle = -m_fSpeed_circle_angle;
    }
    
    
    //判断射线与线段是否相交
//    vector3df t_vecA = m_lastFlowerPos;
//    vector3df t_vecB = m_curFlowerPos;
//    vector3df t_vecC = m_lastPlayerPos;
//    vector3df t_vecD = m_playerPos;
//
//    vector3df t_vec_CB = t_vecB - t_vecC;
//    vector3df t_vec_CA = t_vecA - t_vecC;
//    vector3df t_vec_CD = t_vecD - t_vecC;
//    float t_k_CB = t_vec_CB.Y / t_vec_CB.X;
//    float t_k_CA = t_vec_CA.Y / t_vec_CA.X;
//    float t_k_CD = t_vec_CD.Y / t_vec_CB.X;
//    if (t_k_CD<t_k_CB && t_k_CD>t_k_CA) {
//        //相交，角速度要取反
//        m_fSpeed_circle_angle = -m_fSpeed_circle_angle;
//    }
    
}
