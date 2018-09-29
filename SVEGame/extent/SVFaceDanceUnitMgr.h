//
// Created by lidm on 17/9/5.
//

#ifndef SV_FACEDANCE_UNIT_MGR_H
#define SV_FACEDANCE_UNIT_MGR_H

#include "../event/SVEventProc.h"

//游戏独立个体
class SVFaceDanceUnit;
class SVFaceDanceResConfig;
#define MAX_DOWNPOS 10

class SVFaceDanceUnitMgr : public SVEventProc {
    friend class SVParseGameFaceDance;
public:
    SVFaceDanceUnitMgr(SVInst*_app);

    ~SVFaceDanceUnitMgr();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void stop();
    
    virtual void update(f32 _dt,f32 _gametime);
    
    virtual bool procEvent(SVEvent *_event);
    
    void enalbeTeachMode();
    
    void enableNorMode();
protected:
    
    void _initBornPos();
    
    void _genFaceUnitNor(SVFaceDanceUnit* t_unit);
    
    void _genFaceUnitTeach(SVFaceDanceUnit* t_unit);
    
    stringc m_scenename;
    f32 m_acctime;      //开始后 累计的时间
    f32 m_numPerSec;    //每秒产生的个数
    u32 m_maxNum;       //最大同时支持的笑脸数
    f32 m_maxspeed;
    f32 m_minspeed;
    f32 m_enery;
    u32 m_downPos;
    s32 m_vecDownPos[MAX_DOWNPOS];
    stringc m_spinename;
    stringc m_anidead;
    stringc m_scorepng;
    typedef array<stringc> FACETYPEPOOL;
    FACETYPEPOOL m_faceTypePool;
    
    typedef list<SVFaceDanceUnit*> FACEPOOL;
    FACEPOOL m_faceFreePool;
    FACEPOOL m_faceUsePool;
    //
    u32 m_lastFaceType;
    u32 m_lastFacePos;
    //
    u32 m_model; //模式
    bool m_teachChange; //教学模式 改变
    stringc m_teachAniName;
};


#endif //SV_FACEDANCE_UNIT_MGR_H
