//
// MaTripleDownMgr.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_TRIPLEDOWN_MGR_H
#define SV_TRIPLEDOWN_MGR_H

#include "SVTripleDownDef.h"
#include "../event/SVEventProc.h"
#include "../base/SVCoreArray.h"
#include "../base/SVCoreList.h"

class SVTripleDownUnit;
class SVTripleDownMgr : public SVEventProc {
    friend class SVParseGameTripleDown;
public:
    SVTripleDownMgr(SVInst *_app);

    ~SVTripleDownMgr();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void stop();
    
    virtual void update(float _dt,float _gametime);
    
    virtual bool procEvent(SVEvent *_event);
    
    SVTripleDownUnit* getFreeUnit();
    
protected:
    stringc m_scenename;
    typedef sv::array<stringc> UNITANIMATE;
    UNITANIMATE m_arrAnimate;
    
    s32 m_donutWidth;
    s32 m_donutHeight;
    
    typedef sv::list<SVTripleDownUnit*> TRIPLEPOOL;
    TRIPLEPOOL m_obFreePool;
    TRIPLEPOOL m_obUsePool;
    s32 m_maxNum;
};


#endif //SV_TRIPLEDOWN_MGR_H
