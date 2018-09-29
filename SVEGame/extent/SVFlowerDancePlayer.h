//
// SVTripleDownPlaySys.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_FlOWERDANCE_PLAYER_H
#define SV_FlOWERDANCE_PLAYER_H

#include "../event/SVEventProc.h"
#include "../base/SVVector3d.h"
#include "../base/SVPreDeclare.h"
#include "SVFlowerDanceDef.h"

class SVSpineNode;
class SVSpriteNode;

namespace sv {
    //class SVParticleNode;
};

class SVFlowerDancePlayer : public SVEventProc {
    friend class SVParseGameFlowerDance;
public:
    SVFlowerDancePlayer(SVInst *_app);

    ~SVFlowerDancePlayer();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void update(f32 _dt);
    
    virtual bool procEvent(SVEvent *_event);
    
    void    enter();
    
    void    exit();
    
    void setTailNode(SVParticleNode* _tailnode);
    
protected:
    
    void    _beeMoveCircle(f32 _dt , f32 fVerticleSpeed);
    void    _beeMoveCircle1(f32 _dt, f32 fVerticleSpeed);
    void    _beeMoveLine(f32 _dt);
    void    _computeBeeAngleSpeedDirection();
    void    _computeBeeHeadDirection();
    void    _computeBeeHeadDirection_dir(f32 t_angle);
    //void    _collison();
    f32   m_fSpeed_circle_angle;
    f32   m_fSpeed_verticleout;
    f32   m_fSpeed_verticlein;
    FLOWERDANCEPLAYERSTATE m_ePlayerState;
    f32           m_curFlower_inner;
    f32           m_curFlower_outter;
    f32           m_lastplayerR;//记录从上一次从花跳出时的半径
    f32           m_lastPlayerAngle;//记录从上一次从花跳出时的角度
    vector3df       m_lastPlayerPos;//记录从上一从花跳出时的位置。
    f32           m_moveLen;
    vector3df       m_curFlowerPos;
    vector3df       m_lastFlowerPos;
    vector3df       m_playerPos;
    bool            m_bPressed;
    
    SVSpineNode*    m_AniBee;
    stringc         m_strSpineName;
    
    SVSpriteNode*   m_SpriteBg;
    stringc         m_strBgName;
    
    //增加相机运动的action

    SVActFollowPtr  m_cameraFollow;
    SVParticleNode* m_pTrailNode;    //尾部节点
    //
    f32 m_exist_time;   //生存时间
    f32 m_waste_time;   //浪费时间
};


#endif //SV_FlOWERDANCE_PLAYER_H
