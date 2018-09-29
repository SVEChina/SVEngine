//
// Created by lidm on 17/9/5.
//

#ifndef SV_QCLICK_UNIT_H
#define SV_QCLICK_UNIT_H

#include "../event/SVEventProc.h"
#include "../base/SVVector3d.h"

//游戏独立个体
class SVSpineNode;
class SVBitFontNode;
class SVQClickUnit : public SVEventProc {
public:
    SVQClickUnit(SVInst*_app);

    ~SVQClickUnit();
    
    virtual void init(cptr8 _spinename);
    
    virtual void destroy();
    
    void enter();
    
    void exit();

    void stop();    //结束画面的暂停
    
    virtual void update(f32 _dt,f32 _gametime);
    
    s32 getState();
    
    void setInitPos(f32 _x,f32 _y,f32 _z);
    
    void setLife(f32 _life);
    
    void setInitSpeed(f32 _sp);
    
    void setInitScore(s32 _score);
    
    void setani(cptr8 _aniname);
    
    virtual bool procEvent(SVEvent *_event);
    
protected:
    f32 m_gametime;
    vector3df m_initPos;        //初始化位置
    f32 m_initSpeed;            //初始化速度
    f32 m_score;                //带有的分数
    f32 m_life;
    s32 m_state;                //状态(0生存，1非生命周期死亡，而是触发死亡状态，2生命周期死亡状态)
    stringc m_scenename;
    stringc m_spinename;
    stringc m_aniname;
    SVSpineNode* m_AniSpineNode; //表现节点
};


#endif //SV_QCLICK_UNIT_H
