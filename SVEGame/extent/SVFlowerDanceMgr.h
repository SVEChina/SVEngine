//
// MaTripleDownMgr.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_FLOWERDANCE_MGR_H
#define SV_FLOWERDANCE_MGR_H

#include "SVTripleDownDef.h"
#include "../event/SVEventProc.h"
#include "../base/SVCoreArray.h"
#include "../base/SVCoreList.h"
#include "../base/SVVector3d.h"

class SVFlowerDanceUnit;
class SVFlowerDanceMgr : public SVEventProc {
    friend class SVParseGameFlowerDance;
public:
    SVFlowerDanceMgr(SVInst *_app);

    ~SVFlowerDanceMgr();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void stop();
    
    virtual void update(f32 _dt);
    
    virtual bool procEvent(SVEvent *_event);
    
protected:
    stringc m_scenename;
    typedef sv::array<stringc> UNITANIMATE;
    UNITANIMATE m_arrAnimate;
    
    stringc m_strCircle;
    stringc m_strSpineNamePC;   //瓢虫
    
    typedef sv::list<SVFlowerDanceUnit*> FLOWERPOOL;
    FLOWERPOOL m_obFreePool;
    FLOWERPOOL m_obUsePool;
    s32 m_maxNum;
    
    vector3df  m_vecLastPos;
    stringc    m_strBgMusic;
    s32        m_iGameBgmId;
private:
    SVFlowerDanceUnit* _generateFlower();
};


#endif //SV_TRIPLEDOWN_MGR_H
