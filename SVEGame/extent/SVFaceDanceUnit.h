//
// Created by lidm on 17/9/5.
//

#ifndef SV_FACEDANCE_UNIT_H
#define SV_FACEDANCE_UNIT_H

#include "../event/SVEventProc.h"
#include "../base/SVVector3d.h"

//游戏独立个体
//class SVSpineNode;
//class SVBitFontNode;
class SVFaceDanceUnit : public SVEventProc {
public:
    SVFaceDanceUnit(SVInst*_app);

    ~SVFaceDanceUnit();
    
    virtual void init(const char* _scenename,const char* _spinename,const char* _scorepng);
    
    virtual void destroy();
    
    void enter();
    
    void exit();
    
    void stop();    //结束画面的暂停
    
    virtual void update(float _dt,float _gametime);
    
    s32 getFaceState();
    
    void setInitPos(f32 _x,f32 _y,f32 _z);
    
    void setInitSpeed(f32 _sp);
    
    void setInitScore(s32 _score);
    
    void setInitEnery(f32 _enery);
    
    void setFaceType(s32 _type,const char* _aniname,const char* _deadani);
    
    virtual bool procEvent(SVEvent *_event);
    
    void execDead(bool _all = false);
    
protected:
    f32 m_gametime;
    vector3df m_initPos;       //初始化位置
    f32 m_initSpeed;        //初始化速度
    f32 m_score;            //带有的分数
    f32 m_enery;
    f32 m_life;             //
    s32 m_state;            //状态(0生存，1非生命周期死亡，而是触发死亡状态，2生命周期死亡状态)
    s32 m_facetype;         //个体类型
    stringc m_scenename;
    stringc m_spinename;
    stringc m_aniname;
    stringc m_anidead;
    stringc m_scorePng;
    f32 m_fontwidth;
    s32 m_critPercent;      //暴击概率
    SVSpineNode* m_AniSpineNode; //表现节点
    SVBitFontNode* m_scoreNode; //分数节点
};


#endif //SV_FACEDANCE_UNIT_H
