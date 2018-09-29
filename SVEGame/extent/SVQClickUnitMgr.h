//
// Created by lidm on 17/9/5.
//

#ifndef SV_QCLICK_UNIT_MGR_H
#define SV_QCLICK_UNIT_MGR_H

#include "../event/SVEventProc.h"

//游戏独立个体

class SVQClickUnit;

class SVQClickMain;

class SVQClickUnitMgr : public SVEventProc {
public:
    SVQClickUnitMgr(SVInst*_app,SVQClickMain* _gamemain);

    ~SVQClickUnitMgr();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void update(f32 _dt,f32 _gametime);
    
    virtual bool procEvent(SVEvent *_event);
    
    void setSpineName(cptr8 _name);
    
protected:
    void _birth(f32 _x,f32 _y);
    
    void _dead(SVQClickUnit* _unit);
    
    typedef list<SVQClickUnit*> QPOOL;
    QPOOL m_usePool;     //使用池
    QPOOL m_freePool;    //空闲池
    stringc m_spinename;
    
    SVQClickMain* m_pGameMain;
};


#endif //SV_QCLICK_UNIT_MGR_H
